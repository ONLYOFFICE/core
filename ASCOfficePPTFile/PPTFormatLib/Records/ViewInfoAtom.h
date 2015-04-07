#pragma once
#include "../Reader/Records.h"

class CRecordViewInfoAtom : public CUnknownRecord
{
	SScalingAtom m_oCurScale;
	SScalingAtom m_oPrevScale;
	SPointAtom m_oViewSize;
	SPointAtom m_oOrigin;

	BOOL1 m_bZoomToFit;
	BOOL1 m_bDraftMode;

public:
	
	CRecordViewInfoAtom()
	{
	}

	~CRecordViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oCurScale.FromStream(pStream);
		m_oPrevScale.FromStream(pStream);
		NSStreamReader::Read(pStream, m_oViewSize);
		NSStreamReader::Read(pStream, m_oOrigin);

		m_bZoomToFit = StreamUtils::ReadBYTE(pStream);
		m_bDraftMode = StreamUtils::ReadBYTE(pStream);

		// 2 байта зарезервированы...
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
		
		CDirectory::WriteValueToNode(_T("CurScale"), m_oCurScale.ToString(), &oWriter);
		CDirectory::WriteValueToNode(_T("PrevScale"), m_oPrevScale.ToString(), &oWriter);

		CDirectory::WriteValueToNode(_T("ViewSize"), m_oViewSize.ToString(), &oWriter);
		CDirectory::WriteValueToNode(_T("Origin"), m_oOrigin.ToString(), &oWriter);

		CDirectory::WriteValueToNode(_T("ZoomToFit"), (DWORD)m_bZoomToFit, &oWriter);
		CDirectory::WriteValueToNode(_T("DraftMode"), (DWORD)m_bDraftMode, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};