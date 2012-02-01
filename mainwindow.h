#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <runterminal.h>
#include <configurationfile.h>
#include <aboutprog.h>
#include <windowgrapper.h>
#include <settingsdialog.h>
#include <QtGui>
#include <QtCore>
#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButtonStartrecord_clicked();
    void on_pushButtonStoprecord_clicked();
    void on_actionAbout_triggered();
    void onProcessFinished(int Exitcode);
    void on_actionSettings_triggered();
    void showhidewindow();
    void on_actionConsole_triggered();

    void readstderr();
    void readstdout();

private:
    Ui::MainWindow *ui;


    //fixResolution
    int fixResolution(int number);
    QString setFilename(QString path, QString basename, QString format);


    //Other
    runTerminal *runTerminalClass;
    ConfigurationFile *ConfigurationFileClass;
    WindowGrapper *WindowGrapperClass;

    //Systemtray other
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    void createsystemtray();

    //Systemtray Actions
    QAction *viewhidewindow;
    QAction *stoprecord;
    QAction *quitAction;
    QAction *latestrecording;
    QAction *startFullscreenrecording;
    QAction *startWindowrecording;


};

#endif // MAINWINDOW_H
