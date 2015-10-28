#pragma once
#include "../Reader/Records.h"

class CRecordViewInfoAtom : public CUnknownRecord
{
	SScalingAtom m_oCurScale;
	SScalingAtom m_oPrevScale;
	SPointAtom m_oViewSize;
	SPointAtom m_oOrigin;

	BOOL1 m_bZoomToFit;
	BOOL1 m_bDraftMode;

public:
	
	CRecordViewInfoAtom()
	{
	}

	~CRecordViewInfoAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oCurScale.FromStream(pStream);
		m_oPrevScale.FromStream(pStream);
		NSStreamReader::Read(pStream, m_oViewSize);
		NSStreamReader::Read(pStream, m_oOrigin);

		m_bZoomToFit = StreamUtils::ReadBYTE(pStream);
		m_bDraftMode = StreamUtils::ReadBYTE(pStream);

		// 2 байта зарезервированы...
		StreamUtils::StreamSkip(2, pStream);
	}

};