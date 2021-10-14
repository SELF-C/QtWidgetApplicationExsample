#include "model.h"

Model::Model()
{
    initializeOpenGLFunctions();

    initialize();
}

void Model::initialize()
{
    m_originMatrix = glm::mat4(1.0);

    // transformMatrix
    m_translateMatrix   = glm::mat4(1.0);
    m_rotateMatrix      = glm::mat4(1.0);
    m_scaleMatrix       = glm::mat4(1.0);
}

glm::mat4 Model::originMatrix() const
{
    return m_originMatrix;
}

void Model::update()
{
    m_modelMatrix = m_originMatrix * (m_translateMatrix * m_rotateMatrix * m_scaleMatrix);
}

void Model::draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 parentMatrix)
{
    // set uniforms
    glm::mat4 modelMatrix = parentMatrix * m_modelMatrix;
    glm::mat4 modelView = viewMatrix * modelMatrix;
    glm::mat3 normalMatrix = glm::inverseTranspose(glm::mat3(modelMatrix));
    glm::mat4 mvp = projectionMatrix * viewMatrix * modelMatrix;

    glUniformMatrix4fv(m_location.modelViewMatrix , 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix3fv(m_location.normalMatrix , 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(m_location.mvp , 1, GL_FALSE, glm::value_ptr(mvp));

    // draw
    glEnableVertexAttribArray(m_location.vertex);
    glEnableVertexAttribArray(m_location.normal);

    int offset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glVertexAttribPointer(m_location.vertex, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), nullptr);

    offset += sizeof(glm::vec3);
    glVertexAttribPointer(m_location.normal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), reinterpret_cast<GLvoid*>(offset));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glDrawElements(GL_TRIANGLES, static_cast<int>(m_indexes.size()), GL_UNSIGNED_INT, nullptr);

    foreach (auto child, m_children) {
        child->draw(projectionMatrix, viewMatrix, modelMatrix);
    }
}


Model *Model::child(size_t index) const
{
    Model *child;
    try {
        child = m_children.at(index);
    } catch (std::out_of_range e) {
        std::cout << "some_exception:" << e.what() << std::endl;
        child = nullptr;
    }

    return child;
}

bool Model::isChild()
{
    if ( m_children.size() == 0 ) return false;
    return true;
}

void Model::addChild(Model *child)
{
    m_children.push_back(child);
}

void Model::createBuffer(const std::vector<VertexData> vertices, const std::vector<GLuint> indexes)
{
    m_vertices = vertices;
    m_indexes = indexes;

    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<int>(m_vertices.size() * sizeof(VertexData)), m_vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<int>(m_indexes.size() * sizeof(unsigned int)), m_indexes.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::setOriginMatrix(const glm::mat4 &originMatrix)
{
    m_originMatrix = originMatrix;
}

void Model::setLocation(GLuint m_shader)
{
    m_location.vertex = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexPosition"));
    m_location.normal = static_cast<GLuint>(glGetAttribLocation(m_shader, "VertexNormal"));

    m_location.modelViewMatrix  = glGetUniformLocation(m_shader, "ModelViewMatrix");
    m_location.normalMatrix = glGetUniformLocation(m_shader, "NormalMatrix");
    m_location.mvp = glGetUniformLocation(m_shader, "MVP");
}


void Model::setVertices(std::vector<VertexData> vertices)
{
    m_vertices = vertices;
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
