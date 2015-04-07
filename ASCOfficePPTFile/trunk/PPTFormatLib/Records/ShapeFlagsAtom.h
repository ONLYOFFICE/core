#pragma once
#include "../Reader/Records.h"

class CRecordShapeFlagsAtom : public CUnknownRecord
{
	BYTE m_nFlags;

public:
	
	CRecordShapeFlagsAtom()
	{
	}

	~CRecordShapeFlagsAtom()
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