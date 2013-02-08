#include "process.h"
#include <QtGui>
#include <QtCore>

process::process(QObject *parent) :
    QObject(parent)
{
}

void process::startCommand()
{
    //Creates the new process.
    mprocess = new QProcess();

    //Sets the connections
    connect(mprocess,SIGNAL(readyReadStandardOutput()),this,SLOT(readstdout()));
    connect(mprocess,SIGNAL(readyReadStandardError()),this,SLOT(readstderr()));
    connect(mprocess, SIGNAL(finished(int, QProcess::ExitStatus)),
            this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));

    //ProcessChannelMode
    mprocess->setProcessChannelMode(QProcess::MergedChannels);

    //Stats the process
    mprocess->start(command,arguments);
}

void process::readstdout()
{
    stdoutdata = mprocess->readAllStandardOutput();
    emit stdoutText(stdoutdata);
    qDebug() << stdoutdata;
}


void process::readstderr()
{
    stderrdata = mprocess->readAllStandardError();
    emit stderrText(stderrdata);
    qDebug() << "Standard Error:" << stderrdata;
}

void process::onProcessFinished(int exitCode, QProcess::ExitStatus status)
{
    qDebug() << "Exitcode:" << exitCode;
    qDebug() << "Exitstatus:" << status;
    emit FinishedExitCode(exitCode);

    delete mprocess;
    mprocess = NULL;
}

void process::setCommand(QString newCommand)
{
    command = newCommand;
}

void process::setArguments(QStringList newArguments)
{
    arguments = newArguments;
}

void process::stopCommand()
{
    //This function is more or less from Juergen Heinemann's "qx11grab". Great work from a great person! :-)
    // Visit his project at: http://qt-apps.org/content/show.php?content=89204
    char q = 'q';
    if ( ( mprocess->write ( &q ) != -1 ) && ( mprocess->waitForBytesWritten () ) ){
          mprocess->closeWriteChannel();
    }
    else
    {
        mprocess->kill();
    }
}

