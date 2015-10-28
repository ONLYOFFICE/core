#pragma once
#include "../Reader/Records.h"

class CRecordCString : public CUnknownRecord
{
public:
	std::wstring m_strText;

public:
	
	CRecordCString()
	{
	}

	~CRecordCString()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_strText = StreamUtils::ReadStringW(pStream, m_oHeader.RecLen / 2);
	}

};