#pragma once
#include "..\Reader\Records.h"

class CRecordVBAInfoAtom : public CUnknownRecord
{
	UINT m_nObjStgDataRef;
	UINT m_nHasMacros;
	UINT m_nVersion;

public:
	
	CRecordVBAInfoAtom()
	{
	}

	~CRecordVBAInfoAtom()
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