#pragma once
#include "..\Reader\Records.h"

class CRecordWAVAudioLinkContainer : public CRecordsContainer
{
public:
	
	CRecordWAVAudioLinkContainer()
	{
	}

	~CRecordWAVAudioLinkContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};