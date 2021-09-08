#ifndef FONT_H
#define FONT_H

#include <QApplication>
#include <QFontDatabase>
#include <QDebug>

class Font
{
public:
    int family(QString path)
    {
        int id = QFontDatabase::addApplicationFont(path);
        if (-1 != id)
        {
            QString family = QFontDatabase::applicationFontFamilies(id).at(0);
            QFont font(family, 12, QFont::Normal, false);
            QApplication::setFont(font);
        }
        else
        {
            qCritical() << QString("couldn't add application font.");
            qCritical() << path;
        }

        return id;
    }

};

#endif // FONT_H
