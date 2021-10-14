#ifndef LOADER3D_H
#define LOADER3D_H

// Qt
#include <QVector>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QRegularExpression>
#include <QtDebug>

#include "opengl/format.h"

class Loader3D
{
public:
    Loader3D();
    bool objLoad(const QString &fileName, QStringList &comments, std::vector<Triangle3D> &triangles);
    bool stlLoadAscii();
    bool stlLoadBinary(const QString &fileName, QString &comment, std::vector<Triangle3D> &triangles);

    void createVertexArray(const std::vector<Triangle3D> triangles, std::vector<VertexData> &vertices, std::vector<GLuint> &indexes);
    void createStlBinaryVertexArray(const QString &fileName, std::vector<VertexData> &vertices, std::vector<GLuint> &indexes);
};

#endif // LOADER3D_H
