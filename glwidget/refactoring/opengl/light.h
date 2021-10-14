#ifndef LIGHT_H
#define LIGHT_H

#include <QOpenGLFunctions>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>

class Light : protected QOpenGLFunctions
{
public:
    Light();
    Light(glm::vec4 ,glm::vec3, glm::vec3, glm::vec3);

    void uniform();
    void setLocation(const GLuint shader);

    // Getter
    glm::vec4 position() const;
    glm::vec3 la() const;
    glm::vec3 ld() const;
    glm::vec3 ls() const;

    // Setter
    void setPosition(const glm::vec4 &position);
    void setLa(const glm::vec3 &la);
    void setLd(const glm::vec3 &ld);
    void setLs(const glm::vec3 &ls);


private:
    struct Location
    {
        GLint position;
        GLint la;
        GLint ld;
        GLint ls;
    };
    Location m_location;

    struct LightInfo
    {
        glm::vec4 position;  // 視点座標でのライトの位置
        glm::vec3 la;        // アンビエント ライト強度
        glm::vec3 ld;        // ディフューズ ライト強度
        glm::vec3 ls;        // スペキュラ ライト強度
    };
    LightInfo m_light;


};

#endif // LIGHT_H
