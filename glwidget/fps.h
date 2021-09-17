#ifndef FPS_H
#define FPS_H

#include <QElapsedTimer>
#include <QDebug>

class Fps
{
public:
    Fps(){}
    void measurement()
    {
        if (!m_fpsCountTimer.isValid()) m_fpsCountTimer.start();
        if (m_fpsCountTimer.elapsed() >= 1000)
        {
            m_fps = m_framesInSecond;
            qDebug() << QString("FPS: %1").arg(m_fps);

            m_framesInSecond = 0;
            m_fpsCountTimer.restart();
        }
        m_framesInSecond++;
    }
private:
    QElapsedTimer m_fpsCountTimer;
    int m_framesInSecond = 0;
    int m_fps = 0;
};
#endif // FPS_H
