#include "aboutprog.h"
#include "ui_aboutprog.h"

AboutProg::AboutProg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutProg)
{
    ui->setupUi(this);

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
    ui->textEdit_2->append(trUtf8("Danish - Ole 'Froksen' Holm Frandsen"));
    ui->textEdit_2->append(trUtf8("German - Chris 'saftsocken' Räss"));
    ui->textEdit_2->append(trUtf8("Spanish - Javi"));
}

void AboutProg::appendSpecialThanks()
{
    ui->textEditThanks->append("3electrons");
    ui->textEditThanks->append("kalac");
    ui->textEditThanks->append("swiftscythe");
    ui->textEditThanks->append("jboss1995");
    ui->textEditThanks->append("undefined");
    ui->textEditThanks->append("symbianflo");

}
