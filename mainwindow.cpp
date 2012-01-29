#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonStoprecord->hide();

    ConfigurationFileClass = new ConfigurationFile();
    runTerminalClass = new runTerminal();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonStartrecord_clicked()
{
    QStringList recordingargs;

    //Loads values
    QString videocodec = ConfigurationFileClass->getValue("videocodec","record");
    QString audiocodec = ConfigurationFileClass->getValue("audiocodec","record");
    QString audiochannels = ConfigurationFileClass->getValue("audiochannels","record");
    QString fps = ConfigurationFileClass->getValue("fps","record");
    QString geometry = WindowGrapperClass->Fullscreenaspects();

    //Adds the arguments to the ffmpeg script. See above!
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
    recordingargs << "/home/froksen/optagelseny.avi";

    qDebug() << recordingargs;

    runTerminalClass->runcmd("ffmpeg",recordingargs);


    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
    connect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished()));
}

void MainWindow::on_pushButtonStoprecord_clicked()
{
    runTerminalClass->stopProcess();
}

void MainWindow::onProcessFinished()
{
    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStartrecord,SLOT(show()));
    disconnect(runTerminalClass->process, SIGNAL(finished(int)),ui->pushButtonStoprecord,SLOT(hide()));
    disconnect(runTerminalClass->process, SIGNAL(finished(int)),this,SLOT(onProcessFinished()));

    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStartrecord,SLOT(hide()));
    disconnect(runTerminalClass->process, SIGNAL(started()),ui->pushButtonStoprecord,SLOT(show()));

}


void MainWindow::on_actionAbout_triggered()
{
    AboutProg *AboutProgDialog = new AboutProg();
    AboutProgDialog->show();
}

