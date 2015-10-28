#pragma once
#include "../../Reader/Records.h"

class CRecordBlipStoreEntry : public CUnknownRecord
{
public:
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
};