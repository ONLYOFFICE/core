#pragma once
#include "../Reader/Records.h"

class CRecordNormalViewSetInfoAtom : public CUnknownRecord
{
	SRatioAtom m_oLeftPortion;
	SRatioAtom m_oTopPortion;

	BYTE m_nVertBarState;
	BYTE m_HorizBarState;

	BOOL1 m_fPreferSingleSet;
	BYTE m_nFlags;

public:
	
	CRecordNormalViewSetInfoAtom()
	{
	}

	~CRecordNormalViewSetInfoAtom()
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