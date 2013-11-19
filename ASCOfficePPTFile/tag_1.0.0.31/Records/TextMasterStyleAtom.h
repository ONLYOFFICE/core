#pragma once
#include "..\Reader\Records.h"

class CRecordTextMasterStyleAtom : public CUnknownRecord, public CTextMasterStyle
{
public:
	
	CRecordTextMasterStyleAtom() : CTextMasterStyle()
	{
	}

	~CRecordTextMasterStyleAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_nTextType = m_oHeader.RecInstance;

		BOOL bIsLevelsPresent = (0x05 <= m_oHeader.RecInstance);
		CTextMasterStyle::ReadFromStream(pStream, bIsLevelsPresent);

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
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
		
		oWriter.WriteString(CTextMasterStyle::ToString());

		oWriter.WriteNodeEnd(strName);
		return oWriter.GetXmlString();
	}
};