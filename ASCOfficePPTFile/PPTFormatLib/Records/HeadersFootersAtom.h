#pragma once
#include "../Reader/Records.h"

class CRecordHeadersFootersAtom : public CUnknownRecord
{
public:
	WORD m_nFormatID;
	
	bool m_bHasDate;
	bool m_bHasTodayDate;
	bool m_bHasUserDate;
	bool m_bHasSlideNumber;
	bool m_bHasHeader;
	bool m_bHasFooter;

	CRecordHeadersFootersAtom()
	{
	}

	~CRecordHeadersFootersAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nFormatID = StreamUtils::ReadWORD(pStream);

		USHORT nFlag		= StreamUtils::ReadWORD(pStream);
		m_bHasDate			= ((nFlag & 0x01) == 0x01);
		m_bHasTodayDate		= ((nFlag & 0x02) == 0x02);
		m_bHasUserDate		= ((nFlag & 0x04) == 0x04);
		m_bHasSlideNumber	= ((nFlag & 0x08) == 0x08);
		m_bHasHeader		= ((nFlag & 0x10) == 0x10);
		m_bHasFooter		= ((nFlag & 0x20) == 0x20);
	}
	virtual CString ToString()
	{
		return L"";
	}
};


class CRecordRoundTripHeaderFooterDefaults12Atom : public CUnknownRecord
{
public:
	WORD m_nFormatID;
	
	bool m_bIncludeDate;
	bool m_bIncludeFooter;
	bool m_bIncludeHeader;
	bool m_bIncludeSlideNumber;

	CRecordRoundTripHeaderFooterDefaults12Atom()
	{
	}

	~CRecordRoundTripHeaderFooterDefaults12Atom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		BYTE nFlag				= StreamUtils::ReadBYTE(pStream);

		m_bIncludeDate			= ((nFlag & 0x04) == 0x04);
		m_bIncludeFooter		= ((nFlag & 0x08) == 0x08);
		m_bIncludeHeader		= ((nFlag & 0x10) == 0x10);
		m_bIncludeSlideNumber	= ((nFlag & 0x20) == 0x20);
	}
	virtual CString ToString()
	{
		return _T("");
	}
};

