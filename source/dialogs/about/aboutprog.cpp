#include "aboutprog.h"
#include "ui_aboutprog.h"
#include <QDesktopWidget>

AboutProg::AboutProg(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::AboutProg)
{
    ui->setupUi(this);

//    QRect r = AboutProg::geometry();
//    r.moveCenter(QApplication::desktop()->availableGeometry().center());

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
    ui->textEdit_2->append(trUtf8("Danish") + QString(" - ") + QString("Ole 'Froksen' Holm Frandsen"));
    ui->textEdit_2->append(trUtf8("German") + QString(" - ") + QString("Chris 'saftsocken' Räss"));
    ui->textEdit_2->append(trUtf8("Spanish")+ QString(" - ") + QString("Javi"));
    ui->textEdit_2->append(trUtf8("Italian")+ QString(" - ") + QString("Symbian flo"));
    ui->textEdit_2->append(trUtf8("Romanian")+ QString(" - ") + QString("Mamut R. Ghiunhan (aka V3n3RiX)"));

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
