/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "Records.h"

#include "../Records/RecordsIncluder.h"

#include <iostream>

std::wstring CUnknownRecord::ReadStringW(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return (L"");

    unsigned char* pData = new unsigned char[2 * (lLen + 1)];
    memset (pData, 0, 2 * (lLen + 1));

    pStream->read(pData, 2 * lLen);

    if (sizeof(wchar_t) == 4)
    {
        ConversionResult eUnicodeConversionResult;
        UTF32 *pStrUtf32 = new UTF32 [lLen + 1];
        pStrUtf32[lLen] = 0 ;

        const UTF16 *pStrUtf16_Conv = (const UTF16 *) pData;
        UTF32		*pStrUtf32_Conv =                 pStrUtf32;

        eUnicodeConversionResult = ConvertUTF16toUTF32 ( &pStrUtf16_Conv
                                                         , &pStrUtf16_Conv[lLen]
                                                         , &pStrUtf32_Conv
                                                         , &pStrUtf32 [lLen]
                                                         , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
            delete [] pStrUtf32;
            return (L"");
        }
        std::wstring res((wchar_t*)pStrUtf32, lLen);
        if (pStrUtf32) delete [] pStrUtf32;
        return res;
    }
    else
    {
        std::wstring str((wchar_t*)pData);
        delete[] pData;
        return str;
    }

}
std::string CUnknownRecord::ReadStringA(const CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return ("");

    char* pData = new char[lLen + 1];

	pStream->read((unsigned char*)pData, lLen);

    pData[lLen] = 0;

    std::string str(pData, lLen);

    delete[] pData;
    return str;
}

void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, const CFStreamPtr &pStream)
{
	m_oHeader = oHeader;
	m_arRecords.clear();

	LONG lPosition = pStream->getStreamPointer();
	
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
		pStream->seekFromBegin(lPosition + m_oHeader.RecLen);
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
	CREATE_BY_TYPE(RECORD_TYPE_ENDDOCUMENT					, CRecordEndDocument)
	//CREATE_BY_TYPE(RECORD_TYPE_SSLIDE_LAYOUT_ATOM			, CRecordSSlideLayoutAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_ATOM					, CRecordSlideAtom)
	CREATE_BY_TYPE(RECORD_TYPE_NOTES_ATOM					, CRecordNotesAtom) 
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_PERSIST_ATOM			, CRecordSlidePersistAtom)
	CREATE_BY_TYPE(RECORD_TYPE_ENVIRONMENT					, CRecordDocumentTextInfo)
	CREATE_BY_TYPE(RECORD_TYPE_FONTCOLLECTION				, CRecordFontCollection)
	CREATE_BY_TYPE(RECORD_TYPE_FONTCOLLECTION10				, CRecordFontCollection)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMasterPersistAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordNotesPersistAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SSSLIDE_INFO_ATOM			, CSlideShowSlideInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_GUIDE_ATOM					, CRecordGuideAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordZoomViewInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordNoZoomViewInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordViewInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SLIDE_VIEWINFO_ATOM			, CRecordSlideViewInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_VBAINFO_ATOM					, CRecordVBAInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_SSDOCINFO_ATOM				, CRecordSSDocInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_DOC_ROUTING_SLIP			, CRecordDocRoutingSlipAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EXOBJLIST_ATOM				, CRecordExObjListAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordGridSpacing10Atom)
	CREATE_BY_TYPE(RT_RoundTripTheme12Atom					, CRecordRoundTripThemeAtom)
	CREATE_BY_TYPE(RT_RoundTripColorMapping12Atom			, CRecordRoundTripColorMappingAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_NAMEDSHOW_SLIDES			, CRecordNamedShowSlidesAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_SHEET_PROPERTIES			, CRecordNormalViewSetInfoAtom)

	CREATE_BY_TYPE(RECORD_TYPE_COLOR_SCHEME_ATOM			, CRecordColorSchemeAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_ORIGINALMAINMASTERID			, CRecordTripOriginalMasterId12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_COMPOSITEMASTERID			, CRecordTripCompositeMasterId12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCONTENTMASTERINFO12	, CRecordRoundTripContentMasterInfo12Atom)			// ECMA-376 дубль
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPEID12			, CRecordRoundTripShapeId12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPHFPLACEHOLDER12		, CRecordRoundTripHFPlaceholder12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCONTENTMASTERID	, CRecordRoundTripContentMasterId12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPOARTTEXTSTYLES12	, CRecordRoundTripOArtTextStyles12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPHEADERFOOTERDEFAULTS12ATOM	, CRecordRoundTripHeaderFooterDefaults12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPNOTESMASTERTEXTSTYLES12		, CRecordRoundTripNotesMasterTextStyles12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPDOCFLAGS12						, CRecordRoundTripDocFlags12Atom)
	//CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPECHECKSUMFORCUSTOMLAYOUTS12	, CRecordRoundTripShapeCheckSumForCustomLayouts12Atom)
	CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCUSTOMTABLESTYLES12			, CRecordRoundTripCustomTableStyles12Atom)
	
	//CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_SEED_ATOM			, CRecordBookmarkSeedAtom)
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
	CREATE_BY_TYPE(RECORD_TYPE_FONT_EMBEDDEDDATA			, CRecordFontEmbedDataBlob)
	//CREATE_BY_TYPE(RECORD_TYPE_METAFILE					, CRecordMetafileBlob)
	CREATE_BY_TYPE(RECORD_TYPE_CSTRING						, CRecordCString)
	CREATE_BY_TYPE(RECORD_TYPE_SOUNDCOLL_ATOM				, CRecordSoundCollAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EXOLEOBJ_ATOM				, CRecordExOleObjAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EXEMBEDATOM					, CRecordExOleEmbedAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_ENTITY_ATOM		, CRecordBookmarkEntityAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_EXLINK_ATOM				, CRecordExOleLinkAtom)
	
	CREATE_BY_TYPE(RECORD_TYPE_SRKINSOKU_ATOM				, CRecordKinsokuAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EXHYPERLINK					, CRecordExHyperlinkContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EXHYPERLINK_ATOM				, CRecordExHyperlinkAtom)

	CREATE_BY_TYPE(RECORD_TYPE_HEADERSFOOTERS				, CRecordHeadersFootersContainer)
	CREATE_BY_TYPE(RECORD_TYPE_HEADERSFOOTERS_ATOM			, CRecordHeadersFootersAtom)

	CREATE_BY_TYPE(RECORD_TYPE_SLIDENUMBER_META_ATOM		, CRecordSlideNumberMetaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_DATETIME_META_ATOM			, CRecordDateTimeMetaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_HEADER_META_ATOM				, CRecordHeaderMetaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_FOOTER_META_ATOM				, CRecordFooterMetaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_GENERICDATE_META_ATOM		, CRecordGenericDateMetaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_RTFDATETIME_META_ATOM		, CRecordRTFDateTimeMetaAtom)

	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMouseTextInteractiveInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRecolorInfoAtom)
	//CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordAnimationInfoAtom)

	CREATE_BY_TYPE(RECORD_TYPE_EX_VIDEOCONTAINER			, CRecordExVideoContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_OBJREF_ATOM				, CRecordExObjRefAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EX_MEDIA_ATOM				, CRecordExMediaAtom)
	CREATE_BY_TYPE(RECORD_TYPE_EXCONTROL_ATOM				, CRecordExControlAtom)
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
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_SPLITMENUCOLORS		, CRecordSplitMenuColorRecord)
	CREATE_BY_TYPE(RECORD_TYPE_ESCHER_COLORMRU				, CRecordMostRecentlyUserColors)
	
	CREATE_BY_TYPE(RECORD_TYPE_EX_CDAUDIO					, CRecordExCDAudioContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_WAVAUDIOLINK				, CRecordWAVAudioLinkContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_WAVAUDIOEMBEDDED			, CRecordWAVAudioEmbeddedContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EX_MIDIAUDIO					, CRecordExMIDIAudioContainer)

	// интерактив
	CREATE_BY_TYPE(RECORD_TYPE_INTERACTIVEINFO_ATOM			, CRecordInteractiveInfoAtom)
	CREATE_BY_TYPE(RECORD_TYPE_TXINTERACTIVEINFO_ATOM		, CRecordTextInteractiveInfoAtom)

	// читаем для анимации
	CREATE_BY_TYPE ( RECORD_PROG_TAGS						, SlideProgTagsContainer )

	CREATE_BY_TYPE(RECORD_TYPE_SOUNDCOLLECTION				, CRecordSoundCollectionContainer)
	CREATE_BY_TYPE(RECORD_TYPE_EXOBJLIST					, CRecordExObjListContainer)

	CREATE_BY_TYPE(RECORD_TYPE_SOUND						, CRecordSoundContainer)
	CREATE_BY_TYPE(RECORD_TYPE_SOUNDDATA					, CRecordSoundDataBlob)

	default: 
		{
		}break;		
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
#ifdef _DEBUG
			char str[1024]={};
			sprintf(str, "Unknown record type: %x\t" , oHeader.RecType);
			std::cout << str << GetRecordName((_UINT32)oHeader.RecType) << "\n"; 
#endif
		}
	}
	return pRecord;
}
