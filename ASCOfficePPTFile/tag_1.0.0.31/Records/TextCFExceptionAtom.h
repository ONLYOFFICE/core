#pragma once
#include "..\Reader\Records.h"

class CRecordTextCFExceptionAtom : public CUnknownRecord
{
public:
	STextCFRun m_oCFRun;
	
	CRecordTextCFExceptionAtom()
	{
		m_oCFRun.bIsExt = FALSE;
	}

	~CRecordTextCFExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oCFRun.ReadFromStream(pStream);

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		oWriter.WriteNodeBegin(_T("TextCFExceptionAtom"));

		oWriter.WriteString(m_oCFRun.ToString());

		oWriter.WriteNodeEnd(_T("TextCFExceptionAtom"));
		return oWriter.GetXmlString();
	}
};