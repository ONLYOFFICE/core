#pragma once
#include "../Reader/Records.h"

class CRecordZoomViewInfoAtom : public CUnknownRecord
{
	SScalingAtom m_oCurScale;
	SPointAtom m_nOrigin;

	BOOL1 m_bUseVarScale;
	BOOL1 m_bDraftMode;

public:
	
	CRecordZoomViewInfoAtom()
	{
	}

	~CRecordZoomViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};