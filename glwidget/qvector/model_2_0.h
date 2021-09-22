#ifndef MODEL_2_0_H
#define MODEL_2_0_H

#include <QOpenGLFunctions_2_0>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QString>

#include "stl.h"

class Model_2_0 : protected QOpenGLFunctions_2_0
{
public:
    explicit Model_2_0(const QString filename);
    void update(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix);
    void draw(GLuint m_shader);
    void setTranslate(QVector3D position);
    void setRotate(QVector3D angle);
    void setScale(QVector3D size);
    void setLightPosition(QVector4D v);

protected:
    void initialize();

private:
    struct Location
    {
        int lightPosition;
        int kd;
        int ld;
        int modelViewMatrix;
        int normalMatrix;
        int mvp;
        GLuint vertex;
        GLuint normal;
    };

    struct Light
    {
        QVector4D lightPosition;    // 視点座標でのライトの位置
        QVector3D ld;               // アンビエント ライト強度
        QVector3D kd;               // ディフューズ 反射率
    };
    Light m_light;


    QMatrix4x4 m_projectionMatrix, m_viewMatrix, m_modelMatrix;
    QVector<QVector3D> m_vertices, m_normals;
    QVector<GLuint> m_indexes;
    QVector3D m_position, m_angle, m_size;

    GLuint m_vbo[2];
    GLuint m_ibo;

    void load(const QString filename);
    void createBuffer();
    bool findVertex(QVector3D vertice, QVector3D normal);

};

#endif // MODEL_2_0_H
