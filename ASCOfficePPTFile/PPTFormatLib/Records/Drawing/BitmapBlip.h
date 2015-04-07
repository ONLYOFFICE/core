#pragma once
#include "../../Reader/Records.h"

class CRecordBitmapBlip : public CUnknownRecord
{
	BYTE m_pRgbUid[16];
	BYTE m_nTag;

	BYTE* m_pScan0;

public:
	
	CRecordBitmapBlip()
	{
	}

	~CRecordBitmapBlip()
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