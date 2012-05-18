#ifndef PREVIEWPLAYER_H
#define PREVIEWPLAYER_H

#include <QDialog>
#include <phonon/VideoPlayer>

namespace Ui {
class PreviewPlayer;
}

class PreviewPlayer : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreviewPlayer(QDialog *parent = 0);
    ~PreviewPlayer();

    void setVideofile(QString newVideofile);
    void playVideo();

private slots:
    //Pushbuttons
    void on_pushButtonStart_clicked();
    void on_pushButtonPause_clicked();
    void on_pushButtonStop_clicked();
    void on_pushButtonClose_clicked();

    void setIcons();

private:
    QString videofile;

    Ui::PreviewPlayer *ui;
    Phonon::VideoPlayer *player;


};

#endif // PREVIEWPLAYER_H
