#pragma once
#include "../Reader/Records.h"

class CRecordGridSpacing10Atom : public CUnknownRecord
{
	INT m_nX;
	INT m_nY;

public:
	
	CRecordGridSpacing10Atom()
	{
	}

	~CRecordGridSpacing10Atom()
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