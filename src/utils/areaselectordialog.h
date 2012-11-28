#ifndef AREASELECTORDIALOG_H
#define AREASELECTORDIALOG_H

#include <QDialog>
#include <QRect>
#include "rubberband.h"

namespace Ui {
class AreaSelectorDialog;
}

class AreaSelectorDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AreaSelectorDialog(QWidget *parent = 0);
    ~AreaSelectorDialog();

    QRect getArea();

public slots:

private slots:
    void on_pushButtonOK_clicked();


signals:
    void selectionChanged(bool);

private:
    Ui::AreaSelectorDialog *ui;
    void changedSelection();

    RubberBand *mRubberBand;

protected:
    void resizeEvent(QResizeEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void moveEvent(QMoveEvent *);

};

#endif // AREASELECTORDIALOG_H
