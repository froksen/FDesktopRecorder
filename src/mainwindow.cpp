#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QtGui>
#include <QtCore>
#include <QSystemTrayIcon>
#include <math.h>
#include <iomanip>
#include "utils/areaselectordialog.h"

/*NOTE: Removed the PreviewPlayer. Keept intence since I might add it again in a later version  */
//#include "previewplayer/previewplayer.h"




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    //Makes sure that the program can run i bg
    //QApplication::setQuitOnLastWindowClosed(false);

    ui->actionPreviewrecording->setEnabled(0);

    //Connections used by "Copy to clipboard" button
    connect(ui->pushButtonConsoleCopyToClipboard,SIGNAL(clicked()),ui->textEditConsole,SLOT(selectAll()));
    connect(ui->pushButtonConsoleCopyToClipboard,SIGNAL(clicked()),ui->textEditConsole,SLOT(copy()));


    //Hides objects that needs to be hidden
    ui->dockWidget->hide();
    ui->pushButtonStoprecord->hide();

    //Sets the MainWindow size
    MainWindow::setFixedHeight(107);
    MainWindow::setGeometry(MainWindow::geometry().x(),MainWindow::geometry().y(),625,MainWindow::geometry().height());
    QRect r = MainWindow::geometry();
    r.moveCenter(QApplication::desktop()->availableGeometry().center());
    MainWindow::setGeometry(r);

    //Reads settingsfile
    qDebug() << "-----  Reading settings on startup -----";
    settings.readAll();

    //Set the Startrecord PushButton menu
    QMenu *Recordbuttonmenu = new QMenu();
    QAction *startrecording = new QAction(trUtf8("Start recording"), this);
    connect(startrecording,SIGNAL(triggered()),this,SLOT(on_pushButtonStartrecord_clicked()));
    QAction *startandminimize = new QAction(trUtf8("Minimize and start record"), this);
    connect(startandminimize,SIGNAL(triggered()),this,SLOT(startRecordandminimize()));

    Recordbuttonmenu->addAction(startrecording);
    Recordbuttonmenu->addAction(startandminimize);
    ui->pushButtonStartrecord->setMenu(Recordbuttonmenu);


    //Reads from cfg file
    ui->checkBoxRecordaudio->setChecked(settings.getMicrophonemuted());

    //Creates systemtrayp
    createsystemtray();


    //Sets the icon for the Toolbar
    ui->actionAbout->setIcon(QIcon::fromTheme("help-about"));
    ui->actionConsole->setIcon(QIcon::fromTheme("utilities-terminal"));
    ui->actionSettings->setIcon(QIcon::fromTheme("preferences-system"));
    ui->actionOpen_recording_directory->setIcon(QIcon::fromTheme("system-file-manager"));
    ui->actionPreviewrecording->setIcon(QIcon::fromTheme("video-display"));

    WindowGrapperClass = new WindowGrapper(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//This function appends a number to the end of a filename if filename exists!
QString MainWindow::setFilename(QString path, QString basename, QString format)
{
    QFileInfo Filename = path + "/" + basename + "." + format;
    qDebug() << "..::Checking if file exists::..";
    int indexnumber = 1;
    while(Filename.exists())
    {
        qDebug() << "File exists: " + Filename.absoluteFilePath();
        Filename = path + "/" + basename + QString::number(indexnumber) + "." + format;
        indexnumber += 1;
    }
    qDebug() << "..::Done checking::.. \n New filename: " + Filename.absoluteFilePath();
    QString Filenamestr = Filename.absoluteFilePath();
    return Filenamestr;
}


//This function handles what happens when Start Record is clicked!
void MainWindow::on_pushButtonStartrecord_clicked()
{
    mProcessClass = new process(this);

    //TODO: clean up this method so its not so confusing and so ugly

    qDebug() << "Reading settings";
    settings.readAll();

    QStringList recordingargs;
    recordingargs.clear();

    //Creates the struct element
    recordinginfo mrecordinginfo;

    //------------------------SECTION: Other--------------------------------
    mrecordinginfo.videocodec = settings.getVideocodec();
    mrecordinginfo.audiocodec = settings.getAudiocodec();
    mrecordinginfo.audiochannels = settings.getAudiochannles();
    mrecordinginfo.fps = settings.getFramerate();

    //------------------------SECTION: Geometry--------------------------------


    //If Single Window radio is checked, then to this. Else record fullscreen :-)
    if(ui->radioButtonSinglewindow->isChecked())
    {
        //Informing the user about how to proceed
        QMessageBox msgBox;
        msgBox.setText(QString("<b>%1</b>").arg(trUtf8("Recording a single window")));
        msgBox.setWindowTitle(trUtf8("Recording a single window"));
        msgBox.setInformativeText(trUtf8("When you click 'OK' a small crossair will appear. \n \nWith this you will have to select the window you want to record. \n\nHereafter the recording will start."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setFixedWidth(520);
        msgBox.exec();

        //Starting the singlewindow setup
        QProcess p;
        QStringList argsscript;
        argsscript << "-frame";
        p.start("xwininfo",argsscript);
        p.waitForFinished(-1);

        QString p_stdout = p.readAllStandardOutput();
        QString p_stderr = p.readAllStandardError();

        mrecordinginfo.geometry = WindowGrapperClass->Singlewindowgeometry(p_stdout);
        mrecordinginfo.corners = WindowGrapperClass->Singlewindowcorners(p_stdout);

        //Sets the red rectangle arround the area that is going to be recorded (QRubberband)

        //The size of the window:
        rubberband.setGeometry(0,0,WindowGrapperClass->SinglewindowWidth(p_stdout).toInt()+5,WindowGrapperClass->SinglewindowHeight(p_stdout).toInt()+5);

        //The position of the window
        QString singleCorners = mrecordinginfo.corners;
        singleCorners.remove(0,5);
        QStringList singleCornersList = singleCorners.split(",");
        rubberband.move(QString(singleCornersList[0]).toInt()-2,QString(singleCornersList[1]).toInt()-2);

        //Shows the rectangle
        if(settings.getSinglewindow_redrectangle() != "false")
        {
            rubberband.show();
        }
    }
    else if(ui->radioButtonCustom->isChecked()) {

        AreaSelectorDialog *mAreaSelector = new AreaSelectorDialog(this);
        mAreaSelector->exec();

        mrecordinginfo.geometry = QString::number(WindowGrapperClass->fixResolution(mAreaSelector->frameSize().width())) + "x" + QString::number(WindowGrapperClass->fixResolution(mAreaSelector->frameSize().height()));

        //Corners
        QString frameCornerX = QString::number(mAreaSelector->frameGeometry().x());
        frameCornerX = frameCornerX.replace("+","");
        frameCornerX = frameCornerX.replace("-","");

        QString frameCornerY = QString::number(mAreaSelector->frameGeometry().y());
        frameCornerY = frameCornerY.replace("+","");
        frameCornerY = frameCornerY.replace("-","");

        mrecordinginfo.corners = ":0.0+" + frameCornerX + "," + frameCornerY ;
        rubberband.setGeometry(mAreaSelector->frameGeometry());

//        //Shows the rectangle
        if(settings.getSinglewindow_redrectangle() != "false")
        {
            rubberband.show();
        }
    }
    else
    {
        mrecordinginfo.geometry = WindowGrapperClass->Fullscreenaspects();
        mrecordinginfo.corners = ":0.0";
    }




    //------------------------SECTION: Filename--------------------------------
    //Reads some defaults from CFG file
    mrecordinginfo.defaultpath = settings.getFilenamePath();
    mrecordinginfo.defaultname = QDateTime::currentDateTime().toString();
    mrecordinginfo.defaultformat = settings.getFormat();

    if(!settings.getFilenameUsedate())
    {
       mrecordinginfo.defaultname = settings.getFilenameBase();
    }

    //Sets the final filname!
    filename = setFilename(mrecordinginfo.defaultpath,mrecordinginfo.defaultname,mrecordinginfo.defaultformat);



    //------------------------SECTION: Microphone--------------------------------
    //Microphone: reads the CFG file to find which is predefined!
    mrecordinginfo.recordingdevice = settings.getMicrophonedevice();

    //if NOT Mute-Microphone checkbox is checked, then it will add this section.
    if(! ui->checkBoxRecordaudio->isChecked())
    {
        recordingargs << "-f";
        recordingargs << settings.getAudiosource();
        recordingargs << "-ac";
        recordingargs << QString::number(mrecordinginfo.audiochannels);
        recordingargs << "-i";
        recordingargs << mrecordinginfo.recordingdevice;
    }

    //Argument: apre
    if(settings.getUseapre() == "true")
    {
        recordingargs << "-apre";
        recordingargs << settings.getApre();
    }



    //Argument: what to grap
    recordingargs << "-f";
    recordingargs << "x11grab";

    //Argument: Framerate
    recordingargs << "-r";
    recordingargs << QString::number(mrecordinginfo.fps);

    //Argument: Geometry/Corners
    recordingargs << "-s";
    recordingargs << mrecordinginfo.geometry;
    recordingargs << "-i";
    recordingargs << mrecordinginfo.corners;

    //Argument: VideoCodec
    recordingargs << "-vcodec";
    recordingargs << mrecordinginfo.videocodec;

    //Argument: vpre
    if(settings.getUsevpre() == "true")
    {
        recordingargs << "-vpre";
        recordingargs << settings.getVpre();
    }
    //Argument: preset
    if(settings.getsetUsePreset() == "true")
    {
        recordingargs << "-preset";
        recordingargs << settings.getPreset();
    }

    //Argument: Filename
    recordingargs << "-y";
    recordingargs << filename;

    qDebug() << recordingargs;

    //------------------------SECTION: Set and run the recording--------------------------------
    //Sets the arguments to the program
    mProcessClass->setArguments(recordingargs);
    recordingargs.clear();

    //Sets the program
    mProcessClass->setCommand(settings.FFmpeglocation());

    //Starts the command
    mProcessClass->startCommand();

    //------------------SECTION: Connections---------------------
    //Connections used by the QProcess
    //This will clear the terminaltext
    ui->textEditConsole->clear();
    //These two lines posts the QProcess text to the GUI
    connect(mProcessClass,SIGNAL(stderrText(QString)),ui->textEditConsole,SLOT(append(QString)));
    connect(mProcessClass,SIGNAL(stdoutText(QString)),ui->textEditConsole,SLOT(append(QString)));
    connect(mProcessClass,SIGNAL(stderrText(QString)),this,SLOT(getFPS(QString)));
    connect(mProcessClass,SIGNAL(stdoutText(QString)),this,SLOT(getFPS(QString)));

    //When recording is finished it will send the exitCode, wether its succesfull or not.
    connect(mProcessClass, SIGNAL(FinishedExitCode(int)),this, SLOT(onProcessFinished(int)));

    //This makes sure that the user knows that a recording is happening (The Statusbar text)
    connect(mProcessClass,SIGNAL(stderrText(QString)),this,SLOT(setRecordingStatusbarText()));

    //------------------------SECTION: GUI things--------------------------------

    //Hides what needs to be hidden while recording
    ui->pushButtonStartrecord->setVisible(0);

    //Shows what needs to be shown while recording
    ui->pushButtonStoprecord->setVisible(1);

    //Disables what needs to be disabled while recording
    ui->checkBoxRecordaudio->setEnabled(0);
    ui->radioButtonSinglewindow->setEnabled(0);
    ui->radioButtonEntirescreen->setEnabled(0);
    ui->radioButtonCustom->setEnabled(0);
    ui->actionAbout->setEnabled(0);
    ui->actionSettings->setEnabled(0);
    ui->actionPreviewrecording->setEnabled(0);

    //Enables what needs to be enabled while recording
    ui->pushButtonStoprecord->setEnabled(1);

   //SystemTray
   //trayIcon->setIcon(QIcon(":/trolltech/styles/commonstyle/images/media-stop-16.png"));
   trayIcon->setIcon(QIcon(":/images/recording.png"));
   stoprecord->setEnabled(true);

   //Sets the MainWindow Icon
   setWindowIcon(QPixmap(":/images/recording.png"));

   //StatusBar
   ui->statusBar->showMessage(trUtf8("Recording") + " (" +QFileInfo(filename).fileName() + ")");

   stopwatchtimer = new QTimer(this);
   stopwatchtimeest = 0;
   connect(stopwatchtimer,SIGNAL(timeout()),this,SLOT(updateStopwatch()));
   stopwatchtimer->setInterval(1000);
   stopwatchtimer->start();
}



void MainWindow::on_pushButtonStoprecord_clicked()
{
    //Disables what needs to be disabled
    ui->pushButtonStoprecord->setEnabled(0);

    //Stopwatch
    stopwatchtimer->stop();
    delete stopwatchtimer;

    //StausBar
    ui->statusBar->showMessage(trUtf8("Please wait while saving the recording. Might take some time."));
    ui->statusBar->setUpdatesEnabled(0);

    //Run the stop command
    mProcessClass->stopCommand();
}

void MainWindow::onProcessFinished(int Exitcode)
{  
    //------------------SECTION: COMMON---------------------

    //StatusBar:
    ui->statusBar->setUpdatesEnabled(1);

    //SystemTray
    trayIcon->setIcon((QIcon)":/images/icon.png");
    stoprecord->setEnabled(false);

    //MainWindow Icon
    setWindowIcon(QPixmap(":/images/icon.png"));

    //Shows what needs to be shown on stop
    ui->pushButtonStartrecord->setVisible(1);

    //Hides what needs to be hidden on stop
    ui->pushButtonStoprecord->setVisible(0);

    //Enables what needs to be Enables on Stop:
    ui->checkBoxRecordaudio->setEnabled(1);
    ui->radioButtonEntirescreen->setEnabled(1);
    ui->radioButtonSinglewindow->setEnabled(1);
    ui->radioButtonCustom->setEnabled(1);
    ui->actionAbout->setEnabled(1);
    ui->actionSettings->setEnabled(1);
    ui->pushButtonStartrecord->setEnabled(1);
    ui->actionPreviewrecording->setEnabled(1);

    //removes the red rectangle
    rubberband.hide();


    //------------------SECTION: SUCCESS OR UNSUCCESS---------------------
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

    //Recording: Successful
    if(Exitcode == 0)
    {
        //StatusBar
        ui->statusBar->showMessage(trUtf8("Successfully finished recording") + " (" + trUtf8("Size") + ": " + filesizestring + " - " + QFileInfo(filename).fileName() + ")",5000);

        //Knotification
        doKnotification(trUtf8("Successfully finished recording"),"","normal","doneRecording");


        //CFG: Sets the new information
        settings.readAll();
        QString currentdatetime = QDateTime::currentDateTime().toString();
        settings.setLatestrecording(currentdatetime);
        settings.writeAll();

        //SystemTray: Reads information
        latestrecording->setText(trUtf8("Latest Recording") + ": " + currentdatetime);
    }
    //Recording: Failes
    else
    {
        //StatusBar
        ui->statusBar->showMessage(trUtf8("Unable to start recording!"));

        //Knotification
        doKnotification(trUtf8("Unable to start recording!"),trUtf8("View console output for more info."),"normal","errorRecording");

        //Shows the Console output Messagebox
        QMessageBox msgBox;

        //Creates the custom button
        QPushButton *showDetails = msgBox.addButton(trUtf8("Show output"), QMessageBox::ActionRole);

        //The rest of the combobox
        msgBox.setText(QString("<b>%1</b>").arg(trUtf8("Unable to start recording!")));
        msgBox.setInformativeText(trUtf8("Press <i>'Show output'</i> to see console ouput."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setWindowTitle(trUtf8("Unable to start recording!"));
        msgBox.exec();

        if(msgBox.clickedButton() == showDetails){
            if(ui->dockWidget->isHidden()){
                on_actionConsole_triggered();
            }
        }
    }

    //------------------SECTION: Final---------------------
    //Disconnections!
    disconnect(mProcessClass,SIGNAL(stderrText(QString)),ui->textEditConsole,SLOT(append(QString)));
    disconnect(mProcessClass,SIGNAL(stdoutText(QString)),ui->textEditConsole,SLOT(append(QString)));
    disconnect(mProcessClass, SIGNAL(FinishedExitCode(int)),this, SLOT(onProcessFinished(int)));
    disconnect(mProcessClass,SIGNAL(stderrText(QString)),this,SLOT(setRecordingStatusbarText()));
}


void MainWindow::on_actionAbout_triggered()
{
    AboutProg *AboutProgDialog = new AboutProg();
    //Gets where to place the Dialog.
    // Gets pos of MainWindow, adds the half of the width of the mainwindow (the middle) and then subtracts the haft of the width of the About dialog (the middle)
    int xWindowPos = MainWindow::pos().x() + (MainWindow::width()/2)-(AboutProgDialog->width()/2);
    int yWindowPos = MainWindow::pos().y() + (MainWindow::height()/2)-(AboutProgDialog->height()/2);

    AboutProgDialog->setGeometry(xWindowPos,yWindowPos,AboutProgDialog->width(),AboutProgDialog->height());
    AboutProgDialog->exec();
}



void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *dialogsettings = new SettingsDialog();
    dialogsettings->exec();
    settings.readAll();
}

void MainWindow::createsystemtray()
{
    //Defines the actions
    viewhidewindow = new QAction(tr("Show/Hide window"), this);
    connect(viewhidewindow, SIGNAL(triggered()), this, SLOT(showhidewindow()));
    viewhidewindow->setIcon(QIcon::fromTheme("dashboard-show"));

    stoprecord = new QAction(tr("Stop recording"), this);
    stoprecord->setIcon(QIcon(":images/icon.png"));
    connect(stoprecord, SIGNAL(triggered()), this, SLOT(on_pushButtonStoprecord_clicked()));

    latestrecording = new QAction(trUtf8("&Latest recording: "),this);
    latestrecording->setEnabled(false);
    latestrecording->setIcon(QIcon::fromTheme("dialog-information"));

    QString latestrecordingText = settings.getLatestrecording();

    latestrecording->setText(trUtf8("Latest Recording") + ": " + latestrecordingText);

    quitAction = new QAction(tr("Quit program"), this);
    quitAction->setIcon(QIcon::fromTheme("application-exit"));
    connect(quitAction, SIGNAL(triggered()), qApp , SLOT(quit()));

    // Makes the layout

    trayIcon = new QSystemTrayIcon;

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(latestrecording);
    trayIconMenu->addSeparator();;
    trayIconMenu->addAction(viewhidewindow);
    trayIconMenu->addAction(stoprecord);
    stoprecord->setEnabled(false);
    trayIconMenu->addAction(quitAction);

    //Sets the icon
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon((QIcon)":/images/icon.png");
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //Shows the icon
    trayIcon->show();
}

void MainWindow::doKnotification(QString title, QString text, QString iconType, QString EventName)
{
    //Creates the notification
    knotification = new KNotification(EventName);

    //Sets the title
    knotification->setTitle(title);

    //Sets the text
    knotification->setText(text);

    //Sets the pixmap
    if(iconType == "recording")
    {
        knotification->setPixmap(QPixmap(":images/recording.png"));
    }
    else if (iconType == "normal") {
        knotification->setPixmap(QPixmap(":images/icon.png"));
    }
    else if (iconType == "none") {
        knotification->setPixmap(QIcon::fromTheme("edit-copy").pixmap(22));
    }
    else {
        knotification->setPixmap(QPixmap(":images/icon.png"));
    }


    //Sends the event
    knotification->sendEvent();

    //Deletes it
    delete knotification;
}

// Handles what happens when the systemtray icon is clicked!
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if(stoprecord->isEnabled())
        {
            qDebug() << "Recording stopped";
            on_pushButtonStoprecord_clicked();
            if(MainWindow::isHidden()){
                showhidewindow();
            }
        }
        else
        {
            qDebug() << "No recording started, and there for nothing to stop";
            break;
        }
        break;
    case QSystemTrayIcon::DoubleClick:
        showhidewindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        ;
    }
}

void MainWindow::showhidewindow()
{
    if(MainWindow::isHidden())
    {
       MainWindow::show();
    }
    else
    {
        MainWindow::hide();
    }
}

void MainWindow::on_actionConsole_triggered()
{
    if(ui->dockWidget->isHidden())
    {
        MainWindow::setGeometry(MainWindow::pos().x(),MainWindow::pos().y(),MainWindow::frameGeometry().width(),250);
        MainWindow::setFixedHeight(250);
        ui->dockWidget->show();
    }
    else
    {
        //There is added 53.5 (107/2) to the Y-koordinat which prevents the window from "jumping" up and down console is opened an closed
        MainWindow::setGeometry(MainWindow::pos().x(),MainWindow::pos().y()+53.5,MainWindow::frameGeometry().width(),107);
        MainWindow::setFixedHeight(107);
        ui->dockWidget->hide();
    }
}

void MainWindow::setRecordingStatusbarText()
{
    //If message in statusbar is changed, then this will change it back to the information, so the user knows that the program is recording.
    if (ui->statusBar->currentMessage().isEmpty())
    {
        ui->statusBar->showMessage(trUtf8("Recording") + " (" +QFileInfo(filename).fileName() + ")");
    }

}

void MainWindow::getFPS(QString text)
{

    QRegExp rx("^frame");
    if(text.contains(rx)){
        QStringList wordlist = text.split(" ");

        QRegExp rx2("fps=");
        for(int i=0;i<wordlist.count();i++){
            if(wordlist.at(i).contains(rx2)){

                //for numbers above 10
                recordingFPS = wordlist.at(i+1).toDouble();

                //For numbers under 10
                QRegExp rx3("(\\d+)");
                QStringList list;
                int pos = 0;

                while ((pos = rx3.indexIn(wordlist.at(i), pos)) != -1) {
                    list << rx3.cap(1);
                    pos += rx3.matchedLength();
                    recordingFPS = list.at(0).toDouble();
                }

            }

        }
    }

}


void MainWindow::startRecordandminimize()
{
    showhidewindow();
    on_pushButtonStartrecord_clicked();
}

void MainWindow::on_actionOpen_recording_directory_triggered()
{
    QDesktopServices::openUrl( QUrl::fromLocalFile(settings.getFilenamePath()) );
}

void MainWindow::on_actionPreviewrecording_triggered()
{


//NOTE: Removed previewplayer. Kept the code, since I might add it again some day.
//    if(settings.getPreviewplayerintegrated() == "false")
//    {
//        mProcessClass->setCommand(settings.getPreviewplayer());
//        mProcessClass->setArguments(QStringList() << filename);
//        mProcessClass->startCommand();

    qDebug() << "Using default mediaplayer:" << settings.kdeplayerUsed();

    if(!settings.kdeplayerUsed()){
        qDebug() << "- Using alternative player:" << settings.getPreviewplayer();
        mProcessClass->setCommand(settings.getPreviewplayer());
        mProcessClass->setArguments(QStringList() << filename);
        mProcessClass->startCommand();
    }
    else {
        QDesktopServices::openUrl( QUrl::fromLocalFile(filename) );
    }

//    }
//    else
//    {
//        PreviewPlayer *playernew = new PreviewPlayer();
//        playernew->setVideofile(filename);
//        playernew->playVideo();
//        playernew->exec();
//        connect(playernew,SIGNAL(finished(int)),playernew,SLOT(deleteLater()));
//    }


}

void MainWindow::updateStopwatch()
{
    stopwatchtimeest +=1;
    QTime newtime;

    newtime = stopwatchtime.addSecs(stopwatchtimeest) ;
    QString text = newtime.toString("hh:mm:ss");

    QString fpstext = QString::number(recordingFPS);

    if(recordingFPS<10){
        fpstext = QString("~ ") + QString::number(recordingFPS);
    }

    setWindowTitle(QString("FDesktopRecorder") + QString(" (%1, FPS: %2)").arg(text,fpstext));
}

void MainWindow::on_pushButtonConsoleCopyToClipboard_clicked()
{
    doKnotification(trUtf8("Current output copied to clipboard"),"","none","consoleCopyClipboard");
}
