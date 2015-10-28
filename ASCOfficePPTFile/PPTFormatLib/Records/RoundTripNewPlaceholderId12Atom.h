#pragma once
#include "../Reader/Records.h"

class CRecordRoundTripNewPlaceholderId12Atom : public CUnknownRecord
{
	BYTE m_nFlags;

public:
	
	CRecordRoundTripNewPlaceholderId12Atom()
	{
	}

	~CRecordRoundTripNewPlaceholderId12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}

};