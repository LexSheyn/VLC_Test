#include "VLCPlayer.h"

#ifdef Q_WS_X11
#include <QX11EmbedContainer>
#endif // Q_WS_X11

#include <QtWidgets>

#define POSITION_RESOLUTION 1000

VLCPlayer::VLCPlayer(QWidget *parent)
    : QWidget    (parent)
    , mb_playing (false)
{
    // VLC command.

    const char* const vlcArgs[] = { "--verbose=2" };

#ifdef Q_WS_X11
    m_videoWidget = new QX11EmbedContainer();
#else
    m_videoWidget = new QFrame();
#endif // Q_WS_X11

    m_positionSlider = new QSlider(Qt::Horizontal);
    m_positionSlider->setMaximum(POSITION_RESOLUTION);
    m_positionSlider->setToolTip("Position");

    m_volumeSlider = new QSlider(Qt::Horizontal);
    m_volumeSlider->setMaximum(100);
    m_volumeSlider->setToolTip("Volume");

    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(m_videoWidget);
    layout->addWidget(m_positionSlider);
    layout->addWidget(m_volumeSlider);

    this->setLayout(layout);

    m_updateTimer = new QTimer(this);

    m_vlcInstance = libvlc_new(sizeof(vlcArgs) / sizeof(vlcArgs[0]), vlcArgs);

    m_vlcMediaPlayer = libvlc_media_player_new(m_vlcInstance);

//    connect(m_updateTimer, &QTimer::timeout, this, &VLCPlayer::updateInterface);
    connect(m_positionSlider, QOverload<>::of(&QWidget::update), this, &VLCPlayer::updateInterface);
    connect(m_positionSlider, &QSlider::sliderMoved, this, &VLCPlayer::setPosition);
    connect(m_volumeSlider, &QSlider::sliderMoved, this, &VLCPlayer::setVolume);
}

VLCPlayer::~VLCPlayer()
{
    libvlc_media_player_stop(m_vlcMediaPlayer);
    libvlc_media_player_release(m_vlcMediaPlayer);
    libvlc_release(m_vlcInstance);
}

void VLCPlayer::playFile(const QString &address)
{
    // The file has to be in one of the following formats:
    // [file://]filename                                        Plain media file
    // http://ip:port/file                                      HTTP URL
    // ftp://ip:port/file                                       FTP URL
    // mms://ip:port/file                                       MMS URL
    // screen://                                                Screen capture
    // [dvd://][device][@raw_device]                            DVD device
    // [vcd://][device]                                         VCD device
    // [cdda://][device]                                        Audio CD device
    // udp:[[<source address>]@[<bind address>]@[:<bind port>]] UDP socket

    // Create media descriptor.

    m_vlcMedia = libvlc_media_new_path(m_vlcInstance, address.toUtf8());

    libvlc_media_player_set_media(m_vlcMediaPlayer, m_vlcMedia);

    // Passing the widget to the lib shows VLC at which position it should show up.
    // VLC automatically resizes the video to the given size of the widget and it
    // even resizes the video if the size changes while playing.

#ifdef Q_OS_WIN
    libvlc_media_player_set_drawable(m_vlcMediaPlayer, (quint8)m_videoWidget->winId());
#elifdef Q_OS_MAC
    libvlc_media_player_set_drawable(m_vlcMediaPlayer, m_videoWidget->winId());
#elifdef Q_OS_LINUX
    libvlc_media_player_set_xwindow(m_vlcMediaPlayer, m_videoWidget->winId());
#endif

    libvlc_media_player_play(m_vlcMediaPlayer);

    mb_playing = true;
}

void VLCPlayer::setPosition(qint32 value)
{
    libvlc_media_t* media = libvlc_media_player_get_media(m_vlcMediaPlayer);

    if (!media)
    {
        return;
    }

    float position = (float)value/(float)POSITION_RESOLUTION;

    libvlc_media_player_set_position(m_vlcMediaPlayer, position);
}

void VLCPlayer::setVolume(qint32 value)
{
    libvlc_audio_set_volume(m_vlcMediaPlayer, value);
}

void VLCPlayer::updateInterface()
{
    // TO DO:
    // File: https://wiki.videolan.org/LibVLC_SampleCode_Qt/
    // Line: void Player::updateInterface()
}
