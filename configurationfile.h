#ifndef CONFIGURATIONFILE_H
#define CONFIGURATIONFILE_H

#include <QObject>
#include <QSettings>

class ConfigurationFile : public QObject
{
    Q_OBJECT
public:
    explicit ConfigurationFile(QObject *parent = 0);

signals:

public slots:

public:
    QString getValue(QString key, QString group);
    int ValueExists(QString key, QString group);
    void writeValue(QString keyvalue, QString key, QString group);
    void setDefaults();

    QSettings configurationfile;


private:

};

#endif // CONFIGURATIONFILE_H
