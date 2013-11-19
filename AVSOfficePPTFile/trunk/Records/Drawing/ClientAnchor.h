#pragma once
#include "..\..\Reader\Records.h"

class CRecordClientAnchor : public CUnknownRecord
{
public:
	SSmallRectAtom m_oBounds;

public:
	
	CRecordClientAnchor()
	{
	}

	~CRecordClientAnchor()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;
		m_oBounds.Top = StreamUtils::ReadSHORT(pStream);
		m_oBounds.Left = StreamUtils::ReadSHORT(pStream);
		m_oBounds.Right = StreamUtils::ReadSHORT(pStream);
		m_oBounds.Bottom = StreamUtils::ReadSHORT(pStream);
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