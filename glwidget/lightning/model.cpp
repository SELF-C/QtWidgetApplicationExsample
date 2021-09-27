#include "model.h"

Model::Model(const QString filename)
{
    initializeOpenGLFunctions();

    initialize();

    // 3Dモデルを読み込まない場合バッファを作らない
    if( filename.isEmpty() ) return;

    createVertexArray(filename);
    createBuffer();
}

void Model::initialize()
{
    // transformMatrix
    m_translateMatrix   = glm::mat4(1.0);
    m_rotateMatrix      = glm::mat4(1.0);
    m_scaleMatrix       = glm::mat4(1.0);
}

void Model::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_viewMatrix = viewMatrix;
    m_modelMatrix = m_translateMatrix * m_rotateMatrix * m_scaleMatrix;
}

void Model::draw(GLuint m_shader)
{
    // set uniforms
    glm::mat4 modelView = m_viewMatrix * m_modelMatrix;
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));
    glm::mat4 mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

    GLint mvLocation  = glGetUniformLocation(m_shader, "ModelViewMatrix");
    GLint nmLocation  = glGetUniformLocation(m_shader, "NormalMatrix");
    GLint mvpLocation = glGetUniformLocation(m_shader, "MVP");

    glUniformMatrix4fv(mvLocation   , 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix3fv(nmLocation   , 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(mvpLocation  , 1, GL_FALSE, glm::value_ptr(mvp));

    // draw
    GLuint vertexLocation = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexPosition"));
    GLuint normalLocation = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexNormal"));
    glEnableVertexAttribArray(vertexLocation);
    glEnableVertexAttribArray(normalLocation);

    int offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr);

    offset += sizeof(glm::vec3);
    glVertexAttribPointer(normalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid*>(offset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDrawElements(GL_TRIANGLES, m_indexes.size(), GL_UNSIGNED_INT, nullptr);
}


void Model::createVertexArray(const QString filename)
{
    QVector<Loader3D::Triangle3D> triangles;
    QString commnet;

    // 3Dモデルのファイルを読み込む
    if ( !Loader3D().stlLoadBinary(filename, commnet, triangles) )
    {
        qCritical() << "Can't Open File.";
        return;
    }


    m_vertices.clear();
    m_indexes.clear();

    QVector<QPair<glm::vec3, glm::vec3>> vertices;

    int index = 0;
    for(int i = 0; i < triangles.count(); i++)
    {
        QPair<glm::vec3, glm::vec3> v;

        v.first = triangles.at(i).position1;
        v.second = triangles.at(i).p1Normal;

        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(VertexData{ triangles.at(i).position1, triangles.at(i).p1Normal });
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position2;
        v.second = triangles.at(i).p2Normal;
        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(VertexData{ triangles.at(i).position2, triangles.at(i).p2Normal });
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position3;
        v.second = triangles.at(i).p3Normal;
        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(VertexData{ triangles.at(i).position3, triangles.at(i).p3Normal });
            m_indexes.append(static_cast<GLuint>(m_vertices.size() - 1));
        }
        else
        {
            m_indexes.append(static_cast<GLuint>(index));
        }
    }

}

void Model::createBuffer()
{
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * static_cast<int>(sizeof(VertexData)), m_vertices.constData(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * static_cast<int>(sizeof(unsigned int)), m_indexes.constData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}


void Model::setTranslate(glm::vec3 position)
{
    m_translateMatrix = translate(glm::mat4(1.0f), position);
}

void Model::setTranslate(glm::mat4 translateMatrix)
{
    m_translateMatrix = translateMatrix;
}

void Model::setEulerAngle(glm::vec3 angle)
{
    glm::vec3 radians = glm::radians(angle);
    m_rotateMatrix = glm::eulerAngleZYX(radians.z, radians.y, radians.x);
}

void Model::setRotate(glm::mat4 rotateMatrix)
{
    m_rotateMatrix = rotateMatrix;
}

void Model::setScale(glm::vec3 size)
{
    m_scaleMatrix = glm::scale(glm::mat4(1.0f), size);
}

void Model::setScale(glm::mat4 scaleMatrix)
{
    m_scaleMatrix = scaleMatrix;
}
