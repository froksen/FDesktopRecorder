#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileInfo>
#include <QtGui>
#include <QtCore>
#include <QSystemTrayIcon>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //Makes sure that the program can run i bg
    //QApplication::setQuitOnLastWindowClosed(false);


    //Hides objects that needs to be hidden
    ui->dockWidget->hide();
    ui->pushButtonStoprecord->hide();

    //Sets the MainWindow size
    MainWindow::setFixedHeight(107);
    QRect r = MainWindow::geometry();
    r.moveCenter(QApplication::desktop()->availableGeometry().center());

    //Sets pointers to other dialogs
    runTerminalClass = new runTerminal();
    ConfigurationFileClass = new ConfigurationFile();

    //If there is no cfg file. it will be set.
    ConfigurationFileClass->setDefaults();


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
    if(ConfigurationFileClass->getValue("defaultrecorddeviceMute","startupbehavior") != "false")
    {
        ui->checkBoxRecordaudio->setChecked(1);
    }

    //Creates systemtrayp
    createsystemtray();


}

MainWindow::~MainWindow()
{
    delete ui;
}

//This function appends a number to the end of a filename if filename exists!
QString MainWindow::setFilename(QString path, QString basename, QString format)
{
    QFileInfo Filename = path + "/" + basename + "." + format;
    int indexnumber = 1;
    while(Filename.exists())
    {
        Filename = path + "/" + basename + QString::number(indexnumber) + "." + format;
        indexnumber += 1;
    }
    QString Filenamestr = Filename.absoluteFilePath();
    return Filenamestr;
}


//This function handles what happens when Start Record is clicked!
void MainWindow::on_pushButtonStartrecord_clicked()
{
    QStringList recordingargs;
    recordingargs.clear();

    //Loads values
    ConfigurationFileClass = new ConfigurationFile();
    QString videocodec = ConfigurationFileClass->getValue("videocodec","record");
    QString audiocodec = ConfigurationFileClass->getValue("audiocodec","record");
    QString audiochannels = ConfigurationFileClass->getValue("audiochannels","record");
    QString fps = ConfigurationFileClass->getValue("fps","record");

    QString geometry;
    QString corners;
    if(ui->radioButtonSinglewindow->isChecked())
    {
        QProcess p;
        QStringList argsscript;
        argsscript << "-frame";
        p.start("xwininfo",argsscript);
        p.waitForFinished(-1);

        QString p_stdout = p.readAllStandardOutput();
        QString p_stderr = p.readAllStandardError();

        geometry = WindowGrapperClass->Singlewindowgeometry(p_stdout);
        corners = WindowGrapperClass->Singlewindowcorners(p_stdout);
    }
    else
    {
        geometry = WindowGrapperClass->Fullscreenaspects();
        corners = ":0.0";
    }


    QString defaultpath = ConfigurationFileClass->getValue("defaultpath", "startupbehavior");

    QString defaultnamedatetime = ConfigurationFileClass->getValue("defaultnametimedate","startupbehavior");

    QString defaultname;
    if(defaultnamedatetime != "true")
    {
       defaultname = ConfigurationFileClass->getValue("defaultname", "startupbehavior");
    }
    else
    {
        defaultname = QDateTime::currentDateTime().toString();
    }


    QString defaultformat = ConfigurationFileClass->getValue("defaultformat", "startupbehavior");

    filename = setFilename(defaultpath,defaultname,defaultformat);
    qDebug() << "Filename:" << filename;

    QString recordingdevice = ConfigurationFileClass->getValue("defaultrecorddevice","startupbehavior");

    //Adds the arguments to the ffmpeg script. See above!
    if(! ui->checkBoxRecordaudio->isChecked())
    {
        recordingargs << "-f";
        recordingargs << "alsa";
        recordingargs << "-ac";
        recordingargs << "2";
        recordingargs << "-i";
        recordingargs << recordingdevice;
    }

    recordingargs << "-f";
    recordingargs << "x11grab";

    recordingargs << "-r";
    recordingargs << fps;

    recordingargs << "-s";
    recordingargs << geometry;
    recordingargs << "-i";
    recordingargs << corners;

    recordingargs << "-vcodec";
    recordingargs << videocodec;

    recordingargs << "-y";
    recordingargs << filename;

    qDebug() << recordingargs;

    runTerminalClass->runcmd("ffmpeg",recordingargs);

    recordingargs.clear();


    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

    ui->textEditConsole->clear();
    connect(runTerminalClass->process, SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));

    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished(int)));
    connect(runTerminalClass->process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onProcessFinished(int)));

   ui->pushButtonStoprecord->setEnabled(1);
   ui->checkBoxRecordaudio->setEnabled(0);
   ui->radioButtonSinglewindow->setEnabled(0);
   ui->radioButtonEntirescreen->setEnabled(0);
   ui->actionAbout->setEnabled(0);
   ui->actionSettings->setEnabled(0);
   trayIcon->setIcon(QIcon(":/trolltech/styles/commonstyle/images/media-stop-16.png"));
   stoprecord->setEnabled(true);

   ui->statusBar->showMessage(trUtf8("Recording started") + " (" + filename + ")");
}



void MainWindow::on_pushButtonStoprecord_clicked()
{
    ui->pushButtonStoprecord->setEnabled(0);
    ui->statusBar->showMessage(trUtf8("Please wait while saving the recording. Might take some time."));
    runTerminalClass->stopProcess();
}

void MainWindow::onProcessFinished(int Exitcode)
{
    if(Exitcode == 0)
    {
        ui->statusBar->showMessage(trUtf8("Successfully finished recording") + " (" + filename + ")");

        QString currentdatetime = QDateTime::currentDateTime().toString();
        ConfigurationFileClass->configurationfile.beginGroup("misc");
        ConfigurationFileClass->configurationfile.setValue("latestrecording",currentdatetime);
        ConfigurationFileClass->configurationfile.endGroup();


        latestrecording->setText(trUtf8("Latest Recording") + ": " + currentdatetime);

    }
    else
    {
        ui->statusBar->showMessage(trUtf8("Failed to recording!"));
        QMessageBox msgBox;    msgBox.setText(trUtf8("Failed to start recording!"));
        msgBox.setInformativeText(trUtf8("Press 'show details' to see console ouput."));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDetailedText(QString(ui->textEditConsole->toPlainText()));
        msgBox.setDefaultButton(QMessageBox::Save);
        msgBox.setFixedWidth(520);
        int ret = msgBox.exec();

    }


    if(ui->pushButtonStartrecord->isEnabled())
    {
        ui->checkBoxRecordaudio->setEnabled(1);
        ui->radioButtonEntirescreen->setEnabled(1);
        ui->radioButtonSinglewindow->setEnabled(1);
        ui->actionAbout->setEnabled(1);
        ui->actionSettings->setEnabled(1);

        trayIcon->setIcon((QIcon)":/images/icon.png");
        stoprecord->setEnabled(false);
    }


    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

    disconnect(runTerminalClass->process, SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));

    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
    disconnect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished(int)));
    disconnect(runTerminalClass->process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onProcessFinished(int)));
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

    stoprecord = new QAction(tr("&Stop recording"), this);
    connect(stoprecord, SIGNAL(triggered()), this, SLOT(on_pushButtonStoprecord_clicked()));

    latestrecording = new QAction(trUtf8("&Latest recording: "),this);
    latestrecording->setEnabled(false);

    ConfigurationFileClass = new ConfigurationFile();
    QString latestrecordingText = ConfigurationFileClass->getValue("latestrecording","misc");

    latestrecording->setText(trUtf8("Latest Recording") + ": " + latestrecordingText);

    quitAction = new QAction(tr("&Quit program"), this);
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
        MainWindow::setGeometry(MainWindow::pos().x(),MainWindow::pos().y(),510,227);
        MainWindow::setFixedHeight(227);
        ui->dockWidget->show();
    }
    else
    {
        MainWindow::setGeometry(MainWindow::pos().x(),MainWindow::pos().y(),510,107);
        MainWindow::setFixedHeight(107);
        ui->dockWidget->hide();
    }
}

void MainWindow::readstderr()
{
    QByteArray stderrdata = runTerminalClass->stderrdata;
    ui->textEditConsole->append(stderrdata);

    //If message in statusbar is changed, then this will change it back to the information, so the user knows that the program is recording.
    if (ui->statusBar->currentMessage().isEmpty())
    {
         ui->statusBar->showMessage(trUtf8("Recording started") + " (" + filename + ")");
    }

}

void MainWindow::readstdout()
{
    QByteArray stdout = runTerminalClass->strdata;
    ui->textEditConsole->append(stdout);

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
    QString defaultpath = ConfigurationFileClass->getValue("defaultpath", "startupbehavior");

    QString path = defaultpath;

    QDesktopServices::openUrl( QUrl::fromLocalFile(path) );
}
