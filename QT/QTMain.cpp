#include "MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "BASE/NativeApp.h"
#include "../RFB/ProtocolConnection.h"
#include "../RFB/DataHandler.h"
#include "MATH/MathDef.h"
#include "MATH/Size.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(MAEMO)
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif

    QApplication a(argc, argv);

    QSize res = QApplication::desktop()->screenGeometry().size();
    if (res.width() < res.height())
        res.transpose();
    
    NativeInit(res.width(), res.height());

    MainWindow w;
    w.show();

    return a.exec();
}
