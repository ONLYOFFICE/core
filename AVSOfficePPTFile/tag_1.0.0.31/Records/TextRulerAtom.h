#pragma once
#include "..\Reader\Records.h"

class CRecordTextRulerAtom : public CUnknownRecord
{
public:
	STextRuler m_oTextRuler;

public:
	
	CRecordTextRulerAtom()
	{
	}

	~CRecordTextRulerAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		LONG lOffset = 0;
		StreamUtils::StreamPosition(lOffset, pStream);

		m_oTextRuler.ReadFromStream(pStream);

		// на всякий случай...
		StreamUtils::StreamSeek(lOffset + m_oHeader.RecLen, pStream);
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
		
		oWriter.WriteString(m_oTextRuler.ToString());

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};