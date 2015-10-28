#pragma once
#include "../Reader/Records.h"

class CRecordExControlAtom : public CUnknownRecord
{
	UINT m_nSlideID;

public:
	
	CRecordExControlAtom()
	{
	}

	~CRecordExControlAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};