#pragma once
#include "../../Reader/Records.h"

class CRecordChildAnchor : public CUnknownRecord
{
public:
	RECT m_oBounds;

public:
	
	CRecordChildAnchor()
	{
	}

	~CRecordChildAnchor()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_oBounds.left = StreamUtils::ReadLONG(pStream);
		m_oBounds.top = StreamUtils::ReadLONG(pStream);
		m_oBounds.right = StreamUtils::ReadLONG(pStream);
		m_oBounds.bottom = StreamUtils::ReadLONG(pStream);
	}
	virtual CString ToString()
	{
		CString str = _T("");
		str.Format(_T("(%d,%d,%d,%d)"), m_oBounds.left, m_oBounds.top, m_oBounds.right, m_oBounds.bottom);

		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));
		oWriter.WriteAttribute(_T("rect"), str);

		oWriter.WriteNodeEnd(strName, TRUE);

		return oWriter.GetXmlString();
	}
};