#pragma once
#include "..\Reader\Records.h"

class CRecordShapeFlags10Atom : public CUnknownRecord
{
	BYTE m_nFlags;

public:
	
	CRecordShapeFlags10Atom()
	{
	}

	~CRecordShapeFlags10Atom()
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