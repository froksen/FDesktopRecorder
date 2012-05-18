#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QtGui>
#include <QtCore>
#include <QSystemTrayIcon>
#include "previewplayer/previewplayer.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Makes sure that the program can run i bg
    //QApplication::setQuitOnLastWindowClosed(false);

    ui->actionPreviewrecording->setEnabled(0);

    //Hides objects that needs to be hidden
    ui->dockWidget->hide();
    ui->pushButtonStoprecord->hide();

    //Sets the MainWindow size
    MainWindow::setFixedHeight(107);
    QRect r = MainWindow::geometry();
    r.moveCenter(QApplication::desktop()->availableGeometry().center());

    //Reads settingsfile
    settings.readAll();
    qDebug() << "Reading settings on startup";

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
    if(settings.getMicrophonemuted() != "false")
    {
        ui->checkBoxRecordaudio->setChecked(1);
        qDebug() << "Microphone: Enabled";
    }

    //Creates systemtrayp
    createsystemtray();


    //Sets the icon for the Toolbar
    ui->actionAbout->setIcon(QIcon::fromTheme("help-about"));
    ui->actionConsole->setIcon(QIcon::fromTheme("utilities-terminal"));
    ui->actionSettings->setIcon(QIcon::fromTheme("preferences-system"));
    ui->actionOpen_recording_directory->setIcon(QIcon::fromTheme("system-file-manager"));
    ui->actionPreviewrecording->setIcon(QIcon::fromTheme("package_multimedia_tv"));
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
    qDebug() << "Reading settings";
    settings.readAll();

    QStringList recordingargs;
    recordingargs.clear();
    //------------------------SECTION: Other--------------------------------
    QString videocodec = settings.getVideocodec();
    QString audiocodec = settings.getAudiocodec();
    int audiochannels = settings.getAudiochannles();
    int fps = settings.getFramerate();

    //------------------------SECTION: Geometry--------------------------------
    QString geometry;
    QString corners;

    //If Single Window radio is checked, then to this. Else record fullscreen :-)
    if(ui->radioButtonSinglewindow->isChecked())
    {
        //Informing the user about how to proceed
        QMessageBox msgBox;
        msgBox.setText(trUtf8("<b> Recording a single window </b>"));
        msgBox.setWindowTitle(trUtf8("Recording a single window"));
        msgBox.setInformativeText(trUtf8("When you click 'OK' a small crossair will appear. \n \nWith this you will have to select the window you want to record. \n\nHereafter the recording will start."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.setFixedWidth(520);
        int ret = msgBox.exec();

        //Starting the singlewindow setup
        QProcess p;
        QStringList argsscript;
        argsscript << "-frame";
        p.start("xwininfo",argsscript);
        p.waitForFinished(-1);

        QString p_stdout = p.readAllStandardOutput();
        QString p_stderr = p.readAllStandardError();

        geometry = WindowGrapperClass->Singlewindowgeometry(p_stdout);
        corners = WindowGrapperClass->Singlewindowcorners(p_stdout);

        //Sets the red rectangle arround the area that is going to be recorded (QRubberband)

        //The size of the window:
        rubberband.setGeometry(0,0,WindowGrapperClass->SinglewindowWidth(p_stdout).toInt()+5,WindowGrapperClass->SinglewindowHeight(p_stdout).toInt()+5);

        //The position of the window
        QString singleCorners = corners;
        singleCorners.remove(0,5);
        QStringList singleCornersList = singleCorners.split(",");
        rubberband.move(QString(singleCornersList[0]).toInt()-2,QString(singleCornersList[1]).toInt()-2);

        //Shows the rectangle
        if(settings.getSinglewindow_redrectangle() != "false")
        {
            rubberband.show();
        }
    }
    else
    {
        geometry = WindowGrapperClass->Fullscreenaspects();
        corners = ":0.0";
    }




    //------------------------SECTION: Filename--------------------------------
    //Reads some defaults from CFG file
    QString defaultpath = settings.getFilenamePath();
    QString defaultnamedatetime = settings.getFilenameUsedate();
    QString defaultname;
    QString defaultformat = settings.getFormat();

    if(defaultnamedatetime != "true")
    {
       defaultname = settings.getFilenameBase();
    }
    else
    {
        defaultname = QDateTime::currentDateTime().toString();
    }

    //Sets the final filname!
    filename = setFilename(defaultpath,defaultname,defaultformat);


    //------------------------SECTION: Microphone--------------------------------
    //Microphone: reads the CFG file to find which is predefined!
    QString recordingdevice = settings.getMicrophonedevice();

    //if NOT Mute-Microphone checkbox is checked, then it will add this section.
    if(! ui->checkBoxRecordaudio->isChecked())
    {
        recordingargs << "-f";
        recordingargs << settings.getAudiosource();
        recordingargs << "-ac";
        recordingargs << QString::number(audiochannels);
        recordingargs << "-i";
        recordingargs << recordingdevice;
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
    recordingargs << QString::number(fps);

    //Argument: Geometry/Corners
    recordingargs << "-s";
    recordingargs << geometry;
    recordingargs << "-i";
    recordingargs << corners;

    //Argument: VideoCodec
    recordingargs << "-vcodec";
    recordingargs << videocodec;

    //Argument: vpre
    if(settings.getUsevpre() == "true")
    {
        recordingargs << "-vpre";
        recordingargs << settings.getVpre();
    }

    //Argument: Filename
    recordingargs << "-y";
    recordingargs << filename;

    qDebug() << recordingargs;

    //------------------------SECTION: Set and run the recording--------------------------------
    //Sets the arguments to the program
    mProcessClass.setArguments(recordingargs);
    recordingargs.clear();

    //Sets the program
    mProcessClass.setCommand("ffmpeg");

    //Starts the command
    mProcessClass.startCommand();

    //Connections used by the QProcess
    ui->textEditConsole->clear();
    connect(mProcessClass.mprocess, SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));
    connect(mProcessClass.mprocess,SIGNAL(finished(int)),this,SLOT(onProcessFinished(int)));

    //------------------------SECTION: GUI things--------------------------------

    //Hides what needs to be hidden while recording
    ui->pushButtonStartrecord->setVisible(0);

    //Shows what needs to be shown while recording
    ui->pushButtonStoprecord->setVisible(1);

    //Disables what needs to be disabled while recording
    ui->checkBoxRecordaudio->setEnabled(0);
    ui->radioButtonSinglewindow->setEnabled(0);
    ui->radioButtonEntirescreen->setEnabled(0);
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
   ui->statusBar->showMessage(trUtf8("Recording started") + " (" + filename + ")");

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

    //StausBar
    ui->statusBar->showMessage(trUtf8("Please wait while saving the recording. Might take some time."));
    ui->statusBar->setUpdatesEnabled(0);

    //Run the stop command
    mProcessClass.stopCommand();
}

void MainWindow::onProcessFinished(int Exitcode)
{  
    //------------------SECTION: COMMON---------------------
    //Stopwatch
    stopwatchtimer->stop();
    delete stopwatchtimer;

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
        filesizestring =  QString("%1 GB").arg(filesize/GB);
    }
    else if(filesize > MB)
    {
        filesizestring =  QString("%1 MB").arg(filesize/MB);
    }
    else if(filesize > KB)
    {
        filesizestring =  QString("%1 KB").arg(filesize/KB);
    }
    else
    {
        filesizestring =  QString("%1 K").arg(filesize);
    }

    //Recording: Successful
    if(Exitcode == 0)
    {
        //StatusBar
        ui->statusBar->showMessage(trUtf8("Successfully finished recording") + " (" + trUtf8("Size") + ": " + filesizestring + " - " + filename + ")");

        //Knotification
        knotification = new KNotification("doneRecording");
        knotification->setTitle(trUtf8("Successfully finished recording"));
        knotification->setPixmap(QPixmap(":images/icon.png"));
        knotification->sendEvent();
        delete knotification;

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
        ui->statusBar->showMessage(trUtf8("Failed to start!"));

        //Knotification
        knotification = new KNotification("errorRecording");
        knotification->setTitle(trUtf8("Failed to start!"));
        knotification->setText(trUtf8("View terminaloutput for more info."));
        knotification->setPixmap(QPixmap(":images/icon.png"));
        knotification->sendEvent();
        delete knotification;

        //Shows the TerminalOutput Messagebox
        QMessageBox msgBox;    msgBox.setText(trUtf8("<b>Failed to start recording!</b>"));
        msgBox.setInformativeText(trUtf8("Press <i>'show details'</i> to see console ouput."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDetailedText(QString(ui->textEditConsole->toPlainText()));
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setWindowTitle(trUtf8("Failed to start recording!"));
        int ret = msgBox.exec();
    }

    //------------------SECTION: Final---------------------
    //Disconnections!
    disconnect(mProcessClass.mprocess, SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));
    disconnect(mProcessClass.mprocess,SIGNAL(finished(int)),this,SLOT(onProcessFinished(int)));
}


void MainWindow::on_actionAbout_triggered()
{
    AboutProg *AboutProgDialog = new AboutProg();
    AboutProgDialog->show();
}



void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog *dialogsettings = new SettingsDialog();
    dialogsettings->exec();
}

void MainWindow::createsystemtray()
{
    //Defines the actions
    viewhidewindow = new QAction(tr("&Show/Hide window"), this);
    connect(viewhidewindow, SIGNAL(triggered()), this, SLOT(showhidewindow()));
    viewhidewindow->setIcon(QIcon::fromTheme("dashboard-show"));

    stoprecord = new QAction(tr("&Stop recording"), this);
    stoprecord->setIcon(QIcon(":images/icon.png"));
    connect(stoprecord, SIGNAL(triggered()), this, SLOT(on_pushButtonStoprecord_clicked()));

    latestrecording = new QAction(trUtf8("&Latest recording: "),this);
    latestrecording->setEnabled(false);
    latestrecording->setIcon(QIcon::fromTheme("dialog-information"));

    QString latestrecordingText = settings.getLatestrecording();

    latestrecording->setText(trUtf8("Latest Recording") + ": " + latestrecordingText);

    quitAction = new QAction(tr("&Quit program"), this);
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

// Handles what happens when the systemtray icon is clicked!
void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
        if(stoprecord->isEnabled())
        {
            qDebug() << "Recording stopped";
            on_pushButtonStoprecord_clicked();
            showhidewindow();
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
//    TerminalWindowDialog = new TerminalWindow();
//    TerminalWindowDialog->show();

    if(ui->dockWidget->isHidden())
    {
        MainWindow::setGeometry(MainWindow::pos().x(),MainWindow::pos().y(),MainWindow::frameGeometry().width(),227);
        MainWindow::setFixedHeight(227);
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

void MainWindow::readstderr()
{
    QByteArray stderrdata = mProcessClass.stderrdata;
    ui->textEditConsole->append(stderrdata);

    //If message in statusbar is changed, then this will change it back to the information, so the user knows that the program is recording.
    if (ui->statusBar->currentMessage().isEmpty())
    {
        ui->statusBar->showMessage(trUtf8("Recording started") + " (" +filename + ")");
    }

}

void MainWindow::readstdout()
{
    QByteArray stdoutdata = mProcessClass.stdoutdata;
    ui->textEditConsole->append(stdoutdata);

    if (ui->statusBar->currentMessage().isEmpty())
    {
         ui->statusBar->showMessage(trUtf8("Recording started") + " (" + filename + ")");
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


    if(settings.getPreviewplayerintegrated() == "false")
    {
        mProcessClass.setCommand(settings.getPreviewplayer());
        mProcessClass.setArguments(QStringList() << filename);
        mProcessClass.startCommand();
    }
    else
    {
        PreviewPlayer *playernew = new PreviewPlayer();
        playernew->setVideofile(filename);
        playernew->playVideo();
        playernew->exec();
        connect(playernew,SIGNAL(finished(int)),playernew,SLOT(deleteLater()));
    }


}

void MainWindow::updateStopwatch()
{
    stopwatchtimeest +=1;
    QTime newtime;

    newtime = stopwatchtime.addSecs(stopwatchtimeest) ;
    QString text = newtime.toString("hh:mm:ss");
    qDebug() << "Stopwatch:" << text;

    setWindowTitle(trUtf8("FDesktopRecorder") + QString(" (%1)").arg(text));
}
