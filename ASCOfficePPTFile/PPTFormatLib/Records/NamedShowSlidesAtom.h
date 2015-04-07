#pragma once
#include "../Reader/Records.h"

class CRecordNamedShowSlidesAtom : public CUnknownRecord
{
	std::vector<UINT> m_arRgSlideIdRef;

public:
	
	CRecordNamedShowSlidesAtom()
	{
	}

	~CRecordNamedShowSlidesAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};