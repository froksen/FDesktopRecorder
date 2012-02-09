#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <configurationfile.h>
#include <recordingdevices.h>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);


    getLanguages();

        readSettings();

    on_checkBoxMicMute_clicked();
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::writeSettings()
{
    ConfigurationFile *ConfigurationFileClass = new ConfigurationFile();

    ConfigurationFileClass->configurationfile.beginGroup("record");
    ConfigurationFileClass->configurationfile.setValue("videocodec",ui->lineEditvideocodec->text());
    ConfigurationFileClass->configurationfile.setValue("audiocodec",ui->lineEditaudiocodec->text());

    ConfigurationFileClass->configurationfile.setValue("fps",ui->spinBoxfps->value());
    ConfigurationFileClass->configurationfile.setValue("audiochannels",ui->spinBoxaudiochannels->value());
    ConfigurationFileClass->configurationfile.endGroup();


    ConfigurationFileClass->configurationfile.beginGroup("startupbehavior");

    int comboboxindex = ui->comboBoxrecording->currentIndex();
    ConfigurationFileClass->configurationfile.setValue("defaultrecorddevice",ui->comboBoxrecording->itemData(comboboxindex));

    if(ui->checkBoxMicMute->isChecked())
    {
        ConfigurationFileClass->configurationfile.setValue("defaultrecorddeviceMute","true");
    }
    else
    {
        ConfigurationFileClass->configurationfile.setValue("defaultrecorddeviceMute","false");
    }


    if(ui->lineEditbasename->text().length() > 0 && ui->lineEditbasename->text()!="" && ui->lineEditbasename->text()!=" ")
    {
        ConfigurationFileClass->configurationfile.setValue("defaultname",ui->lineEditbasename->text());
    }
    else
    {
        ConfigurationFileClass->configurationfile.setValue("defaultname",trUtf8("recording"));
    }


    if(ui->checkBoxbasenametimedate->isChecked())
    {
        ConfigurationFileClass->configurationfile.setValue("defaultnametimedate","true");
    }
    else
    {
        ConfigurationFileClass->configurationfile.setValue("defaultnametimedate","false");
    }
    ConfigurationFileClass->configurationfile.setValue("defaultpath",ui->lineEditpath->text());

    int comboxformatindex = ui->comboBoxFormat->currentIndex();
    ConfigurationFileClass->configurationfile.setValue("defaultformat",ui->comboBoxFormat->itemData(comboxformatindex));

    int comboboxLanguage = ui->comboBoxLanguage->currentIndex();
    ConfigurationFileClass->configurationfile.setValue("language",ui->comboBoxLanguage->itemData(comboboxLanguage));

    ConfigurationFileClass->configurationfile.endGroup();
    ConfigurationFileClass->configurationfile.sync();

    delete ConfigurationFileClass;
    ConfigurationFileClass = NULL;

}

void SettingsDialog::on_buttonBox_accepted()
{
    writeSettings();
}

void SettingsDialog::on_pushButtonRestore_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("..::" + trUtf8("Restore everything to default?")+ "::..");
    msgBox.setInformativeText(trUtf8("If you press Yes everything will be restored to default. \n\n Be aware that this can NOT be undone."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //msgBox.setDetailedText(QString(ui->textEditConsole->toPlainText()));
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setFixedWidth(520);
    int ret = msgBox.exec();




    switch (ret)  {
      case QMessageBox::Yes:
            {
                ui->checkBoxMicMute->setChecked(1);
                ui->comboBoxrecording->setEnabled(0);

                ConfigurationFile *ConfigurationFileClass = new ConfigurationFile();
                ConfigurationFileClass->configurationfile.clear();
                ConfigurationFileClass->setDefaults();
                readSettings();

                delete ConfigurationFileClass;
                ConfigurationFileClass = NULL;
              break;
            }
      case QMessageBox::No:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
          break;
      default:
          // should never be reached
          break;
    }
}

void SettingsDialog::readSettings()
{

    RecordingDevices *RecordingDevicesclass = new RecordingDevices();
    RecordingDevicesclass->getRecorddevices();

    //Creates the combobx containing the recording devices.
    int index = 0;
    ui->comboBoxrecording->clear();
    foreach(QString device,RecordingDevicesclass->RecordDeviceHW)
    {
        ui->comboBoxrecording->addItem(RecordingDevicesclass->RecordDeviceDesc[index],device);

        index += 1;
    }

    QStringList ItemFormatList;
    //Addes the diffent formats. PLEASE NOTE: the order is important.
    ItemFormatList << "mkv";
    ItemFormatList << "avi";


    QStringList ItemDescList;
    ItemDescList << "MKV - Matroska Multimedia Container";
    ItemDescList << "AVI - Audio Video Interleaver";


    int Indexnumber = 0;
    foreach(QString format,ItemFormatList)
    {
        ui->comboBoxFormat->addItem(ItemDescList[Indexnumber],format);
        Indexnumber += 1;
    }


    ConfigurationFile *ConfigurationFileClass = new ConfigurationFile();
    //Reads and sets the value from the cfg file
    QString recordingdevice = ConfigurationFileClass->getValue("defaultrecorddevice","startupbehavior");
    int comboboxIndex = ui->comboBoxrecording->findData(recordingdevice);
    ui->comboBoxrecording->setCurrentIndex(comboboxIndex);

    QString formatcfg = ConfigurationFileClass->getValue("defaultformat","startupbehavior");
    int comboboxIndexfind = ui->comboBoxFormat->findData(formatcfg);
    ui->comboBoxFormat->setCurrentIndex(comboboxIndexfind);

    QString languagecfg = ConfigurationFileClass->getValue("language","startupbehavior");
    int comboboxIndexLanguage = ui->comboBoxLanguage->findData(languagecfg);
    ui->comboBoxLanguage->setCurrentIndex(comboboxIndexLanguage);

    if(ConfigurationFileClass->getValue("defaultrecorddeviceMute","startupbehavior") == "false")
    {
        ui->checkBoxMicMute->setChecked(0);
        on_checkBoxMicMute_clicked();
    }

    QString videocodec = ConfigurationFileClass->getValue("videocodec","record");
    QString audiocodec = ConfigurationFileClass->getValue("audiocodec","record");
    QString audiochannels = ConfigurationFileClass->getValue("audiochannels","record");
    QString fps = ConfigurationFileClass->getValue("fps","record");

    QString defaultpath = ConfigurationFileClass->getValue("defaultpath", "startupbehavior");
    QString defaultname = ConfigurationFileClass->getValue("defaultname", "startupbehavior");
    QString defaultformat = ConfigurationFileClass->getValue("defaultformat", "startupbehavior");

    if(ConfigurationFileClass->getValue("defaultnametimedate","startupbehavior") == "true")
    {
        ui->checkBoxbasenametimedate->setChecked(true);
        ui->lineEditbasename->setEnabled(0);
    }
    else
    {
        ui->checkBoxbasenametimedate->setChecked(false);
        ui->lineEditbasename->setEnabled(1);
    }

    ui->lineEditaudiocodec->setText(audiocodec);
    ui->lineEditvideocodec->setText(videocodec);
    ui->spinBoxaudiochannels->setValue(audiochannels.toInt());
    ui->spinBoxfps->setValue(fps.toInt());
    ui->lineEditbasename->setText(defaultname);
    ui->lineEditpath->setText(defaultpath);
}

void SettingsDialog::on_pushButtonpathBrowse_clicked()
{

    QString prefileName = ui->lineEditpath->text();

    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         prefileName,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if(fileName.size()>0){
        ui->lineEditpath->setText(fileName);
    }
    else{
        fileName = prefileName;
        ui->lineEditpath->setText(fileName);
    }

}

void SettingsDialog::on_checkBoxbasenametimedate_clicked()
{
    if(ui->checkBoxbasenametimedate->isChecked())
    {
        ui->lineEditbasename->setEnabled(0);
    }
    else
    {
        ui->lineEditbasename->setEnabled(1);
    }
}

void SettingsDialog::on_checkBoxMicMute_clicked()
{
    if(ui->checkBoxMicMute->isChecked())
    {
        ui->comboBoxrecording->setEnabled(0);
    }
    else
    {
        ui->comboBoxrecording->setEnabled(1);
    }
}

void SettingsDialog::getLanguages()
{
    QDir translationfilepath(":/translations");
    QStringList translationsfiles = translationfilepath.entryList(QDir::Files);
    qDebug() << "Files found:" << translationsfiles;

    ui->comboBoxLanguage->addItem("Use system default","default");
    ui->comboBoxLanguage->addItem("Use original language (English)","STANDARD");

    foreach(QFileInfo file,translationsfiles)
    {
        QString basename = file.baseName();
        QString localending = file.baseName().remove(0,17);

        qDebug() << "Translation basename:" << basename;
        qDebug() << "Translation ending:" << localending;

        ui->comboBoxLanguage->addItem(basename,localending);
    }
}
