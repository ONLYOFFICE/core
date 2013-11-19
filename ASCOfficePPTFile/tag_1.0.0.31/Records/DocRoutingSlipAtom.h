#pragma once
#include "..\Reader\Records.h"

class CRecordDocRoutingSlipAtom : public CUnknownRecord
{
	struct SDocRoutingSlipAtom
	{
		USHORT m_nType;
		USHORT m_nLength;
		CString m_strText;
	};

	UINT m_nLength;
	UINT m_nRecipientCount;
	UINT m_nCurrentRecipient;
	USHORT m_usFlags;

	SDocRoutingSlipAtom m_oOriginatorString;
	SDocRoutingSlipAtom m_oRgRecipientRoutingSlipStrings;
	SDocRoutingSlipAtom m_oSubjectString;
	SDocRoutingSlipAtom m_oMessageString;

public:
	
	CRecordDocRoutingSlipAtom()
	{
	}

	~CRecordDocRoutingSlipAtom()
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