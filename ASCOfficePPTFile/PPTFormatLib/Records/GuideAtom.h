#pragma once
#include "../Reader/Records.h"

class CRecordGuideAtom : public CUnknownRecord
{
	DWORD m_nType;
	DWORD m_nPos;

public:
	
	CRecordGuideAtom()
	{
	}

	~CRecordGuideAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nType = StreamUtils::ReadDWORD(pStream);
		m_nPos = StreamUtils::ReadDWORD(pStream);
	}
};