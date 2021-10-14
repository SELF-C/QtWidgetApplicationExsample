#include "glwidget.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    parent->installEventFilter(this);
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    // clear
    glm::vec4 color = glm::vec4(60, 60, 60 ,1) / 255.0f;
    glClearColor(color.r, color.g, color.b, color.a);


    // enabled
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //shader init
    m_shader = ShaderProgram().addShaderFromSourceFile(":/resource/shader/ads.vert", ":/resource/shader/ads.frag");

    // model load
    auto filename = QString(":/resource/stl/suzanne.stl");
    if( !filename.isEmpty() )
    {
        std::vector<VertexData> vertices;
        std::vector<GLuint> indexes;
        Loader3D().createStlBinaryVertexArray(filename, vertices, indexes);

        m_model = new Model();
        m_model->createBuffer(vertices, indexes);
        m_model->setLocation(m_shader);
    }
    else
    {
        qDebug() << "model file not found.";
    }

    // init lighting
    m_light = new Light();
    m_light->setLocation(m_shader);

    // init material
    m_material = new Material();
    m_material->setLocation(m_shader);

    // init camera
    m_camera = new Camera();
    m_camera->setEye(glm::vec4(0.0f, 0.0f, m_camera->distance(), 1.0f));
    m_camera->setCenter(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    m_camera->setUp(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

    m_angle = glm::vec3(0);
    m_fps = new Fps();

}

void GLWidget::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    const float aspect = static_cast<float>(w) / static_cast<float>(h);
    const float zNear = 0.01f, zFar = 1000.0f, fov = 45.0f;

    m_projectionMatrix = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

void GLWidget::paintGL()
{
    m_fps->measurement();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(m_shader);

    // View
    m_camera->setEye(glm::vec4(0.0f, 0.0f, m_camera->distance(), 1.0f));
    m_light->setPosition(glm::vec4(m_camera->position(), 1.f));
    glm::mat4 viewMatrix = m_camera->viewMatrix();


    // Draw Call
    m_model->update();

    m_light->uniform();
    m_material->uniform();
    m_model->draw(m_projectionMatrix, viewMatrix, glm::mat4(1));

    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = glm::vec2(event->pos().x(), event->pos().y());
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
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

void GLWidget::wheelEvent(QWheelEvent *event)
{
    /* マウスホイールでカメラの奥行きの移動量を設定 */
    if (event->orientation() == Qt::Vertical) {
        m_camera->depth(event->delta());
        update();
    }

    event->accept();
}

bool GLWidget::eventFilter(QObject *obj, QEvent *event)
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
