#pragma once
#include "../Reader/Records.h"

class CRecordInteractiveInfoAtom : public CUnknownRecord
{
public:
	
	CRecordInteractiveInfoAtom()
	{
	}

	~CRecordInteractiveInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
	virtual CString ToString()
	{
		return CUnknownRecord::ToString();
	}
};