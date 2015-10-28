#pragma once
#include "../Reader/Records.h"

class CRecordNoZoomViewInfoAtom : public CUnknownRecord
{
	SScalingAtom m_oCurScale;
	SPointAtom m_nOrigin;

	BOOL1 m_bDraftMode;

public:
	
	CRecordNoZoomViewInfoAtom()
	{
	}

	~CRecordNoZoomViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};