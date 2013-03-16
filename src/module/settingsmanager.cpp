#include "settingsmanager.h"
#include <QDebug>
#include <QDir>
#include <KGlobalSettings>

SettingsManager::SettingsManager(QObject *parent) :
    QObject(parent)
{
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
    settings.setValue("Preset",Preset);
    settings.setValue("usevpre",usevpre);
    settings.setValue("useapre",useapre);
    settings.setValue("usePreset",usePreset);
    settings.endGroup();

    settings.beginGroup("startupbehavior");
    settings.setValue("defaultrecorddevice",microphonedevice);
    settings.setValue("defaultrecorddeviceMute",microphonemuted);
    settings.setValue("defaultname",filenameBase);
    settings.setValue("defaultnametimedate",filenameUsedate);
    settings.setValue("defaultpath",filenamePath);
    settings.setValue("defaultformat",format);
    settings.setValue("language",language);
    settings.setValue("ffmpeglocation",ffmpeglocation);
    settings.endGroup();

    settings.beginGroup("misc");
    settings.setValue("previewplayer",previewplayer);
    settings.setValue("useKDEplayer",usekdeplayer);
    settings.setValue("previewplayerintegrated",previewplayerintegrated);
    settings.setValue("Singlewindow_redrectangle",SingleWindow_redrectangle);
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
    Preset = settings.value("Preset","ultrafast").toString();
    usevpre = settings.value("usevpre","false").toString();
    useapre = settings.value("useapre","false").toString();
    usePreset = settings.value("usePreset","true").toString();
    settings.endGroup();

    settings.beginGroup("startupbehavior");
    recordingdevices.getRecorddevices();
    microphonedevice = settings.value("defaultrecorddevice",recordingdevices.RecordDeviceHW[0]).toString();
    microphonemuted = settings.value("defaultrecorddeviceMute","true").toBool();
    /*: Translate this into what a good basename for a recording would be in your language */
    filenameBase = settings.value("defaultname",trUtf8("recording")).toString();
    filenameUsedate = settings.value("defaultnametimedate","false").toBool();
    filenamePath = settings.value("defaultpath",XDG_VIDEOS_DIR()).toString();
    format = settings.value("defaultformat","avi").toString();
    language = settings.value("language","default").toString();
    ffmpeglocation = settings.value("ffmpeglocation","ffmpeg").toString();
    settings.endGroup();

    settings.beginGroup("misc");
    previewplayer = settings.value("previewplayer","xdg-open").toString();
    usekdeplayer = settings.value("useKDEplayer","true").toBool();
    previewplayerintegrated = settings.value("previewplayerintegrated","false").toString();
    SingleWindow_redrectangle = settings.value("Singlewindow_redrectangle","true").toString();
    latestrecording = settings.value("latestrecording","Unknown").toString();
    settings.endGroup();
}

QString SettingsManager::XDG_VIDEOS_DIR()
{
    return KGlobalSettings::videosPath();
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

void SettingsManager::setMicrophonemuted(bool state)
{
    microphonemuted = state;
}

bool SettingsManager::getMicrophonemuted()
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

void SettingsManager::setFilenameUsedate(bool state)
{
    filenameUsedate = state;
}

bool SettingsManager::getFilenameUsedate()
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

void SettingsManager::setPreset(QString newString)
{
    Preset = newString;
}

QString SettingsManager::getPreset()
{
    return Preset;
}

void SettingsManager::setUsePreset(QString state)
{
    usePreset = state;
}

QString SettingsManager::getsetUsePreset()
{
    return usePreset;
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

void SettingsManager::setSinglewindow_redrectangle(QString newValue)
{
    SingleWindow_redrectangle = newValue;
}

QString SettingsManager::getSinglewindow_redrectangle()
{
    return SingleWindow_redrectangle;
}

void SettingsManager::setFFmpeglocation(QString location)
{
   ffmpeglocation = location;
}

QString SettingsManager::FFmpeglocation()
{
    return ffmpeglocation;
}

void SettingsManager::useKDEplayer(bool status)
{
    usekdeplayer = status;
}

bool SettingsManager::kdeplayerUsed()
{
    return usekdeplayer;
}

void SettingsManager::removeSettingsfile()
{
    QFile settingsfile (settings.fileName());
    settingsfile.remove();
    settings.sync();
}


