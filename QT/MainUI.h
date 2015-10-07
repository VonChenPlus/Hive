#ifndef MAINUI_H
#define MAINUI_H

#include "EXTERNALS/glew/GL/glew.h"
#include <QTouchEvent>
#include <QMouseEvent>
#include <QGLWidget>

class MainUI final : public QGLWidget
{
    Q_OBJECT
public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

signals:
    void doubleClick();
    void newFrame();

private:
    void timerEvent(QTimerEvent *) {
        updateGL();
        emit newFrame();
    }

    bool event(QEvent *e) override;

    void initializeGL() override;
    void paintGL() override;

    void updateRunLoop();
};

#endif // MAINUI_H
