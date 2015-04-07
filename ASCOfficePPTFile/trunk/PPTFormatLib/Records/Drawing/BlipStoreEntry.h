#pragma once
#include "../../Reader/Records.h"

class CRecordBlipStoreEntry : public CUnknownRecord
{
	NSOfficeDrawing::BlipType m_btWin32;
	NSOfficeDrawing::BlipType m_btMacOS;
	
	BYTE m_pRgbUid[16];
	WORD m_nTag;
	DWORD m_nSize;
	DWORD m_nCountRef;
	DWORD m_nFoDelay;

	NSOfficeDrawing::BlipUsage m_eUsage;
	BYTE m_nLenName;

	BYTE m_nUnused2;
	BYTE m_nUnused3;

public:
	
	CRecordBlipStoreEntry()
	{
	}

	~CRecordBlipStoreEntry()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_btWin32 = (NSOfficeDrawing::BlipType)StreamUtils::ReadBYTE(pStream);
		m_btMacOS = (NSOfficeDrawing::BlipType)StreamUtils::ReadBYTE(pStream);

		pStream->read(m_pRgbUid, 16);

		m_nTag = StreamUtils::ReadWORD(pStream);

		m_nSize = StreamUtils::ReadDWORD(pStream);
		m_nCountRef = StreamUtils::ReadDWORD(pStream);
		m_nFoDelay = StreamUtils::ReadDWORD(pStream);

		m_eUsage = (NSOfficeDrawing::BlipUsage)StreamUtils::ReadBYTE(pStream);
		
		m_nLenName = StreamUtils::ReadBYTE(pStream);
		
		m_nUnused2 = StreamUtils::ReadBYTE(pStream);
		m_nUnused3 = StreamUtils::ReadBYTE(pStream);
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
		
		CDirectory::WriteValueToNode(_T("Win32"), (DWORD)m_btWin32, &oWriter);
		CDirectory::WriteValueToNode(_T("MacOS"), (DWORD)m_btMacOS, &oWriter);

		//CDirectory::WriteValueToNode(_T("rgbUid"), CDirectory::BYTEArrayToString(m_pRgbUid, 16), &oWriter);

		CDirectory::WriteValueToNode(_T("Tag"), (DWORD)m_nTag, &oWriter);

		CDirectory::WriteValueToNode(_T("Size"), m_nSize, &oWriter);
		CDirectory::WriteValueToNode(_T("CountRef"), m_nCountRef, &oWriter);
		CDirectory::WriteValueToNode(_T("FoDelay"), m_nFoDelay, &oWriter);

		CDirectory::WriteValueToNode(_T("Usage"), (DWORD)m_eUsage, &oWriter);

		CDirectory::WriteValueToNode(_T("LenName"), (DWORD)m_nLenName, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};