#include "settingsmanager.h"
#include <QDebug>
#include <QDir>

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
}
void SettingsManager::setDefaults()
{
    framerate = 30;
    videocodec = "libx264";
    audiocodec = "flac";
    audiochannels = 2;
    audiosource = "alsa";
    vpre = "";
    apre = "";
    usevpre = "false";
    useapre = "false";

    recordingdevices.getRecorddevices();
    microphonedevice = recordingdevices.RecordDeviceHW[0];
    microphonemuted = "true";

    filenameBase = trUtf8("recording");
    filenameUsedate = "false";
    filenamePath = QDir::homePath();
    format = "avi";

    language = "default";
    latestrecording = "Unknown";

    //Writes them
    writeAll();
}

void SettingsManager::checkDefaults()
{
    int index;
    QStringList values2check;
    QStringList values2write;

    values2check << "videocodec" << "audiocodec" << "audiochannels" << "fps" << "audiosource" << "vpre" << "apre" << "usevpre" << "useapre";
    values2write << "libx264" << "flac" << "2" << "30" << "alsa" << "" << "" << "false" << "false";

    index = 0;
    foreach(QString item, values2check)
    {
        settings.beginGroup("record");
        if(!settings.contains(item))
        {
            settings.setValue(item,values2write[index]);
        }
        settings.endGroup();
        index += 1;
    }

    values2check.clear();
    values2write.clear();

    recordingdevices.getRecorddevices();
    values2check << "defaultrecorddevice" << "defaultformat" << "defaultname" << "defaultpath" << "defaultrecorddeviceMute" << "language";
    values2write << recordingdevices.RecordDeviceHW[0] << "avi" << trUtf8("recording") << QDir::homePath() << "true" << "default";

    index = 0;
    foreach(QString item, values2check)
    {
        settings.beginGroup("startupbehavior");
        if(!settings.contains(item))
        {
            settings.setValue(item,values2write[index]);
        }
        settings.endGroup();
        index += 1;
    }

    values2check.clear();
    values2write.clear();

    values2check << "latestrecording";
    values2write << "Unknown";

    index = 0;
    foreach(QString item, values2check)
    {
        settings.beginGroup("misc");
        if(!settings.contains(item))
        {
            settings.setValue(item,values2write[index]);
        }
        settings.endGroup();
        index += 1;
    }
}

void SettingsManager::writeAll()
{
    settings.beginGroup("record");
    settings.setValue("fps",framerate);
    settings.setValue("videocodec",videocodec);
    settings.setValue("audiosource",audiosource);
    settings.setValue("audiocodec",audiocodec);
    settings.setValue("audiochannels",audiochannels);
    settings.setValue("vpre",vpre);
    settings.setValue("apre",apre);
    settings.setValue("usevpre",usevpre);
    settings.setValue("useapre",useapre);
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
    settings.setValue("previewplayer",previewplayer);
    settings.setValue("previewplayerintegrated",previewplayerintegrated);
    settings.setValue("latestrecording",latestrecording);
    settings.endGroup();

    settings.sync();
}

void SettingsManager::readAll()
{
    settings.beginGroup("record");
    framerate = settings.value("fps",30).toInt();
    videocodec = settings.value("videocodec","libx264").toString();
    audiosource = settings.value("audiosource","alsa").toString();
    audiocodec = settings.value("audiocodec","flac").toString();
    audiochannels = settings.value("audiochannels",2).toInt();
    vpre = settings.value("vpre").toString();
    apre = settings.value("apre").toString();
    usevpre = settings.value("usevpre","false").toString();
    useapre = settings.value("useapre","false").toString();
    settings.endGroup();

    settings.beginGroup("startupbehavior");
    recordingdevices.getRecorddevices();
    microphonedevice = settings.value("defaultrecorddevice",recordingdevices.RecordDeviceHW[0]).toString();
    microphonemuted = settings.value("defaultrecorddeviceMute","true").toString();
    filenameBase = settings.value("defaultname",trUtf8("recording")).toString();
    filenameUsedate = settings.value("defaultnametimedate","false").toString();
    filenamePath = settings.value("defaultpath",QDir::homePath()).toString();
    format = settings.value("defaultformat","avi").toString();
    language = settings.value("language","default").toString();
    settings.endGroup();

    settings.beginGroup("misc");
    previewplayer = settings.value("previewplayer","mplayer").toString();
    previewplayerintegrated = settings.value("previewplayerintegrated","true").toString();
    latestrecording = settings.value("latestrecording","Unknown").toString();
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

void SettingsManager::setAudiosource(QString newSource)
{
    audiosource = newSource;
}

QString SettingsManager::getAudiosource()
{
    return audiosource;
}


void SettingsManager::setAudiocodec(QString newAudiocodec)
{
    audiocodec = newAudiocodec;
}

QString SettingsManager::getAudiocodec()
{
    return audiocodec;
}

void SettingsManager::setAudiochannels(int newValue)
{
    audiochannels = newValue;
}

int SettingsManager::getAudiochannles()
{
    return audiochannels;
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

void SettingsManager::setVpre(QString newString)
{
    vpre = newString;
}

QString SettingsManager::getVpre()
{
    return vpre;
}

void SettingsManager::setApre(QString newString)
{
    apre = newString;
}

QString SettingsManager::getApre()
{
    return apre;
}

void SettingsManager::setUseapre(QString state)
{
    useapre = state;
}

QString SettingsManager::getUseapre()
{
    return useapre;
}

void SettingsManager::setUsevpre(QString state)
{
    usevpre = state;
}

QString SettingsManager::getUsevpre()
{
    return usevpre;
}

void SettingsManager::setPreviewplayer(QString newPlayer)
{
    previewplayer = newPlayer;
}

QString SettingsManager::getPreviewplayer()
{
    return previewplayer;
}

void SettingsManager::setPreviewplayerintegrated(QString newValue)
{
    previewplayerintegrated = newValue;
}

QString SettingsManager::getPreviewplayerintegrated()
{
    return previewplayerintegrated;
}


