#include "model_2_0.h"

Model_2_0::Model_2_0()
{
    initializeOpenGLFunctions();
}

void Model_2_0::initialize()
{
    m_light.lightPosition = QVector4D(-25.0f, 25.0f, 25.0f, 1.0f);
    m_light.ld = QVector3D(1.0f, 1.0f, 1.0f);
    m_light.kd = QVector3D(0.8f, 0.8f, 0.8f);
}

void Model_2_0::update(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_viewMatrix = viewMatrix;

    m_modelMatrix.setToIdentity();
//    m_modelMatrix.translate(m_position);
//    m_modelMatrix.rotate(m_angle.z(), QVector3D(0,0,1));
//    m_modelMatrix.rotate(m_angle.y(), QVector3D(0,1,0));
//    m_modelMatrix.rotate(m_angle.x(), QVector3D(1,0,0));
//    m_modelMatrix.scale(m_size);

}

void Model_2_0::draw(GLuint m_shader)
{
    Location location;
    location.vertex = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexPosition"));
    location.normal = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexNormal"));

    location.lightPosition      = glGetUniformLocation(m_shader, "LightPosition");
    location.kd                 = glGetUniformLocation(m_shader, "Kd");
    location.ld                 = glGetUniformLocation(m_shader, "Ld");
    location.modelViewMatrix    = glGetUniformLocation(m_shader, "ModelViewMatrix");
    location.normalMatrix       = glGetUniformLocation(m_shader, "NormalMatrix");
    location.mvp                = glGetUniformLocation(m_shader, "MVP");


    QMatrix4x4 modelView = m_viewMatrix * m_modelMatrix;
    QMatrix3x3 normalMatrix = m_modelMatrix.normalMatrix();
    QMatrix4x4 mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

    float li[] = { m_light.lightPosition.x(), m_light.lightPosition.y(), m_light.lightPosition.z(), m_light.lightPosition.w()};
    float kd[] = { m_light.kd.x(), m_light.kd.y(), m_light.kd.z() };
    float ld[] = { m_light.ld.x(), m_light.ld.y(), m_light.ld.z() };



    glUniform4f(location.lightPosition     , -25.0f, 25.0f, 25.0f, 1.0f);
    glUniform3f(location.kd                , 1.0f, 1.0f, 1.0f);
    glUniform3f(location.ld                , 0.8f, 0.8f, 0.8f);
    glUniformMatrix4fv(location.modelViewMatrix , 1, GL_FALSE, modelView.constData());
    glUniformMatrix3fv(location.normalMatrix    , 1, GL_FALSE, normalMatrix.constData());
    glUniformMatrix4fv(location.mvp             , 1, GL_FALSE, mvp.constData());

    glEnableVertexAttribArray(location.vertex);
    glVertexAttribPointer(location.vertex, 3, GL_FLOAT, GL_FALSE, 0, m_vertices.constData());

    glEnableVertexAttribArray(location.normal);
    glVertexAttribPointer(location.normal, 3, GL_FLOAT, GL_FALSE, 0, m_normals.constData());

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
    m_normals.clear();
    for(int i = 0; i < triangles.count(); i++)
    {
        m_vertices << triangles.at(i).position1
                   << triangles.at(i).position2
                   << triangles.at(i).position3;

        m_normals << triangles.at(i).normal
                  << triangles.at(i).normal
                  << triangles.at(i).normal;
    }

}

void Model_2_0::setTranslate(QVector3D position)
{

}

void Model_2_0::setRotate(QVector3D angle)
{

}

void Model_2_0::setScale(QVector3D size)
{

}
