#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QOpenGLFunctions_2_0>
#include <QElapsedTimer>
#include <QDebug>

struct Fps
{
    void measurement()
    {
        if (!m_fpsCountTimer.isValid()) m_fpsCountTimer.start();
        if (m_fpsCountTimer.elapsed() >= 1000)
        {
            m_fps = m_framesInSecond;
            qDebug() << QString("FPS: %1").arg(m_fps);

            m_framesInSecond = 0;
            m_fpsCountTimer.restart();
        }
        m_framesInSecond++;
    }
private:
    QElapsedTimer m_fpsCountTimer;
    int m_framesInSecond = 0;
    int m_fps = 0;
};


class GLWidget : public QGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);



protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
};

#endif // GLWIDGET_H
