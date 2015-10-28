#pragma once
#include "../Reader/Records.h"

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

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};