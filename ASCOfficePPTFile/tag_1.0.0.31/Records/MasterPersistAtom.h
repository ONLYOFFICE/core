#pragma once
#include "..\Reader\Records.h"

class CRecordMasterPersistAtom : public CUnknownRecord
{
	UINT m_nPsrRef;
	INT m_nMasterID;

public:
	
	CRecordMasterPersistAtom()
	{
	}

	~CRecordMasterPersistAtom()
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