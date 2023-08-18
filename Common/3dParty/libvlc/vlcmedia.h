#ifndef VLC_MEDIA_H
#define VLC_MEDIA_H

#include "libvlc_base.h"

#include <QObject>

class CVlcMedia : public QObject
{
	Q_OBJECT

public:
	CVlcMedia(libvlc_instance_t* pVlcInstance, const QString& sFile, bool bEventForwarding = true);
	CVlcMedia(libvlc_media_t* pVlcMedia, bool bEventForwarding = true);
	virtual ~CVlcMedia();

	void init(bool bEventForwarding);

public:
	// libvlc callbacks
	static void onParsedChanged(const libvlc_event_t *pEvent, void *pData);

public:
	libvlc_time_t duration();
	void parse();

signals:
	void parsedChanged(bool bNewStatus);

public:
	libvlc_media_t* m_pMedia;
	libvlc_event_manager_t* m_pEventManager;
};

#endif // VLC_MEDIA_H
