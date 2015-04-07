#pragma once
#include "../Reader/Records.h"

class CRecordExObjListAtom : public CUnknownRecord
{
	INT m_nObjectIdSeed;

public:
	
	CRecordExObjListAtom()
	{
	}

	~CRecordExObjListAtom()
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