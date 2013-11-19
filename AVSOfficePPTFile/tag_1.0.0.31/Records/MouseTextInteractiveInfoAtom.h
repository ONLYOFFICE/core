#pragma once
#include "..\Reader\Records.h"

class CRecordMouseTextInteractiveInfoAtom : public CUnknownRecord
{
	STextRange m_oRange;

public:
	
	CRecordMouseTextInteractiveInfoAtom()
	{
	}

	~CRecordMouseTextInteractiveInfoAtom()
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