#ifndef STL_H
#define STL_H

#include <QString>
#include <QFile>
#include <QDataStream>
#include <QVector>
#include <QDebug>

// glm
#include <glm/vec3.hpp>

namespace ThreeDSystems
{
    class Stl
    {
    public:
        struct Triangle3D
        {
            glm::vec3 position1, position2, position3;
            glm::vec3 normal;
        };

        Stl(){}

        // .stlのASCIIファイルから三角形をロードする
        bool parserAscii()
        {
            // Not implemented
        }

        // .stlのバイナリファイルから三角形をロードする
        bool parserBinary(const QString &fileName, QString &comment, QVector<Triangle3D> &triangles)
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
                triangle.normal = glm::vec3(nx, ny, nz);

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
}


#endif // STL_H
