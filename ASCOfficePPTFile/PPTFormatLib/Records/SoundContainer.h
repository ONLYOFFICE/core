#pragma once
#include "../Reader/Records.h"

class CRecordSoundContainer : public CRecordsContainer
{
public:
	
	CRecordSoundContainer()
	{
	}

	~CRecordSoundContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};