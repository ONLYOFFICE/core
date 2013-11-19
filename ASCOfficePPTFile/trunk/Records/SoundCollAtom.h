#pragma once
#include "..\Reader\Records.h"

class CRecordSoundCollAtom : public CUnknownRecord
{
	INT m_nObjectIdSeed;

public:
	
	CRecordSoundCollAtom()
	{
	}

	~CRecordSoundCollAtom()
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