#ifndef RECORDINGDEVICES_H
#define RECORDINGDEVICES_H

#include <QObject>
#include <configurationfile.h>
#include <QStringList>

class RecordingDevices : public QObject
{
    Q_OBJECT
public:
    explicit RecordingDevices(QObject *parent = 0);

    QStringList RecordDeviceDesc;
    QStringList RecordDeviceHW;

    void getRecorddevices();

signals:

public slots:

private:
    void setRecordingdevice();

    //other
    ConfigurationFile *Configurationfile;

};

#endif // RECORDINGDEVICES_H
