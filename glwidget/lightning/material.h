#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLFunctions>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>


class Material : protected QOpenGLFunctions
{
public:
    Material()
    {
        initializeOpenGLFunctions();

        m_ka        = glm::vec3( 0.7f );
        m_kd        = glm::vec3( 0.6f );
        m_ks        = glm::vec3( 0.3f );
        m_shininess = 100;
    }

    void uniform(const GLuint shader)
    {
        GLint ka        = glGetUniformLocation(shader, "Material.Ka");
        GLint kd        = glGetUniformLocation(shader, "Material.Kd");
        GLint ks        = glGetUniformLocation(shader, "Material.Ks");
        GLint shininess = glGetUniformLocation(shader, "Material.Shininess");

        glUniform3fv(ka, 1, glm::value_ptr(m_ka));
        glUniform3fv(kd, 1, glm::value_ptr(m_kd));
        glUniform3fv(ks, 1, glm::value_ptr(m_ks));
        glUniform1f(shininess, m_shininess);
    }

    glm::vec3 ka() const{ return m_ka; }
    void setKa(const glm::vec3 &ka){ m_ka = ka; }

    glm::vec3 kd() const{ return m_kd; }
    void setKd(const glm::vec3 &kd){ m_kd = kd; }

    glm::vec3 ks() const{ return m_ks; }
    void setKs(const glm::vec3 &ks){ m_ks = ks; }

    GLfloat shininess() const{ return m_shininess; }
    void setShininess(const GLfloat &shininess){ m_shininess = shininess; }

private:
    glm::vec3 m_ka;         // アンビエント 反射率
    glm::vec3 m_kd;         // ディフューズ 反射率
    glm::vec3 m_ks;          // スペキュラ 反射率
    GLfloat   m_shininess;  // スペキュラ 輝き係数
};

#endif // MATERIAL_H
