#include "aboutprog.h"
#include "ui_aboutprog.h"
#include <QDesktopWidget>
#include <QTextStream>
#include <QDebug>

AboutProg::AboutProg(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::AboutProg)
{
    ui->setupUi(this);

//    QRect r = AboutProg::geometry();
//    r.moveCenter(QApplication::desktop()->availableGeometry().center());


    //About
    about();

    //GPL
    readGPL();

    //Append texts
    appendTranslators();
}

AboutProg::~AboutProg()
{
    delete ui;
}

//This method makes it eaiser to add new translators, without having to retranste the entire textedit.
void AboutProg::appendTranslators()
{
    ui->textEdit_2->setText(trUtf8("FDesktopRecorder have been translated into some languages. If you want to translate it into your language you can contact me on opendesktop.org by sending me a message."));

    ui->textEdit_2->append(QString("<br><h3>%1</h3>").arg(trUtf8("Translations")));
    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Danish")));
    ui->textEdit_2->append(QString::fromUtf8("Ole 'Froksen' Holm Frandsen"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Chinese")));
    ui->textEdit_2->append(QString::fromUtf8("'bruce.oy'"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("French")));
    ui->textEdit_2->append(QString::fromUtf8("'aishen'"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("German")));
    ui->textEdit_2->append(QString::fromUtf8("Max 'mx01' Huber"));
    ui->textEdit_2->append(QString::fromUtf8("Chris 'saftsocken' Räss"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Greek")));
    ui->textEdit_2->append(QString::fromUtf8("Γιάννης 'Evropi' Ανθυμίδης"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Spanish")));
    ui->textEdit_2->append(QString::fromUtf8("Javi"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Italian")));
    ui->textEdit_2->append(QString::fromUtf8("Vincenzo 'smart2128' Reale"));
    ui->textEdit_2->append(QString::fromUtf8("Symbian flo"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Polish")));
    ui->textEdit_2->append(QString::fromUtf8("Daniel 'napcok' Napora"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Portuguese (Brazil)")));
    ui->textEdit_2->append(QString::fromUtf8("Alexandro 'salmora8' Casanova"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Romanian")));
    ui->textEdit_2->append(QString::fromUtf8("Mamut R. 'V3n3RiX' Ghiunhan"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Russian")));
    ui->textEdit_2->append(QString::fromUtf8("Danilov 'modos189' Alexandr"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Ukrainian")));
    ui->textEdit_2->append(QString::fromUtf8("Eugene 'Gannet'"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Turkish")));
    ui->textEdit_2->append(QString::fromUtf8("Emre 'emfi' FIRAT"));
    ui->textEdit_2->append("<br>");
}

void AboutProg::about()
{
    ui->textEdit->setHtml(QString("<h3> %1</h3>GPLv2 (http://www.gnu.org/licenses/gpl-2.0.html)  <h3>%2 </h3>Ole Holm Frandsen <h3>%3</h3>%4 <h3>%6</h3>https://groups.google.com/forum/?fromgroups#!forum/fdesktoprecorder <h3>%5</h3>https://github.com/froksen/FDesktopRecorder").arg(trUtf8("License"),
                                                                                                                                           trUtf8("Created by"),
                                                                                                                                           trUtf8("Based on"),
                                                                                                                                                   trUtf8("A ffmpeq script by Peter Lybeth from the danish GNU/Linux show 'Kanal Tux': http://www.kanaltux.dk"),trUtf8("Source code"),trUtf8("Mailing list")));
}

int AboutProg::readGPL()
{
    QFile file(":/licenseGPLv2.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->plainTextEditGPL->setPlainText("Was unable to find file " + file.fileName() + ". Find GPLv2 at http://www.gnu.org/licenses/gpl-2.0.html");
        return -1;
    }

    QTextStream in(&file);
    ui->plainTextEditGPL->setPlainText(in.readAll());
    return 0;
}
