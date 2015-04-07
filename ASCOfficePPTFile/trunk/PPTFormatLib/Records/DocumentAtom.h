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
	virtual CString ToString()
	{
		XmlUtils::CXmlWriter oWriter;
		CString strName = GetRecordName((DWORD)m_oHeader.RecType);
		
		oWriter.WriteNodeBegin(strName, TRUE);
		oWriter.WriteAttribute(_T("length"), CDirectory::ToString(m_oHeader.RecLen));
		oWriter.WriteAttribute(_T("type"), CDirectory::ToString(m_oHeader.RecType));
		oWriter.WriteAttribute(_T("instance"), CDirectory::ToString(m_oHeader.RecInstance));

		oWriter.WriteNodeEnd(strName, TRUE, FALSE);
		
		oWriter.WriteString(m_oSlideSize.ToString());
		oWriter.WriteString(m_oNotesSize.ToString());

		oWriter.WriteString(m_oServerZoom.ToString());
		
		CDirectory::WriteValueToNode(_T("NotesMasterPersistIDRef"), (DWORD)m_nNotesMasterPersistIDRef, &oWriter);
		CDirectory::WriteValueToNode(_T("HandoutMasterPersistIDRef"), (DWORD)m_nHandoutMasterPersistIDRef, &oWriter);
		
		CDirectory::WriteValueToNode(_T("FirstSlideNum"), (DWORD)m_nFirstSlideNum, &oWriter);
		CDirectory::WriteValueToNode(_T("SlideSizeType"), (DWORD)m_nSlideSizeType, &oWriter);
		
		CDirectory::WriteValueToNode(_T("SaveWithFonts"), (DWORD)m_bSaveWithFonts, &oWriter);
		CDirectory::WriteValueToNode(_T("OmitTitlePlace"), (DWORD)m_bOmitTitlePlace, &oWriter);
		CDirectory::WriteValueToNode(_T("RightToLeft"), (DWORD)m_bRightToLeft, &oWriter);
		CDirectory::WriteValueToNode(_T("ShowComments"), (DWORD)m_bShowComments, &oWriter);

		oWriter.WriteNodeEnd(strName);

		return oWriter.GetXmlString();
	}
};