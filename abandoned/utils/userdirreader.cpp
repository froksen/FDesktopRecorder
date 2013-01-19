#include "userdirreader.h"
#include <QTextStream>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QRegExp>

UserdirReader::UserdirReader(QObject *parent) :
    QObject(parent)
{
}

bool UserdirReader::fileExists()
{
    return readFile(QDir::homePath() + "/.config/user-dirs.dirs");
}




bool UserdirReader::readFile(QString filename)
{
    QFile mFile(filename);
    if(!mFile.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "XDG (user-dirs.dirs) not found";
        return false;
    }

    qDebug() << "XDG (user-dirs.dirs) found:" << mFile.fileName();

    QTextStream in(&mFile);
    QString line = in.readLine();
    while (!line.isNull()) {
        if(line.startsWith("XDG_VIDEOS_DIR")){
            xdg_videos_dir = line.remove("XDG_VIDEOS_DIR=").remove(0,1).remove(-1,1).replace("$HOME",QDir::homePath());
            qDebug() << "XDG_VIDEOS_DIR:" << xdg_videos_dir;
        }
        line = in.readLine();
    }

    mFile.close();
    return true;
}


QString UserdirReader::getXdg_videos_dir()
{
    return xdg_videos_dir;
}
