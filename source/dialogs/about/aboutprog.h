#ifndef ABOUTPROG_H
#define ABOUTPROG_H

#include <QWidget>

namespace Ui {
    class AboutProg;
}

class AboutProg : public QWidget
{
    Q_OBJECT

public:
    explicit AboutProg(QWidget *parent = 0);
    ~AboutProg();

private:
    Ui::AboutProg *ui;

    void appendTranslators();
};

#endif // ABOUTPROG_H
