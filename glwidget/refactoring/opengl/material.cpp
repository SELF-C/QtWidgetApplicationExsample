#include "material.h"

Material::Material()
{
    initializeOpenGLFunctions();

    m_material.ka        = glm::vec3( 0.2f );
    m_material.kd        = glm::vec3( 0.8f );
    m_material.ks        = glm::vec3( 0.0f );
    m_material.shininess = 100;
}

Material::Material(glm::vec3 ka, glm::vec3 kd, glm::vec3 ks, GLfloat shininess)
{
    initializeOpenGLFunctions();

    m_material.ka        = ka;
    m_material.kd        = kd;
    m_material.ks        = ks;
    m_material.shininess = shininess;
}

void Material::uniform()
{
    glUniform3fv(m_location.ka, 1, glm::value_ptr(m_material.ka));
    glUniform3fv(m_location.kd, 1, glm::value_ptr(m_material.kd));
    glUniform3fv(m_location.ks, 1, glm::value_ptr(m_material.ks));
    glUniform1f(m_location.shininess, m_material.shininess);
}

void Material::setLocation(const GLuint shader)
{
    m_location.ka        = glGetUniformLocation(shader, "Material.Ka");
    m_location.kd        = glGetUniformLocation(shader, "Material.Kd");
    m_location.ks        = glGetUniformLocation(shader, "Material.Ks");
    m_location.shininess = glGetUniformLocation(shader, "Material.Shininess");
}

// Accessor
glm::vec3 Material::ka() const{ return m_material.ka; }
glm::vec3 Material::kd() const{ return m_material.kd; }
glm::vec3 Material::ks() const{ return m_material.ks; }
GLfloat Material::shininess() const{ return m_material.shininess; }

void Material::setKa(const glm::vec3 &ka){ m_material.ka = ka; }
void Material::setKd(const glm::vec3 &kd){ m_material.kd = kd; }
void Material::setKs(const glm::vec3 &ks){ m_material.ks = ks; }
void Material::setShininess(const GLfloat &shininess){ m_material.shininess = shininess; }
