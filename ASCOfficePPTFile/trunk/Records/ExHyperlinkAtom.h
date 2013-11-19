#pragma once
#include "..\Reader\Records.h"

class CRecordExHyperlinkAtom : public CUnknownRecord
{
	UINT m_nHyperlinkID;

public:
	
	CRecordExHyperlinkAtom()
	{
	}

	~CRecordExHyperlinkAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};