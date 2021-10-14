#include "camera.h"

Camera::Camera()
{
    m_angle = glm::vec2(0,0);
    m_distance = 5;
}

Camera::Camera(glm::vec2 angle, float distance)
{
    m_angle = angle;
    m_distance = distance;
}

glm::mat4 Camera::viewMatrix()
{
    glm::mat4 matrix = glm::mat4(1.0f);
    glm::vec2 radians = glm::radians(m_angle);
    matrix = glm::rotate(matrix, radians.x, glm::vec3(0.0f, 1.0f, 0.0f));
    matrix = glm::rotate(matrix, radians.y, glm::vec3(1.0f, 0.0f, 0.0f));

    glm::vec3 eye    = matrix * m_eye;    // 仮想3Dカメラが配置されているポイント
    glm::vec3 center = matrix * m_center; // カメラが注視するポイント（シーンの中心）
    glm::vec3 up     = matrix * m_up;     // 3Dワールドの上方向を定義

    m_position = eye;
    return glm::lookAt(eye, center, up);
}

void Camera::angleLimit(glm::vec2 delta)
{
    const float sensitivity = 1.0f;
    const int xmin =0, xmax=360, ymin = -90, ymax = 90;
    m_angle -= delta * sensitivity;

    // x rotation range
    if (m_angle.x > xmax)
    {
        m_angle.x = xmin;
    }
    else if (m_angle.x < xmin)
    {
        m_angle.x = xmax;
    }

    // y rotation range
    if (m_angle.y < ymin)
    {
        m_angle.y = ymin;
    }
    else if (m_angle.y > ymax)
    {
        m_angle.y = ymax;
    }
}

void Camera::depth(const float delta)
{
    if (delta < 0) {
        m_distance *= 1.1f;
    }
    else if (delta > 0)
    {
        m_distance *= 0.9f;
    }
}

float Camera::distance() const
{
    return m_distance;
}

glm::vec3 Camera::position() const
{
    return m_position;
}

void Camera::setEye(const glm::vec4 &eye)
{
    m_eye = eye;
}

void Camera::setCenter(const glm::vec4 &center)
{
    m_center = center;
}

void Camera::setUp(const glm::vec4 &up)
{
    m_up = up;
}
