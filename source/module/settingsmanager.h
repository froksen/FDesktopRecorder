#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <module/recordingdevices.h>

class SettingsManager : public QObject
{
    Q_OBJECT
public:
    explicit SettingsManager(QObject *parent = 0);
    void setFramerate(int newFramerate);
    int getFramerate();

    void setVideocodec(QString newVideocodec);
    QString getVideocodec();

    void setAudiosource(QString newSource);
    QString getAudiosource();

    void setAudiocodec(QString newAudiocodec);
    QString getAudiocodec();

    void setAudiochannels(int newValue);
    int getAudiochannles();

    void setMicrophonedevice(QString newDevice);
    QString getMicrophonedevice();

    void setMicrophonemuted(QString state);
    QString getMicrophonemuted();

    void setFilenameBase(QString newBasename);
    QString getFilenameBase();

    void setFilenameUsedate(QString state);
    QString getFilenameUsedate();

    void setFilenamePath(QString newPath);
    QString getFilenamePath();

    void setFormat(QString newFormat);
    QString getFormat();

    void setLanguage(QString newLanguage);
    QString getLanguage();

    void setLatestrecording(QString newLatest);
    QString getLatestrecording();

    void setVpre(QString newString);
    QString getVpre();

    void setApre(QString newString);
    QString getApre();

    void setUseapre(QString state);
    QString getUseapre();

    void setUsevpre(QString state);
    QString getUsevpre();

    void setPreset(QString newString);
    QString getPreset();

    void setUsePreset(QString state);
    QString getsetUsePreset();

    void setPreviewplayer(QString newPlayer);
    QString getPreviewplayer();

    void setPreviewplayerintegrated(QString newValue);
    QString getPreviewplayerintegrated();

    void setSinglewindow_redrectangle(QString newValue);
    QString getSinglewindow_redrectangle();

    //----------------SECTION: Common-----------------
    void setDefaults();
    void checkDefaults();

public slots:
    void writeAll();
    void readAll();

private slots:

private:
//----------------SECTION: Common------------------
    QSettings settings;
//----------------SECTION: Record settings------------------
    //Video
    int framerate;
    QString videocodec;
    //Audio
    QString audiosource;
    QString audiocodec;
    int audiochannels;
    //Microphone
    QString microphonedevice;
    QString microphonemuted;
    //Advanced
    QString vpre;
    QString usevpre;
    QString apre;
    QString useapre;
    QString Preset;
    QString usePreset;
    //Filename
    QString filenameBase;
    QString filenameUsedate;
    QString filenamePath;
    //Format
    QString format;
//----------------SECTION: Application settings------------------
    QString language;
    QString previewplayer;
    QString previewplayerintegrated;
    QString SingleWindow_redrectangle;
//----------------SECTION: OTHER------------------
    QString latestrecording;
    RecordingDevices recordingdevices;


};

#endif // SETTINGSMANAGER_H
