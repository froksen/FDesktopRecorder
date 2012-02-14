#include "settingsmanager.h"
#include <QDebug>

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
}

void SettingsManager::writeAll()
{
    settings.beginGroup("record");
    settings.setValue("fps",framerate);
    settings.setValue("videocodec",videocodec);
    settings.setValue("audiocodec",audiocodec);
    settings.setValue("audiochannels",audiochannels);
    settings.endGroup();

    settings.beginGroup("startupbehavior");
    settings.setValue("defaultrecorddevice",microphonedevice);
    settings.setValue("defaultrecorddeviceMute",microphonemuted);
    settings.setValue("defaultname",filenameBase);
    settings.setValue("defaultnametimedate",filenameUsedate);
    settings.setValue("defaultpath",filenamePath);
    settings.setValue("defaultformat",format);
    settings.setValue("language",language);
    settings.endGroup();

    settings.beginGroup("misc");
    settings.setValue("latestrecording",latestrecording);
    settings.endGroup();
}

void SettingsManager::readAll()
{
    settings.beginGroup("record");
    framerate = settings.value("fps").toInt();
    videocodec = settings.value("videocodec").toString();
    audiocodec = settings.value("audiocodec").toString();
    audiochannels = settings.value("audiochannels").toInt();
    settings.endGroup();

    settings.beginGroup("startupbehavior");
    microphonedevice = settings.value("defaultrecorddevice").toString();
    microphonemuted = settings.value("defaultrecorddeviceMute").toString();
    filenameBase = settings.value("defaultname").toString();
    filenameUsedate = settings.value("defaultnametimedate").toString();
    filenamePath = settings.value("defaultpath").toString();
    format = settings.value("defaultformat").toString();
    language = settings.value("language").toString();
    settings.endGroup();

    settings.beginGroup("misc");
    latestrecording = settings.value("latestrecording").toString();
    settings.endGroup();
}

void SettingsManager::setFramerate(int newFramerate)
{
    framerate = newFramerate;
}

int SettingsManager::getFramerate()
{
    return framerate;
}

void SettingsManager::setVideocodec(QString newVideocodec)
{
    videocodec = newVideocodec;
}

QString SettingsManager::getVideocodec()
{
    return videocodec;
}

void SettingsManager::setAudiocodec(QString newAudiocodec)
{
    audiocodec = newAudiocodec;
}

QString SettingsManager::getAudiocodec()
{
    return audiocodec;
}

void SettingsManager::setMicrophonedevice(QString newDevice)
{
    microphonedevice = newDevice;
}

QString SettingsManager::getMicrophonedevice()
{
    return microphonedevice;
}

void SettingsManager::setMicrophonemuted(QString state)
{
    microphonemuted = state;
}

QString SettingsManager::getMicrophonemuted()
{
    return microphonemuted;
}

void SettingsManager::setFilenameBase(QString newBasename)
{
    filenameBase = newBasename;
}

QString SettingsManager::getFilenameBase()
{
    return filenameBase;
}

void SettingsManager::setFilenameUsedate(QString state)
{
    filenameUsedate = state;
}

QString SettingsManager::getFilenameUsedate()
{
    return filenameUsedate;
}

void SettingsManager::setFilenamePath(QString newPath)
{
    filenamePath = newPath;
}

QString SettingsManager::getFilenamePath()
{
    return filenamePath;
}

void SettingsManager::setFormat(QString newFormat)
{
    format = newFormat;
}

QString SettingsManager::getFormat()
{
    return format;
}

void SettingsManager::setLanguage(QString newLanguage)
{
    language = newLanguage;
}

QString SettingsManager::getLanguage()
{
    return language;
}

void SettingsManager::setLatestrecording(QString newLatest)
{
    latestrecording = newLatest;
}

QString SettingsManager::getLatestrecording()
{
    return latestrecording;
}

