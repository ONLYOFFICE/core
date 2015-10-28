#pragma once
#include "../Reader/Records.h"

class CRecordExHyperlinkContainer : public CRecordsContainer
{
public:
	CRecordExHyperlinkContainer()
	{
	}

	~CRecordExHyperlinkContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordsContainer::ReadFromStream(oHeader, pStream);
	}
};
class CRecordExHyperlinkAtom : public CUnknownRecord
{

public:
	UINT m_nHyperlinkID;
	
	CRecordExHyperlinkAtom()
	{
	}

	~CRecordExHyperlinkAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nHyperlinkID = StreamUtils::ReadDWORD(pStream);
	}
};