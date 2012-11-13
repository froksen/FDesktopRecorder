#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "dialogs/about/aboutprog.h"
#include "utils/windowgrapper.h"
#include "dialogs/settingsdialog/settingsdialog.h"
#include "plugins/pluginmanager.h"
#include <QtGui>
#include <QtCore>
#include <QMainWindow>
#include "module/process.h"
#include "module/settingsmanager.h"
#include <knotification.h>
#include <QPainter>
#include "utils/rubberband.h"
#include <QTime>
#include <QTimer>
#include <QClipboard>

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

    //Pushbutton Menu slots
    void startRecordandminimize();

    //Systemtray slots
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_actionOpen_recording_directory_triggered();
    void on_actionPreviewrecording_triggered();

    //The stopwatch
    void updateStopwatch();

    //Process stuff
    void setRecordingStatusbarText();

    //Gets the FPS from the terminaloutput
    void getFPS(QString text);


    void on_pushButtonConsoleCopyToClipboard_clicked();

private:
    Ui::MainWindow *ui;


    //fixResolution
    int fixResolution(int number);
    QString setFilename(QString path, QString basename, QString format);


    //Variables used in recordingscript
    QString filename;


    //Other
    WindowGrapper *WindowGrapperClass;
    process *mProcessClass;
    SettingsManager settings;
    RubberBand rubberband;

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

    //KNotification
    void doKnotification(QString title,QString text,QString iconType,QString EventName);

    KNotification *knotification;

    //The stopwatch
    int stopwatchtimeest;
    QTime stopwatchtime;
    QTimer *stopwatchtimer;

    //FPS
    double recordingFPS;

    //Plugin
    PluginManager *mPluginManger;


protected:
    struct recordinginfo{

            //Basic things
            QString title;
            int audiochannels;
            int fps;
            QString recordingdevice;

            //Codecs
            QString videocodec;
            QString audiocodec;

            //Geometry
            QString geometry;
            QString corners;

            //Fileinformation
            QString defaultpath;
            QString defaultname;
            QString defaultformat;
        };

};

#endif // MAINWINDOW_H
