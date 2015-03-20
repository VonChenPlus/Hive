#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

float CalculateDPIScale()
{
    // Sane default rather than check DPI
#ifdef USING_GLES2
    return 1.2f;
#else
    return 1.0f;
#endif
}

int main(int argc, char *argv[])
{
#if defined(Q_OS_LINUX) && !defined(MAEMO)
    QApplication::setAttribute(Qt::AA_X11InitThreads, true);
#endif

    QApplication a(argc, argv);

    QSize res = QApplication::desktop()->screenGeometry().size();
    if (res.width() < res.height())
        res.transpose();
    
    MainWindow w;
    w.show();

    return a.exec();
}
