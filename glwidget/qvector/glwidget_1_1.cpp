#include "glwidget_1_1.h"

GLWidget_1_1::GLWidget_1_1(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    parent->installEventFilter(this);
}

GLWidget_1_1::~GLWidget_1_1()
{
    delete m_model;
    delete m_camera;
}

void GLWidget_1_1::initializeGL()
{
    initializeOpenGLFunctions();

    m_model = new Model_1_1();
    m_model->load(":/stl/suzanne.stl");

    m_camera = new Camera();

    m_angle = QVector3D(0.0f, 0.0f, 0.0f);

    glClearColor(0,0,0,1);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    float lightPosition[] = { 0.5f, 5.0f, 7.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    glShadeModel(GL_FLAT);
}


void GLWidget_1_1::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    const GLfloat aspect = static_cast<GLfloat>(w) / static_cast<GLfloat>(h);
    const GLfloat zNear = 0.01f, zFar = 1000.0f, fov = 45.0f;

    QMatrix4x4 projection;
    projection.perspective(fov, aspect, zNear, zFar);
    glLoadMatrixf(projection.constData());

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget_1_1::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // View
    QMatrix4x4 cameraMatrix = m_camera->matrix();

    QVector3D eye    = cameraMatrix * QVector3D(0.0f, 0.0f, m_camera->distance());    // 仮想3Dカメラが配置されているポイント
    QVector3D center = cameraMatrix * QVector3D(0.0f, 0.0f, 0.0f);                    // カメラが注視するポイント（シーンの中心）
    QVector3D up     = cameraMatrix * QVector3D(0.0f, 1.0f, 0.0f);                    // 3Dワールドの上方向を定義

    QMatrix4x4 viewMatrix;
    viewMatrix.lookAt(eye, center, up);

    // Draw Call
    m_model->update(viewMatrix);
    m_model->draw();

    update();
}

void GLWidget_1_1::mousePressEvent(QMouseEvent *event)
{
    m_mousePosition = QVector2D(event->pos());
    event->accept();
}

void GLWidget_1_1::mouseMoveEvent(QMouseEvent *event)
{
    // マウスの右クリックドラッグでカメラ回転
    if (event->buttons() & Qt::RightButton)
    {
        QVector2D(event->pos()) - m_mousePosition;
        m_camera->angleLimit(QVector2D(event->pos()) - m_mousePosition);
        update();
    }
    m_mousePosition = QVector2D(event->pos());

    event->accept();
}

void GLWidget_1_1::wheelEvent(QWheelEvent *event)
{
    /* マウスホイールでカメラの奥行きの移動量を設定 */
    if (event->orientation() == Qt::Vertical) {
        m_camera->depth(event->delta());
        update();
    }

    event->accept();
}

bool GLWidget_1_1::eventFilter(QObject *obj, QEvent *event)
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
            m_angle.setZ(m_angle.x() - 1);
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
