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
    ui->pushButtonStoprecord->hide();


    //Sets pointers
    runTerminalClass = new runTerminal();
    ConfigurationFileClass = new ConfigurationFile();

    ConfigurationFileClass->setDefaults();

    createsystemtray();
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
    QString geometry = WindowGrapperClass->Fullscreenaspects();

    QString defaultpath = ConfigurationFileClass->getValue("defaultpath", "startupbehavior");
    QString defaultname = ConfigurationFileClass->getValue("defaultname", "startupbehavior");
    QString defaultformat = ConfigurationFileClass->getValue("defaultformat", "startupbehavior");

    QString filename = setFilename(defaultpath,defaultname,defaultformat);
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
    recordingargs << ":0.0";

    recordingargs << "-vcodec";
    recordingargs << videocodec;

    recordingargs << "-y";
    recordingargs << filename;

    qDebug() << recordingargs;

    runTerminalClass->runcmd("ffmpeg",recordingargs);

    recordingargs.clear();


    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished(int)));
    connect(runTerminalClass->process,SIGNAL(finished(int,QProcess::ExitStatus)),this,SLOT(onProcessFinished(int)));

   ui->checkBoxRecordaudio->setEnabled(0);
   ui->radioButtonEntirescreen->setEnabled(0);
   ui->actionAbout->setEnabled(0);
   ui->actionSettings->setEnabled(0);
   trayIcon->setIcon(QIcon(":/trolltech/styles/commonstyle/images/media-stop-16.png"));
   stoprecord->setEnabled(true);

   ui->statusBar->showMessage(trUtf8("Recording started") + " (" + filename + ")");
}

void MainWindow::on_pushButtonStoprecord_clicked()
{
    runTerminalClass->stopProcess();
}

void MainWindow::onProcessFinished(int Exitcode)
{
    if(Exitcode == 0)
    {
        ui->statusBar->showMessage(trUtf8("Finished recording successfully"));
    }
    else
    {
        ui->statusBar->showMessage(trUtf8("Failed to start recording!"));
    }
//    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
//    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
//    disconnect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished()));


//    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
//    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

    if(ui->pushButtonStartrecord->isEnabled())
    {
        ui->checkBoxRecordaudio->setEnabled(1);
        ui->radioButtonEntirescreen->setEnabled(1);
        ui->actionAbout->setEnabled(1);
        ui->actionSettings->setEnabled(1);

        trayIcon->setIcon((QIcon)":/images/icon.png");
        stoprecord->setEnabled(false);
    }
}


void MainWindow::on_actionAbout_triggered()
{
    AboutProg *AboutProgDialog = new AboutProg();
    AboutProgDialog->show();
}


void MainWindow::on_actionShow_Terminal_output_triggered()
{
    DialogTerminalOutput *TerminalOutputDialog = new DialogTerminalOutput();
    TerminalOutputDialog->show();
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

//    settingsfile.beginGroup("misc");
//    if(settingsfile.contains("latestrecording"))
//    {
//        latestrecording->setText(trUtf8("Latest recording: ") + settingsfile.value("latestrecording").toString());
//        qDebug() << "Latest recording: " + settingsfile.value("latestrecording").toString();
//    }
//    settingsfile.endGroup();

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
    //trayIcon->setIcon(QIcon(":/trolltech/styles/commonstyle/images/media-stop-16.png"));
    //connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            //this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    //Shows the icon
    trayIcon->show();
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
