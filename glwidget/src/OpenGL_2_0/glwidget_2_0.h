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

#include "OpenGL_2_0/ShaderProgram.h"
#include "model_2_0.h"
#include "camera_qt.h"

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
    GLuint m_shader;
    QMatrix4x4 m_projectionMatrix, m_viewMatrix;

    QVector2D m_mousePosition;

    Model_2_0* m_model;
    Camera_qt* m_camera;

};

#endif // GLWIDGET_2_0_H
