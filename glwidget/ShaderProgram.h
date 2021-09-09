#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLFunctions_2_0>
#include <QDebug>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class ShaderProgram : protected QOpenGLFunctions_2_0
{
    ShaderProgram()
    {
        initializeOpenGLFunctions();
    }

    int readShaderSource(GLuint shaderObj, std::string fileName)
    {
        //ファイルの読み込み
        std::ifstream ifs(fileName);
        if (!ifs)
        {
            std::cout << "error" << std::endl;
            return -1;
        }

        std::string source;
        std::string line;
        while (getline(ifs, line))
        {
            source += line + "\n";
        }

        // シェーダのソースプログラムをシェーダオブジェクトへ読み込む
        const GLchar *sourcePtr = static_cast<const GLchar *>(source.c_str());
        GLint length = static_cast<GLint>(source.length());
        glShaderSource(shaderObj, 1, &sourcePtr, &length);

        return 0;
    }


    GLint addShaderFromSourceFile(std::string vertexFileName, std::string fragmentFileName)
    {
        // シェーダーオブジェクト作成
        GLuint vertShaderObj = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShaderObj = glCreateShader(GL_FRAGMENT_SHADER);
        GLuint shader;

        // シェーダーコンパイルとリンクの結果用変数
        GLint compiled, linked;

        /* シェーダーのソースプログラムの読み込み */
        if (readShaderSource(vertShaderObj, vertexFileName)) return -1;
        if (readShaderSource(fragShaderObj, fragmentFileName)) return -1;

        // シェーダーのコンパイル
        glCompileShader(vertShaderObj);
        glGetShaderiv(vertShaderObj, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE)
        {
            qWarning() << "Compile error in vertex shader.";
            return -1;
        }

        glCompileShader(fragShaderObj);
        glGetShaderiv(fragShaderObj, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_FALSE)
        {
            qWarning() << "Compile error in fragment shader.";
            return -1;
        }

        // シェーダープログラム作成
        shader = glCreateProgram();

        glAttachShader(shader, vertShaderObj);
        glAttachShader(shader, fragShaderObj);

        glDeleteShader(vertShaderObj);
        glDeleteShader(fragShaderObj);

        // リンク
        glLinkProgram(shader);
        glGetProgramiv(shader, GL_LINK_STATUS, &linked);
        if (linked == GL_FALSE)
        {
            qWarning() << "Link error.";
            return -1;
        }

        return static_cast<GLint>(shader);
    }

};

#endif // SHADERPROGRAM_H
