#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include <vlc/vlc.h>

#ifdef Q_WS_X11
#include <QX11EmbedContainer>
#endif // Q_WS_X11

#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;
QT_END_NAMESPACE

class VLCPlayer : public QWidget
{
    Q_OBJECT;

public:

    explicit VLCPlayer(QWidget* parent = nullptr);
    virtual ~VLCPlayer() override;

public slots:

    void playFile(const QString& address);
    void setPosition(qint32 value);
    void setVolume(qint32 value);

private slots:

    void updateInterface();

private:

#ifdef Q_WS_X11
    QX11EmbedContainer*    m_videoWidget;
#else
    QFrame*                m_videoWidget;
#endif // Q_WS_X11

    QSlider*               m_positionSlider;
    QSlider*               m_volumeSlider;
    QTimer*                m_updateTimer;
    bool                   mb_playing;
    libvlc_instance_t*     m_vlcInstance;
    libvlc_media_player_t* m_vlcMediaPlayer;
    libvlc_media_t*        m_vlcMedia;
};

#endif // VLCPLAYER_H
