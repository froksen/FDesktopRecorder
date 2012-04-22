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

    //Making connections
    connect(player,SIGNAL(finished()),this,SLOT(on_pushButtonStop_clicked()));
    connect(player,SIGNAL(destroyed()),this,SLOT(deleteLater()));

    //Connecting to the Seekslider + volume
    ui->seekSlider->setMediaObject(player->mediaObject());
    ui->volumeSlider->setAudioOutput(player->audioOutput());

    //Settings icons for buttons
    ui->pushButtonStart->setIcon(QIcon(QIcon::fromTheme("media-playback-start")));
    ui->pushButtonStop->setIcon(QIcon::fromTheme("media-playback-stop"));
}

PreviewPlayer::~PreviewPlayer()
{
    delete ui;

    if(player)
    {
        player->stop();
    }
}

void PreviewPlayer::setVideofile(QString newVideofile)
{
    videofile = newVideofile;
}

void PreviewPlayer::playVideo()
{
    setWindowTitle("Preview: " + videofile);

    player->play(Phonon::MediaSource(videofile));

    setIcons();
}

void PreviewPlayer::on_pushButtonStart_clicked()
{
    //does the rest
    if(player->isPlaying())
    {
        player->pause();
    }
    else
    {
        if(player->isPaused())
        {
            player->play();
        }
        else
        {
            playVideo();
        }
    }

    //Sets the propper icon
    setIcons();

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
    setIcons();
}

void PreviewPlayer::on_pushButtonClose_clicked()
{
}

void PreviewPlayer::setIcons()
{

    if(!player->isPlaying())
    {
        ui->pushButtonStart->setIcon(QIcon(QIcon::fromTheme("media-playback-pause")));
    }
    else
    {
        ui->pushButtonStart->setIcon(QIcon(QIcon::fromTheme("media-playback-start")));
    }
}
