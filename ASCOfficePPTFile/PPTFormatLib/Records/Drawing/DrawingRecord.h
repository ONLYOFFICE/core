#pragma once
#include "../../Reader/Records.h"

class CRecordDrawingRecord : public CUnknownRecord
{
	UINT m_nCountShapes;
    /// The last MSOSPID given to an SP in this DG
    UINT m_nPidCur;

public:
	
	CRecordDrawingRecord()
	{
	}

	~CRecordDrawingRecord()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nCountShapes = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nPidCur = (UINT)StreamUtils::ReadDWORD(pStream);
	}
};