#pragma once
#include "../Reader/Records.h"

class CRecordTextHeaderAtom : public CUnknownRecord
{
public:
	UINT m_nTextType;

public:
	
	CRecordTextHeaderAtom()
	{
	}

	~CRecordTextHeaderAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nTextType = (UINT)StreamUtils::ReadDWORD(pStream);
	}

};