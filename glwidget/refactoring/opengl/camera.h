#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec2 angle, float distance);

    glm::mat4 viewMatrix();
    glm::vec3 position() const;

    void angleLimit(glm::vec2 delta);
    void depth(const float delta);
    float distance() const;

    void setEye(const glm::vec4 &eye);
    void setCenter(const glm::vec4 &center);
    void setUp(const glm::vec4 &up);


private:
    glm::vec4 m_eye, m_center, m_up;
    glm::vec3 m_position;
    glm::vec2 m_angle;
    float m_distance;
};

#endif // CAMERA_H
