#pragma once
#include "../../Reader/Records.h"

class CRecordChildAnchor : public CUnknownRecord
{
public:
	RECT m_oBounds;

public:
	
	CRecordChildAnchor()
	{
	}

	~CRecordChildAnchor()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_oBounds.left = StreamUtils::ReadLONG(pStream);
		m_oBounds.top = StreamUtils::ReadLONG(pStream);
		m_oBounds.right = StreamUtils::ReadLONG(pStream);
		m_oBounds.bottom = StreamUtils::ReadLONG(pStream);
	}
};