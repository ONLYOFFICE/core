#pragma once
#include "..\Reader\Records.h"

class CRecordFontEntityAtom : public CUnknownRecord
{
public:
	CStringW m_strFaceName;

    BYTE m_lfCharSet;

	bool m_bEmbedSubsetted;
	bool m_bIsRaster;
	bool m_bIsDevice;
	bool m_bIsTrueType;
	bool m_bIsNoFontSubstitution;

    BYTE m_lfPitchAndFamily;

public:
	
	CRecordFontEntityAtom()
	{
	}

	~CRecordFontEntityAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_strFaceName = StreamUtils::ReadCStringW(pStream, 32);
		m_lfCharSet = StreamUtils::ReadBYTE(pStream);
		
		BYTE Mem = 0;
		Mem = StreamUtils::ReadBYTE(pStream);
		m_bEmbedSubsetted = ((Mem & 0x01) == 0x01);

		Mem = 0;
		Mem = StreamUtils::ReadBYTE(pStream);

		m_bIsRaster = ((Mem & 0x01) == 0x01);
		m_bIsDevice = ((Mem & 0x02) == 0x02);
		m_bIsTrueType = ((Mem & 0x04) == 0x04);
		m_bIsNoFontSubstitution = ((Mem & 0x08) == 0x08);

		m_lfPitchAndFamily = StreamUtils::ReadBYTE(pStream);
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
		
		CDirectory::WriteValueToNode(_T("FaceName"), StreamUtils::ConvertCStringWToCString(m_strFaceName), &oWriter);
		
		CDirectory::WriteValueToNode(_T("CharSet"), (DWORD)m_lfCharSet, &oWriter);
		
		CDirectory::WriteValueToNode(_T("EmbedSubsetted"), m_bEmbedSubsetted, &oWriter);
		CDirectory::WriteValueToNode(_T("IsRaster"), m_bIsRaster, &oWriter);
		CDirectory::WriteValueToNode(_T("IsDevice"), m_bIsDevice, &oWriter);
		CDirectory::WriteValueToNode(_T("IsTrueType"), m_bIsTrueType, &oWriter);
		CDirectory::WriteValueToNode(_T("IsNoFontSubstitution"), m_bIsNoFontSubstitution, &oWriter);

		CDirectory::WriteValueToNode(_T("PitchAndFamily"), (DWORD)m_lfPitchAndFamily, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};