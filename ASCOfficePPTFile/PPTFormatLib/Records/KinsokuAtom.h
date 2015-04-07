#pragma once
#include "../Reader/Records.h"

class CRecordKinsokuAtom : public CUnknownRecord
{
	UINT m_nLevel;

public:
	
	CRecordKinsokuAtom()
	{
	}

	~CRecordKinsokuAtom()
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