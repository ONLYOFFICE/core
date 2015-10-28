#pragma once
#include "../Reader/Records.h"

class CRecordTextRulerAtom : public CUnknownRecord
{
public:
	NSPresentationEditor::CTextRuler m_oTextRuler;

public:
	
	CRecordTextRulerAtom()
	{
	}

	~CRecordTextRulerAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		LONG lOffset = 0;
		StreamUtils::StreamPosition(lOffset, pStream);

		NSStreamReader::Read(pStream, m_oTextRuler);

		// на всякий случай...
		StreamUtils::StreamSeek(lOffset + m_oHeader.RecLen, pStream);
	}

};