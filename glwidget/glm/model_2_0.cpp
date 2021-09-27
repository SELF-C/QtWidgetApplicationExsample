#include "model_2_0.h"

Model_2_0::Model_2_0(const QString filename)
{
    initializeOpenGLFunctions();

    initialize();
    createVertexArray(filename);
    createBuffer();
}

void Model_2_0::initialize()
{
    // light
    m_light.position        = glm::vec4( 0.5f, 5.0f, 7.0f, 1.0f );
    m_light.la              = glm::vec3( 0.8f );
    m_light.ld              = glm::vec3( 0.5f );
    m_light.ls              = glm::vec3( 0.5f );

    // material
    m_material.ka           = glm::vec3( 0.7f );
    m_material.kd           = glm::vec3( 0.6f );
    m_material.ks           = glm::vec3( 0.3f );
    m_material.shininess    = 100;

    // transform
    m_position  = glm::vec3(0);
    m_angle     = glm::vec3(0);
    m_size      = glm::vec3(1);
}


void Model_2_0::update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
    m_projectionMatrix = projectionMatrix;
    m_viewMatrix = viewMatrix;

    glm::vec3 radians = glm::radians(m_angle);

    glm::mat4 translateMatrix = translate(glm::mat4(1.0f), m_position);
    glm::mat4 rotateMatrix = glm::eulerAngleZYX(radians.z, radians.y, radians.x);
    glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_size);

    m_modelMatrix = translateMatrix * rotateMatrix * scaleMatrix;
}

void Model_2_0::draw(GLuint m_shader)
{
    // get locations
    setUniformLocations(m_shader);

    // set uniforms
    glm::mat4 modelView = m_viewMatrix * m_modelMatrix;
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(m_modelMatrix));
    glm::mat4 mvp = m_projectionMatrix * m_viewMatrix * m_modelMatrix;

    glUniform4fv(m_uniformLocations[UniformLocation::Position], 1, glm::value_ptr(m_light.position));
    glUniform3fv(m_uniformLocations[UniformLocation::La]      , 1, glm::value_ptr(m_light.la));
    glUniform3fv(m_uniformLocations[UniformLocation::Ld]      , 1, glm::value_ptr(m_light.ld));
    glUniform3fv(m_uniformLocations[UniformLocation::Ls]      , 1, glm::value_ptr(m_light.ls));
    glUniform3fv(m_uniformLocations[UniformLocation::Ka]      , 1, glm::value_ptr(m_material.ka));
    glUniform3fv(m_uniformLocations[UniformLocation::Kd]      , 1, glm::value_ptr(m_material.kd));
    glUniform3fv(m_uniformLocations[UniformLocation::Ks]      , 1, glm::value_ptr(m_material.ks));
    glUniform1f(m_uniformLocations[UniformLocation::Shininess], m_material.shininess);

    glUniformMatrix4fv(m_uniformLocations[UniformLocation::ModelViewMatrix] , 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix3fv(m_uniformLocations[UniformLocation::NormalMatrix]    , 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(m_uniformLocations[UniformLocation::MVP]             , 1, GL_FALSE, glm::value_ptr(mvp));

    // draw
    GLuint vertexLocation = static_cast<GLuint>(m_uniformLocations[UniformLocation::Vertex]);
    GLuint normalLocation = static_cast<GLuint>(m_uniformLocations[UniformLocation::Normal]);
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


void Model_2_0::createVertexArray(const QString filename)
{
    QVector<ThreeDSystems::Stl::Triangle3D> triangles;
    QString commnet;

    // 3Dモデルのファイルを読み込む
    if ( !ThreeDSystems::Stl().parserBinary(filename, commnet, triangles) )
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
        v.second = triangles.at(i).normal;

        index = vertices.indexOf(v);
        if( index == -1 )
        {
            vertices.append(v);
            m_vertices.append(VertexData{ triangles.at(i).position1, triangles.at(i).normal });
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
            m_vertices.append(VertexData{ triangles.at(i).position2, triangles.at(i).normal });
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
            m_vertices.append(VertexData{ triangles.at(i).position3, triangles.at(i).normal });
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
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * static_cast<int>(sizeof(VertexData)), m_vertices.constData(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indexes.size() * static_cast<int>(sizeof(unsigned int)), m_indexes.constData(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model_2_0::setUniformLocations(GLuint m_shader)
{
    if( m_uniformLocations.isEmpty() )
    {
        m_uniformLocations = QVector<int>(UniformLocation::Count);
        m_uniformLocations[UniformLocation::Vertex]             = glGetAttribLocation(m_shader, "VertexPosition");
        m_uniformLocations[UniformLocation::Normal]             = glGetAttribLocation(m_shader, "VertexNormal");
        m_uniformLocations[UniformLocation::Position]           = glGetUniformLocation(m_shader, "Light.Position");
        m_uniformLocations[UniformLocation::La]                 = glGetUniformLocation(m_shader, "Light.La");
        m_uniformLocations[UniformLocation::Ld]                 = glGetUniformLocation(m_shader, "Light.Ld");
        m_uniformLocations[UniformLocation::Ls]                 = glGetUniformLocation(m_shader, "Light.Ls");
        m_uniformLocations[UniformLocation::Ka]                 = glGetUniformLocation(m_shader, "Material.Ka");
        m_uniformLocations[UniformLocation::Kd]                 = glGetUniformLocation(m_shader, "Material.Kd");
        m_uniformLocations[UniformLocation::Ks]                 = glGetUniformLocation(m_shader, "Material.Ks");
        m_uniformLocations[UniformLocation::Shininess]          = glGetUniformLocation(m_shader, "Material.Shininess");
        m_uniformLocations[UniformLocation::ModelViewMatrix]    = glGetUniformLocation(m_shader, "ModelViewMatrix");
        m_uniformLocations[UniformLocation::NormalMatrix]       = glGetUniformLocation(m_shader, "NormalMatrix");
        m_uniformLocations[UniformLocation::MVP]                = glGetUniformLocation(m_shader, "MVP");
    }
}

void Model_2_0::setTranslate(glm::vec3 position)
{
    m_position = position;
}

void Model_2_0::setRotate(glm::vec3 angle)
{
    m_angle = angle;
}

void Model_2_0::setScale(glm::vec3 size)
{
    m_size = size;
}

void Model_2_0::setLight(Light light)
{
    m_light = light;
}

void Model_2_0::setMaterial(Material material)
{
    m_material = material;
}

Light Model_2_0::light()
{
    return m_light;
}

Material Model_2_0::material()
{
    return m_material;
}
