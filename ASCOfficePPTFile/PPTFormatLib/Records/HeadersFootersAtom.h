#pragma once
#include "../Reader/Records.h"

class CRecordHeadersFootersContainer : public CRecordsContainer
{
};

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
};

class CRecordMetaCharacterAtom : public CUnknownRecord
{
public:
	DWORD m_nPosition;


	CRecordMetaCharacterAtom()
	{
		m_nPosition = -1;
	}

	~CRecordMetaCharacterAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nPosition =  StreamUtils::ReadDWORD(pStream);
	}
};

class CRecordGenericDateMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordRTFDateTimeMetaAtom : public CRecordGenericDateMetaAtom
{
public:
	std::string m_strFormat;
	
	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordGenericDateMetaAtom::ReadFromStream(oHeader, pStream);

		m_strFormat = StreamUtils::ReadStringA(pStream, 128);
	}
};

class CRecordDateTimeMetaAtom : public CRecordGenericDateMetaAtom
{
public:
	BYTE m_FormatID;
	
	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		CRecordGenericDateMetaAtom::ReadFromStream(oHeader, pStream);

		m_FormatID = StreamUtils::ReadBYTE(pStream);
	}
};


class CRecordFooterMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordHeaderMetaAtom : public CRecordMetaCharacterAtom
{
};

class CRecordSlideNumberMetaAtom : public CRecordMetaCharacterAtom
{
};