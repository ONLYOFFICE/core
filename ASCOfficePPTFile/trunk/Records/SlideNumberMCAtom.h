#pragma once
#include "..\Reader\Records.h"

class CRecordSlideNumberMCAtom : public CUnknownRecord
{
	UINT m_nPosition;

public:
	
	CRecordSlideNumberMCAtom()
	{
	}

	~CRecordSlideNumberMCAtom()
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