#ifndef RUNTERMINAL_H
#define RUNTERMINAL_H

#include <QObject>
#include <QProcess>

class runTerminal : public QObject
{
    Q_OBJECT
public:
    explicit runTerminal(QObject *parent = 0);

    QString sayHello();

    QString statusDescription;

    //QProcess
    QProcess *process;

signals:

public slots:
    void runcmd(QString cmd, QStringList args);
    void readstdoutput();
    void readstderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);
    void stopProcess();

private:

};

#endif // RUNTERMINAL_H
