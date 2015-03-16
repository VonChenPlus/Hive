#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>

#include "base/display.h"

float CalculateDPIScale()
{
    // Sane default rather than check DPI
#ifdef __SYMBIAN32__
    return 1.4f;
#elif defined(USING_GLES2)
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
    pixel_xres = res.width();
    pixel_yres = res.height();
    g_dpi_scale = CalculateDPIScale();
    dp_xres = (int)(pixel_xres * g_dpi_scale); dp_yres = (int)(pixel_yres * g_dpi_scale);

    MainWindow w;
    w.show();

    return a.exec();
}
