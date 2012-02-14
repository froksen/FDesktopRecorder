#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QObject>
#include <QSettings>
#include <recordingdevices.h>

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

    //----------------SECTION: Common------------------
    void writeAll();
    void readAll();
    void setDefaults();
    void checkDefaults();

public slots:

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
    //Filename
    QString filenameBase;
    QString filenameUsedate;
    QString filenamePath;
    //Format
    QString format;
//----------------SECTION: Application settings------------------
    QString language;
//----------------SECTION: OTHER------------------
    QString latestrecording;
    RecordingDevices recordingdevices;


};

#endif // SETTINGSMANAGER_H
