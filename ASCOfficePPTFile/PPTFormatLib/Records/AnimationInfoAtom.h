#pragma once
#include "../Reader/Records.h"

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

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};