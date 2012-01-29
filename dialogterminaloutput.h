#ifndef DIALOGTERMINALOUTPUT_H
#define DIALOGTERMINALOUTPUT_H

#include <QDialog>


namespace Ui {
    class DialogTerminalOutput;
}

class DialogTerminalOutput : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTerminalOutput(QWidget *parent = 0);
    ~DialogTerminalOutput();

    void stderroutput(QByteArray Text);

private:
    Ui::DialogTerminalOutput *ui;
};

#endif // DIALOGTERMINALOUTPUT_H
