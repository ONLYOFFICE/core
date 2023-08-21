#ifndef VLC_PLAYER_H
#define VLC_PLAYER_H

#include "libvlc_base.h"
#include "vlcmedia.h"

#include <QWidget>

class CVlcPlayer : public QWidget
{
	Q_OBJECT

public:
	CVlcPlayer();
	virtual ~CVlcPlayer();

public:
	// libvlc event callbacks
	static void onStateChanged(const libvlc_event_t *pEvent, void *pData);
	static void onTimeChanged(const libvlc_event_t *pEvent, void *pData);

public:
	void integrateIntoWidget(QWidget* pWidget);
	void open(CVlcMedia* pMedia);
	void pause();
	void play();
	void stop();
	void setVolume(int nVolume);
	void setTime(int nTime);
	bool isAudio();
	libvlc_state_t getState();

signals:
	void stateChanged(int newState);
	void timeChanged(qint64 nNewTime);

public:
	libvlc_instance_t* m_pVlcInstance;
	libvlc_media_player_t* m_pVlcPlayer;
	libvlc_event_manager_t* m_pEventManager;
};

#endif  // VLC_PLAYER_H
