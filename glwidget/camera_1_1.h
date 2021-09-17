#ifndef CAMERA_1_1_H
#define CAMERA_1_1_H

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>

class Camera_1_1
{
public:
    Camera_1_1(glm::vec2 angle = glm::vec2(0,0), float distance = 5) {
        m_angle = angle;
        m_distance = distance;
    }

    glm::mat4 matrix()
    {
        glm::mat4 matrix = glm::mat4(1.0f);
        glm::vec2 radians = glm::radians(m_angle);
        matrix = glm::rotate(matrix, radians.x, glm::vec3(0.0f, 1.0f, 0.0f));
        matrix = glm::rotate(matrix, radians.y, glm::vec3(1.0f, 0.0f, 0.0f));

        return matrix;
    }

    void angleLimit(glm::vec2 delta)
    {
        m_angle -= delta;

        // x rotation range
        if (m_angle.x > 360)
        {
            m_angle.x = 0;
        }
        else if (m_angle.x < 0)
        {
            m_angle.x =  360;
        }

        // y rotation range
        if (m_angle.y < -90)
        {
            m_angle.y = -90;
        }
        else if (m_angle.y > 90)
        {
            m_angle.y = 90;
        }
    }

    void depth(float delta)
    {
        if (delta < 0) {
            m_distance *= 1.1f;
        }
        else if (delta > 0)
        {
            m_distance *= 0.9f;
        }
    }

    float distance()
    {
        return m_distance;
    }

private:
    glm::vec2 m_angle;
    float m_distance;
};

#endif // CAMERA_1_1_H
