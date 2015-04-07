#pragma once
#include "../Reader/Records.h"

class CRecordExOleLinkAtom : public CUnknownRecord
{
	UINT m_nSlideID;
	UINT m_nOleUpdateMode;

public:
	
	CRecordExOleLinkAtom()
	{
	}

	~CRecordExOleLinkAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};