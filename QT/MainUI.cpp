#include "MainUI.h"
#include "BASE/NativeApp.h"
using _INPUT::TouchInput;
using _INPUT::TOUCH_DOWN;
using _INPUT::TOUCH_UP;
using _INPUT::TOUCH_MOVE;
using _INPUT::KeyInput;
using _INPUT::DEVICE_ID_KEYBOARD;
#include "BASE/NKCodeFromQt.h"
#include "UTILS/TIME/Time.h"
using UTILS::TIME::time_update;

namespace GLOBAL
{
    extern int &dpXRes();
    extern int &dpYRes();
    extern int &pixelXRes();
    extern int &pixelYRes();
    extern int &dpi();
    extern float &dpiScale();
    extern float &pixelInDPS();
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

        NativeResized();
        return true;
    }
    return false;
}

bool MainUI::event(QEvent *e) {
    TouchInput input;
    switch(e->type())
    {
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
    case QEvent::KeyPress:
        NativeKey(KeyInput(DEVICE_ID_KEYBOARD, KeyMapRawQttoNative.find(((QKeyEvent*)e)->key())->second, KEY_DOWN));
        break;
    case QEvent::KeyRelease:
        NativeKey(KeyInput(DEVICE_ID_KEYBOARD, KeyMapRawQttoNative.find(((QKeyEvent*)e)->key())->second, KEY_UP));
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
}

void MainUI::paintGL()
{
    time_update();
    updateRunLoop();
}

void MainUI::updateRunLoop()
{
    NativeUpdate(inputstate_);
    NativeRender();
}
