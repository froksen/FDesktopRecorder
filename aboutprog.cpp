#include "aboutprog.h"
#include "ui_aboutprog.h"

AboutProg::AboutProg(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AboutProg)
{
    ui->setupUi(this);
}

AboutProg::~AboutProg()
{
    delete ui;
}
