#pragma once
#include "../Reader/Records.h"

class CRecordTextPFExceptionAtom : public CUnknownRecord
{
public:
	CTextPFRun_ppt m_oPFRun;
	
	CRecordTextPFExceptionAtom()
	{
	}

	~CRecordTextPFExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oPFRun.LoadFromStream(pStream, false);

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("TextPFExceptionAtom"));

		oWriter.WriteString(m_oPFRun.m_oRun.ToString(0));

		oWriter.WriteNodeEnd(_T("TextPFExceptionAtom"));
		return oWriter.GetXmlString();
	}
};