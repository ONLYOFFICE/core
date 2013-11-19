#pragma once
#include "..\Reader\Records.h"

class CRecordExObjListContainer : public CRecordsContainer
{
public:
	
	CRecordExObjListContainer()
	{
	}

	~CRecordExObjListContainer()
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