#include "previewplayer.h"
#include "ui_previewplayer.h"

PreviewPlayer::PreviewPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPlayer)
{
    ui->setupUi(this);
    player = new Phonon::VideoPlayer(Phonon::VideoCategory,parent);

    ui->volumeSlider->setFixedWidth(150);

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
    player->play(Phonon::MediaSource(videofile));
    ui->gridLayout_3->addWidget(player);

    //Connecting to the Seekslider + volume
    ui->seekSlider->setMediaObject(player->mediaObject());
    ui->volumeSlider->setAudioOutput(player->audioOutput());
}
