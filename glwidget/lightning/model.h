#ifndef MODEL_2_0_H
#define MODEL_2_0_H

// Qt
#include <QOpenGLFunctions_2_0>
#include <QVector>
#include <QString>

// glm
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// others
#include "loader3d.h"

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
};

class Model : protected QOpenGLFunctions_2_0
{
public:
    explicit Model(const QString filename);
    virtual void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
    virtual void draw(GLuint m_shader);
    void setVertices();
    void setTranslate(glm::vec3 position);
    void setTranslate(glm::mat4 translateMatrix);
    void setEulerAngle(glm::vec3 angle);
    void setRotate(glm::mat4 rotateMatrix);
    void setScale(glm::vec3 size);
    void setScale(glm::mat4 scaleMatrix);

protected:
    virtual void initialize();

private:
    enum UniformLocation
    {
        // Vertex
        Vertex, Normal,
        // Light
        Position, La, Ld, Ls,
        // Material
        Ka, Kd, Ks, Shininess,
        // matrix
        ModelViewMatrix, NormalMatrix, MVP,
        // end
        Count
    };
    QVector<int> m_uniformLocations;

    glm::mat4 m_projectionMatrix, m_viewMatrix, m_modelMatrix;
    glm::mat4 m_translateMatrix, m_rotateMatrix, m_scaleMatrix;
    glm::vec3 m_position, m_angle, m_size;

    QVector<VertexData> m_vertices;
    QVector<GLuint> m_indexes;

    GLuint m_vbo;
    GLuint m_ibo;

    void createVertexArray(const QString filename);
    void createBuffer();

};

#endif // MODEL_2_0_H
