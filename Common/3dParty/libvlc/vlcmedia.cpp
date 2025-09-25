#include "vlcmedia.h"

#include <QUrl>

CVlcMedia::CVlcMedia(libvlc_instance_t* pVlcInstance, const QString& sFile, bool bEventForwarding)
{
	// create vlc media from local file
	QUrl url = QUrl::fromLocalFile(sFile);
	m_pMedia = libvlc_media_new_location(pVlcInstance, url.toString(QUrl::FullyEncoded).toUtf8().constData());

	init(bEventForwarding);
}

CVlcMedia::CVlcMedia(libvlc_media_t* pVlcMedia, bool bEventForwarding) : m_pMedia(pVlcMedia)
{
	init(bEventForwarding);
}

CVlcMedia::~CVlcMedia()
{
	if (m_pMedia)
		libvlc_media_release(m_pMedia);
}

void CVlcMedia::init(bool bEventForwarding)
{
	// if caller don't want to use QT signals
	if (bEventForwarding)
	{
		// attach event callbacks
		m_pEventManager = libvlc_media_event_manager(m_pMedia);
		libvlc_event_attach(m_pEventManager, libvlc_MediaParsedChanged, onParsedChanged, this);
	}
}

void CVlcMedia::onParsedChanged(const libvlc_event_t* pEvent, void* pData)
{
	CVlcMedia* pMedia = reinterpret_cast<CVlcMedia*>(pData);
	emit pMedia->parsedChanged(static_cast<bool>(pEvent->u.media_parsed_changed.new_status));
}

libvlc_time_t CVlcMedia::duration()
{
	return libvlc_media_get_duration(m_pMedia);
}

void CVlcMedia::parse()
{
	libvlc_media_parse_with_options(m_pMedia, libvlc_media_parse_local, -1);
}
