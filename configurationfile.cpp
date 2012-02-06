#include "configurationfile.h"
#include <QStringList>
#include <QDir>
#include <recordingdevices.h>
#include <QDebug>



ConfigurationFile::ConfigurationFile(QObject *parent) :
    QObject(parent)
{
}

QString ConfigurationFile::getValue(QString key, QString group)
{
    configurationfile.beginGroup(group);
    QString returnvalue = configurationfile.value(key).toString();
    configurationfile.endGroup();

    return returnvalue;
}

int ConfigurationFile::ValueExists(QString key, QString group)
{
    configurationfile.beginGroup(group);
    if(configurationfile.contains(key))
    {
            return true;
    }
    else
    {
            return false;
    }
    configurationfile.endGroup();

}

void ConfigurationFile::writeValue(QString keyvalue, QString key, QString group)
{
    configurationfile.beginGroup(group);
        configurationfile.setValue(key, keyvalue);
    configurationfile.endGroup();

    configurationfile.sync();
}

void ConfigurationFile::setDefaults()
{
    int index;
    QStringList values2check;
    QStringList values2write;

    values2check << "videocodec" << "audiocodec" << "audiochannels" << "fps";
    values2write << "libx264" << "flac" << "2" << "30";

    index = 0;
    foreach(QString item, values2check)
    {
        configurationfile.beginGroup("record");
        if(!configurationfile.contains(item))
        {
            configurationfile.setValue(item,values2write[index]);
        }
        configurationfile.endGroup();
        index += 1;
    }

    values2check.clear();
    values2write.clear();

    RecordingDevices *RecordingDevicesClass = new RecordingDevices();
    RecordingDevicesClass->getRecorddevices();
    values2check << "defaultrecorddevice" << "defaultformat" << "defaultname" << "defaultpath" << "defaultrecorddeviceMute";
    values2write << RecordingDevicesClass->RecordDeviceHW[0] << "avi" << trUtf8("recording") << QDir::homePath() << "true";

    index = 0;
    foreach(QString item, values2check)
    {
        configurationfile.beginGroup("startupbehavior");
        if(!configurationfile.contains(item))
        {
            configurationfile.setValue(item,values2write[index]);
        }
        configurationfile.endGroup();
        index += 1;
    }

    //Writes the settings
    configurationfile.sync();
}
