#ifndef RUNTERMINAL_H
#define RUNTERMINAL_H

#include <QWidget>
#include <QProcess>

class runTerminal : public QWidget
{
    Q_OBJECT
public:
    explicit runTerminal(QWidget *parent = 0);

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
};

#endif // RUNTERMINAL_H
