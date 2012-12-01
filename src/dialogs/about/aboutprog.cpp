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


    //GPL
    readGPL();

    //Append texts
    appendTranslators();
    appendSpecialThanks();
}

AboutProg::~AboutProg()
{
    delete ui;
}

//This method makes it eaiser to add new translators, without having to retranste the entire textedit.
void AboutProg::appendTranslators()
{
    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Danish")));
    ui->textEdit_2->append(QString::fromUtf8("Ole 'Froksen' Holm Frandsen"));
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

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Portuguese (Brazil)")));
    ui->textEdit_2->append(QString::fromUtf8("Alexandro 'salmora8' Casanova"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Romanian")));
    ui->textEdit_2->append(QString::fromUtf8("Mamut R. 'V3n3RiX' Ghiunhan"));
    ui->textEdit_2->append("<br>");

    ui->textEdit_2->append(QString::fromUtf8("<b>%1</b>").arg(trUtf8("Russian")));
    ui->textEdit_2->append(QString::fromUtf8("Danilov 'modos189' Alexandr"));
    ui->textEdit_2->append("<br>");
}

void AboutProg::appendSpecialThanks()
{
    ui->textEditThanks->append("3electrons");
    ui->textEditThanks->append("kalac");
    ui->textEditThanks->append("swiftscythe");
    ui->textEditThanks->append("jboss1995");
    ui->textEditThanks->append("undefined");
    ui->textEditThanks->append("symbianflo");
    ui->textEditThanks->append("Mamut R. Ghiunhan (aka V3n3RiX)");

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
