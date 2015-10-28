#pragma once
#include "../Reader/Records.h"

class CRecordOutlineTextRefAtom : public CUnknownRecord
{
public:
	DWORD m_nIndex;

public:
	
	CRecordOutlineTextRefAtom()
	{
	}

	~CRecordOutlineTextRefAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nIndex = StreamUtils::ReadDWORD(pStream);
	}
};