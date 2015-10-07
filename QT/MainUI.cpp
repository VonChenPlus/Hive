#include "MainUI.h"
#include "BASE/AutoreleasePool.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"

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
    auto renderView = new(std::nothrow) GRAPH::RenderView();
    renderView->setFrameSize(this->width(), this->height());
    renderView->autorelease();

    GRAPH::Director::getInstance().setRenderView(renderView);
    GRAPH::Director::getInstance().getRenderView()->setDesignResolutionSize(this->width(), this->height(), GRAPH::ResolutionPolicy::SHOW_ALL);

}

void MainUI::paintGL() {
    updateRunLoop();
}

void MainUI::updateRunLoop() {
    GRAPH::Director::getInstance().mainLoop();
    PoolManager::getInstance().getCurrentPool()->clear();
}
