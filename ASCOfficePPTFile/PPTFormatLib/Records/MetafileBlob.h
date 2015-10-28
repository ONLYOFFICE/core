#pragma once
#include "../Reader/Records.h"

class CRecordMetafileBlob : public CUnknownRecord
{
	SHORT m_nMM;
	SHORT m_nExtX;
	SHORT m_nExtY;
	BYTE* m_pData;

public:
	
	CRecordMetafileBlob()
	{
	}

	~CRecordMetafileBlob()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}

};