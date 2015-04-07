#pragma once
#include "../Reader/Records.h"

class CRecordTripOriginalMasterId12Atom : public CUnknownRecord
{
public:
	DWORD m_dwID;

public:
	
	CRecordTripOriginalMasterId12Atom()
	{
		m_dwID = 0xFFFFFFFF;
	}

	~CRecordTripOriginalMasterId12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_dwID = StreamUtils::ReadDWORD(pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		CDirectory::WriteValueToNode(_T("ID"), m_dwID, &oWriter);
		
		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};