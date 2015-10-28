#pragma once

#include "../../Reader/Records.h"

class CPPTDocumentInfo;


class CRecordOfficeArtBlip  : public CUnknownRecord
{
public:
	CPPTDocumentInfo *	m_oDocumentInfo;
	std::wstring		m_sFileName;

	CString				m_strMemoryForder;

	CRecordOfficeArtBlip()
	{
		m_oDocumentInfo = NULL;
	}

	~CRecordOfficeArtBlip()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream);

};

class CRecordBitmapBlip : public CUnknownRecord
{
	BYTE m_pRgbUid[16];
	BYTE m_nTag;

	BYTE* m_pScan0;

public:
	
	CRecordBitmapBlip()
	{
	}

	~CRecordBitmapBlip()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		return CUnknownRecord::ReadFromStream(oHeader, pStream);
	}

};