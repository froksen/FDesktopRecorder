#include "previewplayer.h"
#include "ui_previewplayer.h"

PreviewPlayer::PreviewPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PreviewPlayer)
{
    ui->setupUi(this);
    Phonon::VideoPlayer *player =
             new Phonon::VideoPlayer(Phonon::VideoCategory,parent);
}

PreviewPlayer::~PreviewPlayer()
{
    delete ui;
}
