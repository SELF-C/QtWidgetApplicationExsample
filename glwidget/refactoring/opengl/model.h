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
#include <iostream>
#include <vector>
#include <stdexcept>
#include "loader3d.h"
#include "opengl/format.h"

class Model : protected QOpenGLFunctions_2_0
{
public:
    explicit Model();
    virtual void update();
    virtual void draw(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::mat4 parentMatrix);
    virtual void createBuffer(const std::vector<VertexData> vertices, const std::vector<GLuint> indexes);

    glm::mat4 originMatrix() const;
    Model *child(size_t index) const;
    bool isChild();
    void addChild(Model *child);

    void setOriginMatrix(const glm::mat4 &originMatrix);
    void setLocation(GLuint m_shader);
    void setVertices(std::vector<VertexData> vertices);
    void setTranslate(glm::vec3 position);
    void setTranslate(glm::mat4 translateMatrix);
    void setEulerAngle(glm::vec3 angle);
    void setRotate(glm::mat4 rotateMatrix);
    void setScale(glm::vec3 size);
    void setScale(glm::mat4 scaleMatrix);

protected:
    virtual void initialize();

private:
    struct Location
    {
        GLint modelViewMatrix;
        GLint normalMatrix;
        GLint mvp;
        GLuint vertex;
        GLuint normal;
    };
    Location m_location;

    glm::mat4 m_modelMatrix, m_originMatrix;
    glm::mat4 m_translateMatrix, m_rotateMatrix, m_scaleMatrix;

    std::vector<VertexData> m_vertices;
    std::vector<GLuint> m_indexes;
    std::vector<Model*> m_children;

    GLuint m_vbo;
    GLuint m_ibo;
};

#endif // MODEL_2_0_H
