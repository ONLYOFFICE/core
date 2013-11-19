#pragma once
#include "..\Reader\Records.h"

class CRecordGenericDateMCAtom : public CUnknownRecord
{
	UINT m_nPosition;

public:
	
	CRecordGenericDateMCAtom()
	{
	}

	~CRecordGenericDateMCAtom()
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