#pragma once
#include "../Reader/Records.h"

class CRecordNotesPersistAtom : public CUnknownRecord
{
	UINT m_nPsrRef;
	INT m_nNotesID;

public:
	
	CRecordNotesPersistAtom()
	{
	}

	~CRecordNotesPersistAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};