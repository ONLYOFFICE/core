#pragma once
#include "..\Reader\Records.h"

class CRecordSlidePersistAtom : public CUnknownRecord
{
public:
	DWORD m_nPsrRef;
	
	bool m_bShouldCollapse;
	bool m_bNonOutlineData;

	INT m_nNumberText;
	DWORD m_nSlideID;

public:
	
	CRecordSlidePersistAtom()
	{
	}

	~CRecordSlidePersistAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_nPsrRef = StreamUtils::ReadDWORD(pStream);

		BYTE Mem = 0;
		Mem = StreamUtils::ReadBYTE(pStream);
		m_bShouldCollapse = ((Mem & 0x02) == 0x02);
		m_bNonOutlineData = ((Mem & 0x04) == 0x04);

		StreamUtils::StreamSkip(3, pStream);

		m_nNumberText = (INT)StreamUtils::ReadLONG(pStream);
		m_nSlideID = StreamUtils::ReadDWORD(pStream);

		StreamUtils::StreamSkip(4, pStream);
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
		
		CDirectory::WriteValueToNode(_T("PsrRef"), m_nPsrRef, &oWriter);
		
		CDirectory::WriteValueToNode(_T("ShouldCollapse"), m_bShouldCollapse, &oWriter);
		CDirectory::WriteValueToNode(_T("NonOutlineData"), m_bNonOutlineData, &oWriter);

		CDirectory::WriteValueToNode(_T("NumberText_PlaceholderShapes"), (DWORD)m_nNumberText, &oWriter);
		CDirectory::WriteValueToNode(_T("SlideID"), m_nSlideID, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};