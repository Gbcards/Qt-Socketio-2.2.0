#ifndef YEAST_H
#define YEAST_H

#include <QString>
#include <QtMath>
#include <QTimer>
#include <QDateTime>

class Yeast
{
public:
    Yeast();
    static QString encode(int num);
    static int decode(QString str);
    static QString getYeast();
};

#endif // YEAST_H
