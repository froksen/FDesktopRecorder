#include "runterminal.h"
#include <QDebug>
#include <QProcess>
#include <mainwindow.h>

runTerminal::runTerminal(QObject *parent) : QObject(parent)
{
    TermianlOutputDialog = new DialogTerminalOutput;
}

QString runTerminal::sayHello()
{
    return "Hello";
}

void runTerminal::runcmd(QString cmd, QStringList args)
{
    QString tmparg;
    foreach(tmparg,args)
    {
        tmparg += tmparg;
    }

    qDebug() << tmparg;
    qDebug () << "Command to run:" << cmd;

    //Creates the new process.
    process = new QProcess(this);

    //Stats the process
    process->start(cmd, args);

    //Sets the connections
    connect(process,SIGNAL(readyReadStandardOutput()),this,SLOT(readstdoutput()));
    connect(process,SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));
    connect(process, SIGNAL(finished(int, QProcess::ExitStatus)),this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));

}

void runTerminal::readstdoutput()
{
    strdata = process->readAllStandardOutput();
    qDebug() << "Standard output:" << strdata;
}

void runTerminal::readstderr()
{
    stderrdata = process->readAllStandardError();
    qDebug() << "Standard Error:" <<stderrdata;
    TermianlOutputDialog->stderroutput(stderrdata);
}

void runTerminal::onProcessFinished(int exitCode, QProcess::ExitStatus status)
{

    qDebug() << "Exitcode:" << exitCode << "\nExitstatus:" << status;

    if(exitCode == 0)
    {
        statusDescription = trUtf8("Done recording");
    }
    else
    {
        statusDescription = trUtf8("Recording failed!");

    }

    qDebug() << statusDescription;

    delete process;
    process = NULL;

}

void runTerminal::stopProcess()
{
    process->kill();
}
