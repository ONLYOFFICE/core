#pragma once
#include "../Reader/Records.h"

class CRecordPlaceHolderAtom : public CUnknownRecord
{
public:
	LONG m_nPosition;
	BYTE m_nPlacementID;
	BYTE m_nSize;

public:
	
	CRecordPlaceHolderAtom()
	{
	}

	~CRecordPlaceHolderAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nPosition = StreamUtils::ReadLONG(pStream);
		m_nPlacementID = StreamUtils::ReadBYTE(pStream);
		m_nSize = StreamUtils::ReadBYTE(pStream);

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
		
		CDirectory::WriteValueToNode(_T("Position"), (DWORD)m_nPosition, &oWriter);
		CDirectory::WriteValueToNode(_T("PlacementID"), (DWORD)m_nPlacementID, &oWriter);
		CDirectory::WriteValueToNode(_T("Size"), (DWORD)m_nSize, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};