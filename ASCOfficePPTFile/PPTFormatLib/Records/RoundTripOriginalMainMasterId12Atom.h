#pragma once
#include "../Reader/Records.h"

class CRecordTripOriginalMasterId12Atom : public CUnknownRecord
{
public:
	DWORD m_dwID;

public:
	
	CRecordTripOriginalMasterId12Atom()
	{
		m_dwID = 0xFFFFFFFF;
	}

	~CRecordTripOriginalMasterId12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_dwID = StreamUtils::ReadDWORD(pStream);
	}

};
