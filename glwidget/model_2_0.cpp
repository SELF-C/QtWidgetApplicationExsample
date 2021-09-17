#include "model_2_0.h"

Model_2_0::Model_2_0()
{
    initializeOpenGLFunctions();
}

void Model_2_0::initialize()
{

}

void Model_2_0::update()
{

}

void Model_2_0::draw(GLuint m_shader, QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix)
{
    QMatrix4x4 modelMatrix;
    modelMatrix.setToIdentity();
    modelMatrix.translate(0, 0, 0);

    QMatrix4x4 mvp =  projectionMatrix * viewMatrix * modelMatrix;

    auto mvpLocation = glGetUniformLocation(m_shader, "MVP");
    glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, mvp.constData());

    GLfloat color[] = {1,0,0};
    auto location = glGetUniformLocation(m_shader, "Color");
    glUniform3fv(location, 1, color);

    GLuint vertexLocation = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexPosition"));
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, 0, m_vertices.constData());
    glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
}


void Model_2_0::load(const QString filename)
{
    QVector<ThreeDSystems::Stl::Triangle3D> triangles;
    QString commnet;

    // objファイルの読み込み
    if ( !ThreeDSystems::Stl().parserBinary(filename, commnet, triangles) )
    {
        qCritical() << "Can't Open File.";
        return;
    }


    m_vertices.clear();

    for(int i = 0; i < triangles.count(); i++)
    {
        m_vertices.append(triangles.at(i).position1);
        m_vertices.append(triangles.at(i).position2);
        m_vertices.append(triangles.at(i).position3);
    }

}
