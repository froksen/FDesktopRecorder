#include "areaselectordialog.h"
#include <QDebug>
#include "ui_areaselectordialog.h"

AreaSelectorDialog::AreaSelectorDialog(QWidget *parent) :
    QDialog(parent, Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint),
    ui(new Ui::AreaSelectorDialog)
{
    ui->setupUi(this);
    setWindowOpacity(0.5);

    mRubberBand = new RubberBand(this);
    mRubberBand->show();
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

void AreaSelectorDialog::moveEvent(QMoveEvent *)
{
    changedSelection();
}

void AreaSelectorDialog::changedSelection()
{
    emit selectionChanged(true);
    qDebug() << "Selection changed";

    //X and Y
    ui->labelXY->setText(QString("X: %1, Y: %2").arg(QString::number(frameGeometry().x()),
                                                     QString::number(frameGeometry().y())));

    //Width and Height
    ui->labelWidthHeight->setText(QString("%1: %2, %3: %4").arg(trUtf8("Width"),
                                                        QString::number(frameSize().width()),
                                                        trUtf8("Height"),
                                                        QString::number(frameSize().height())));

    mRubberBand->setGeometry(0,0,geometry().width(),geometry().height());
}
