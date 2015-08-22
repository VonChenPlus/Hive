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

class  AppDelegate : private GRAPH::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();

    virtual bool applicationDidFinishLaunching() {
        // initialize director
        auto director = GRAPH::Director::getInstance();
        auto glview = director->getOpenGLView();
        if(!glview) {
            glview = GRAPH::GLViewImpl::create("Cpp Tests");
            director->setOpenGLView(glview);
        }
    }

    virtual void applicationDidEnterBackground() {

    }

    virtual void applicationWillEnterForeground() {

    }
};

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(MAEMO)
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif

    QApplication a(argc, argv);

    MATH::Sizef size;

    QSize res = QApplication::desktop()->screenGeometry().size();
    if (res.width() < res.height())
        res.transpose();
    
    NativeInit(res.width(), res.height());

    MainWindow w;
    w.show();

    return a.exec();
}
