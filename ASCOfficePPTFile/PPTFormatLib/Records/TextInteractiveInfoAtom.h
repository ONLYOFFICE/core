#pragma once
#include "../Reader/Records.h"

class CRecordTextInteractiveInfoAtom : public CUnknownRecord
{
public:
	LONG m_lStart;
	LONG m_lEnd;

public:
	
	CRecordTextInteractiveInfoAtom()
	{
		m_lStart	= 0;
		m_lEnd		= 0;
	}

	~CRecordTextInteractiveInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		
		m_lStart	= StreamUtils::ReadLONG(pStream);
		m_lEnd		= StreamUtils::ReadLONG(pStream);
	}

};