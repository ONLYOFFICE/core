#pragma once
#include "../Reader/Records.h"

class CRecordTextBytesAtom : public CUnknownRecord
{
public:
	CString m_strText;

public:
	
	CRecordTextBytesAtom()
	{
	}

	~CRecordTextBytesAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

        std::string tmpStrTextA = string2std_string(StreamUtils::ReadCStringA(pStream, m_oHeader.RecLen));

        std::wstring tmpStrTextW (tmpStrTextA.begin(), tmpStrTextA.end());

        m_strText = std_string2string(tmpStrTextW);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));
		oWriter.WriteAttribute(_T("text"), m_strText);

		oWriter.WriteNodeEnd(strName, TRUE);

		return oWriter.GetXmlString();
	}
};
