#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions_2_0>
#include <QDebug>
#include <QFile>
#include <QTextStream>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class ShaderProgram : protected QOpenGLFunctions_2_0
{
public:
    ShaderProgram()
    {
        initializeOpenGLFunctions();
    }

    GLuint addShaderFromSourceFile(const QString vertexFileName, const QString fragmentFileName)
    {
        const GLuint errorCode = static_cast<GLuint>(-1);

        // シェーダーオブジェクト作成
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

        /* シェーダーのソースプログラムの読み込み */
        if ( readShaderSource(vertexShader, vertexFileName) == GL_FALSE ) return errorCode;
        if ( readShaderSource(fragmentShader, fragmentFileName) == GL_FALSE ) return errorCode;

        // シェーダーのコンパイル
        if( compileSourceCode(vertexShader) == GL_FALSE )
        {
            qCritical() << QString("Error compiling shader %1").arg(vertexFileName);
            return errorCode;
        }

        if( compileSourceCode(fragmentShader) == GL_FALSE )
        {
            qCritical() << QString("Error compiling shader %1").arg(fragmentFileName);
            return errorCode;
        }


        // シェーダープログラム作成
        GLuint shader;

        shader = glCreateProgram();

        glAttachShader(shader, vertexShader);
        glAttachShader(shader, fragmentShader);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // リンク
        GLint linked;

        glLinkProgram(shader);
        glGetProgramiv(shader, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
        {
            qCritical() << "Link error.";
            return errorCode;
        }

        return shader;
    }

private:

    int readShaderSource(GLuint shaderObj, const QString fileName)
    {
        QFile f(fileName);
        if( !f.open(QFile::ReadOnly | QFile::Text) )
        {
            qCritical() << "Can't Open File.";
            return GL_FALSE;
        }

        QTextStream ts(&f);
        QString src = ts.readAll();
        QByteArray array = src.toLocal8Bit();
        const GLchar *sourcePtr = array.data();

        GLint length = static_cast<GLint>(src.length());
        glShaderSource(shaderObj, 1, &sourcePtr, &length);

        return GL_TRUE;
    }


    int compileSourceCode(GLuint shader)
    {
        GLint compiled;

        glCompileShader(shader);
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (compiled == GL_FALSE)
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &infoLen);

            if(infoLen > 1)
            {
                char *infoLog = new char[static_cast<uint>(infoLen)];
                glGetShaderInfoLog(shader, infoLen, nullptr, infoLog );

                qCritical().noquote() << infoLog;
                delete[] infoLog;
            }

            return GL_FALSE;
        }

        return GL_TRUE;
    }
//    int readShaderSource(GLuint shaderObj, std::string fileName)
//    {
//        //ファイルの読み込み
//        std::ifstream ifs(fileName);
//        if (!ifs)
//        {
//            qWarning() << "error";
//            return -1;
//        }

//        std::string source;
//        std::string line;
//        while (getline(ifs, line))
//        {
//            source += line + "\n";
//        }

//        // シェーダのソースプログラムをシェーダオブジェクトへ読み込む
//        const GLchar *sourcePtr = static_cast<const GLchar *>(source.c_str());
//        GLint length = static_cast<GLint>(source.length());
//        glShaderSource(shaderObj, 1, &sourcePtr, &length);

//        return 0;
//    }
};

#endif // SHADERPROGRAM_H
