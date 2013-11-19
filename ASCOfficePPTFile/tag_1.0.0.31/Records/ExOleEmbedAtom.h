#pragma once
#include "..\Reader\Records.h"

class CRecordExOleEmbedAtom : public CUnknownRecord
{
	UINT m_nColorFollow;
	BOOL1 m_nCantLockServer;
	BOOL1 m_nNoSizeToServer;
	BOOL1 m_nIsTable;

public:
	
	CRecordExOleEmbedAtom()
	{
	}

	~CRecordExOleEmbedAtom()
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