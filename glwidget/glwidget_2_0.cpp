#include "glwidget_2_0.h"

GLWidget_2_0::GLWidget_2_0(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    parent->installEventFilter(this);
}

void GLWidget_2_0::initializeGL()
{
    initializeOpenGLFunctions();

    //shader init
    m_shader = ShaderProgram().addShaderFromSourceFile(":/shader.vert", ":/shader.frag");

    m_model = new Model_2_0();
    m_model->load(":/cube.stl");

    m_camera.angle = QVector2D(20.0, -20.0);
    m_camera.distance = 2.5f;

    // clear
    glClearColor(0,0,0,1);

    // enabled
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
}

void GLWidget_2_0::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    const float aspect = static_cast<float>(w) / static_cast<float>(h);
    const float zNear = 0.01f, zFar = 1000.0f, fov = 45.0f;

    m_projectionMatrix.setToIdentity();
    m_projectionMatrix.perspective(fov, aspect, zNear, zFar);
}

void GLWidget_2_0::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);

    // view
    QMatrix4x4 cameraMatrix;  // カメラを原点に沿って回転させる
    cameraMatrix.rotate(m_camera.angle.x(), QVector3D(0.0f, 1.0f, 0.0f));
    cameraMatrix.rotate(m_camera.angle.y(), QVector3D(1.0f, 0.0f, 0.0f));

    auto eye    = cameraMatrix * QVector3D(0.0f, 0.0f, m_camera.distance);   // 仮想3Dカメラが配置されているポイント
    auto center = cameraMatrix * QVector3D(0.0f, 0.0f, 0.0f);               // カメラが注視するポイント（シーンの中心）
    auto up     = cameraMatrix * QVector3D(0.0f, 1.0f, 0.0f);               // 3Dワールドの上方向を定義

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(eye, center, up);

    // draw call
    m_model->draw(m_shader, m_projectionMatrix, m_viewMatrix);

    update();
}

void GLWidget_2_0::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = event->pos();
    event->accept();
}

void GLWidget_2_0::mouseMoveEvent(QMouseEvent *event)
{
    /* カメラの回転量をマウスの移動量から設定 */
    int deltaX = event->x() - m_mousePosition.x();
    int deltaY = event->y() - m_mousePosition.y();

    // マウスの右クリックドラッグでカメラ回転
    if (event->buttons() & Qt::RightButton) {
        /* カメラのX方向への回転角度制限 */
        m_camera.angle.setX(m_camera.angle.x() - deltaX);
        if (m_camera.angle.x() < 0)
            m_camera.angle.setX(m_camera.angle.x() + 360);

        if (m_camera.angle.x() >= 360)
            m_camera.angle.setX(m_camera.angle.x() - 360);

        /* カメラのY方向への回転角度制限 */
        m_camera.angle.setY(m_camera.angle.y() - deltaY);
        if (m_camera.angle.y() < -90)
            m_camera.angle.setY(-90);

        if (m_camera.angle.y() > 90)
            m_camera.angle.setY(90);
    }
    m_mousePosition = event->pos();

    event->accept();

    update();
}

void GLWidget_2_0::wheelEvent(QWheelEvent *event)
{
    /* マウスホイールでカメラの奥行きの移動量を設定 */
    float delta = event->delta();
    if (event->orientation() == Qt::Vertical) {
        if (delta < 0) {
            m_camera.distance *= 1.1f;
        }
        else if (delta > 0)
        {
            m_camera.distance *= 0.9f;
        }
    }
    event->accept();

    update();
}
