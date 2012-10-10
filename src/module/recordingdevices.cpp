#include "recordingdevices.h"
#include <QProcess>
#include <QDebug>

RecordingDevices::RecordingDevices(QObject *parent) :
    QObject(parent)
{
}

void RecordingDevices::getRecorddevices(){
    //Clears old values
    RecordDeviceDesc.clear();
    RecordDeviceHW.clear();

    QProcess p;
    p.start("arecord -l");
    p.waitForFinished(-1);

    QString p_stdout = p.readAllStandardOutput();
    QString p_stderr = p.readAllStandardError();

    QTextStream in(&p_stdout);
       while ( !in.atEnd() )
       {
          QString line = in.readLine();
          if(line.left(4) == "card"){
              QString orgline = line;

              // Gets the cardnumber
              QString CardHW;
              CardHW = line.remove(6,100);
              CardHW = line.remove(0,5);

              //Gets the devicenumber
              QRegExp devicepattern("(device\\s\\d)");
              QString devicenumber;

              int pos = devicepattern.indexIn(orgline);
              if (pos > -1) {
                  devicenumber = devicepattern.cap(1);

                  QRegExp patternDVremove ("device\\s");
                  devicenumber = devicenumber.remove(patternDVremove);
              }


              //Adding Cardnumber and devicenumber
              CardHW = "hw:" + CardHW +","+devicenumber;
              RecordDeviceHW << CardHW;

              //Gets the Card Description
              QRegExp patternCard ("^card\\s\\d:\\s");
              QRegExp patternDV2remove ("device\\s\\d..");

              QString CardDesc;
              CardDesc = orgline.remove(patternCard);
              CardDesc = CardDesc.remove(patternDV2remove);

              CardDesc = "(" + CardHW + ") " + CardDesc;
              RecordDeviceDesc << CardDesc;
          }
       }

       RecordDeviceDesc << trUtf8("Pulse Audio (might not work)");
       RecordDeviceHW << "pulse";

       qDebug() << "Record devices found:";
       foreach(QString device, RecordDeviceDesc){
           qDebug() << "-" << device;
       }
}
