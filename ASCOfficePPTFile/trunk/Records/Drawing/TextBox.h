#pragma once
#include "..\..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};