#include "recordinginfo.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QtCore>
#include <QtGui>

RecordingInfo::RecordingInfo()
{
}

Q_EXPORT_PLUGIN2( RecordingInfo, RecordingInfo )

QString RecordingInfo::Name() const
{
    return "RecordingInfo";
}

QString RecordingInfo::Description() const
{
    return trUtf8("This plugin will make a popup (QMessagebox) displaying information about the recording");
}

void RecordingInfo::PostRecording(QString recordingFilename,int recordingduration_s) const
{
    //Gets the duration of the recording
    QString duration = returnTime(recordingduration_s);

    //Size of recording
    QString filesize = fileSize(recordingFilename);

    //The messagebox
    QMessageBox msgbox;
    //Msgbox buttons
    QPushButton *playButton = msgbox.addButton(tr("Play"), QMessageBox::ActionRole);
    QPushButton *openDirButton = msgbox.addButton(tr("Open directory"), QMessageBox::ActionRole);
    msgbox.setStandardButtons(QMessageBox::Close);

    //msgbox Text
    msgbox.setText(QString( "<h1>About recording</h1> <b>FileName:</b> %1 <br> <b>Duration:</b> %2 <br> <b>Filesize:</b> %3").arg(QFileInfo(recordingFilename).fileName(), duration,filesize));

    //msg exec
    msgbox.exec();

    //When a button is pressed
    if (msgbox.clickedButton() == openDirButton) {
        QDesktopServices::openUrl( QUrl::fromLocalFile(QFileInfo(recordingFilename).absolutePath()));
    }
    if (msgbox.clickedButton() == playButton) {
        QDesktopServices::openUrl( QUrl::fromLocalFile(recordingFilename));
    }
}

QString RecordingInfo::returnTime(int numberofsecs) const
{
    //Creates the Qtime
    QTime newtime;
    QTime stopwatchtime;

    //MAkes it a QString
    newtime = stopwatchtime.addSecs(numberofsecs) ;
    QString text = newtime.toString("hh:mm:ss");

    //Returns the result
    return text;
}

QString RecordingInfo::fileSize(QString filename) const
{
    //Makes it a QString and figures out how the size is best shown.
    int B = 1; //byte
    int KB = 1024 * B; //kilobyte
    int MB = 1024 * KB; //megabyte
    int GB = 1024 * MB; //gigabyte
    QFileInfo filenameInfo (filename);
    double filesize = filenameInfo.size();
    qDebug() << filenameInfo.size();
    QString filesizestring;

    if(filesize > GB)
    {
        filesizestring =  QString("%1 GB").arg(QString::number(filesize/GB, 'f',2));
    }
    else if(filesize > MB)
    {
        filesizestring =  QString("%1 MB").arg(QString::number(filesize/MB, 'f',2));
    }
    else if(filesize > KB)
    {
        filesizestring =  QString("%1 KB").arg(QString::number(filesize/KB, 'f',2));
    }
    else
    {
        filesizestring =  QString("%1 K").arg(QString::number(filesize, 'f',2));
    }

    return filesizestring;
}
