#include "configurationfile.h"

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
