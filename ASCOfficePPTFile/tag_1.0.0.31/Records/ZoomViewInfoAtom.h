#pragma once
#include "..\Reader\Records.h"

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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};