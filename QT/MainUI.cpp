#include "MainUI.h"
#include "../UI/LogoLayer.h"
#include "BASE/HData.h"
#include "BASE/AutoreleasePool.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UNITY3D/GLTexture.h"

namespace GRAPH
{
    HData getTextureDataForText(const char * text, const FontDefinition& textDefinition, TextAlign align, int &width, int &height, bool& hasPremultipliedAlpha)
    {
        QFont* qfont = new QFont();
        qfont->setPointSize(textDefinition.fontSize);
        QFontMetrics fm(*qfont);
        QSize size = fm.size(0, QString::fromUtf8(text));
        QImage image(size.width(), size.height(), QImage::Format_ARGB32_Premultiplied);
        if (image.isNull())
            return HData();
        image.fill(255);
        QPainter painter;
        painter.begin(&image);
        painter.setFont(*qfont);
        painter.setPen(QColor(textDefinition.fontFillColor.red, textDefinition.fontFillColor.green, textDefinition.fontFillColor.blue));
        painter.drawText(image.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromUtf8(text));
        painter.end();
        HData ret;
        ret.copy(image.bits(), image.byteCount());
        width = size.width();
        height = size.height();
        return ret;
    }
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

bool MainUI::event(QEvent *e) {
    switch(e->type())
    {
    case QEvent::MouseButtonDblClick:
        break;
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        break;
    case QEvent::MouseMove:
        break;
    case QEvent::Wheel:
        break;
    case QEvent::KeyPress:
        break;
    case QEvent::KeyRelease:
        break;
    default:
        return QWidget::event(e);
    }
    e->accept();
    return true;
}

void MainUI::initializeGL() {
    glewInit();
    
    GRAPH::GLTexture::getTextureDataForText = GRAPH::getTextureDataForText;

    auto renderView = new(std::nothrow) GRAPH::RenderView();
    renderView->setFrameSize(this->width(), this->height());
    renderView->autorelease();

    GRAPH::Director::getInstance().setRenderView(renderView);
    GRAPH::Director::getInstance().getRenderView()->setDesignResolutionSize(this->width(), this->height(), GRAPH::ResolutionPolicy::SHOW_ALL);

    // create a scene. it's an autorelease object
    auto scene = LogoLayer::scene();

    // run
    GRAPH::Director::getInstance().runWithScene(scene);
}

void MainUI::paintGL() {
    updateRunLoop();
}

void MainUI::updateRunLoop() {
    GRAPH::Director::getInstance().mainLoop();
    PoolManager::getInstance().getCurrentPool()->clear();
}
