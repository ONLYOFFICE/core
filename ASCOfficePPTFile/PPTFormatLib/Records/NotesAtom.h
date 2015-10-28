#pragma once
#include "../Reader/Records.h"

class CRecordNotesAtom : public CUnknownRecord
{
	UINT m_nSlideIDRef;
	
	bool m_bMasterObjects;
	bool m_bMasterScheme;
	bool m_bMasterBackground;

public:
	
	CRecordNotesAtom()
	{
	}

	~CRecordNotesAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_nSlideIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

		USHORT nFlag = StreamUtils::ReadWORD(pStream);
		m_bMasterObjects = ((nFlag & 0x01) == 0x01);
		m_bMasterScheme = ((nFlag & 0x02) == 0x02);
		m_bMasterBackground = ((nFlag & 0x04) == 0x04);

		StreamUtils::StreamSkip(2, pStream);
	}
};