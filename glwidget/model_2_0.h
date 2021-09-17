#ifndef MODEL_2_0_H
#define MODEL_2_0_H

#include <QOpenGLFunctions_2_0>
#include <QVector>
#include <QVector3D>
#include <QMatrix4x4>
#include <QString>

#include "stl.h"

class Model_2_0 : protected QOpenGLFunctions_2_0
{
public:
    explicit Model_2_0();
    void update();
    void draw(GLuint m_shader, QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix);
    void load(const QString filename);

protected:
    void initialize();

private:
    QVector<QVector3D> m_vertices;
};

#endif // MODEL_2_0_H
