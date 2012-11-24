#include "areaselectordialog.h"
#include <QDebug>
#include "ui_areaselectordialog.h"

AreaSelectorDialog::AreaSelectorDialog(QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint),
    ui(new Ui::AreaSelectorDialog)
{
    ui->setupUi(this);
    setWindowOpacity(0.5);
}

AreaSelectorDialog::~AreaSelectorDialog()
{
    delete ui;
}

QRect AreaSelectorDialog::getArea()
{
    return this->geometry();
}


void AreaSelectorDialog::on_pushButtonOK_clicked()
{
    changedSelection();
    close();
}

void AreaSelectorDialog::resizeEvent(QResizeEvent *)
{
    changedSelection();
}

void AreaSelectorDialog::mouseDoubleClickEvent(QMouseEvent *)
{
    changedSelection();
    close();
}

void AreaSelectorDialog::changedSelection()
{
    emit selectionChanged(true);
    qDebug() << "Selection changed";
}
