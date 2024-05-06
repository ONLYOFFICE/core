#ifndef VLC_PLAYER_H
#define VLC_PLAYER_H

#include "libvlc_base.h"
#include "vlcmedia.h"
#include "vlcinstance.h"

#include <QWidget>

class CVlcPlayer : public QWidget
{
	Q_OBJECT

public:
	CVlcPlayer(QWidget* parent = nullptr);
	virtual ~CVlcPlayer();

public:
	// libvlc event callbacks
	static void onStateChanged(const libvlc_event_t* pEvent, void *pData);
	static void onTimeChanged(const libvlc_event_t* pEvent, void *pData);
	static void onPositionChanged(const libvlc_event_t* pEvent, void *pData);

public:
	void integrateIntoWidget(QWidget* pWidget);
	void open(CVlcMedia* pMedia);
	// control playback
	void pause();
	void play();
	void stop();
	// volume
	void setVolume(int nVolume);
	// time (in ms)
	void setTime(qint64 nTime);
	qint64 time();
	// position (in range 0.0...1.0)
	void setPosition(float fPos);
	float position();

	bool isAudio();
	bool isPlaying();
	libvlc_state_t getState();

signals:
	void stateChanged(int newState);
	void timeChanged(qint64 nNewTime);
	void positionChanged(float fNewPos);

public:
	libvlc_media_player_t* m_pVlcPlayer;
	libvlc_event_manager_t* m_pEventManager;
};

#endif  // VLC_PLAYER_H
