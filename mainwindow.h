#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <runterminal.h>
#include <configurationfile.h>
#include <aboutprog.h>
#include <windowgrapper.h>

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
    void onProcessFinished();

private:
    Ui::MainWindow *ui;


    //fixResolution
    int fixResolution(int number);


    //Other
    runTerminal *runTerminalClass;
    ConfigurationFile *ConfigurationFileClass;
    WindowGrapper *WindowGrapperClass;
};

#endif // MAINWINDOW_H
