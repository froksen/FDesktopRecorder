#ifndef RUNTERMINAL_H
#define RUNTERMINAL_H

#include <QObject>
#include <QProcess>
#include <dialogterminaloutput.h>

class runTerminal : public QObject
{
    Q_OBJECT
public:
    explicit runTerminal(QObject *parent = 0);

    QString sayHello();

    QString statusDescription;

    //QProcess
    QProcess *process;
    QByteArray strdata;
    QByteArray stderrdata;



signals:

public slots:
    void runcmd(QString cmd, QStringList args);
    void readstdoutput();
    void readstderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void stopProcess();

private:
    DialogTerminalOutput *TermianlOutputDialog;

};

#endif // RUNTERMINAL_H
