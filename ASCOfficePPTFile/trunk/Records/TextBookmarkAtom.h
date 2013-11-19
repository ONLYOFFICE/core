#pragma once
#include "..\Reader\Records.h"

class CRecordTextBookmarkAtom : public CUnknownRecord
{
	UINT m_nBegin;
	UINT m_nEnd;
	UINT m_nBookmarkID;

public:
	
	CRecordTextBookmarkAtom()
	{
	}

	~CRecordTextBookmarkAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};