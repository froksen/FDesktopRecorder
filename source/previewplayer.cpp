#include "previewplayer.h"
#include "ui_previewplayer.h"

PreviewPlayer::PreviewPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPlayer)
{
    ui->setupUi(this);

    //Creates the player
    player = new Phonon::VideoPlayer(Phonon::VideoCategory,parent);

    //set Fixed with to the volumeslider
    ui->volumeSlider->setFixedWidth(150);

    //adds the player to the grid
    ui->gridLayout_3->addWidget(player);

    //Connecting to the Seekslider + volume
    ui->seekSlider->setMediaObject(player->mediaObject());
    ui->volumeSlider->setAudioOutput(player->audioOutput());
}

PreviewPlayer::~PreviewPlayer()
{
    delete ui;
}

void PreviewPlayer::setVideofile(QString newVideofile)
{
    videofile = newVideofile;
}

void PreviewPlayer::playVideo()
{
    setWindowTitle("Preview: " + videofile);
    if(!player->isPlaying())
    {
        if(player->isPaused())
        {
            player->play();
        }
        else
        {
            player->play(Phonon::MediaSource(videofile));

        }
    }
    else
    {
        player->play(Phonon::MediaSource(videofile));
    }


}

void PreviewPlayer::on_pushButtonStart_clicked()
{
        playVideo();
}

void PreviewPlayer::on_pushButtonPause_clicked()
{
    if(player->isPaused())
    {
        player->play();
    }
    else
    {
        player->pause();
    }
}

void PreviewPlayer::on_pushButtonStop_clicked()
{
    player->stop();
}

void PreviewPlayer::on_pushButtonClose_clicked()
{
}
