#pragma once
#include "..\Reader\Records.h"

class CRecordNamedShowSlidesAtom : public CUnknownRecord
{
	CAtlArray<UINT> m_arRgSlideIdRef;

public:
	
	CRecordNamedShowSlidesAtom()
	{
	}

	~CRecordNamedShowSlidesAtom()
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