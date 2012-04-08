#include "previewplayer.h"
#include "ui_previewplayer.h"

PreviewPlayer::PreviewPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPlayer)
{
    ui->setupUi(this);
    Phonon::VideoPlayer *player =
             new Phonon::VideoPlayer(Phonon::VideoCategory,parent);
//    player->play(Phonon::MediaSource("/home/froksen/optagelse.avi"));

    ui->gridLayout_3->addWidget(player);

    //Connecting to the Seekslider + volume
    ui->seekSlider->setMediaObject(player->mediaObject());
    ui->volumeSlider->setAudioOutput(player->audioOutput());
}

PreviewPlayer::~PreviewPlayer()
{
    delete ui;
}
