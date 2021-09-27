#ifndef LOADER3D_H
#define LOADER3D_H

// Qt
#include <QVector>
#include <QString>
#include <QFile>
#include <QDataStream>
#include <QRegularExpression>
#include <QtDebug>

// glm
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Loader3D
{
public:
    struct Triangle3D
    {
        glm::vec3 position1, position2, position3;
        glm::vec3 p2Normal, p1Normal, p3Normal;
        glm::vec2 p1TexCoord, p2TexCoord, p3TexCoord;
    };

    Loader3D() {}
    bool objLoad(const QString &fileName, QStringList &comments, QVector<Triangle3D> &triangles)
    {
        comments.clear();
        triangles.clear();

        QFile file(fileName);
        if(!file.exists())
        {
            qWarning() << "File does not exist";
            return false;
        }

        QVector<glm::vec3> v, vn;
        QVector<glm::vec2> vt;

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
                    v.append(glm::vec3(lineParts.at(1).toFloat(),
                                       lineParts.at(2).toFloat(),
                                       lineParts.at(3).toFloat()));
                }
                // UV座標の場合(vt)
                else if(lineParts.at(0).compare("vt", Qt::CaseInsensitive) == 0)
                {
                    vt.append(glm::vec2(lineParts.at(1).toFloat(),
                                        lineParts.at(2).toFloat()));
                }
                // 法線位置の場合(vn)
                else if(lineParts.at(0).compare("vn", Qt::CaseInsensitive) == 0)
                {
                    vn.append(glm::vec3(lineParts.at(1).toFloat(),
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

                    if(vt.count() > 0) // UV座標があるか確認
                    {
                        triangle.p1TexCoord = vt.at(lineParts.at(1).split("/").at(1).toInt() - 1);
                        triangle.p2TexCoord = vt.at(lineParts.at(2).split("/").at(1).toInt() - 1);
                        triangle.p3TexCoord = vt.at(lineParts.at(3).split("/").at(1).toInt() - 1);
                    }

                    // 法線位置を取得
                    triangle.p1Normal = vn.at(lineParts.at(1).split("/").at(2).toInt() - 1);
                    triangle.p2Normal = vn.at(lineParts.at(2).split("/").at(2).toInt() - 1);
                    triangle.p3Normal = vn.at(lineParts.at(3).split("/").at(2).toInt() - 1);

                    triangles.append(triangle);
                }

            }
        }
        file.close();

        return true;
    }

    // .stlのASCIIファイルから三角形をロードする
    bool stlLoadAscii()
    {
        // Not implemented
    }

    // .stlのバイナリファイルから三角形をロードする
    bool stlLoadBinary(const QString &fileName, QString &comment, QVector<Triangle3D> &triangles)
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

            triangles.append(triangle);
        }

        file.close();

        return true;
    }
};

#endif // LOADER3D_H
