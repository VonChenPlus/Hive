#include "MainUI.h"
#include "BASE/AutoreleasePool.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH\UI\Layer.h"
#include "GRAPH\Scene.h"
#include "GRAPH\Director.h"
#include "GRAPH\RenderView.h"
#include "GRAPH\UI\CONTROLS\Label.h"
#include "GRAPH\Sprite.h"

namespace GRAPH
{
    class HelloWorld : public Layer
    {
    public:
        virtual bool init() override {
            if (!Layer::init())
            {
                return false;
            }

            auto visibleSize = Director::getInstance().getRenderView()->getVisibleSize();
            auto origin = Director::getInstance().getRenderView()->getVisibleOrigin();
            auto label = Label::createWithSystemFont("Hello World", "arial.ttf", 51);

            // position the label on the center of the screen
            label->setPosition(origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height - label->getContentSize().height);

            this->addChild(label, 1);

            // add "HelloWorld" splash screen"
            auto sprite = Sprite::create("HelloWorld.png");

            // position the sprite on the center of the screen
            sprite->setPosition(MATH::Vector2f(visibleSize / 2) + origin);

            // add the sprite as a child to this layer
            this->addChild(sprite);
        }

        static Scene* scene() {
            // 'scene' is an autorelease object
            auto scene = Scene::create();

            // 'layer' is an autorelease object
            HelloWorld *layer = HelloWorld::create();

            // add layer as a child to scene
            scene->addChild(layer);

            // return the scene
            return scene;
        }

        // a selector callback
        void menuCloseCallback(HObject* sender);

        static HelloWorld* create() {
            HelloWorld *pRet = new(std::nothrow) HelloWorld(); \
                if (pRet && pRet->init()) {
                    pRet->autorelease();
                    return pRet;
                }
                else {
                    delete pRet;
                    pRet = NULL;
                    return NULL;
                }
        }
    };
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
    auto renderView = new(std::nothrow) GRAPH::RenderView();
    renderView->setFrameSize(this->width(), this->height());
    renderView->autorelease();

    GRAPH::Director::getInstance().setRenderView(renderView);
    GRAPH::Director::getInstance().getRenderView()->setDesignResolutionSize(this->width(), this->height(), GRAPH::ResolutionPolicy::SHOW_ALL);

    // create a scene. it's an autorelease object
    auto scene = GRAPH::HelloWorld::scene();

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
