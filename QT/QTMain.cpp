#include "MainWindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "BASE/NativeApp.h"
#include "../RFB/ProtocolConnection.h"
#include "../RFB/DataHandler.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(MAEMO)
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif

    RFB::DataHandler *handler = NULLPTR;
    RFB::ProtocolConnection rfb("192.168.1.103", 5900, *handler);
    while (true)
        rfb.process();

    QApplication a(argc, argv);

    QSize res = QApplication::desktop()->screenGeometry().size();
    if (res.width() < res.height())
        res.transpose();
    
    NativeInit(res.width(), res.height());

    MainWindow w;
    w.show();

    return a.exec();
}
