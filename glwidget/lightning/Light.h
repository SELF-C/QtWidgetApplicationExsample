#ifndef LIGHT_H
#define LIGHT_H

#include <QOpenGLFunctions>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>


class Light : protected QOpenGLFunctions
{
public:
    Light()
    {
        initializeOpenGLFunctions();

        m_position  = glm::vec4( 0.5f, 5.0f, 7.0f, 1.0f );
        m_la        = glm::vec3( 0.0f );
        m_ld        = glm::vec3( 1.0f );
        m_ls        = glm::vec3( 1.0f );
    }

    void uniform(const GLuint shader)
    {
        GLint position  = glGetUniformLocation(shader, "Light.Position");
        GLint la        = glGetUniformLocation(shader, "Light.La");
        GLint ld        = glGetUniformLocation(shader, "Light.Ld");
        GLint ls        = glGetUniformLocation(shader, "Light.Ls");

        glUniform4fv(position,  1, glm::value_ptr(m_position));
        glUniform3fv(la,        1, glm::value_ptr(m_la));
        glUniform3fv(ld,        1, glm::value_ptr(m_ld));
        glUniform3fv(ls,        1, glm::value_ptr(m_ls));
    }

    glm::vec4 position() const{ return m_position; }
    void setPosition(const glm::vec4 &position){ m_position = position; }

    glm::vec3 la() const{ return m_la; }
    void setLa(const glm::vec3 &la){ m_la = la; }

    glm::vec3 ld() const{ return m_ld; }
    void setLd(const glm::vec3 &ld){ m_ld = ld; }

    glm::vec3 ls() const{ return m_ls; }
    void setLs(const glm::vec3 &ls){ m_ls = ls; }

private:
    glm::vec4 m_position;  // 視点座標でのライトの位置
    glm::vec3 m_la;        // アンビエント ライト強度
    glm::vec3 m_ld;        // ディフューズ ライト強度
    glm::vec3 m_ls;        // スペキュラ ライト強度
};

#endif // LIGHT_H
