#pragma once
#include "..\Reader\Records.h"

class CRecordGuideAtom : public CUnknownRecord
{
	DWORD m_nType;
	DWORD m_nPos;

public:
	
	CRecordGuideAtom()
	{
	}

	~CRecordGuideAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		m_nType = StreamUtils::ReadDWORD(pStream);
		m_nPos = StreamUtils::ReadDWORD(pStream);
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
		
		CString str = _T("Unknown");
		if (0 == m_nType)
		{
			str = _T("horizontal");
		}
		else if (1 == m_nType)
		{
			str = _T("vertical");
		}
		CDirectory::WriteValueToNode(_T("Type"), str, &oWriter);
		CDirectory::WriteValueToNode(_T("Pos"), m_nPos, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};