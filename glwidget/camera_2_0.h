#ifndef CAMERA_2_0_H
#define CAMERA_2_0_H

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>

class Camera_2_0
{
public:
    Camera_2_0(QVector2D angle = QVector2D(0,0), float distance = 5) {
        m_angle = angle;
        m_distance = distance;
    }

    QMatrix4x4 matrix()
    {
        QMatrix4x4 matrix;
        matrix.rotate(m_angle.x(), QVector3D(0,1,0));
        matrix.rotate(m_angle.y(), QVector3D(1,0,0));

        return matrix;
    }

    void angleLimit(QVector2D delta)
    {
        const float sensitivity = 1.0f;
        const int xmin =0, xmax=360, ymin = -90, ymax = 90;
        m_angle -= delta * sensitivity;

        // x rotation range
        if (m_angle.x() > xmax)
        {
            m_angle.setX(xmin);
        }
        else if (m_angle.x() < xmin)
        {
            m_angle.setX(xmax);
        }

        // y rotation range
        if (m_angle.y() < ymin)
        {
            m_angle.setY(ymin);
        }
        else if (m_angle.y() > ymax)
        {
            m_angle.setY(ymax);
        }
    }

    void depth(float delta)
    {
        if (delta < 0) {
            m_distance *= 1.1f;
        }
        else if (delta > 0)
        {
            m_distance *= 0.9f;
        }
    }

    float distance()
    {
        return m_distance;
    }

private:
    QVector2D m_angle;
    float m_distance;
};

#endif // CAMERA_2_0_H
