#pragma once
#include "../Reader/Records.h"

class CRecordRoundTripThemeAtom : public CUnknownRecord
{
	CString m_strData;

public:
	
	CRecordRoundTripThemeAtom()
	{
	}

	~CRecordRoundTripThemeAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};