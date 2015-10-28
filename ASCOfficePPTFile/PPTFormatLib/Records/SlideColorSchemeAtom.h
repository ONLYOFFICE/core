#pragma once
#include "../Reader/Records.h"

class CRecordSlideColorSchemeAtom : public CUnknownRecord
{
	SColorAtom m_arColors[8];

public:
	
	CRecordSlideColorSchemeAtom()
	{
	}

	~CRecordSlideColorSchemeAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};

typedef		CRecordSlideColorSchemeAtom		CRecordSchemeListElementColorSchemeAtom;