#ifndef PREVIEWPLAYER_H
#define PREVIEWPLAYER_H

#include <QWidget>
#include <phonon/VideoPlayer>

namespace Ui {
class PreviewPlayer;
}

class PreviewPlayer : public QWidget
{
    Q_OBJECT
    
public:
    explicit PreviewPlayer(QWidget *parent = 0);
    ~PreviewPlayer();
    
private:
    Ui::PreviewPlayer *ui;

    Phonon::VideoPlayer *player;
};

#endif // PREVIEWPLAYER_H
