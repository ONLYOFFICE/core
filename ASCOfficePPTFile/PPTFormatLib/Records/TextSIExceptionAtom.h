#pragma once
#include "../Reader/Records.h"

class CRecordTextSIExceptionAtom : public CUnknownRecord
{
public:
	CTextSIRun m_oSIRun;
	
	CRecordTextSIExceptionAtom()
	{
	}

	~CRecordTextSIExceptionAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		NSStreamReader::Read(pStream, m_oSIRun, false);

		// это на всякий случай...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}

};