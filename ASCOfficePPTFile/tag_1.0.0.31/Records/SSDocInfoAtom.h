#pragma once
#include "..\Reader\Records.h"

class CRecordSSDocInfoAtom : public CUnknownRecord
{
	SColorAtom m_oColor;
	INT m_nRestartTime;
	SHORT m_nStartSlide;
	SHORT m_nEndSlide;

	USHORT m_pName[32];
	USHORT m_nFlags;

public:
	
	CRecordSSDocInfoAtom()
	{
	}

	~CRecordSSDocInfoAtom()
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