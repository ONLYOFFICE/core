#include "vlcplayer.h"

CVlcPlayer::CVlcPlayer(QWidget* parent) : QWidget(parent)
{
	// initialize vlc media player
	m_pVlcPlayer = libvlc_media_player_new(GetVlcInstance());
	// disable event handling by vlc internals
	libvlc_video_set_mouse_input(m_pVlcPlayer, false);
	libvlc_video_set_key_input(m_pVlcPlayer, false);
	// get event manager
	m_pEventManager = libvlc_media_player_event_manager(m_pVlcPlayer);

	// attach event callbacks
	for (int nEvent = static_cast<int>(libvlc_MediaPlayerNothingSpecial); nEvent <= static_cast<int>(libvlc_MediaPlayerEncounteredError); nEvent++)
	{
		libvlc_event_attach(m_pEventManager, nEvent, onStateChanged, this);
	}
	libvlc_event_attach(m_pEventManager, libvlc_MediaPlayerTimeChanged , onTimeChanged, this);
	libvlc_event_attach(m_pEventManager, libvlc_MediaPlayerPositionChanged , onPositionChanged, this);

	libvlc_event_attach(m_pEventManager, libvlc_MediaPlayerVout , onVideoOutputChanged, this);
}

CVlcPlayer::~CVlcPlayer()
{
	if (m_pVlcPlayer)
		libvlc_media_player_release(m_pVlcPlayer);
}

void CVlcPlayer::onStateChanged(const libvlc_event_t* pEvent, void* pData)
{
	CVlcPlayer* pVlcPlayer = reinterpret_cast<CVlcPlayer*>(pData);
	emit pVlcPlayer->stateChanged(pVlcPlayer->getState());
}

void CVlcPlayer::onTimeChanged(const libvlc_event_t* pEvent, void* pData)
{
	CVlcPlayer* pVlcPlayer = reinterpret_cast<CVlcPlayer*>(pData);
	emit pVlcPlayer->timeChanged(pEvent->u.media_player_time_changed.new_time);
}

void CVlcPlayer::onPositionChanged(const libvlc_event_t* pEvent, void* pData)
{
	CVlcPlayer* pVlcPlayer = reinterpret_cast<CVlcPlayer*>(pData);
	emit pVlcPlayer->positionChanged(pEvent->u.media_player_position_changed.new_position);
}

void CVlcPlayer::onVideoOutputChanged(const libvlc_event_t* pEvent, void* pData)
{
	CVlcPlayer* pVlcPlayer = reinterpret_cast<CVlcPlayer*>(pData);
	emit pVlcPlayer->videoOutputChanged(pEvent->u.media_player_vout.new_count);
}

void CVlcPlayer::integrateIntoWidget(QWidget* pWidget)
{
#if defined(_MAC)
	libvlc_media_player_set_nsobject(m_pVlcPlayer, reinterpret_cast<void*>(pWidget->winId()));
#elif defined(_LINUX)
	libvlc_media_player_set_xwindow(m_pVlcPlayer, pWidget->winId());
#elif defined(_WIN32)
	libvlc_media_player_set_hwnd(m_pVlcPlayer, reinterpret_cast<HWND>(pWidget->winId()));
#endif
}

void CVlcPlayer::open(CVlcMedia* pMedia)
{
	// stop playing old media
	if (libvlc_media_player_is_playing(m_pVlcPlayer))
		stop();

	// set new media
	libvlc_media_player_set_media(m_pVlcPlayer, pMedia->m_pMedia);

	// start playback
	libvlc_media_player_play(m_pVlcPlayer);
}

void CVlcPlayer::pause()
{
	// set on pause if currently playing
	if (libvlc_media_player_is_playing(m_pVlcPlayer))
	{
		libvlc_media_player_pause(m_pVlcPlayer);
	}
}

void CVlcPlayer::play()
{
	// play if not playing already
	if (!libvlc_media_player_is_playing(m_pVlcPlayer))
	{
		libvlc_media_player_play(m_pVlcPlayer);
	}
}

void CVlcPlayer::setVolume(int nVolume)
{
	libvlc_audio_set_volume(m_pVlcPlayer, nVolume);
}

void CVlcPlayer::setTime(qint64 nTime)
{
	libvlc_media_player_set_time(m_pVlcPlayer, nTime);
}

qint64 CVlcPlayer::time()
{
	return libvlc_media_player_get_time(m_pVlcPlayer);
}

void CVlcPlayer::setPosition(float fPos)
{
	libvlc_media_player_set_position(m_pVlcPlayer, fPos);
	emit positionChanged(libvlc_media_player_get_position(m_pVlcPlayer));
}

float CVlcPlayer::position()
{
	return libvlc_media_player_get_position(m_pVlcPlayer);
}

bool CVlcPlayer::isAudio()
{
	return !libvlc_media_player_has_vout(m_pVlcPlayer);
}

bool CVlcPlayer::isPlaying()
{
	return libvlc_media_player_is_playing(m_pVlcPlayer);
}

libvlc_state_t CVlcPlayer::getState()
{
	return libvlc_media_player_get_state(m_pVlcPlayer);
}

void CVlcPlayer::stop()
{
	// stop the media player
	libvlc_media_player_stop(m_pVlcPlayer);
}
