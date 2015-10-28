#pragma once
#include "../../Reader/Records.h"

class CRecordGroupShape : public CUnknownRecord
{
public:
	RECT m_oBounds;

public:
	
	CRecordGroupShape()
	{
	}

	~CRecordGroupShape()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oBounds.left		= StreamUtils::ReadLONG(pStream);
		m_oBounds.top		= StreamUtils::ReadLONG(pStream);
		m_oBounds.right		= StreamUtils::ReadLONG(pStream);
		m_oBounds.bottom	= StreamUtils::ReadLONG(pStream);

        m_oBounds.left		= (std::max)(0, (int)m_oBounds.left);
        m_oBounds.top		= (std::max)(0, (int)m_oBounds.top);
        m_oBounds.right		= (std::max)(0, (int)m_oBounds.right);
        m_oBounds.bottom	= (std::max)(0, (int)m_oBounds.bottom);
	}
};
