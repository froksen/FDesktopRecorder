#ifndef PROCESS_H
#define PROCESS_H
#include <QtGui>
#include <QtCore>


class process : public QObject
{
    Q_OBJECT

private:
    QString command;
    QStringList arguments;


public:
    explicit process(QObject *parent = 0);
    QProcess *mprocess;
    QByteArray stderrdata;
    QByteArray stdoutdata;

    void setCommand(QString newCommand);
    void setArguments(QStringList newArguments);
    void startCommand();
    void stopCommand();

signals:
    void stdoutText(QString text);
    void stderrText(QString text);

private slots:
    void readstdout();
    void readstderr();
    void onProcessFinished(int exitCode, QProcess::ExitStatus status);



};

#endif // PROCESS_H
