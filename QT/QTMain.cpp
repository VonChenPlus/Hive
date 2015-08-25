#include "MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "BASE/NativeApp.h"
#include "../RFB/ProtocolConnection.h"
#include "../RFB/DataHandler.h"
#include "MATH/MathDef.h"
#include "MATH/Size.h"

#include "GRAPH/BASE/Application.h"
#include "GRAPH/BASE/Director.h"
#include "GRAPH/RENDERER/GLView.h"
#include "GRAPH/BASE/Scene.h"
#include "GRAPH/BASE/Layer.h"
#include "GRAPH/UI/BASE/UILayout.h"
#include "GRAPH/UI/CONTROLS/UIText.h"
#include "GRAPH/UI/CONTROLS/UIButton.h"

namespace GRAPH
{
    MATH::Rectf s_visibleRect;
    class UIButtonTest : public Scene
    {
    public:
        UIButtonTest() { init(); }
        virtual ~UIButtonTest() {}

        virtual bool init() {
            s_visibleRect = Director::getInstance()->getOpenGLView()->getVisibleRect();
            if (Scene::init()) {
                // add title and subtitle
                TTFConfig ttfConfig("S:\\OpenSource\\Hive\\fonts\\arial.ttf", 26);
                _titleLabel = Label::createWithTTF(ttfConfig, "UIScene");
                addChild(_titleLabel, 9999);
                MATH::Vector2f visiableCenter(s_visibleRect.origin.x+s_visibleRect.size.width/2, s_visibleRect.origin.y+s_visibleRect.size.height/2);
                MATH::Vector2f visiableTop(s_visibleRect.origin.x + s_visibleRect.size.width / 2, s_visibleRect.origin.y + s_visibleRect.size.height);
                _titleLabel->setPosition(visiableCenter.x, visiableTop.y - 30);

                _uiLayer = Layer::create();
                addChild(_uiLayer);
                _widget = UI::Layout::create();
                _uiLayer->addChild(_widget);

                MATH::Sizef widgetSize = _widget->getContentSize();

                // Add a label in which the button events will be displayed
                _displayValueLabel = UI::Text::create("No Event", "S:\\OpenSource\\Hive\\fonts\\Marker Felt.ttf", 32);
                _displayValueLabel->setAnchorPoint(MATH::Vector2f(0.5f, -1.0f));
                _displayValueLabel->setPosition(MATH::Vector2f(widgetSize.width / 2.0f, widgetSize.height / 2.0f));
                _uiLayer->addChild(_displayValueLabel);

                // Add the alert
                UI::Text* alert = UI::Text::create("Button", "fonts/Marker Felt.ttf", 30);
                alert->setColor(Color3B(159, 168, 176));

                alert->setPosition(MATH::Vector2f(widgetSize.width / 2.0f,
                    widgetSize.height / 2.0f - alert->getContentSize().height * 1.75f));

                _uiLayer->addChild(alert);

                // Create the button
                UI::Button* button = UI::Button::create("cocosui/animationbuttonnormal.png",
                    "cocosui/animationbuttonpressed.png");
                button->setPosition(MATH::Vector2f(widgetSize.width / 2.0f, widgetSize.height / 2.0f));
                button->addTouchEventListener(CC_CALLBACK_2(UIButtonTest::touchEvent, this));
                button->setZoomScale(0.4f);
                button->setPressedActionEnabled(true);
                button->setOpacity(100);
                _uiLayer->addChild(button);
                return true;
            }
        }

        void touchEvent(HObject* sender, UI::Widget::TouchEventType type)
        {
            switch (type)
            {
            case UI::Widget::TouchEventType::BEGAN:
                _displayValueLabel->setString("Touch Down");
                break;

            case UI::Widget::TouchEventType::MOVED:
                _displayValueLabel->setString("Touch Move");
                break;

            case UI::Widget::TouchEventType::ENDED:
            {
                _displayValueLabel->setString("Touch Up");
                UI::Button *btn = (UI::Button*) sender;
                btn->loadTextureNormal("cocosui/animationbuttonnormal.png");
            }
            break;

            case UI::Widget::TouchEventType::CANCELED:
                _displayValueLabel->setString("Touch Cancelled");
                break;

            default:
                break;
            }
        }

    protected:
        Layer* _uiLayer;
        UI::Layout* _widget;
        UI::Text* _sceneTitle;
        Label* _titleLabel;
        UI::Text* _displayValueLabel;
    };

    class  AppDelegate : private GRAPH::Application
    {
    public:
        AppDelegate() {}
        virtual ~AppDelegate() {}

        virtual bool applicationDidFinishLaunching() {
            // initialize director
            auto director = GRAPH::Director::getInstance();
            auto glview = director->getOpenGLView();
            if (!glview) {
                glview = GRAPH::GLViewImpl::create("Cpp Tests");
                director->setOpenGLView(glview);
            }

            auto screenSize = glview->getFrameSize();
            auto designSize = MATH::Sizef(480, 320);
            auto resourceSize = MATH::Sizef(960, 640);
            director->setContentScaleFactor(resourceSize.height / designSize.height);
            glview->setDesignResolutionSize(designSize.width, designSize.height, GRAPH::ResolutionPolicy::SHOW_ALL);

            Director::getInstance()->replaceScene(new UIButtonTest());
            return true;
        }

        virtual void applicationDidEnterBackground() {

        }

        virtual void applicationWillEnterForeground() {

        }
    };
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(MAEMO)
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif

    GRAPH::AppDelegate app;
    return GRAPH::Application::getInstance()->run();

    //QApplication a(argc, argv);

    //QSize res = QApplication::desktop()->screenGeometry().size();
    //if (res.width() < res.height())
    //    res.transpose();
    //
    //NativeInit(res.width(), res.height());

    //MainWindow w;
    //w.show();

    //return a.exec();
}
