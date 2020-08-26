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
#include "../Records/Animations/_includer.h"
#include "../Enums/RecordType.h"

#include <iostream>
#include <iomanip>

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
    CREATE_BY_TYPE(RT_AnimationInfo, CRecordAnimationInfoContainer)
    CREATE_BY_TYPE(RT_AnimationInfoAtom, CRecordAnimationInfoAtom)
    CREATE_BY_TYPE(RT_HashCodeAtom, CRecordHashCode10Atom)
    CREATE_BY_TYPE(RT_BuildList, CRecordBuildListContainer)
    CREATE_BY_TYPE(RT_ParaBuild, CRecordParaBuildContainer)
    CREATE_BY_TYPE(RT_ChartBuild, CRecordChartBuildContainer)
    CREATE_BY_TYPE(RT_DiagramBuild, CRecordDiagramBuildContainer)
    CREATE_BY_TYPE(RT_ParaBuildAtom, CRecordParaBuildAtom)
    CREATE_BY_TYPE(RT_LevelInfoAtom, CRecordLevelInfoAtom)
    CREATE_BY_TYPE(RT_ChartBuildAtom, CRecordChartBuildAtom)
    CREATE_BY_TYPE(RT_DiagramBuildAtom, CRecordDiagramBuildAtom)
    CREATE_BY_TYPE(RT_TimeExtTimeNodeContainer, CRecordExtTimeNodeContainer)
    CREATE_BY_TYPE(RT_TimeSubEffectContainer, CRecordSubEffectContainer)
    CREATE_BY_TYPE(RT_TimeNode, CRecordTimeNodeAtom)
    CREATE_BY_TYPE(RT_TimePropertyList, CRecordTimePropertyList4TimeNodeContainer)
    CREATE_BY_TYPE(RT_TimeAnimateBehaviorContainer, CRecordTimeAnimateBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeAnimateBehavior, CRecordTimeAnimateBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeAnimationValueList, CRecordTimeAnimationValueListContainer)
    CREATE_BY_TYPE(RT_TimeAnimationValue, CRecordTimeAnimationValueAtom)
    CREATE_BY_TYPE(RT_TimeBehaviorContainer, CRecordTimeBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeVariantList, CRecordTimeStringListContainer)
    CREATE_BY_TYPE(RT_TimeClientVisualElement, CRecordClientVisualElementContainer)
    CREATE_BY_TYPE(RT_VisualPageAtom, CRecordVisualPageAtom)
//    CREATE_BY_TYPE(RT_VisualShapeAtom, CRecordVisualShapeOrSoundAtom)
    CREATE_BY_TYPE(RT_VisualShapeAtom, CRecordVisualSoundAtom)
    CREATE_BY_TYPE(RT_TimeColorBehaviorContainer, CRecordTimeColorBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeColorBehavior, CRecordTimeColorBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeEffectBehaviorContainer, CRecordTimeEffectBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeEffectBehavior, CRecordTimeEffectBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeMotionBehaviorContainer, CRecordTimeMotionBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeMotionBehavior, CRecordTimeMotionBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeRotationBehaviorContainer, CRecordTimeRotationBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeRotationBehavior, CRecordTimeRotationBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeScaleBehaviorContainer, CRecordTimeScaleBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeScaleBehavior, CRecordTimeScaleBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeSetBehaviorContainer, CRecordTimeSetBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeSetBehavior, CRecordTimeSetBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeCommandBehaviorContainer, CRecordTimeCommandBehaviorContainer)
    CREATE_BY_TYPE(RT_TimeCommandBehavior, CRecordTimeCommandBehaviorAtom)
    CREATE_BY_TYPE(RT_TimeIterateData, CRecordTimeIterateDataAtom)
    CREATE_BY_TYPE(RT_TimeSequenceData, CRecordTimeSequenceDataAtom)
    CREATE_BY_TYPE(RT_TimeConditionContainer, CRecordTimeConditionContainer)
    CREATE_BY_TYPE(RT_TimeCondition, CRecordTimeConditionAtom)
    CREATE_BY_TYPE(RT_TimeModifier, CRecordTimeModifierAtom)
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
//    CREATE_BY_TYPE()
    CREATE_BY_TYPE(RT_SlideListWithText			, CRecordSlideListWithText)
    CREATE_BY_TYPE(RT_DocumentAtom				, CRecordDocumentAtom)
    CREATE_BY_TYPE(RT_EndDocumentAtom			, CRecordEndDocument)
                //CREATE_BY_TYPE(RECORD_TYPE_SSLIDE_LAYOUT_ATOM			, CRecordSSlideLayoutAtom)
    CREATE_BY_TYPE(RT_SlideAtom					, CRecordSlideAtom)
    CREATE_BY_TYPE(RT_NotesAtom					, CRecordNotesAtom)
    CREATE_BY_TYPE(RT_SlidePersistAtom			, CRecordSlidePersistAtom)
    CREATE_BY_TYPE(RT_Environment				, CRecordDocumentTextInfo)
    CREATE_BY_TYPE(RT_FontCollection				, CRecordFontCollection)
    CREATE_BY_TYPE(RT_FontCollection10				, CRecordFontCollection)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMasterPersistAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordNotesPersistAtom)
    CREATE_BY_TYPE(RT_SlideShowSlideInfoAtom			, CSlideShowSlideInfoAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_GUIDE_ATOM					, CRecordGuideAtom)
    CREATE_BY_TYPE(RT_ViewInfoAtom			, CRecordZoomViewInfoAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordNoZoomViewInfoAtom)
//    CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordViewInfoAtom)
    CREATE_BY_TYPE(RT_SlideViewInfoAtom			, CRecordSlideViewInfoAtom)
    CREATE_BY_TYPE(RT_VbaInfoAtom					, CRecordVBAInfoAtom)
    CREATE_BY_TYPE(RT_SlideShowDocInfoAtom				, CRecordSSDocInfoAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_DOC_ROUTING_SLIP			, CRecordDocRoutingSlipAtom)
    CREATE_BY_TYPE(RT_ExternalObjectListAtom				, CRecordExObjListAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordGridSpacing10Atom)
    CREATE_BY_TYPE(RT_RoundTripTheme12Atom					, CRecordRoundTripThemeAtom)
    CREATE_BY_TYPE(RT_RoundTripColorMapping12Atom			, CRecordRoundTripColorMappingAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_NAMEDSHOW_SLIDES			, CRecordNamedShowSlidesAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_SHEET_PROPERTIES			, CRecordNormalViewSetInfoAtom)

    CREATE_BY_TYPE(RT_ColorSchemeAtom			, CRecordColorSchemeAtom)

    CREATE_BY_TYPE(RT_RoundTripOriginalMainMasterId12Atom			, CRecordTripOriginalMasterId12Atom)
    CREATE_BY_TYPE(RT_RoundTripCompositeMasterId12Atom			, CRecordTripCompositeMasterId12Atom)
    CREATE_BY_TYPE(RT_RoundTripContentMasterInfo12Atom	, CRecordRoundTripContentMasterInfo12Atom)			// ECMA-376 дубль
                //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPEID12			, CRecordRoundTripShapeId12Atom)
    CREATE_BY_TYPE(RT_RoundTripHFPlaceholder12Atom		, CRecordRoundTripHFPlaceholder12Atom)
                //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPCONTENTMASTERID	, CRecordRoundTripContentMasterId12Atom)
                //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPOARTTEXTSTYLES12	, CRecordRoundTripOArtTextStyles12Atom)
    CREATE_BY_TYPE(RT_RoundTripHeaderFooterDefaults12Atom	, CRecordRoundTripHeaderFooterDefaults12Atom)
    CREATE_BY_TYPE(RT_RoundTripNotesMasterTextStyles12Atom	, CRecordRoundTripNotesMasterTextStyles12Atom)
                //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPDOCFLAGS12						, CRecordRoundTripDocFlags12Atom)
                //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPECHECKSUMFORCUSTOMLAYOUTS12	, CRecordRoundTripShapeCheckSumForCustomLayouts12Atom)
    CREATE_BY_TYPE(RT_RoundTripCustomTableStyles12Atom			, CRecordRoundTripCustomTableStyles12Atom)

                //CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_SEED_ATOM			, CRecordBookmarkSeedAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordBlipEntityAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordExObjRefAtom)
    CREATE_BY_TYPE(RT_PlaceholderAtom			, CRecordPlaceHolderAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlagsAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlags10Atom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordRoundTripNewPlaceholderId12Atom)

    CREATE_BY_TYPE(RT_OutlineTextRefAtom			, CRecordOutlineTextRefAtom)
    CREATE_BY_TYPE(RT_TextHeaderAtom				, CRecordTextHeaderAtom)
    CREATE_BY_TYPE(RT_TextCharsAtom				, CRecordTextCharsAtom)
    CREATE_BY_TYPE(RT_StyleTextPropAtom			, CRecordStyleTextPropAtom)
    CREATE_BY_TYPE(RT_TextSpecialInfoAtom			, CRecordTextSpecInfoAtom)
    CREATE_BY_TYPE(RT_MasterTextPropAtom			, CRecordMasterTextPropAtom)
    CREATE_BY_TYPE(RT_TextMasterStyleAtom			, CRecordTextMasterStyleAtom)

    CREATE_BY_TYPE(RT_TextCharFormatExceptionAtom				, CRecordTextCFExceptionAtom)
    CREATE_BY_TYPE(RT_TextParagraphFormatExceptionAtom				, CRecordTextPFExceptionAtom)
    CREATE_BY_TYPE(RT_TextSpecialInfoDefaultAtom			, CRecordTextSIExceptionAtom)

    CREATE_BY_TYPE(RT_TextRulerAtom				, CRecordTextRulerAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_TEXTBOOKMARK_ATOM			, CRecordTextBookmarkAtom)
    CREATE_BY_TYPE(RT_TextBytesAtom				, CRecordTextBytesAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordOutlineTextPropsHeaderExAtom)

    CREATE_BY_TYPE(RT_GuideAtom					, CRecordGuideAtom)

    CREATE_BY_TYPE(RT_FontEntityAtom				, CRecordFontEntityAtom)
    CREATE_BY_TYPE(RT_FontEmbedDataBlob			, CRecordFontEmbedDataBlob)
                //CREATE_BY_TYPE(RECORD_TYPE_METAFILE					, CRecordMetafileBlob)
    CREATE_BY_TYPE(RT_CString						, CRecordCString)
    CREATE_BY_TYPE(RT_SoundCollectionAtom				, CRecordSoundCollAtom)
    CREATE_BY_TYPE(RT_ExternalOleObjectAtom				, CRecordExOleObjAtom)
    CREATE_BY_TYPE(RT_ExternalOleEmbedAtom					, CRecordExOleEmbedAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_ENTITY_ATOM		, CRecordBookmarkEntityAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_EXLINK_ATOM				, CRecordExOleLinkAtom)

    CREATE_BY_TYPE(RT_KinsokuAtom				, CRecordKinsokuAtom)
    CREATE_BY_TYPE(RT_ExternalHyperlink					, CRecordExHyperlinkContainer)
    CREATE_BY_TYPE(RT_ExternalHyperlinkAtom				, CRecordExHyperlinkAtom)

    CREATE_BY_TYPE(RT_HeadersFooters				, CRecordHeadersFootersContainer)
    CREATE_BY_TYPE(RT_HeadersFootersAtom			, CRecordHeadersFootersAtom)

    CREATE_BY_TYPE(RT_SlideNumberMetaCharAtom		, CRecordSlideNumberMetaAtom)
    CREATE_BY_TYPE(RT_DateTimeMetaCharAtom			, CRecordDateTimeMetaAtom)
    CREATE_BY_TYPE(RT_HeaderMetaCharAtom				, CRecordHeaderMetaAtom)
    CREATE_BY_TYPE(RT_FooterMetaCharAtom				, CRecordFooterMetaAtom)
    CREATE_BY_TYPE(RT_GenericDateMetaCharAtom		, CRecordGenericDateMetaAtom)
    CREATE_BY_TYPE(RT_RtfDateTimeMetaCharAtom		, CRecordRTFDateTimeMetaAtom)

                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMouseTextInteractiveInfoAtom)
                //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRecolorInfoAtom)

    CREATE_BY_TYPE(RT_ExternalVideo			, CRecordExVideoContainer)
    CREATE_BY_TYPE(RT_ExternalObjectRefAtom				, CRecordExObjRefAtom)
    CREATE_BY_TYPE(RT_ExternalMediaAtom				, CRecordExMediaAtom)
    CREATE_BY_TYPE(RT_ExternalOleControlAtom				, CRecordExControlAtom)
    CREATE_BY_TYPE(RT_BlipCollection9		, CRecordBlipStoreContainer)
    CREATE_BY_TYPE(RT_BlipEntity9Atom					, CRecordBlipStoreEntry)

    CREATE_BY_TYPE(RT_Drawing			, CRecordDrawingContainer)
    CREATE_BY_TYPE(RT_DrawingGroup					, CRecordDrawingGroup)
//    CREATE_BY_TYPE(RECORD_TYPE_ESCHER_DG					, CRecordDrawingRecord)
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
