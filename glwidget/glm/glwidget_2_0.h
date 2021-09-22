#ifndef GLWIDGET_2_0_H
#define GLWIDGET_2_0_H

#include <QGLWidget>
#include <QOpenGLFunctions_2_0>
#include <QVector>
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
#include "ShaderProgram.h"
#include "model_2_0.h"
#include "camera.h"

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
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    GLuint m_shader;
    glm::mat4 m_projectionMatrix, m_viewMatrix;

    Camera* m_camera;
    Light m_light;
    Model_2_0* m_model;

    glm::vec3 m_angle;
    glm::vec2 m_mousePosition;
};

#endif // GLWIDGET_2_0_H
