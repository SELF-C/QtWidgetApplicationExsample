#ifndef FLOATINGWIDGET_H
#define FLOATINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

class FloatingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FloatingWidget(QWidget *parent = nullptr) : QWidget(parent){}

protected:
    void mousePressEvent(QMouseEvent* event) override
    {
        m_isDrag = true;
        m_startPos = event->pos();
        QWidget::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        m_isDrag = false;
        QWidget::mouseReleaseEvent(event);
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (m_isDrag)
        {
            this->move(this->mapToParent(event->pos() - m_startPos));
        }
        QWidget::mouseMoveEvent(event);
    }

    void paintEvent(QPaintEvent *) override
    {
        QStyleOption opt;
        opt.init(this);
        QPainter p(this);
        style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    }

private:
    bool m_isDrag = false;
    QPoint m_startPos = QPoint(0,0);
};

#endif // FLOATINGWIDGET_H
