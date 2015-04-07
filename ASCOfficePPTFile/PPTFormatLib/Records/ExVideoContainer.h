#pragma once
#include "../Reader/Records.h"

class CRecordExVideoContainer : public CRecordsContainer
{
public:
	
	CRecordExVideoContainer()
	{
	}

	~CRecordExVideoContainer()
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