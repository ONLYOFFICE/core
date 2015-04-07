#pragma once
#include "ExMediaAtom.h"

class CRecordWAVAudioEmbeddedContainer : public CRecordsContainer
{
public:
	CRecordExMediaAtom	m_oMedia;
	
	DWORD				m_nSoundID;
	LONG				m_nDuration;

public:
	
	CRecordWAVAudioEmbeddedContainer()
	{
		m_nSoundID		= 0;
		m_nDuration		= 0;
	}

	~CRecordWAVAudioEmbeddedContainer()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		SRecordHeader oExHeader;
		oExHeader.ReadFromStream(pStream);

		m_oMedia.ReadFromStream(oExHeader, pStream);

		StreamUtils::StreamSkip(8, pStream);

		m_nSoundID	= StreamUtils::ReadDWORD(pStream);
		m_nDuration	= StreamUtils::ReadLONG(pStream);		
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteAttribute(_T("start"), CDirectory::ToString(m_nSoundID));
		oWriter.WriteAttribute(_T("end"), CDirectory::ToString(m_nDuration));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		oWriter.WriteString(m_oMedia.ToString());

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};