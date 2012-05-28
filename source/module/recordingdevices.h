#ifndef RECORDINGDEVICES_H
#define RECORDINGDEVICES_H

#include <QObject>
#include <QStringList>
#include <QSettings>

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

};

#endif // RECORDINGDEVICES_H
