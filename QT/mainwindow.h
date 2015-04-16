#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "MainUI.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, bool fullscreen = false);
    ~MainWindow() {}

private slots:
    void fullscrAct();

private:
    void setZoom(int zoom);

    MainUI* uigl_;
};

#endif // MAINWINDOW_H
