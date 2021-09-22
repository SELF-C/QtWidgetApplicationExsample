#include "glwidget_2_0.h"

GLWidget_2_0::GLWidget_2_0(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    parent->installEventFilter(this);
}

void GLWidget_2_0::initializeGL()
{
    initializeOpenGLFunctions();

    //shader init
    m_shader = ShaderProgram().addShaderFromSourceFile(":/shader/shader.vert", ":/shader/shader.frag");

    m_model = new Model_2_0(":/stl/suzanne.stl");
    m_camera = new Camera();

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

    // View
    QMatrix4x4 cameraMatrix = m_camera->matrix();
    auto eye    = cameraMatrix * QVector3D(0.0f, 0.0f, m_camera->distance());   // 仮想3Dカメラが配置されているポイント
    auto center = cameraMatrix * QVector3D(0.0f, 0.0f, 0.0f);                   // カメラが注視するポイント（シーンの中心）
    auto up     = cameraMatrix * QVector3D(0.0f, 1.0f, 0.0f);                   // 3Dワールドの上方向を定義

    m_viewMatrix.setToIdentity();
    m_viewMatrix.lookAt(eye, center, up);

    // Draw Call
    m_model->setLightPosition(QVector4D(eye,0));
    m_model->setRotate(m_angle);
    m_model->update(m_projectionMatrix, m_viewMatrix);
    m_model->draw(m_shader);

}

void GLWidget_2_0::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = QVector2D(event->pos());
    event->accept();
}

void GLWidget_2_0::mouseMoveEvent(QMouseEvent *event)
{
    auto eventPos = QVector2D(event->pos());

    // マウスの右クリックドラッグでカメラ回転
    if (event->buttons() & Qt::RightButton)
    {
        m_camera->angleLimit(eventPos - m_mousePosition);
        update();
    }
    m_mousePosition = eventPos;

    event->accept();
}

void GLWidget_2_0::wheelEvent(QWheelEvent *event)
{
    /* マウスホイールでカメラの奥行きの移動量を設定 */
    if (event->orientation() == Qt::Vertical) {
        m_camera->depth(event->delta());
        update();
    }

    event->accept();
}

bool GLWidget_2_0::eventFilter(QObject *obj, QEvent *event)
{
    (void) obj;

    switch(event->type()){
    case QEvent::KeyPress:
        // 回転操作
        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_A)
        {
            m_angle.setY(m_angle.y() - 1);
            if ( m_angle.y() < 0) m_angle.setY( 360 );
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_D)
        {
            m_angle.setY(m_angle.y() + 1);
            if ( m_angle.y() > 360) m_angle.setY( 0 );
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_W)
        {
            m_angle.setX(m_angle.x() - 1);
            if ( m_angle.x() < 0) m_angle.setX( 360 );
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_S)
        {
            m_angle.setX(m_angle.x() + 1);
            if ( m_angle.x() > 360) m_angle.setX( 0 );
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Q)
        {
            m_angle.setZ(m_angle.z() - 1);
            if ( m_angle.z() < 0) m_angle.setZ( 360 );
        }
        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_E)
        {
            m_angle.setZ(m_angle.z() + 1);
            if ( m_angle.z() > 360) m_angle.setZ( 0 );
        }

        update();
        break;
    default:
        break;
    }

    return false;
}
