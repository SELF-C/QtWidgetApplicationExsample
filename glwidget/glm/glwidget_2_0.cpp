#include "glwidget_2_0.h"

GLWidget_2_0::GLWidget_2_0(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    parent->installEventFilter(this);
}

void GLWidget_2_0::initializeGL()
{
    initializeOpenGLFunctions();

    //shader init
    m_shader = ShaderProgram().addShaderFromSourceFile(":/shader/ads.vert", ":/shader/ads.frag");

    m_model = new Model_2_0(":/stl/suzanne.stl");
    m_light = m_model->light();
    m_camera = new Camera();

    m_angle = glm::vec3(0);

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

    m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

void GLWidget_2_0::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);

    // View
    glm::mat4 cameraMatrix = m_camera->matrix();

    glm::vec3 eye    = cameraMatrix * glm::vec4(0.0f, 0.0f, m_camera->distance(), 1.0f);    // 仮想3Dカメラが配置されているポイント
    glm::vec3 center = cameraMatrix * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);                    // カメラが注視するポイント（シーンの中心）
    glm::vec3 up     = cameraMatrix * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);                    // 3Dワールドの上方向を定義
    m_viewMatrix = glm::lookAt(eye, center, up);

    // Draw Call
    m_light.position = glm::vec4(eye,0);
    m_model->setLight(m_light);
    m_model->setRotate(m_angle);
    m_model->update(m_projectionMatrix, m_viewMatrix);
    m_model->draw(m_shader);
}

void GLWidget_2_0::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = glm::vec2(event->pos().x(), event->pos().y());
    event->accept();
}

void GLWidget_2_0::mouseMoveEvent(QMouseEvent *event)
{
    glm::vec2 pos = glm::vec2(event->pos().x(), event->pos().y());

    // マウスの右クリックドラッグでカメラ回転
    if (event->buttons() & Qt::RightButton)
    {
        m_camera->angleLimit(pos - m_mousePosition);
        update();
    }
    m_mousePosition = pos;

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
            m_angle.y--;
            if ( m_angle.y < 0) m_angle.y = 360;
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_D)
        {
            m_angle.y++;
            if ( m_angle.y > 360) m_angle.y = 0;
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_W)
        {
            m_angle.x--;
            if ( m_angle.x < 0) m_angle.x = 360;
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_S)
        {
            m_angle.x++;
            if ( m_angle.x > 360) m_angle.x = 0;
        }

        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_Q)
        {
            m_angle.z--;
            if ( m_angle.z < 0) m_angle.z = 360;
        }
        if (static_cast<QKeyEvent*>(event)->key() == Qt::Key_E)
        {
            m_angle.z++;
            if ( m_angle.z > 360) m_angle.z = 0;
        }

        update();
        break;
    default:
        break;
    }

    return false;
}
