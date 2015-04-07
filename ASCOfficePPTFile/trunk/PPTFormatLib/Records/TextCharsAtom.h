#pragma once
#include "../Reader/Records.h"

class CRecordTextCharsAtom : public CUnknownRecord
{
public:
	CStringW m_strText;

public:
	
	CRecordTextCharsAtom()
	{
	}

	~CRecordTextCharsAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_strText = StreamUtils::ReadCStringW(pStream, m_oHeader.RecLen / 2);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));
		oWriter.WriteAttribute(_T("text"), (CString)m_strText);

		oWriter.WriteNodeEnd(strName, TRUE);

		return oWriter.GetXmlString();
	}
};