#pragma once
#include "..\Reader\Records.h"

class CRecordSSlideLayoutAtom : public CUnknownRecord
{
	INT m_nGeom;
	BYTE m_pPlaceHolderID[8];

public:
	
	CRecordSSlideLayoutAtom()
	{
	}

	~CRecordSSlideLayoutAtom()
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