#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:
    void on_buttonBox_accepted();

    void on_pushButtonRestore_clicked();

    void on_pushButtonpathBrowse_clicked();

    void on_checkBoxbasenametimedate_clicked();

    void on_checkBoxMicMute_clicked();

private:
    Ui::SettingsDialog *ui;

    //ffmpeg settings
    void writeSettings();
    void readSettings();


    //Application settings
    void getLanguages();
};

#endif // SETTINGSDIALOG_H
