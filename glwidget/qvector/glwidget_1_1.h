#ifndef GLWIDGET_1_1_H
#define GLWIDGET_1_1_H

// Qt
#include <QGLWidget>
#include <QOpenGLFunctions_1_1>
#include <QtMath>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QMouseEvent>
#include <QWheelEvent>

// others
#include "model_1_1.h"
#include "camera.h"
#include "fps.h"

class GLWidget_1_1 : public QGLWidget, protected QOpenGLFunctions_1_1
{
    Q_OBJECT
public:
    explicit GLWidget_1_1(QWidget *parent = nullptr);
    ~GLWidget_1_1() override;

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    QMatrix4x4 m_viewMatrix;

    Camera * m_camera;
    Model_1_1* m_model;

    QVector3D m_angle;
    QVector2D m_mousePosition;

    Fps* m_fps;
};

#endif // GLWIDGET_1_1_H
