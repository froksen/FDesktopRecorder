#include "dialogterminaloutput.h"
#include "ui_dialogterminaloutput.h"
#include <runterminal.h>
#include <QDebug>

DialogTerminalOutput::DialogTerminalOutput(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTerminalOutput)
{
    ui->setupUi(this);
}

DialogTerminalOutput::~DialogTerminalOutput()
{
    delete ui;
}

void DialogTerminalOutput::stderroutput(QByteArray Text)
{
    ui->textEditOutput->append(Text);
}
