#pragma once
#include "../Reader/Records.h"

class CRecordBookmarkEntityAtom : public CUnknownRecord
{
	UINT m_nID;
	CStringW m_strName;

public:
	
	CRecordBookmarkEntityAtom()
	{
	}

	~CRecordBookmarkEntityAtom()
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