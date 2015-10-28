#pragma once
#include "../Reader/Records.h"

class CRecordTripCompositeMasterId12Atom : public CUnknownRecord
{
public:
	DWORD m_dwID;

public:
	
	CRecordTripCompositeMasterId12Atom()
	{
		m_dwID = 0xFFFFFFFF;
	}

	~CRecordTripCompositeMasterId12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_dwID = StreamUtils::ReadDWORD(pStream);
	}
};