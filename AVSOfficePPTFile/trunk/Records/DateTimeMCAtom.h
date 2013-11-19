#pragma once
#include "..\Reader\Records.h"

class CRecordDateTimeMCAtom : public CUnknownRecord
{
	UINT m_mPosition;
	BYTE m_nIndex;

public:
	
	CRecordDateTimeMCAtom()
	{
	}

	~CRecordDateTimeMCAtom()
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