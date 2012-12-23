#ifndef USERDIRREADER_H
#define USERDIRREADER_H

#include <QObject>
#include <QtCore>

class UserdirReader : public QObject
{
    Q_OBJECT
public:
    explicit UserdirReader(QObject *parent = 0);
    
    bool fileExists();
    QString getXdg_videos_dir();

signals:
    
public slots:

private:
    //variables
    QString contentoffile;
    QString xdg_videos_dir;

protected:
    bool readFile(QString filename);
};

#endif // USERDIRREADER_H
