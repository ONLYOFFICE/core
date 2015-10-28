#pragma once
#include "../Reader/Records.h"

class CRecordWAVAudioLinkContainer : public CRecordsContainer
{
public:
	
	CRecordWAVAudioLinkContainer()
	{
	}

	~CRecordWAVAudioLinkContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
};