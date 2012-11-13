#include "pluginmanager.h"
#include <QFileInfo>

PluginManager::PluginManager(QObject *parent) :
    QObject(parent)
{
}

QMap<QString, QString> PluginManager::PluginInformation()
{
    return plugininformation;
}

void PluginManager::setEnabledPlugins(QStringList list)
{
    enabledpluginslist = list;
}

bool PluginManager::PluginFolderExists(QString path)
{
    QFileInfo pathinfo (path);
    if(pathinfo.exists()){
        return true;
    }

    return false;
}

int PluginManager::setPluginpath(QString PluginPath)
{
    if(PluginFolderExists(PluginPath)){
        pluginpath = PluginPath;
        qDebug() << "PluginPath set to:" << PluginPath;
        return 0;
    }
    else {
        return -1;
    }
}

int PluginManager::createPluginPath(QString PluginPath)
{
    QDir mPath;
    return mPath.mkdir(PluginPath);
}

void PluginManager::LoadAllPlugins()
{
    //QDir path(QDir::currentPath() + "/plugins");
    QDir path(pluginpath);

    //Display the current path and switch to the plugin directory
    qDebug() << "current path is: " << path.absolutePath();

    //try to load the plugins
    foreach( QString filename, path.entryList(QDir::Files))
    {
        LoadPlugin(path.absolutePath() + "/" +  filename,"none","none",0);
    }

}

void PluginManager::LoadPlugin(QString pluginFilename,QString KindOf,QString absolutFilepath,int recordingduration_s)
{

    qDebug() << "PluginFile: " << pluginFilename;
    QPluginLoader loader(pluginFilename);
    QObject *possiblePlugin = loader.instance();

    if(possiblePlugin)
    {

        PluginInterface *plugin = qobject_cast<PluginInterface*>( possiblePlugin );
        if( plugin )
        {
            QString pluginname = plugin->Name();
            QString plugindesc = plugin->Description();

            if(enabledpluginslist.contains(pluginname)){
                qDebug() << "Plugin found and enabled: " << pluginname;

                if(KindOf == "postrec"){
                    plugin->PostRecording(absolutFilepath,recordingduration_s);
                }
                if(KindOf == "prerec"){
                }
            }
            else {
                qDebug() << "Plugin found, but not enabled: " << pluginname;
            }

            //Apends to "PluginInformation"
            plugininformation.insert(pluginname,plugindesc);
        }
    }
    else
    {
        qDebug() << "ERROR: " << pluginFilename << " = " << loader.errorString();

    }
}

void PluginManager::LoadPostRecPluings(QString recordingFilename, int recordingduration_s)
{
    QDir path(pluginpath);

    qDebug() << "Loading plugins for Post recording";
    //try to load the plugins

    foreach(QString filename, path.entryList(QDir::Files))
    {
        LoadPlugin(path.absolutePath() + "/" +  filename,"postrec",recordingFilename,recordingduration_s);
    }
}
