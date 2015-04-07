#pragma once
#include "../Reader/Records.h"

class CRecordBookmarkSeedAtom : public CUnknownRecord
{
	INT m_nBookmarkID;

public:
	
	CRecordBookmarkSeedAtom()
	{
	}

	~CRecordBookmarkSeedAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};