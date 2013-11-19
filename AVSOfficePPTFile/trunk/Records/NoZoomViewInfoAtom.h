#pragma once
#include "..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};