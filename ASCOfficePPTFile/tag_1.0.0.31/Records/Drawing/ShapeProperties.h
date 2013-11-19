#pragma once
#include "..\..\Reader\Records.h"

class CRecordShapeProperties : public CUnknownRecord
{
public:
	CProperties m_oProperties;

public:
	
	CRecordShapeProperties()
	{
	}

	~CRecordShapeProperties()
	{
	}


	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oProperties.FromStream(pStream, m_oHeader.RecInstance);
		DWORD dwLen = m_oProperties.GetLen();

		// это на всякий случай, может там напридумывают проперти с complex - 
		// которые мы не поддерживаем...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		CString str = m_oProperties.ToString();

		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		oWriter.WriteString(str);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};