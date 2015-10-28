#pragma once
#include "../Reader/Records.h"

class CRecordEndDocument : public CUnknownRecord
{
public:
	
	CRecordEndDocument()
	{
	}

	~CRecordEndDocument()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}
};