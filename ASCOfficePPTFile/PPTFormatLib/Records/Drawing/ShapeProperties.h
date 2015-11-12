#pragma once

#include "../../Reader/Records.h"
#include "../../../../ASCOfficePPTXFile/Editor/Drawing/Shapes/BaseShape/PPTShape/ElementSettings.h"

class CRecordShapeProperties : public CUnknownRecord
{
public:
	CProperties m_oProperties;

public:
	
	CRecordShapeProperties()
	{
	}

	~CRecordShapeProperties()
	{
	}


	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		LONG lPosition = 0;
		StreamUtils::StreamPosition(lPosition, pStream);

		m_oProperties.FromStream(pStream, m_oHeader.RecInstance);
		DWORD dwLen = m_oProperties.GetLen();

		// это на всякий случай, может там напридумывают проперти с complex - 
		// которые мы не поддерживаем...
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
};