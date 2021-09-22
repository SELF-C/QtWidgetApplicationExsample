#ifndef MODEL_1_1_H
#define MODEL_1_1_H

// Qt
#include <QOpenGLFunctions_1_1>
#include <QVector>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>

// others
#include "stl.h"

class Model_1_1 : protected QOpenGLFunctions_1_1
{
public:
    Model_1_1();
    void update(QMatrix4x4 viewMatrix);
    void draw();
    void load(const QString filename);
    void setTranslate(QVector3D position);
    void setRotate(QVector3D angle);
    void setScale(QVector3D size);
    void setColor(QVector4D color);

protected:
    void initialize();

private:
    QVector<QVector3D> m_vertices, m_normals;
    QVector4D m_color;
    QVector3D m_position, m_angle, m_size;

};

#endif // MODEL_1_1_H
