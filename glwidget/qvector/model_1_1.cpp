#include "model_1_1.h"

Model_1_1::Model_1_1()
{
    initializeOpenGLFunctions();
    initialize();
}

void Model_1_1::initialize()
{
    m_color     = QVector4D(1, 1, 1, 1);
    m_position  = QVector3D(0, 0, 0);
    m_angle     = QVector3D(0, 0, 0);
    m_size      = QVector3D(1, 1, 1);
}

void Model_1_1::update(QMatrix4x4 viewMatrix)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();

    modelMatrix.translate(m_position);
    modelMatrix.rotate(m_angle.x(), 0, 0, 1);
    modelMatrix.rotate(m_angle.y(), 0, 1, 0);
    modelMatrix.rotate(m_angle.x(), 1, 0, 0);
    modelMatrix.scale(m_size);

    QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    glLoadMatrixf(modelViewMatrix.constData());
}

void Model_1_1::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    float color[] = {1, 1, 1, 1};
    glColor4fv(color);

    glVertexPointer(3 , GL_FLOAT , 0 , m_vertices.constData());
    glNormalPointer(GL_FLOAT, 0, m_normals.constData());

    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());

    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
}

void Model_1_1::load(const QString filename)
{
    QVector<ThreeDSystems::Stl::Triangle3D> triangles;
    QString commnet;

    if ( !ThreeDSystems::Stl().parserBinary(filename, commnet, triangles) )
    {
        qCritical() << "Can't Open File.";
        return;
    }

    m_vertices.clear();
    m_normals.clear();
    for(int i = 0; i < triangles.count(); i++)
    {
        m_vertices.append(triangles.at(i).position1);
        m_vertices.append(triangles.at(i).position2);
        m_vertices.append(triangles.at(i).position3);

        m_normals.append(triangles.at(i).normal);
        m_normals.append(triangles.at(i).normal);
        m_normals.append(triangles.at(i).normal);
    }
}

void Model_1_1::setTranslate(QVector3D position)
{
    m_position = position;
}

void Model_1_1::setRotate(QVector3D angle)
{
    m_angle = angle;
}

void Model_1_1::setScale(QVector3D size)
{
    m_size = size;
}

void Model_1_1::setColor(QVector4D color)
{
    m_color = color;
}
