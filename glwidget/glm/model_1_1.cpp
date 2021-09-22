#include "model_1_1.h"

Model_1_1::Model_1_1()
{
    initializeOpenGLFunctions();
    initialize();
}

void Model_1_1::initialize()
{
    m_color     = glm::vec4(1);
    m_position  = glm::vec3(0);
    m_angle     = glm::vec3(0);
    m_size      = glm::vec3(1);
}

void Model_1_1::update(glm::mat4 viewMatrix)
{
    glm::vec3 radians = glm::radians(m_angle);

    glm::mat4 translateMatrix = translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotateMatrix = glm::eulerAngleZYX(radians.z, radians.y, radians.x);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_size);

    glm::mat4 modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;

    glm::mat4 modelViewMatrix = viewMatrix * modelMatrix;
    glLoadMatrixf(glm::value_ptr(modelViewMatrix));
}

void Model_1_1::draw()
{
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);

    glColor4fv(glm::value_ptr(m_color));

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

void Model_1_1::setTranslate(glm::vec3 position)
{
    m_position = position;
}

void Model_1_1::setRotate(glm::vec3 angle)
{
    m_angle = angle;
}

void Model_1_1::setScale(glm::vec3 size)
{
    m_size = size;
}

void Model_1_1::setColor(glm::vec4 color)
{
    m_color = color;
}
