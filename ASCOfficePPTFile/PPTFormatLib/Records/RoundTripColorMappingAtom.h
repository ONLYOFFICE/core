#pragma once
#include "../Reader/Records.h"

class CRecordRoundTripColorMappingAtom : public CUnknownRecord
{
	CStringW m_pString;

public:
	
	CRecordRoundTripColorMappingAtom()
	{
	}

	~CRecordRoundTripColorMappingAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};