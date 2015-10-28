#pragma once
#include "../../Reader/Records.h"

class CRecordClientAnchor : public CUnknownRecord
{
public:
	SRectAtom m_oBounds;

public:
	
	CRecordClientAnchor()
	{
	}

	~CRecordClientAnchor()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		if (m_oHeader.RecLen == 0x00000008) //SSmallRectAtom 
		{
			m_oBounds.Top		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Left		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Right		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Bottom	= StreamUtils::ReadSHORT(pStream);
		}
		if (m_oHeader.RecLen == 0x00000010) //SRectAtom 
		{
			m_oBounds.Top		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Left		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Right		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Bottom	= StreamUtils::ReadLONG(pStream);
		}
	}

};