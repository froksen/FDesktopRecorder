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
    //This function is more or less from Juergen Heinemann's "qx11grab". Great work from a great person! :-)
    // Visit his project at: http://qt-apps.org/content/show.php?content=89204
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

QString WindowGrapper::Singlewindowgeometry(QString Text)
{
    QString geometry;
    QStringList argsscript;
    QString geometryWidth;
    QString geometryHeight;
    QString corners;
    QString argcorners;

    QString p_stdout = Text;

    //Gets the width of the screen.
    QTextStream inw(&p_stdout);
    while ( !inw.atEnd() )
    {
       QString widthline = inw.readLine();
       if(widthline.left(8) == "  Width:")
       {
       geometryWidth = widthline.remove(0,9);
       }
    }
    geometryWidth = QString::number(fixResolution(geometryWidth.toInt()));
    qDebug() << "Width:" << geometryWidth;

    //Gets the height of the screen.
    QTextStream inh(&p_stdout);
    while ( !inh.atEnd() )
    {
        QString heightline = inh.readLine();
        if(heightline.left(9) == "  Height:")
        {
        geometryHeight = heightline.remove(0,10);
        qDebug() << "Height:" << geometryHeight;
        }
    }

    geometryHeight = QString::number(fixResolution(geometryHeight.toInt()));
    qDebug() << "Height:" << geometryHeight;

    geometry = geometryWidth + "x" + geometryHeight;

    return geometry;


    argsscript.clear();
}

QString WindowGrapper::Singlewindowcorners(QString Text)
{
    QString geometry;
    QStringList argsscript;
    QString geometryWidth;
    QString geometryHeight;
    QString corners;
    QString argcorners;

    QString p_stdout = Text;

    QTextStream in(&p_stdout);
    while ( !in.atEnd() )
    {
       QString line = in.readLine();
       if(line.left(10) == "  Corners:")
       {
       qDebug() << "Geometry line found in xwininfo:" << line;
       corners = line.remove(0,13);
       corners = corners.remove(8,100);
       corners = corners.replace("+",",");
       corners = corners.replace("-",",");
       qDebug() << "Corners:" << corners;
       }

       argcorners = ":0.0+"+corners;
    }

    return argcorners;
}
