#include "MainUI.h"

#include <memory>
using std::shared_ptr;
using std::make_shared;

#include "BASE/NativeApp.h"
using UI::TouchInput;
using UI::TOUCH_DOWN;
using UI::TOUCH_UP;
using UI::TOUCH_MOVE;
using UI::KeyInput;
using UI::DEVICE_ID_KEYBOARD;
#include "NKCodeFromQt.h"
#include "UTILS/TIME/TimeUtils.h"
using UTILS::TIME::TimeUpdate;
#include "GRAPH/GFX/DrawBuffer.h"
using GFX::DrawBuffer;
#include "GRAPH/GFX/Texture.h"
using GFX::Atlas;
using GFX::AtlasImage;
#include "GRAPH/UI/Theme.h"
using UI::Theme;
#include "GRAPH/UI/UIContext.h"
using UI::UIContext;
#include "MATH/Bounds.h"
using MATH::Boundsf;
#include "MATH/Matrix.h"
using MATH::Matrix4;
#include "GRAPH/THIN3D/Thin3D.h"
using THIN3D::Thin3DTexture;
using THIN3D::Thin3DContext;
using THIN3D::T3DShaderSetPreset;
using THIN3D::T3DImageType;
#include "GRAPH/UI/ScreenManager.h"
using UI::ScreenManager;
#include "QTTextDrawer.h"
#include "../UI/LogoScreen.h"
#include "IMAGE/TinyZim.h"

namespace GLOBAL
{
    extern int &dpXRes();
    extern int &dpYRes();
    extern int &pixelXRes();
    extern int &pixelYRes();
    extern int &dpi();
    extern float &dpiScale();
    extern float &pixelInDPS();
    extern Thin3DContext &thin3DContext();
    extern ScreenManager &screenManager();

    // This is the drawbuffer used for UI. Remember to flush it at the end of the frame.
    // TODO: One should probably pass it in through UIInit.
    shared_ptr<DrawBuffer> _DrawBuf2D;
    DrawBuffer &drawBuffer2D() { return *_DrawBuf2D; }
    shared_ptr<DrawBuffer> _DrawBuf2DFront;	// for things that need to be on top of the rest
    DrawBuffer &drawBuffer2DFront() { return *_DrawBuf2DFront; }
    shared_ptr<Theme> _UITheme;
    Theme &uiTheme() { return *_UITheme; }
    shared_ptr<UIContext> _UIContext;
    UIContext &uiContext() { return *_UIContext; }
}

MainUI::MainUI(QWidget *parent)
    : QGLWidget(parent) {

    setAttribute(Qt::WA_AcceptTouchEvents);
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    setAttribute(Qt::WA_LockLandscapeOrientation);
#endif
    setFocus();
    setFocusPolicy(Qt::StrongFocus);
    startTimer(16);

    GLOBAL::_UITheme = make_shared<Theme>();
    GLOBAL::_DrawBuf2D = make_shared<DrawBuffer>();
    GLOBAL::_DrawBuf2DFront = make_shared<DrawBuffer>();

    // custom init
    GLOBAL::uiTheme().uiFont = UI::FontStyle(UBUNTU24, "", 20);
    GLOBAL::uiTheme().uiFontSmall = UI::FontStyle(UBUNTU24, "", 14);
    GLOBAL::uiTheme().uiFontSmaller = UI::FontStyle(UBUNTU24, "", 11);

    GLOBAL::uiTheme().checkOn = I_CHECKEDBOX;
    GLOBAL::uiTheme().whiteImage = I_SOLIDWHITE;
    GLOBAL::uiTheme().dropShadow4Grid = I_DROP_SHADOW;

    GLOBAL::uiTheme().itemStyle.background = UI::Drawable(0x55000000);
    GLOBAL::uiTheme().itemStyle.fgColor = 0xFFFFFFFF;
    GLOBAL::uiTheme().itemFocusedStyle.background = UI::Drawable(0xFFedc24c);
    GLOBAL::uiTheme().itemDownStyle.background = UI::Drawable(0xFFbd9939);
    GLOBAL::uiTheme().itemDownStyle.fgColor = 0xFFFFFFFF;
    GLOBAL::uiTheme().itemDisabledStyle.background = UI::Drawable(0x55E0D4AF);
    GLOBAL::uiTheme().itemDisabledStyle.fgColor = 0x80EEEEEE;
    GLOBAL::uiTheme().itemHighlightedStyle.background = UI::Drawable(0x55bdBB39);
    GLOBAL::uiTheme().itemHighlightedStyle.fgColor = 0xFFFFFFFF;

    GLOBAL::uiTheme().buttonStyle = GLOBAL::uiTheme().itemStyle;
    GLOBAL::uiTheme().buttonFocusedStyle = GLOBAL::uiTheme().itemFocusedStyle;
    GLOBAL::uiTheme().buttonDownStyle = GLOBAL::uiTheme().itemDownStyle;
    GLOBAL::uiTheme().buttonDisabledStyle = GLOBAL::uiTheme().itemDisabledStyle;
    GLOBAL::uiTheme().buttonHighlightedStyle = GLOBAL::uiTheme().itemHighlightedStyle;

    GLOBAL::uiTheme().popupTitle.fgColor = 0xFFE3BE59;
}

MainUI::~MainUI() {

}

bool MainUI::updateScreenScale(int width, int height) {
    GLOBAL::dpi() = 72;
    GLOBAL::dpiScale() = 1.0f;
    #if defined(_WIN32)
        if (width < 480 + 80) {
            GLOBAL::dpiScale() = 2.0f;
        }
    #endif

    GLOBAL::pixelInDPS() = 1.0f / GLOBAL::dpiScale();

    int new_dp_xres = width * GLOBAL::dpiScale();
    int new_dp_yres = height * GLOBAL::dpiScale();

    bool dp_changed = new_dp_xres != GLOBAL::dpXRes() || new_dp_yres != GLOBAL::dpYRes();
    bool px_changed = GLOBAL::pixelXRes() != width || GLOBAL::pixelYRes() != height;

    if (dp_changed || px_changed) {
        GLOBAL::dpXRes() = new_dp_xres;
        GLOBAL::dpYRes() = new_dp_yres;
        GLOBAL::pixelXRes() = width;
        GLOBAL::pixelYRes() = height;

        // Modifying the bounds here can be used to "inset" the whole image to gain borders for TV overscan etc.
        // The UI now supports any offset but not the EmuScreen yet.
        if (GLOBAL::_UIContext)
            GLOBAL::uiContext().setBounds(Boundsf(0, 0, GLOBAL::dpXRes(), GLOBAL::dpYRes()));

        return true;
    }
    return false;
}

bool MainUI::event(QEvent *e) {
    TouchInput input;
    switch(e->type())
    {
    case QEvent::MouseButtonDblClick:
        // DO NOT NEED THIS NOW
        //emit doubleClick();
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        inputstate_.pointer_down[0] = (e->type() == QEvent::MouseButtonPress);
        inputstate_.pointer_x[0] = ((QMouseEvent*)e)->pos().x() * GLOBAL::dpiScale();
        inputstate_.pointer_y[0] = ((QMouseEvent*)e)->pos().y() * GLOBAL::dpiScale();

        input.x = ((QMouseEvent*)e)->pos().x() * GLOBAL::dpiScale();
        input.y = ((QMouseEvent*)e)->pos().y() * GLOBAL::dpiScale();
        input.flags = (e->type() == QEvent::MouseButtonPress) ? TOUCH_DOWN : TOUCH_UP;
        input.id = 0;
        NativeTouch(input);
        break;
    case QEvent::MouseMove:
        inputstate_.pointer_x[0] = ((QMouseEvent*)e)->pos().x() * GLOBAL::dpiScale();
        inputstate_.pointer_y[0] = ((QMouseEvent*)e)->pos().y() * GLOBAL::dpiScale();

        input.x = ((QMouseEvent*)e)->pos().x() * GLOBAL::dpiScale();
        input.y = ((QMouseEvent*)e)->pos().y() * GLOBAL::dpiScale();
        input.flags = TOUCH_MOVE;
        input.id = 0;
        NativeTouch(input);
        break;
    case QEvent::Wheel:
        NativeKey(KeyInput(DEVICE_ID_MOUSE, ((QWheelEvent*)e)->delta()<0 ? NKCODE_EXT_MOUSEWHEEL_DOWN : NKCODE_EXT_MOUSEWHEEL_UP, KEY_DOWN));
        break;
    case QEvent::KeyPress: {
        std::map<int, int>::const_iterator it = KeyMapRawQttoNative.find(((QKeyEvent*) e)->key());
        if (it != KeyMapRawQttoNative.end())
            NativeKey(KeyInput(DEVICE_ID_KEYBOARD, it->second, KEY_DOWN));
        else
            NativeKey(KeyInput(DEVICE_ID_KEYBOARD, ((QKeyEvent*) e)->key(), KEY_CHAR));
    }
        break;
    case QEvent::KeyRelease: {
        std::map<int, int>::const_iterator it = KeyMapRawQttoNative.find(((QKeyEvent*) e)->key());
        if (it != KeyMapRawQttoNative.end())
            NativeKey(KeyInput(DEVICE_ID_KEYBOARD, it->second, KEY_UP));
        else
            NativeKey(KeyInput(DEVICE_ID_KEYBOARD, ((QKeyEvent*) e)->key(), KEY_UP));
    }
        break;
    default:
        return QWidget::event(e);
    }
    e->accept();
    return true;
}

void MainUI::initializeGL()
{
    glewInit();

    NativeInitGraphics();

    GLOBAL::drawBuffer2D().init(&GLOBAL::thin3DContext());
    GLOBAL::drawBuffer2DFront().init(&GLOBAL::thin3DContext());

    GLOBAL::_UIContext = make_shared<UIContext>();
    GLOBAL::uiContext().theme = &GLOBAL::uiTheme();
    GLOBAL::uiContext().init(&GLOBAL::thin3DContext(),
        GLOBAL::thin3DContext().getShaderSetPreset(T3DShaderSetPreset::SS_TEXTURE_COLOR_2D),
        GLOBAL::thin3DContext().getShaderSetPreset(T3DShaderSetPreset::SS_COLOR_2D),
        NULLPTR, &GLOBAL::drawBuffer2D(), &GLOBAL::drawBuffer2DFront(), new QTTextDrawer(&GLOBAL::thin3DContext()));
    if (GLOBAL::uiContext().text())
        GLOBAL::uiContext().text()->setFont("Tahoma", 20, 0);

    GLOBAL::screenManager().setUIContext(&GLOBAL::uiContext());
    GLOBAL::screenManager().switchScreen(new LogoScreen);
}

void MainUI::paintGL()
{
    TimeUpdate();
    updateRunLoop();
}

void MainUI::updateRunLoop()
{
    NativeUpdate(inputstate_);

    // Apply the UIContext bounds as a 2D transformation matrix.
    Matrix4 ortho;
    Matrix4::createOrthographicOffCenter(0.0f, GLOBAL::dpXRes(), GLOBAL::dpYRes(), 0.0f, -1.0f, 1.0f, &ortho);
    GLOBAL::drawBuffer2D().setDrawMatrix(ortho);
    GLOBAL::drawBuffer2DFront().setDrawMatrix(ortho);

    NativeRender();
}
