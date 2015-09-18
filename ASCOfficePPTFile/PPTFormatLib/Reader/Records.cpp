#include "Records.h"

#include "../Records/RecordsIncluder.h"

#include <iostream>

void SRecordHeader::ToXmlWriter(XmlUtils::CXmlWriter* pWriter, POLE::Stream* pStream)
{
	if (IsContainer())
	{
		pWriter->WriteNodeBegin(GetRecordName((DWORD)RecType), TRUE);
		pWriter->WriteAttribute(_T("length"), (int)RecLen);
		pWriter->WriteNodeEnd(GetRecordName((DWORD)RecType), TRUE, FALSE);
	
		UINT lCurLen = 0;
		SRecordHeader oRec;
		
		while (lCurLen < RecLen)
		{
			if (oRec.ReadFromStream(pStream) == FALSE )
			{
				break;
			}
			
			lCurLen += (8 + oRec.RecLen);
			oRec.ToXmlWriter(pWriter, pStream);
		}
		pWriter->WriteNodeEnd(GetRecordName((DWORD)RecType));
	}
	else
	{
		IRecord* pRecord = CreateByType(*this);
		pRecord->ReadFromStream(*this, pStream);
		pWriter->WriteString(pRecord->ToString());
	}
}

void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
{
	m_oHeader = oHeader;
	m_arRecords.clear();

	LONG lPosition = 0;
	StreamUtils::StreamPosition(lPosition, pStream);
	
	m_oHeader = oHeader;

	UINT lCurLen = 0;
	ULONG lReadLen = 0;
	SRecordHeader oRec;
	while (lCurLen < m_oHeader.RecLen)
	{
		if (oRec.ReadFromStream(pStream) == FALSE )
		{
			break;
		}
		
		IRecord* pRecord = CreateByType(oRec);
		pRecord->ReadFromStream(oRec, pStream);

		m_arRecords.push_back(pRecord);
		lCurLen += (8 + oRec.RecLen);
	}
	if (lCurLen != m_oHeader.RecLen)
	{
		// нужно разобраться, что тут такое!!!
		LONG lPosition = 0;
		StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
	}
}

IRecord* CreateByType(SRecordHeader oHeader)
{
	IRecord* pRecord = NULL;
	switch (oHeader.RecType)
	{
	CREATE_BY_TYPE(RECORD_TYPE_SLIDELISTWITHTEXT			, CRecordSlideListWithText)
	CREATE_BY_TYPE(RECORD_TYPE_DOCUMENT_ATOM				, CRecordDocumentAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_ENDDOCUMENT				, CRecordEndDocument)
	//CREATE_BY_TYPE(RECORD_TYPE_SSLIDE_LAYOUT_ATOM			, CRecordSSlideLayoutAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_ATOM					, CRecordSlideAtom)
	CREATE_BY_TYPE(RECORD_TYPE_NOTES_ATOM					, CRecordNotesAtom) 
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_PERSIST_ATOM			, CRecordSlidePersistAtom)
	CREATE_BY_TYPE(RECORD_TYPE_ENVIRONMENT					, CRecordDocumentTextInfo)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMasterPersistAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordNotesPersistAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SSSLIDE_INFO_ATOM			, CSlideShowSlideInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_GUIDE_ATOM					, CRecordGuideAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordZoomViewInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordNoZoomViewInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordViewInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_VIEWINFO_ATOM			, CRecordSlideViewInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_VBAINFO_ATOM				, CRecordVBAInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_SSDOCINFO_ATOM				, CRecordSSDocInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_DOC_ROUTING_SLIP			, CRecordDocRoutingSlipAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXOBJLIST_ATOM				, CRecordExObjListAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordGridSpacing10Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRoundTripThemeAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRoundTripColorMappingAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_NAMEDSHOW_SLIDES			, CRecordNamedShowSlidesAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_SHEET_PROPERTIES			, CRecordNormalViewSetInfoAtom)

	CREATE_BY_TYPE(RECORD_TYPE_COLOR_SCHEME_ATOM			, CRecordColorSchemeAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_ORIGINALMAINMASTERID			, CRecordTripOriginalMasterId12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_COMPOSITEMASTERID			, CRecordTripCompositeMasterId12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCONTENTMASTERINFO12	, CRecordRoundTripContentMasterInfo12Atom)			// ECMA-376 дубль
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPEID12			, CRecordRoundTripShapeId12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPHFPLACEHOLDER12	, CRecordRoundTripHFPlaceholder12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCONTENTMASTERID	, CRecordRoundTripContentMasterId12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPOARTTEXTSTYLES12	, CRecordRoundTripOArtTextStyles12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPHEADERFOOTERDEFAULTS12ATOM		, CRecordRoundTripHeaderFooterDefaults12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPDOCFLAGS12						, CRecordRoundTripDocFlags12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPECHECKSUMFORCUSTOMLAYOUTS12	, CRecordRoundTripShapeCheckSumForCustomLayouts12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPNOTESMASTERTEXTSTYLES12			, CRecordRoundTripNotesMasterTextStyles12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCUSTOMTABLESTYLES12				, CRecordRoundTripCustomTableStyles12Atom)
	
	//CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_SEED_ATOM			, CRecordBookmarkSeedAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_COLOR_SCHEME_ATOM			, CRecordSlideColorSchemeAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordSchemeListElementColorSchemeAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordBlipEntityAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordExObjRefAtom)
	CREATE_BY_TYPE(RECORD_TYPE_OEPLACEHOLDER_ATOM			, CRecordPlaceHolderAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlagsAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlags10Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordRoundTripNewPlaceholderId12Atom)
	
	CREATE_BY_TYPE(RECORD_TYPE_OUTLINE_TEXTREF_ATOM			, CRecordOutlineTextRefAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TEXT_HEADER_ATOM				, CRecordTextHeaderAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TEXT_CHARS_ATOM				, CRecordTextCharsAtom)
	CREATE_BY_TYPE(RECORD_TYPE_STYLE_TEXTPROP_ATOM			, CRecordStyleTextPropAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TEXTSPECINFO_ATOM			, CRecordTextSpecInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_BASE_TEXTPROP_ATOM			, CRecordMasterTextPropAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TXMASTERSTYLE_ATOM			, CRecordTextMasterStyleAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_TXCFSTYLE_ATOM				, CRecordTextCFExceptionAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TXPFSTYLE_ATOM				, CRecordTextPFExceptionAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TXSISTYLE_ATOM				, CRecordTextSIExceptionAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_TEXTRULER_ATOM				, CRecordTextRulerAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_TEXTBOOKMARK_ATOM			, CRecordTextBookmarkAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TEXTBYTES_ATOM				, CRecordTextBytesAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordOutlineTextPropsHeaderExAtom)

	CREATE_BY_TYPE(RECORD_TYPE_GUIDE_ATOM					, CRecordGuideAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_FONTENTITY_ATOM				, CRecordFontEntityAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_FONT_EMBEDDEDDATA			, CRecordFontEmbedDataBlob)
	//CREATE_BY_TYPE(RECORD_TYPE_METAFILE					, CRecordMetafileBlob)
	CREATE_BY_TYPE(RECORD_TYPE_CSTRING						, CRecordCString)
	//CREATE_BY_TYPE(RECORD_TYPE_EXOLEOBJ_ATOM				, CRecordExOleObjAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXEMBEDATOM				, CRecordExOleEmbedAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_ENTITY_ATOM		, CRecordBookmarkEntityAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXLINK_ATOM				, CRecordExOleLinkAtom)
	
	//CREATE_BY_TYPE(RECORD_TYPE_SRKINSOKU_ATOM				, CRecordKinsokuAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXHYPERLINK_ATOM			, CRecordExHyperlinkAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordSlideNumberMCAtom)
	CREATE_BY_TYPE(RECORD_TYPE_HEADERSFOOTERS_ATOM			, CRecordHeadersFootersAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMouseTextInteractiveInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRecolorInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordAnimationInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordDateTimeMCAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordGenericDateMCAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EX_VIDEOCONTAINER			, CRecordExVideoContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_OBJREF_ATOM				, CRecordExObjRefAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EX_MEDIA_ATOM				, CRecordExMediaAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXCONTROL_ATOM				, CRecordExControlAtom)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_BSTORE_CONTAINER		, CRecordBlipStoreContainer)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_BSE					, CRecordBlipStoreEntry)

	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_DG_CONTAINER			, CRecordDrawingContainer)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_DGG					, CRecordDrawingGroup)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_DG					, CRecordDrawingRecord)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SPGR					, CRecordGroupShape)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SPGR_CONTAINER		, CRecordGroupShapeContainer)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SP_CONTAINER			, CRecordShapeContainer)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SP					, CRecordShape)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_CLIENTANCHOR			, CRecordClientAnchor)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_CHILDANCHOR			, CRecordChildAnchor)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_OPT					, CRecordShapeProperties)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SECONDARY_OPT			, CRecordShapeProperties)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_TETRIARY_OPT			, CRecordShapeProperties)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_REGROUPITEMS			, CRecordRegGroupItems)

	CREATE_BY_TYPE(RECORD_TYPE_EX_CDAUDIO					, CRecordExCDAudioContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_WAVAUDIOLINK				, CRecordWAVAudioLinkContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_WAVAUDIOEMBEDDED			, CRecordWAVAudioEmbeddedContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_MIDIAUDIO					, CRecordExMIDIAudioContainer)

	// интерактив
	CREATE_BY_TYPE(RECORD_TYPE_INTERACTIVEINFO_ATOM			, CRecordInteractiveInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TXINTERACTIVEINFO_ATOM		, CRecordTextInteractiveInfoAtom)

	// читаем для анимации
	CREATE_BY_TYPE ( RECORD_PROG_TAGS						,	SlideProgTagsContainer )

	CREATE_BY_TYPE(RECORD_TYPE_SOUNDCOLLECTION				, CRecordSoundCollectionContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EXOBJLIST					, CRecordExObjListContainer)

	CREATE_BY_TYPE(RECORD_TYPE_SOUND						, CRecordSoundContainer)
	CREATE_BY_TYPE(RECORD_TYPE_SOUNDDATA					, CRecordSoundDataBlob)

	default: 
	{
#ifdef _DEBUG
		char str[1024]={};
		sprintf(str, "Unknown record type: %xd\n" , oHeader.RecType);
		std::cout << str;
#endif
	}
		break;
	};

	if (NULL == pRecord)
	{
		if (oHeader.IsContainer())
		{
			pRecord = new CRecordsContainer();
		}
		else
		{
			pRecord = new CUnknownRecord();
		}
	}
	return pRecord;
}
