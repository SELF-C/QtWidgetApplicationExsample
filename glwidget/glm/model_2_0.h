#ifndef MODEL_2_0_H
#define MODEL_2_0_H

// Qt
#include <QOpenGLFunctions_2_0>
#include <QVector>
#include <QString>

// glm
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

// others
#include "stl.h"

struct Light {
    glm::vec4 position;  // 視点座標でのライトの位置
    glm::vec3 la;        // アンビエント ライト強度
    glm::vec3 ld;        // ディフューズ ライト強度
    glm::vec3 ls;        // スペキュラ ライト強度
};

struct Material {
    glm::vec3 ka;       // アンビエント 反射率
    glm::vec3 kd;       // ディフューズ 反射率
    glm::vec3 ks;       // スペキュラ 反射率
    GLfloat shininess;  // スペキュラ 輝き係数
};

class Model_2_0 : protected QOpenGLFunctions_2_0
{
public:
    explicit Model_2_0(const QString filename);
    void update(glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
    void draw(GLuint m_shader);
    void setTranslate(glm::vec3 position);
    void setRotate(glm::vec3 angle);
    void setScale(glm::vec3 size);
    void setLight(Light light);
    void setMaterial(Material material);


    Light light();
    Material material();

protected:
    void initialize();

private:
    enum UniformLocation
    {
        // Light
        Position, La, Ld, Ls,
        // Material
        Ka, Kd, Ks, Shininess,
        // matrix
        ModelViewMatrix, NormalMatrix, MVP,
        // end
        Count
    };
    QVector<int> m_uniformLocations;

    Light m_light;
    Material m_material;

    glm::mat4 m_projectionMatrix, m_viewMatrix, m_modelMatrix;
    glm::vec3 m_position, m_angle, m_size;
    QVector<glm::vec3> m_vertices, m_normals;
    QVector<GLuint> m_indexes;

    GLuint m_vbo[2];
    GLuint m_ibo;

    void createVertexArray(const QString filename);
    void createBuffer();
    void setUniformLocations(GLuint m_shader);

};

#endif // MODEL_2_0_H
