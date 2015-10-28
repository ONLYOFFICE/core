#pragma once
#include "../../Reader/Records.h"

class CRecordTextBox : public CUnknownRecord
{
	BYTE* m_pData;

public:
	
	CRecordTextBox()
	{
	}

	~CRecordTextBox()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}

};