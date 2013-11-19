#pragma once
#include "..\Reader\Records.h"

class CRecordTextPFExceptionAtom : public CUnknownRecord
{
public:
	STextPFRun m_oPFRun;
	
	CRecordTextPFExceptionAtom()
	{
		m_oPFRun.bIsExt = FALSE;
	}

	~CRecordTextPFExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oPFRun.ReadFromStream(pStream);

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("TextPFExceptionAtom"));

		oWriter.WriteString(m_oPFRun.ToString());

		oWriter.WriteNodeEnd(_T("TextPFExceptionAtom"));
		return oWriter.GetXmlString();
	}
};