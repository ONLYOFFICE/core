#pragma once
#include "../Reader/Records.h"

class CRecordDocumentAtom : public CUnknownRecord
{
public:
	SPointAtom m_oSlideSize;
	SPointAtom m_oNotesSize;

	SRatioAtom m_oServerZoom; 
	
	UINT m_nNotesMasterPersistIDRef;
	UINT m_nHandoutMasterPersistIDRef;
	
	USHORT m_nFirstSlideNum; 
	NSOfficePPT::SlideSizeType m_nSlideSizeType;

	BOOL1 m_bSaveWithFonts;  
	BOOL1 m_bOmitTitlePlace;

	BOOL1 m_bRightToLeft;

	BOOL1 m_bShowComments;

public:
	
	CRecordDocumentAtom()
	{
	}

	~CRecordDocumentAtom()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_oSlideSize.X = StreamUtils::ReadLONG(pStream);
		m_oSlideSize.Y = StreamUtils::ReadLONG(pStream);

		m_oNotesSize.X = StreamUtils::ReadLONG(pStream);
		m_oNotesSize.Y = StreamUtils::ReadLONG(pStream);

		m_oServerZoom.Number = StreamUtils::ReadLONG(pStream);
		m_oServerZoom.Denom = StreamUtils::ReadLONG(pStream);

		m_nNotesMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);
		m_nHandoutMasterPersistIDRef = (UINT)StreamUtils::ReadDWORD(pStream);

		m_nFirstSlideNum = StreamUtils::ReadWORD(pStream);
		m_nSlideSizeType = (NSOfficePPT::SlideSizeType)StreamUtils::ReadWORD(pStream);

		m_bSaveWithFonts = StreamUtils::ReadBYTE(pStream);
		m_bOmitTitlePlace = StreamUtils::ReadBYTE(pStream);
		m_bRightToLeft = StreamUtils::ReadBYTE(pStream);
		m_bShowComments = StreamUtils::ReadBYTE(pStream);
	}

};