#ifndef MATERIAL_H
#define MATERIAL_H

#include <QOpenGLFunctions>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Material : protected QOpenGLFunctions
{
public:
    Material();
    Material(glm::vec3, glm::vec3, glm::vec3, GLfloat);

    void uniform();
    void setLocation(const GLuint shader);

    // Getter
    glm::vec3 ka() const;
    glm::vec3 kd() const;
    glm::vec3 ks() const;
    GLfloat shininess() const;

    // Setter
    void setKa(const glm::vec3 &ka);
    void setKd(const glm::vec3 &kd);
    void setKs(const glm::vec3 &ks);
    void setShininess(const GLfloat &shininess);

private:
    struct Location
    {
        GLint ka;
        GLint kd;
        GLint ks;
        GLint shininess;
    };
    Location m_location;

    struct MaterialInfo
    {
        glm::vec3 ka;         // アンビエント 反射率
        glm::vec3 kd;         // ディフューズ 反射率
        glm::vec3 ks;          // スペキュラ 反射率
        GLfloat   shininess;  // スペキュラ 輝き係数
    };
    MaterialInfo m_material;

};

#endif // MATERIAL_H
