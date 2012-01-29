#include "windowgrapper.h"
#include <QDebug>
#include <QApplication>
#include <QDesktopWidget>

WindowGrapper::WindowGrapper(QObject *parent) :
    QObject(parent)
{
}

int WindowGrapper::fixResolution(int number)
{
    number = ( ( number % 2 ) != 0 ) ? ( number + 1 ) : number;
    qDebug() << "Fixed resolution:" << number;
    return number;
}

QString WindowGrapper::Fullscreenaspects()
{
    QString geometry;
    QString geometryWidth;
    QString geometryHeight;

    //Gets width of screen
    geometryWidth = QString::number(fixResolution(QApplication::desktop()->width()));
    qDebug() << "QT Width:" << geometryWidth;

    geometryHeight = QString::number(fixResolution(QApplication::desktop()->height()));
    qDebug() << "QT Height:" << geometryHeight;

    geometry = geometryWidth + "x" + geometryHeight;
    qDebug () << "Fullscreen geometry:" << geometry;
    return geometry;
}
