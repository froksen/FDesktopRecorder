#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->pushButtonStoprecord->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonStartrecord_clicked()
{
    ui->pushButtonStartrecord->hide();
    ui->pushButtonStoprecord->show();
}

void MainWindow::on_pushButtonStoprecord_clicked()
{
    ui->pushButtonStartrecord->show();
    ui->pushButtonStoprecord->hide();
}
