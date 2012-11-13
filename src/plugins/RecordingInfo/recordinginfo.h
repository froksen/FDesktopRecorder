#ifndef RECORDINGINFO_H
#define RECORDINGINFO_H

#include <QObject>
#include <QtPlugin>
#include <QDebug>
#include "PluginInterface.h"
#include <QTime>

class RecordingInfo : public QObject, PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

public:
    RecordingInfo();

    /**
    * Returns the name of the plugin
    */
    QString Name() const;
    QString Description() const;

    void PostRecording(QString recordingFilename,int recordingduration_s=0) const;

private:
    QString returnTime(int numberofsecs) const;
    QString fileSize(QString filename) const;


signals:

public slots:

};

#endif // RECORDINGINFO_H
