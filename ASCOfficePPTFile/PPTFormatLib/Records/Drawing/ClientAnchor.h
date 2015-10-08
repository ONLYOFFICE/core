#pragma once
#include "../../Reader/Records.h"

class CRecordClientAnchor : public CUnknownRecord
{
public:
	SRectAtom m_oBounds;

public:
	
	CRecordClientAnchor()
	{
	}

	~CRecordClientAnchor()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		if (m_oHeader.RecLen == 0x00000008) //SSmallRectAtom 
		{
			m_oBounds.Top		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Left		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Right		= StreamUtils::ReadSHORT(pStream);
			m_oBounds.Bottom	= StreamUtils::ReadSHORT(pStream);
		}
		if (m_oHeader.RecLen == 0x00000010) //SRectAtom 
		{
			m_oBounds.Top		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Left		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Right		= StreamUtils::ReadLONG(pStream);
			m_oBounds.Bottom	= StreamUtils::ReadLONG(pStream);
		}
	}
	virtual CString ToString()
	{
		CString str = _T("");
		str.Format(_T("(%d,%d,%d,%d)"), m_oBounds.Left, m_oBounds.Top, m_oBounds.Right, m_oBounds.Bottom);

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