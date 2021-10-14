#include "loader3d.h"

// .opjファイルから三角形をロードする
Loader3D::Loader3D() {}

bool Loader3D::objLoad(const QString &fileName, QStringList &comments, std::vector<Triangle3D> &triangles)
{
    comments.clear();
    triangles.clear();

    QFile file(fileName);
    if(!file.exists())
    {
        qWarning() << "File does not exist";
        return false;
    }

    std::vector<glm::vec3> v, vn;
    std::vector<glm::vec2> vt;

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << "Can't open file";
        return false;
    }

    while(!file.atEnd())
    {
        QString line = file.readLine().trimmed();
        QStringList lineParts = line.split(QRegularExpression("\\s+"));
        if(lineParts.count() > 0)
        {

            // コメントなら
            if(lineParts.at(0).compare("#", Qt::CaseInsensitive) == 0)
            {
                comments.append(line.remove(0, 1).trimmed());
            }
            // 頂点位置の場合(v)
            else if(lineParts.at(0).compare("v", Qt::CaseInsensitive) == 0)
            {
                v.push_back(glm::vec3(lineParts.at(1).toFloat(),
                                   lineParts.at(2).toFloat(),
                                   lineParts.at(3).toFloat()));
            }
            // UV座標の場合(vt)
            else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
            {
                vt.push_back(glm::vec2(lineParts.at(1).toFloat(),
                                    lineParts.at(2).toFloat()));
            }
            // 法線位置の場合(vn)
            else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
            {
                vn.push_back(glm::vec3(lineParts.at(1).toFloat(),
                                    lineParts.at(2).toFloat(),
                                    lineParts.at(3).toFloat()));
            }
            // 面データの場合。すべて三角形であるとする。
            else if(lineParts.at(0).compare("f", Qt::CaseInsensitive) == 0)
            {
                Triangle3D triangle;


                // 頂点座標を取得
                triangle.position1 = v.at(lineParts.at(1).split("/").at(0).toInt() - 1);
                triangle.position2 = v.at(lineParts.at(2).split("/").at(0).toInt() - 1);
                triangle.position3 = v.at(lineParts.at(3).split("/").at(0).toInt() - 1);

                if(vt.size() > 0) // UV座標があるか確認
                {
                    triangle.p1TexCoord = vt.at(lineParts.at(1).split("/").at(1).toInt() - 1);
                    triangle.p2TexCoord = vt.at(lineParts.at(2).split("/").at(1).toInt() - 1);
                    triangle.p3TexCoord = vt.at(lineParts.at(3).split("/").at(1).toInt() - 1);
                }

                // 法線位置を取得
                triangle.p1Normal = vn.at(lineParts.at(1).split("/").at(2).toInt() - 1);
                triangle.p2Normal = vn.at(lineParts.at(2).split("/").at(2).toInt() - 1);
                triangle.p3Normal = vn.at(lineParts.at(3).split("/").at(2).toInt() - 1);

                triangles.push_back(triangle);
            }

        }
    }
    file.close();

    return true;
}

// .stlのASCIIファイルから三角形をロードする
bool Loader3D::stlLoadAscii()
{
    // Not implemented
    return true;
}

// .stlのバイナリファイルから三角形をロードする
bool Loader3D::stlLoadBinary(const QString &fileName, QString &comment, std::vector<Triangle3D> &triangles)
{
    QFile file(fileName);

    // ファイルの存在確認
    if(!file.exists())
    {
        qWarning() << "File does not exist";
        return false;
    }

    // ファイルオープンの成否
    if( !file.open(QIODevice::ReadOnly) )
    {
        qWarning() << "Can't open file";
        return false;
    }

    QDataStream data(&file);
    data.setByteOrder(QDataStream::LittleEndian);
    data.setFloatingPointPrecision(QDataStream::SinglePrecision);

    // コメントを取得
    QByteArray b = QByteArray();
    b.resize(80);
    data.readRawData(b.data(), 80);
    comment = QString::fromUtf8(b).trimmed();

    // 三角形の数を取得
    uint32_t tri_count;
    data >> tri_count;

    // ファイルが適切なサイズであることを確認
    // byte 0 ～ 79      : コメントの記述
    // byte 80 ～ 83     : 三角形の総数（N）
    // byte 84 + n × 50  : ファイルサイズ
    if (file.size() != 84 + tri_count * 50)
    {
        qWarning() << "File is NULL";
    }

    for (int i = 0; i < static_cast<int>(tri_count); i++) {
        Triangle3D triangle;

        // 法線位置を取得
        float nx, ny, nz;
        data >> nx >> ny >> nz;
        triangle.p1Normal = glm::vec3(nx, ny, nz);
        triangle.p2Normal = glm::vec3(nx, ny, nz);
        triangle.p3Normal = glm::vec3(nx, ny, nz);

        // 頂点位置を取得
        float x, y, z;
        data >> x >> y >> z;
        triangle.position1 = glm::vec3(x, y, z);

        data >> x >> y >> z;
        triangle.position2= glm::vec3(x, y, z);

        data >> x >> y >> z;
        triangle.position3 = glm::vec3(x, y, z);

        // 未使用データをスキップ
        quint16 unused;
        data >> unused;

        triangles.push_back(triangle);
    }

    file.close();

    return true;
}

void Loader3D::createVertexArray(const std::vector<Triangle3D> triangles, std::vector<VertexData> &vertices, std::vector<GLuint> &indexes)
{
    vertices.clear();
    indexes.clear();

    QVector<QPair<glm::vec3, glm::vec3>> tempVertices;

    int index = 0;
    for(int i = 0; i < triangles.size(); i++)
    {
        QPair<glm::vec3, glm::vec3> v;

        v.first = triangles.at(i).position1;
        v.second = triangles.at(i).p1Normal;

        index = tempVertices.indexOf(v);
        if( index == -1 )
        {
            tempVertices.push_back(v);
            vertices.push_back(VertexData{ triangles.at(i).position1, triangles.at(i).p1Normal });
            indexes.push_back(static_cast<GLuint>(vertices.size() - 1));
        }
        else
        {
            indexes.push_back(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position2;
        v.second = triangles.at(i).p2Normal;
        index = tempVertices.indexOf(v);
        if( index == -1 )
        {
            tempVertices.push_back(v);
            vertices.push_back(VertexData{ triangles.at(i).position2, triangles.at(i).p2Normal });
            indexes.push_back(static_cast<GLuint>(vertices.size() - 1));
        }
        else
        {
            indexes.push_back(static_cast<GLuint>(index));
        }

        v.first = triangles.at(i).position3;
        v.second = triangles.at(i).p3Normal;
        index = tempVertices.indexOf(v);
        if( index == -1 )
        {
            tempVertices.push_back(v);
            vertices.push_back(VertexData{ triangles.at(i).position3, triangles.at(i).p3Normal });
            indexes.push_back(static_cast<GLuint>(vertices.size() - 1));
        }
        else
        {
            indexes.push_back(static_cast<GLuint>(index));
        }
    }
}

void Loader3D::createStlBinaryVertexArray(const QString &fileName, std::vector<VertexData> &vertices, std::vector<GLuint> &indexes)
{
    if( !fileName.isEmpty() )
    {
        std::vector<Triangle3D> triangles;
        QString comment;
        stlLoadBinary(fileName, comment, triangles);

        vertices.clear();
        indexes.clear();
        createVertexArray(triangles, vertices, indexes);
    }
    else
    {
        qDebug() << "model file not found.";
    }
}
