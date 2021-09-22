#include "model_2_0.h"
#include <QDebug>
#include <QQuaternion>
Model_2_0::Model_2_0(const QString filename)
{
    initializeOpenGLFunctions();

    initialize();
    load(filename);
    createBuffer();
}

void Model_2_0::initialize()
{
    m_light.lightPosition = QVector4D(0.5f, 5.0f, 7.0f, 1.0f );
    m_light.ld = QVector3D(1.0f, 1.0f, 1.0f);
    m_light.kd = QVector3D(0.9f, 0.9f, 0.9f);

    m_position = QVector3D(0, 0, 0);
    m_angle = QVector3D(0, 0, 0);
    m_size = QVector3D(1, 1, 1);
}


void Model_2_0::update(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_viewMatrix = viewMatrix;


    auto rotate = QQuaternion::fromEulerAngles(m_angle);

    m_modelMatrix.setToIdentity();
    m_modelMatrix.translate(m_position);
    m_modelMatrix.rotate(rotate);
    m_modelMatrix.scale(m_size);

}

void Model_2_0::draw(GLuint m_shader)
{
    // get locations
    Location location;
    location.vertex = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexPosition"));
    location.normal = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexNormal"));

    location.lightPosition      = glGetUniformLocation(m_shader, "LightPosition");
    location.kd                 = glGetUniformLocation(m_shader, "Kd");
    location.ld                 = glGetUniformLocation(m_shader, "Ld");
    location.modelViewMatrix    = glGetUniformLocation(m_shader, "ModelViewMatrix");
    location.normalMatrix       = glGetUniformLocation(m_shader, "NormalMatrix");
    location.mvp                = glGetUniformLocation(m_shader, "MVP");


    // set uniforms
    QMatrix4x4 modelView = m_viewMatrix * m_modelMatrix;
    QMatrix3x3 normalMatrix = m_modelMatrix.normalMatrix();
    QMatrix4x4 mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

    glUniform4fv(location.lightPosition     , 1, &m_light.lightPosition[0]);
    glUniform3fv(location.kd                , 1, &m_light.kd[0]);
    glUniform3fv(location.ld                , 1, &m_light.ld[0]);
    glUniformMatrix4fv(location.modelViewMatrix , 1, GL_FALSE, modelView.constData());
    glUniformMatrix3fv(location.normalMatrix    , 1, GL_FALSE, normalMatrix.constData());
    glUniformMatrix4fv(location.mvp             , 1, GL_FALSE, mvp.constData());

    // draw
    glEnableVertexAttribArray(location.vertex);
    glEnableVertexAttribArray(location.normal);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glVertexAttribPointer(location.vertex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glVertexAttribPointer(location.normal, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, nullptr);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glDisableVertexAttribArray(location.vertex);
    glDisableVertexAttribArray(location.normal);
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
    m_indexes.clear();

    QVector<QPair<QVector3D, QVector3D>> vertices;

    int index = 0;
    for(int i = 0; i < triangles.count(); i++)
    {
        QPair<QVector3D, QVector3D> v;

        v.first = triangles.at(i).position1;
        v.second = triangles.at(i).normal;

        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(triangles.at(i).position1);
            m_normals.append(triangles.at(i).normal);
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position2;
        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(triangles.at(i).position2);
            m_normals.append(triangles.at(i).normal);
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position3;
        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(triangles.at(i).position3);
            m_normals.append(triangles.at(i).normal);
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }

    }



}

void Model_2_0::createBuffer()
{
    glGenBuffers(2, m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * static_cast<int>(sizeof(QVector3D)), m_vertices.constData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, m_normals.size() * static_cast<int>(sizeof(QVector3D)), m_normals.constData(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * static_cast<int>(sizeof(unsigned int)), m_indexes.constData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model_2_0::setTranslate(QVector3D position)
{
    m_position = position;
}

void Model_2_0::setRotate(QVector3D angle)
{
    m_angle = angle;
}

void Model_2_0::setScale(QVector3D size)
{
    m_size = size;
}

void Model_2_0::setLightPosition(QVector4D v)
{
    m_light.lightPosition = v;
}
