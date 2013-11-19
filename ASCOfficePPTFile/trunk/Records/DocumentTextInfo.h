#pragma once
#include "..\Reader\Records.h"

class CRecordDocumentTextInfo : public CRecordsContainer
{
public:
	
	CRecordDocumentTextInfo()
	{
	}

	~CRecordDocumentTextInfo()
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