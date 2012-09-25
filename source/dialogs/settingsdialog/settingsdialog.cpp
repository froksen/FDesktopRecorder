#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include <module/recordingdevices.h>
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    qDebug() << "-----  Settingsdialog: Reading settings -----";
    readSettings();

    on_checkBoxMicMute_clicked();    
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_buttonBox_accepted()
{
    //other
    settings.setFramerate(ui->spinBoxfps->value());
    settings.setVideocodec(ui->lineEditvideocodec->text());
    settings.setAudiocodec(ui->lineEditaudiocodec->text());
    settings.setAudiochannels(ui->spinBoxaudiochannels->value());
    settings.setAudiosource(ui->lineEditAudiosource->text());

    //Microphone
    int MicIndex = ui->comboBoxrecording->currentIndex();
    settings.setMicrophonedevice(ui->comboBoxrecording->itemData(MicIndex).toString());

    //Microphone: Mute?
    if(ui->checkBoxMicMute->isChecked())
    {
        settings.setMicrophonemuted("true");
    }
    else
    {
        settings.setMicrophonemuted("false");
    }

    settings.setFilenameBase(ui->lineEditbasename->text());

    //Filename: use date and time
    if(ui->checkBoxbasenametimedate->isChecked())
    {
        settings.setFilenameUsedate("true");
    }
    else
    {
        settings.setFilenameUsedate("false");
    }

    settings.setFilenamePath(ui->lineEditpath->text());

    //Format
    int formatindex = ui->comboBoxFormat->currentIndex();
    settings.setFormat(ui->comboBoxFormat->itemData(formatindex).toString());

    //Language
    int langindex = ui->comboBoxLanguage->currentIndex();
    settings.setLanguage(ui->comboBoxLanguage->itemData(langindex).toString());

    //TODO: Remove this option. since the player is buggy and yeah no need to redo work others have done.
    //NOTE: Removed previewplayer. Kept code since I might include it again someday. PLEASE NOTE, that the hide option is simply an easy
    // way to remove the checkbox.

    ui->checkBoxPreviewplayer->hide();
    //Preview player
    settings.setPreviewplayer(ui->lineEditPreviewplayer->text());
//    if(!ui->checkBoxPreviewplayer->isChecked())
//    {
//        settings.setPreviewplayerintegrated("false");
//    }
//    else
//    {
//        settings.setPreviewplayerintegrated("true");
//    }

    //Single window: use red rectangle
    if(!ui->checkBoxSinglewindowredrectangle->isChecked())
    {
        settings.setSinglewindow_redrectangle("false");
    }
    else
    {
        settings.setSinglewindow_redrectangle("true");
    }

    //advanced
    settings.setVpre(ui->lineEditadvancedvpre->text());
    if(ui->checkBoxadvancedvpreuse->isChecked())
    {
        settings.setUsevpre("true");
    }
    else
    {
        settings.setUsevpre("false");
    }

    settings.setApre(ui->lineEditadvancedapre->text());
    if(ui->checkBoxadvancedapre->isChecked())
    {
        settings.setUseapre("true");
    }
    else
    {
        settings.setUseapre("false");
    }

    settings.setPreset(ui->lineEditadvancedpreset->text());
    if(ui->checkBoxadvancepreset->isChecked()){
        settings.setUsePreset("true");
    }
    else {
        settings.setUsePreset("false");
    }

    //FFmpeg location
    if(!ui->lineEditFFmpeg->text().isEmpty()){
        settings.setFFmpeglocation(ui->lineEditFFmpeg->text());
    }

    //Writes the data
    qDebug() << "-----  Settingsdialog: Writting settings -----";
    settings.writeAll();
    qDebug() << "-----  Settingsdialog: Reading settings -----";
    settings.readAll();
}

void SettingsDialog::on_pushButtonRestore_clicked()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(trUtf8("Restore everything to default?"));
    msgBox.setText("<b>" + trUtf8("Restore everything to default?")+ "</b>");
    msgBox.setInformativeText(trUtf8("If you press Yes everything will be restored to default. \n\nBe aware that this can NOT be undone."));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    //msgBox.setDetailedText(QString(ui->textEditConsole->toPlainText()));
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setFixedWidth(520);
    int ret = msgBox.exec();

    switch (ret)  {
      case QMessageBox::Yes:
            {
            qDebug() << "-----  Settingsdialog: Restoring settings to defaults -----";
                //in the GUI
                ui->checkBoxMicMute->setChecked(1);
                ui->comboBoxFormat->clear();
                ui->comboBoxLanguage->clear();
                ui->comboBoxrecording->clear();

                //Restores the defaults
                settings.removeSettingsfile();
                readSettings();
              break;
            }
      case QMessageBox::No:
            qDebug() << "-----  Settingsdialog: Did NOT reset anything -----";
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
 //TODO: Merge settings.readAll() and recordingdevices.getRecorddevices() better together, so
 // they make more sense.

    settings.readAll();
// -----------------SECTION: Recordingdevice------------------------------
    //Refreshes the list
    recordingdevices.getRecorddevices();

    //Creates the combobox containing the recording devices that are on the machine.
    int index = 0;
    ui->comboBoxrecording->clear();
    foreach(QString device,recordingdevices.RecordDeviceHW)
    {
        ui->comboBoxrecording->addItem(recordingdevices.RecordDeviceDesc[index],device);

        index += 1;
    }

    //Reads the prefered value
    QString recordingdevice = settings.getMicrophonedevice();
    int comboboxIndex = ui->comboBoxrecording->findData(recordingdevice);
    ui->comboBoxrecording->setCurrentIndex(comboboxIndex);

// -----------------SECTION: Format------------------------------
    //Addes the diffent formats to the combox. PLEASE NOTE: the order is important.
    QStringList ItemFormatList;
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

    //Reads the prefered value
    QString formatcfg = settings.getFormat();
    int comboboxIndexfind = ui->comboBoxFormat->findData(formatcfg);
    ui->comboBoxFormat->setCurrentIndex(comboboxIndexfind);

    // -----------------SECTION: Language------------------------------

    //Updates the list
    findLanguages();

    //Sets the prefered
    QString languagecfg = settings.getLanguage();
    int comboboxIndexLanguage = ui->comboBoxLanguage->findData(languagecfg);
    ui->comboBoxLanguage->setCurrentIndex(comboboxIndexLanguage);

    // -----------------SECTION: Previewplayer------------------------------

    //NOTE: Removed previewplayer. Kept code since I might include it again someday. PLEASE NOTE, that the hide option is simply an easy
    // way to remove the checkbox.
    ui->checkBoxPreviewplayer->hide();
    ui->lineEditPreviewplayer->setText(settings.getPreviewplayer());

//    if(settings.getPreviewplayerintegrated() == "false")
//    {
//        ui->checkBoxPreviewplayer->setChecked(false);
        ui->lineEditPreviewplayer->setEnabled(true);
//    }
//    else
//    {
//        ui->checkBoxPreviewplayer->setChecked(true);
//        ui->lineEditPreviewplayer->setEnabled(false);
//    }

    // -----------------SECTION: Single window: Use red rectangle------------------------------
    if(settings.getSinglewindow_redrectangle() == "false")
    {
        ui->checkBoxSinglewindowredrectangle->setChecked(false);
    }
    else
    {
        ui->checkBoxSinglewindowredrectangle->setChecked(true);
    }

    // -----------------SECTION: Microphonemuted------------------------------
    if(settings.getMicrophonemuted() == "false")
    {
        ui->checkBoxMicMute->setChecked(0);
    }

    // -----------------SECTION: Filename use time and date------------------------------
    if(settings.getFilenameUsedate() == "true")
    {
        ui->checkBoxbasenametimedate->setChecked(true);
        ui->lineEditbasename->setEnabled(0);
    }
    else
    {
        ui->checkBoxbasenametimedate->setChecked(false);
        ui->lineEditbasename->setEnabled(1);
    }    

    // -----------------SECTION: FFmpeg location------------------------------
    ui->lineEditFFmpeg->setText(settings.FFmpeglocation());

    // -----------------SECTION: Other------------------------------
    ui->lineEditaudiocodec->setText(settings.getAudiocodec());
    ui->lineEditvideocodec->setText(settings.getVideocodec());
    ui->spinBoxaudiochannels->setValue(settings.getAudiochannles());
    ui->spinBoxfps->setValue(settings.getFramerate());
    ui->lineEditbasename->setText(settings.getFilenameBase());
    ui->lineEditpath->setText(settings.getFilenamePath());
    ui->lineEditAudiosource->setText(settings.getAudiosource());

    // -----------------SECTION: Advanced------------------------------
    //vpre
    ui->lineEditadvancedvpre->setText(settings.getVpre());
    if(settings.getUsevpre() == "true")
    {
        ui->checkBoxadvancedvpreuse->setChecked(1);
    }

    //apre
    ui->lineEditadvancedapre->setText(settings.getApre());
    if(settings.getUseapre() == "true")
    {
        ui->checkBoxadvancedapre->setChecked(1);
    }

    //preset
    ui->lineEditadvancedpreset->setText(settings.getPreset());
    if(settings.getsetUsePreset() == "true")
    {
        ui->checkBoxadvancepreset->setChecked(1);
    }
}

void SettingsDialog::on_pushButtonpathBrowse_clicked()
{
    QString prefileName = settings.getFilenamePath();
    QString fileName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                         prefileName,
                                                         QFileDialog::ShowDirsOnly
                                                         | QFileDialog::DontResolveSymlinks);
    if(!fileName.isEmpty())
    {
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
//    if(ui->checkBoxMicMute->isChecked())
//    {
//        ui->comboBoxrecording->setEnabled(0);
//    }
//    else
//    {
//        ui->comboBoxrecording->setEnabled(1);
//    }
}

void SettingsDialog::findLanguages()
{
    QDir translationfilepath(":/translations");
    QStringList translationsfiles = translationfilepath.entryList(QDir::Files);
    qDebug() << "Files found:" << translationsfiles;

    ui->comboBoxLanguage->addItem(trUtf8("Use system default"),"default");
    ui->comboBoxLanguage->addItem(QString("English (%1)").arg(trUtf8("Original")),"STANDARD");

    QStringList localeName;
    localeName<< QString::fromUtf8("Dansk") << QString::fromUtf8("Deutch") << QString::fromUtf8("Romantism") << QString::fromUtf8("Italiano") << QString::fromUtf8("Español");

    QStringList localeEnding;
    localeEnding << "da_DK" << "de_DE" << "ro_RO" << "it_IT" << "es_ES";



    foreach(QFileInfo file,translationsfiles)
    {
        QString basename = file.baseName();
        QString localending = file.baseName().remove(0,17);

        for(int i=0;i<localeEnding.count();i++){
            if(localeEnding.at(i) == localending){
                basename = localeName.at(i);
            }
        }


        qDebug() << "Translation basename:" << basename;
        qDebug() << "Translation ending:" << localending;

        ui->comboBoxLanguage->addItem(basename,localending);

    }
}

void SettingsDialog::on_checkBoxPreviewplayer_clicked()
{
    if(ui->checkBoxPreviewplayer->isChecked())
    {
        ui->lineEditPreviewplayer->setEnabled(false);
    }
    else
    {
        ui->lineEditPreviewplayer->setEnabled(true);
    }
}


void SettingsDialog::on_comboBoxLanguage_activated(const QString &arg1)
{
    QPalette labelLanguagePalette = ui->labelLanguage->palette();
    labelLanguagePalette.setColor(ui->labelLanguage->foregroundRole(),Qt::red);
    ui->labelLanguage->setPalette(labelLanguagePalette);

    QPalette label2Palette = ui->label_2->palette();
    label2Palette.setColor(ui->label_2->foregroundRole(),Qt::red);
    ui->label_2->setPalette(label2Palette);
}

void SettingsDialog::on_pushButtonFFmpegbrowse_clicked()
{
     QFileDialog dialog;
     QString fileName = dialog.getOpenFileName(this, tr("FFmpeg location"),
                                               QDir::rootPath(),"FFmpeg (ffmpeg)");

    if(!fileName.isEmpty()){
        ui->lineEditFFmpeg->setText(fileName);
    }
}
