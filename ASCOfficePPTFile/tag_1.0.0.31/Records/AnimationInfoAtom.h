#pragma once
#include "..\Reader\Records.h"

class CRecordAnimationInfoAtom : public CUnknownRecord
{
	// поля...

public:
	
	CRecordAnimationInfoAtom()
	{
	}

	~CRecordAnimationInfoAtom()
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