#pragma once
#include "../Reader/Records.h"

class CRecordSlideViewInfoAtom : public CUnknownRecord
{
	BOOL1 m_bShowGuides;
	BOOL1 m_bSnapToGrid;
	BOOL1 m_bSnapToShape;

public:
	
	CRecordSlideViewInfoAtom()
	{
	}

	~CRecordSlideViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_bShowGuides = StreamUtils::ReadBYTE(pStream);
		m_bSnapToGrid = StreamUtils::ReadBYTE(pStream);
		m_bSnapToShape = StreamUtils::ReadBYTE(pStream);
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
		
		CDirectory::WriteValueToNode(_T("ShowGuides"), (DWORD)m_bShowGuides, &oWriter);
		CDirectory::WriteValueToNode(_T("SnapToGrid"), (DWORD)m_bSnapToGrid, &oWriter);
		CDirectory::WriteValueToNode(_T("SnapToShape"), (DWORD)m_bSnapToShape, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};