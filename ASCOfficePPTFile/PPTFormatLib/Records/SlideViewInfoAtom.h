#pragma once
#include "../Reader/Records.h"

class CRecordSlideViewInfoAtom : public CUnknownRecord
{
	BOOL1 m_bShowGuides;
	BOOL1 m_bSnapToGrid;
	BOOL1 m_bSnapToShape;

public:
	
	CRecordSlideViewInfoAtom()
	{
	}

	~CRecordSlideViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_bShowGuides = StreamUtils::ReadBYTE(pStream);
		m_bSnapToGrid = StreamUtils::ReadBYTE(pStream);
		m_bSnapToShape = StreamUtils::ReadBYTE(pStream);
	}

};