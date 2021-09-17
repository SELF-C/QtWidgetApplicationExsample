#ifndef MODEL_1_1_H
#define MODEL_1_1_H

// Qt
#include <QOpenGLFunctions_1_1>
#include <QVector>

// glm
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// others
#include "stl.h"

class Model_1_1 : protected QOpenGLFunctions_1_1
{
public:
    Model_1_1();
    void update(glm::mat4 viewMatrix);
    void draw();
    void load(const QString filename);
    void setTranslate(glm::vec3 position);
    void setRotate(glm::vec3 angle);
    void setScale(glm::vec3 size);
    void setColor(glm::vec4 color);

protected:
    void initialize();

private:
    QVector<glm::vec3> m_vertices, m_normals;
    glm::vec4 m_color;
    glm::vec3 m_position, m_angle, m_size;

};

#endif // MODEL_1_1_H
