#ifndef MAINUI_H
#define MAINUI_H

#include "EXTERNALS/glew/GL/glew.h"

#include <QTouchEvent>
#include <QMouseEvent>
#include <QGLWidget>

#include "GRAPH/UI/InputState.h"

class MainUI final : public QGLWidget
{
    Q_OBJECT
public:
    explicit MainUI(QWidget *parent = 0);
    ~MainUI();

signals:
    void doubleClick();
    void newFrame();

protected:
    void resizeEvent(QResizeEvent * e)
    {
        updateScreenScale(e->size().width(), e->size().height());
    }

private:
    bool updateScreenScale(int width, int height);

    void timerEvent(QTimerEvent *) {
        updateGL();
        emit newFrame();
    }

    bool event(QEvent *e) override;

    void initializeGL() override;
    void paintGL() override;

    void updateRunLoop();

private:
    UI::InputState inputstate_;
};

#endif // MAINUI_H
