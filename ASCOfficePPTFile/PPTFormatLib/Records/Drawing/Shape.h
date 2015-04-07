#pragma once
#include "../../Reader/Records.h"

class CRecordShape : public CUnknownRecord
{
public:
	UINT m_nID;
	
	bool m_bGroup;  
	bool m_bChild;
	bool m_bPatriarch;     
	bool m_bDeleted;  
	bool m_bOleShape;
	bool m_bHaveMaster;  
	bool m_bFlipH;
	bool m_bFlipV;
	bool m_bConnector;
	bool m_bHaveAnchor;
	bool m_bBackground;
	bool m_bHaveSpt;

public:
	
	CRecordShape()
	{
	}

	~CRecordShape()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_nID = (UINT)StreamUtils::ReadDWORD(pStream);

		UINT nFlag = (UINT)StreamUtils::ReadDWORD(pStream);

		m_bGroup = ((nFlag & 0x01) == 0x01);
        m_bChild = ((nFlag & 0x02) == 0x02);
        m_bPatriarch = ((nFlag & 0x04) == 0x04);
        m_bDeleted = ((nFlag & 0x08) == 0x08);
        m_bOleShape = ((nFlag & 0x10) == 0x10);
        m_bHaveMaster = ((nFlag & 0x20) == 0x20);
        m_bFlipH = ((nFlag & 0x40) == 0x40);
        m_bFlipV = ((nFlag & 0x80) == 0x80);
        m_bConnector = ((nFlag & 0x0100) == 0x0100);
        m_bHaveAnchor = ((nFlag & 0x0200) == 0x0200);
        m_bBackground = ((nFlag & 0x0400) == 0x0400);
        m_bHaveSpt = ((nFlag & 0x0800) == 0x0800);

		LONG lSize = m_oHeader.RecLen - 8;
		if (lSize > 0)
		{
			StreamUtils::StreamSkip(lSize, pStream);
		}
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
		
		CDirectory::WriteValueToNode(_T("ShapeID"), (DWORD)m_nID, &oWriter);
		
		CDirectory::WriteValueToNode(_T("Group"), m_bGroup, &oWriter);
		CDirectory::WriteValueToNode(_T("Child"), m_bChild, &oWriter);
		CDirectory::WriteValueToNode(_T("Patriarch"), m_bPatriarch, &oWriter);
		CDirectory::WriteValueToNode(_T("Deleted"), m_bDeleted, &oWriter);
		CDirectory::WriteValueToNode(_T("OleShape"), m_bOleShape, &oWriter);
		CDirectory::WriteValueToNode(_T("HaveMaster"), m_bHaveMaster, &oWriter);
		CDirectory::WriteValueToNode(_T("FlipH"), m_bFlipH, &oWriter);
		CDirectory::WriteValueToNode(_T("FlipV"), m_bFlipV, &oWriter);
		CDirectory::WriteValueToNode(_T("Connector"), m_bConnector, &oWriter);
		CDirectory::WriteValueToNode(_T("HaveAnchor"), m_bHaveAnchor, &oWriter);
		CDirectory::WriteValueToNode(_T("Background"), m_bBackground, &oWriter);
		CDirectory::WriteValueToNode(_T("HaveSpt"), m_bHaveSpt, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};