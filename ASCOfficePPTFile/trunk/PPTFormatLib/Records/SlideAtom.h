#pragma once
#include "../Reader/Records.h"

class CRecordSlideAtom : public CUnknownRecord
{
public:
	SSlideLayoutAtom m_oLayout;
	UINT m_nMasterIDRef;
	UINT m_nNotesIDRef;
	
	bool m_bMasterObjects;
	bool m_bMasterScheme;
	bool m_bMasterBackground;

public:
	
	CRecordSlideAtom()
	{
	}

	~CRecordSlideAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oLayout.m_nGeom = (INT)StreamUtils::ReadLONG(pStream);
		for (int nIndex = 0; nIndex < 8; nIndex++)
		{
			m_oLayout.m_pPlaceHolderID[nIndex] = StreamUtils::ReadBYTE(pStream);
		}

		m_nMasterIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nNotesIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

		USHORT nFlag = StreamUtils::ReadWORD(pStream);
		m_bMasterObjects = ((nFlag & 0x01) == 0x01);
		m_bMasterScheme = ((nFlag & 0x02) == 0x02);
		m_bMasterBackground = ((nFlag & 0x04) == 0x04);

		StreamUtils::StreamSkip(2, pStream);
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
		
		oWriter.WriteString(m_oLayout.ToString());
				
		CDirectory::WriteValueToNode(_T("MasterIDRef"), (DWORD)m_nMasterIDRef, &oWriter);
		CDirectory::WriteValueToNode(_T("NotesIDRef"), (DWORD)m_nNotesIDRef, &oWriter);
		
		CDirectory::WriteValueToNode(_T("MasterObjects"), m_bMasterObjects, &oWriter);
		CDirectory::WriteValueToNode(_T("MasterScheme"), m_bMasterScheme, &oWriter);
		CDirectory::WriteValueToNode(_T("MasterBackground"), m_bMasterBackground, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};