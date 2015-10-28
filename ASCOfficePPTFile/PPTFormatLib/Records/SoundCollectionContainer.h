#pragma once
#include "../Reader/Records.h"

class CRecordSoundCollectionContainer : public CRecordsContainer
{
public:
	
	CRecordSoundCollectionContainer()
	{
	}

	~CRecordSoundCollectionContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
};