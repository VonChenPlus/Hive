#include "MainWindow.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QtCore>

MainWindow::MainWindow(QWidget *parent, bool fullscreen) :
    QMainWindow(parent)
{
    QDesktopWidget *desktop = QApplication::desktop();
    int screenNum = QProcessEnvironment::systemEnvironment().value("SDL_VIDEO_FULLSCREEN_HEAD", "0").toInt();

    // Move window to top left coordinate of selected screen
    QRect rect = desktop->screenGeometry(screenNum);
    move(rect.topLeft());

    uigl_ = new MainUI(this);
    setCentralWidget(uigl_);

    setZoom(2);

    if (fullscreen)
        fullscrAct();

    QObject::connect(uigl_, SIGNAL(doubleClick()), this, SLOT(fullscrAct()));
}

void MainWindow::setZoom(int zoom) {
    if (isFullScreen())
        fullscrAct();
    if (zoom < 1) zoom = 1;
    if (zoom > 4) zoom = 4;

    uigl_->setFixedSize(480 * zoom, 320 * zoom);
    setFixedSize(sizeHint());
}

void MainWindow::fullscrAct()
{
    if(isFullScreen()) {
        showNormal();
    }
    else {
        uigl_->setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

        showFullScreen();
    }

    QTimer::singleShot(1000, this, SLOT(raiseTopMost()));
}
