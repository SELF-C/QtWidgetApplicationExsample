#ifndef GLWIDGET_2_0_H
#define GLWIDGET_2_0_H

#include <QGLWidget>
#include <QOpenGLFunctions_2_0>
#include <QtMath>
#include <QVector>
#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

#include <QMouseEvent>
#include <QWheelEvent>

#include "shaderprogram.h"
#include "model_2_0.h"

class GLWidget_2_0  : public QGLWidget, protected QOpenGLFunctions_2_0
{
    Q_OBJECT
public:
    explicit GLWidget_2_0(QWidget *parent = nullptr);

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;
private:
    struct Camera
    {
        QVector2D angle;
        float distance;
    };

    GLuint m_shader;
    QMatrix4x4 m_projectionMatrix, m_viewMatrix;

    QPoint m_mousePosition;
    Camera m_camera;

    Model_2_0* m_model;

};

#endif // GLWIDGET_2_0_H
