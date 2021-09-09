#include <QtGui>
#include <QtOpenGL>

#include "glwidget.h"
#include "shaderprogram.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{

}


void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_MULTISAMPLE);
    //glEnable(GL_LIGHTING);
    //glEnable(GL_LIGHT0);
    //static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    //glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::paintGL()
{
    Fps().measurement();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glBegin(GL_TRIANGLES);
        glColor3f(1.0, 0.0, 0.0); glVertex3f(-0.5, -0.5, 0);
        glColor3f(0.0, 1.0, 0.0); glVertex3f( 0.5, -0.5, 0);
        glColor3f(0.0, 0.0, 1.0); glVertex3f( 0.0,  0.5, 0);
    glEnd();


    update();
}

void GLWidget::resizeGL(int w, int h)
{
    //int side = qMin(w, h);
    //glViewport((w - side) / 2, (h - side) / 2, side, side);
    glViewport(0, 0, w, h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    GLdouble aspect = w / h ? h : 1;
    const GLdouble zNear = -30.0, zFar = 10000.0, fov = 60.0;
    perspective(fov, aspect, zNear, zFar);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::perspective(GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
    GLdouble xmin, xmax, ymin, ymax;

    ymax = zNear * tan( fovy * M_PI / 360.0 );
    ymin = -ymax;
    xmin = ymin * aspect;
    xmax = ymax * aspect;

    glFrustum( xmin, xmax, ymin, ymax, zNear, zFar );
}
