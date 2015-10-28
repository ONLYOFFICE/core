#pragma once
#include "../Reader/Records.h"

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

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}

};