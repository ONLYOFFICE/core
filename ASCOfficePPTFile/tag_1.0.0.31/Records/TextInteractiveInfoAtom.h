#pragma once
#include "..\Reader\Records.h"

class CRecordTextInteractiveInfoAtom : public CUnknownRecord
{
public:
	LONG m_lStart;
	LONG m_lEnd;

public:
	
	CRecordTextInteractiveInfoAtom()
	{
		m_lStart	= 0;
		m_lEnd		= 0;
	}

	~CRecordTextInteractiveInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		
		m_lStart	= StreamUtils::ReadLONG(pStream);
		m_lEnd		= StreamUtils::ReadLONG(pStream);
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
		
		CDirectory::WriteValueToNode(_T("start"), m_lStart, &oWriter);
		CDirectory::WriteValueToNode(_T("end"), m_lEnd, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};