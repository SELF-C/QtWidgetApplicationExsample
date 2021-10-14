#ifndef OPENGLFORMAT_H
#define OPENGLFORMAT_H

#include <QOpenGLFunctions>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct Triangle3D
{
    glm::vec3 position1;
    glm::vec3 position2;
    glm::vec3 position3;

    glm::vec3 p2Normal;
    glm::vec3 p1Normal;
    glm::vec3 p3Normal;

    glm::vec2 p1TexCoord;
    glm::vec2 p2TexCoord;
    glm::vec2 p3TexCoord;
};

struct VertexData
{
    glm::vec3 position;
    glm::vec3 normal;
};

#endif // OPENGLFORMAT_H
