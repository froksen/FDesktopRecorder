#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QString>
#include <QPluginLoader>
#include "PluginInterface.h"
#include <QMap>

class PluginManager : public QObject
{
    Q_OBJECT
public:
    explicit PluginManager(QObject *parent = 0);

    QMap <QString, QString> PluginInformation();
    void setEnabledPlugins(QStringList list);
    
signals:

private:
    bool PluginFolderExists(QString path);

    //Variables
    QString pluginpath;
    QStringList enabledpluginslist;

    //Plugin names and Descriptions
    QMap <QString, QString> plugininformation;

public slots:
    void LoadAllPlugins();
    void LoadPlugin(QString Filename, QString plugintype,QString absolutFilepath = "", int recordingduration_s = 0);
    void LoadPostRecPluings(QString recordingFilename="", int recordingduration_s=0);
    int setPluginpath(QString PluginPath);
    int createPluginPath(QString PluginPath);
    
};

#endif // PLUGINMANAGER_H
