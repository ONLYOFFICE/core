#pragma once
#include "..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};