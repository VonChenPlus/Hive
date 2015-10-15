#include "MainUI.h"
#include "BASE/AutoreleasePool.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UI/Layer.h"
#include "GRAPH/Scene.h"
#include "GRAPH/Director.h"
#include "GRAPH/RenderView.h"
#include "GRAPH/UI/CONTROLS/Label.h"
#include "GRAPH/Sprite.h"
#include "GRAPH/UNITY3D/Texture2D.h"

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
            auto label = Label::createWithSystemFont("Hello World", "arial.ttf", 24);

            // position the label on the center of the screen
            label->setPosition(origin.x + visibleSize.width / 2,
                origin.y + visibleSize.height - label->getContentSize().height);

            this->addChild(label, 1);

            return true;
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
    
    GRAPH::Texture2D::getTextureDataForText = GRAPH::getTextureDataForText;

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
