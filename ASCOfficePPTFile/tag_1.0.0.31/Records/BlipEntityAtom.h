#pragma once
#include "..\Reader\Records.h"

class CRecordBlipEntityAtom : public CUnknownRecord
{
	BYTE m_nWinBlipType;
	BYTE* m_pImage;

public:
	
	CRecordBlipEntityAtom()
	{
	}

	~CRecordBlipEntityAtom()
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