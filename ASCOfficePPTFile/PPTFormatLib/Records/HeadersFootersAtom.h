#pragma once
#include "../Reader/Records.h"

class CRecordHeadersFootersAtom : public CUnknownRecord
{
	WORD m_nFormatID;
	
	bool m_bHasDate;
	bool m_bHasTodayDate;
	bool m_bHasUserDate;
	bool m_bHasSlideNumber;
	bool m_bHasHeader;
	bool m_bHasFooter;

public:
	
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
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		CDirectory::WriteValueToNode(_T("FormatID"), (DWORD)m_nFormatID, &oWriter);
		
		CDirectory::WriteValueToNode(_T("HasDate"), m_bHasDate, &oWriter);
		CDirectory::WriteValueToNode(_T("HasTodayDate"), m_bHasTodayDate, &oWriter);
		CDirectory::WriteValueToNode(_T("HasUserDate"), m_bHasUserDate, &oWriter);
		CDirectory::WriteValueToNode(_T("HasSlideNumber"), m_bHasSlideNumber, &oWriter);
		CDirectory::WriteValueToNode(_T("HasHeader"), m_bHasHeader, &oWriter);
		CDirectory::WriteValueToNode(_T("HasFooter"), m_bHasFooter, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};


class CRecordRoundTripHeaderFooterDefaults12Atom : public CUnknownRecord
{
	WORD m_nFormatID;
	
	bool m_bIncludeDate;
	bool m_bIncludeFooter;
	bool m_bIncludeHeader;
	bool m_bIncludeSlideNumber;

public:
	
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

