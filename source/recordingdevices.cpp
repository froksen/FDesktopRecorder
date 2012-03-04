#include "recordingdevices.h"
#include <QProcess>
#include <QDebug>

RecordingDevices::RecordingDevices(QObject *parent) :
    QObject(parent)
{
}

void RecordingDevices::getRecorddevices(){
    QProcess p;
    p.start("arecord -l");
    p.waitForFinished(-1);

    QString p_stdout = p.readAllStandardOutput();
    QString p_stderr = p.readAllStandardError();

    qDebug() << "Record devices found:";

    QTextStream in(&p_stdout);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          if(line.left(4) == "card"){
              QString orgline = line;

              // Gets the HW name
              QString CardHW;
              CardHW = line.remove(6,100);
              CardHW = line.remove(0,5);
              CardHW = "hw:" + CardHW +",0";
              RecordDeviceHW << CardHW;

              //Gets the Card Description
              QString CardDesc;
              CardDesc = orgline.remove(0,8);
              CardDesc = "(" + CardHW + ") " + CardDesc;
              RecordDeviceDesc << CardDesc;

              qDebug() << CardHW << "-" << CardDesc;
          }
       }

       RecordDeviceDesc << trUtf8("Pulse Audio (might not work)");
       RecordDeviceHW << "pulse";
}
