#pragma once
#include "ExMediaAtom.h"

class CRecordExCDAudioContainer : public CRecordsContainer
{
public:
	CRecordExMediaAtom	m_oMedia;
	
	double				m_dStartTime;
	double				m_dEndTime;

public:
	
	CRecordExCDAudioContainer()
	{
		m_dStartTime	= 0;
		m_dEndTime		= 0;
	}

	~CRecordExCDAudioContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		SRecordHeader oExHeader;
		oExHeader.ReadFromStream(pStream);

		m_oMedia.ReadFromStream(oExHeader, pStream);

		StreamUtils::StreamSkip(8, pStream);

		BYTE nTrack1		= StreamUtils::ReadBYTE(pStream);
		BYTE nMinute1		= StreamUtils::ReadBYTE(pStream);
		BYTE nSecond1		= StreamUtils::ReadBYTE(pStream);
		BYTE nFrame1		= StreamUtils::ReadBYTE(pStream);

		BYTE nTrack2		= StreamUtils::ReadBYTE(pStream);
		BYTE nMinute2		= StreamUtils::ReadBYTE(pStream);
		BYTE nSecond2		= StreamUtils::ReadBYTE(pStream);
		BYTE nFrame2		= StreamUtils::ReadBYTE(pStream);
		
		m_dStartTime		= 60000 * nMinute1 + 1000 * nSecond1;
		m_dEndTime			= 60000 * nMinute2 + 1000 * nSecond2;
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteAttribute(_T("start"), m_dStartTime);
		oWriter.WriteAttribute(_T("end"), m_dEndTime);

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		oWriter.WriteString(m_oMedia.ToString());

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};