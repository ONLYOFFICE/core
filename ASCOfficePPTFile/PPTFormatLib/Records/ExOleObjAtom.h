#pragma once
#include "../Reader/Records.h"

class CRecordExOleObjAtom : public CUnknownRecord
{
	UINT m_nDrawAspect;
	UINT m_nType;
	UINT m_nExObjID;
	UINT m_nSubType;
	UINT m_nPersistID;

public:
	
	CRecordExOleObjAtom()
	{
	}

	~CRecordExOleObjAtom()
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