#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions_2_0>
#include <QDebug>
#include <QFile>
#include <QTextStream>

class ShaderProgram : protected QOpenGLFunctions_2_0
{
public:
    ShaderProgram();
    GLuint addShaderFromSourceFile(const QString vertexFileName, const QString fragmentFileName);

private:
    int readShaderSource(GLuint shaderObj, const QString fileName);
    int compileSourceCode(GLuint shader);
};

#endif // SHADERPROGRAM_H
