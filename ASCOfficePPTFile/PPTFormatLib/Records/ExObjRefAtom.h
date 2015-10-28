#pragma once
#include "../Reader/Records.h"

class CRecordExObjRefAtom : public CUnknownRecord
{
public:
	DWORD m_nExObjID;

public:
	
	CRecordExObjRefAtom()
	{
	}

	~CRecordExObjRefAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nExObjID = StreamUtils::ReadDWORD(pStream);
	}
};