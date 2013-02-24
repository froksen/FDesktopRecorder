#ifndef ABOUTPROG_H
#define ABOUTPROG_H

#include <QWidget>
#include <QDialog>
#include <QFile>
#include <QTextStream>

namespace Ui {
    class AboutProg;
}

class AboutProg : public QDialog
{
    Q_OBJECT

public:
    explicit AboutProg(QDialog *parent = 0);
    ~AboutProg();

private:
    Ui::AboutProg *ui;

    void appendTranslators();

    void about();

    //Changelog
    int readGPL();
};

#endif // ABOUTPROG_H
