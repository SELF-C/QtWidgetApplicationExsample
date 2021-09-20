#ifndef GLWIDGET_H
#define GLWIDGET_H

// Qt
#include <QGLWidget>
#include <QOpenGLFunctions_1_1>
#include <QVector>
#include <QVector3D>
#include <QMouseEvent>
#include <QWheelEvent>

// glm
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/gtc/type_ptr.hpp>

// others
#include "model_1_1.h"
#include "camera_glm.h"
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
    Camera_glm* m_camera;
    Model_1_1* m_model;

    glm::mat4 m_viewMatrix;

    glm::vec3 m_angle;
    glm::vec2 m_mousePosition;
};

#endif // GLWIDGET_H
