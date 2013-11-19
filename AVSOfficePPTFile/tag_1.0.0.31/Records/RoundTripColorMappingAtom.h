#pragma once
#include "..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};