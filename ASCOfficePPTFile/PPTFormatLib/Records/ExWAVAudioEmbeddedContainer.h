#pragma once
#include "ExMediaAtom.h"

class CRecordWAVAudioEmbeddedContainer : public CRecordsContainer
{
public:
	CRecordExMediaAtom	m_oMedia;
	
	DWORD				m_nSoundID;
	LONG				m_nDuration;

public:
	
	CRecordWAVAudioEmbeddedContainer()
	{
		m_nSoundID		= 0;
		m_nDuration		= 0;
	}

	~CRecordWAVAudioEmbeddedContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		SRecordHeader oExHeader;
		oExHeader.ReadFromStream(pStream);

		m_oMedia.ReadFromStream(oExHeader, pStream);

		StreamUtils::StreamSkip(8, pStream);

		m_nSoundID	= StreamUtils::ReadDWORD(pStream);
		m_nDuration	= StreamUtils::ReadLONG(pStream);		
	}
};