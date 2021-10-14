#include "light.h"

Light::Light()
{
    initializeOpenGLFunctions();

    m_light.position  = glm::vec4( 0.0f, 0.0f, 1.0f, 0.0f );
    m_light.la        = glm::vec3( 0.0f );
    m_light.ld        = glm::vec3( 1.0f );
    m_light.ls        = glm::vec3( 1.0f );
}

Light::Light(glm::vec4 position, glm::vec3 la, glm::vec3 ld, glm::vec3 ls)
{
    initializeOpenGLFunctions();

    m_light.position  = position;
    m_light.la        = la;
    m_light.ld        = ld;
    m_light.ls        = ls;
}

void Light::uniform()
{
    glUniform4fv(m_location.position,  1, glm::value_ptr(m_light.position));
    glUniform3fv(m_location.la,        1, glm::value_ptr(m_light.la));
    glUniform3fv(m_location.ld,        1, glm::value_ptr(m_light.ld));
    glUniform3fv(m_location.ls,        1, glm::value_ptr(m_light.ls));
}

void Light::setLocation(const GLuint shader)
{
    m_location.position  = glGetUniformLocation(shader, "Light.Position");
    m_location.la        = glGetUniformLocation(shader, "Light.La");
    m_location.ld        = glGetUniformLocation(shader, "Light.Ld");
    m_location.ls        = glGetUniformLocation(shader, "Light.Ls");
}

// Accessor
glm::vec4 Light::position() const{ return m_light.position; }
glm::vec3 Light::la() const{ return m_light.la; }
glm::vec3 Light::ld() const{ return m_light.ld; }
glm::vec3 Light::ls() const{ return m_light.ls; }

void Light::setPosition(const glm::vec4 &position){ m_light.position = position; }
void Light::setLa(const glm::vec3 &la){ m_light.la = la; }
void Light::setLd(const glm::vec3 &ld){ m_light.ld = ld; }
void Light::setLs(const glm::vec3 &ls){ m_light.ls = ls; }

