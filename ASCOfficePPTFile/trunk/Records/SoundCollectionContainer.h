#pragma once
#include "..\Reader\Records.h"

class CRecordSoundCollectionContainer : public CRecordsContainer
{
public:
	
	CRecordSoundCollectionContainer()
	{
	}

	~CRecordSoundCollectionContainer()
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