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

namespace PPT
{

//-------------------------------------------------------------------------------
#define CREATE_BY_TYPE(RECORD_TYPE, CLASS_RECORD_NAME)							\
    case RECORD_TYPE: { pRecord = new CLASS_RECORD_NAME(); break; }				\
    //-------------------------------------------------------------------------------

CUnknownRecord::CUnknownRecord()
{
}

CUnknownRecord::~CUnknownRecord()
{
}

void CUnknownRecord::ReadFromStream(SRecordHeader &oHeader, const XLS::CFStreamPtr &pStream)
{
    m_oHeader = oHeader;

    pStream->seekFromCurForward(m_oHeader.RecLen);
}

void CUnknownRecord::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    StreamUtils::StreamSkip((long)m_oHeader.RecLen, pStream);
}

std::wstring CUnknownRecord::ReadStringW(const XLS::CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return (L"");

    unsigned char* pData = new unsigned char[2 * (lLen + 1)];
    memset(pData, 0, 2 * (lLen + 1));

    pStream->read(pData, 2 * lLen);

    if (sizeof(wchar_t) == 4)
    {
        ConversionResult eUnicodeConversionResult;
        UTF32 *pStrUtf32 = new UTF32[lLen + 1];
        pStrUtf32[lLen] = 0;

        const UTF16 *pStrUtf16_Conv = (const UTF16 *)pData;
        UTF32		*pStrUtf32_Conv = pStrUtf32;

        eUnicodeConversionResult = ConvertUTF16toUTF32(&pStrUtf16_Conv
                                                       , &pStrUtf16_Conv[lLen]
                                                       , &pStrUtf32_Conv
                                                       , &pStrUtf32[lLen]
                                                       , strictConversion);

        if (conversionOK != eUnicodeConversionResult)
        {
            delete[] pStrUtf32;
            return (L"");
        }
        std::wstring res((wchar_t*)pStrUtf32, lLen);
        if (pStrUtf32) delete[] pStrUtf32;
        return res;
    }
    else
    {
        std::wstring str((wchar_t*)pData);
        delete[] pData;
        return str;
    }

}
std::string CUnknownRecord::ReadStringA(const XLS::CFStreamPtr &pStream, int lLen)
{
    if (!pStream) return ("");

    char* pData = new char[lLen + 1];

    pStream->read((unsigned char*)pData, lLen);

    pData[lLen] = 0;

    std::string str(pData, lLen);

    delete[] pData;
    return str;
}

CRecordsContainer::CRecordsContainer() : m_arRecords()
{
}

CRecordsContainer::~CRecordsContainer()
{
    Clear();
}

void CRecordsContainer::Clear()
{
    size_t nCount = m_arRecords.size();
    while (0 != nCount)
    {
        if (NULL != m_arRecords[nCount - 1])
        {
            delete m_arRecords[nCount - 1];
            m_arRecords[nCount - 1] = NULL;
        }
        m_arRecords.pop_back();
        --nCount;
    }
}

void CRecordsContainer::ReadFromStream(SRecordHeader & oHeader, const XLS::CFStreamPtr &pStream)
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
        if (oRec.ReadFromStream(pStream) == FALSE)
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
        if (oRec.ReadFromStream(pStream) == FALSE)
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


#define RECORDNAME_BY_TYPE(RECORD_TYPE, RECORD_NAME)						\
    case RECORD_TYPE: { name = RECORD_NAME; break;}			\

std::string GetRecordName(PPT::RecordType dwType)
{
    std::string name;
    switch (dwType) {
    RECORDNAME_BY_TYPE(RT_Document, "DocumentContainer")
            RECORDNAME_BY_TYPE(RT_DocumentAtom, "DocumentAtom")
            RECORDNAME_BY_TYPE(RT_EndDocumentAtom, "EndDocumentAtom")
            RECORDNAME_BY_TYPE(RT_Slide, "SlideContainer")
            RECORDNAME_BY_TYPE(RT_SlideAtom, "SlideAtom")
            RECORDNAME_BY_TYPE(RT_Notes, "Notes")
            RECORDNAME_BY_TYPE(RT_NotesAtom, "NotesAtom")
            RECORDNAME_BY_TYPE(RT_Environment, "Environment")
            RECORDNAME_BY_TYPE(RT_SlidePersistAtom, "SlidePersistAtom")
            RECORDNAME_BY_TYPE(RT_MainMaster, "MainMaster")
            RECORDNAME_BY_TYPE(RT_SlideShowSlideInfoAtom, "SlideShowSlideInfoAtom")
            RECORDNAME_BY_TYPE(RT_SlideViewInfo, "SlideViewInfo")
            RECORDNAME_BY_TYPE(RT_GuideAtom, "GuideAtom")
            RECORDNAME_BY_TYPE(RT_ViewInfoAtom, "ViewInfoAtom")
            RECORDNAME_BY_TYPE(RT_SlideViewInfoAtom, "SlideViewInfoAtom")
            RECORDNAME_BY_TYPE(RT_VbaInfo, "VbaInfo")
            RECORDNAME_BY_TYPE(RT_VbaInfoAtom, "VbaInfoAtom")
            RECORDNAME_BY_TYPE(RT_SlideShowDocInfoAtom, "SlideShowDocInfoAtom")
            RECORDNAME_BY_TYPE(RT_Summary, "Summary")
            RECORDNAME_BY_TYPE(RT_DocRoutingSlipAtom, "DocRoutingSlipAtom")
            RECORDNAME_BY_TYPE(RT_OutlineViewInfo, "OutlineViewInfo")
            RECORDNAME_BY_TYPE(RT_SorterViewInfo, "SorterViewInfo")
            RECORDNAME_BY_TYPE(RT_ExternalObjectList, "ExternalObjectList")
            RECORDNAME_BY_TYPE(RT_ExternalObjectListAtom, "ExternalObjectListAtom")
            RECORDNAME_BY_TYPE(RT_DrawingGroup, "DrawingGroup")
            RECORDNAME_BY_TYPE(RT_Drawing, "Drawing")
            RECORDNAME_BY_TYPE(RT_GridSpacing10Atom, "GridSpacing10Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripTheme12Atom, "RoundTripTheme12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripColorMapping12Atom, "RoundTripColorMapping12Atom")
            RECORDNAME_BY_TYPE(RT_NamedShows, "NamedShows")
            RECORDNAME_BY_TYPE(RT_NamedShow, "NamedShow")
            RECORDNAME_BY_TYPE(RT_NamedShowSlidesAtom, "NamedShowSlidesAtom")
            RECORDNAME_BY_TYPE(RT_NotesTextViewInfo9, "NotesTextViewInfo9")
            RECORDNAME_BY_TYPE(RT_NormalViewSetInfo9, "NormalViewSetInfo9")
            RECORDNAME_BY_TYPE(RT_NormalViewSetInfo9Atom, "NormalViewSetInfo9Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripOriginalMainMasterId12Atom, "RoundTripOriginalMainMasterId12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripCompositeMasterId12Atom, "RoundTripCompositeMasterId12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripContentMasterInfo12Atom, "RoundTripContentMasterInfo12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripShapeId12Atom, "RoundTripShapeId12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripHFPlaceholder12Atom, "RoundTripHFPlaceholder12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripContentMasterId12Atom, "RoundTripContentMasterId12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripOArtTextStyles12Atom, "RoundTripOArtTextStyles12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripHeaderFooterDefaults12Atom, "RoundTripHeaderFooterDefaults12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripDocFlags12Atom, "RoundTripDocFlags12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripShapeCheckSumForCL12Atom, "RoundTripShapeCheckSumForCL12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripNotesMasterTextStyles12Atom, "RoundTripNotesMasterTextStyles12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripCustomTableStyles12Atom, "RoundTripCustomTableStyles12Atom")
            RECORDNAME_BY_TYPE(RT_List, "List")
            RECORDNAME_BY_TYPE(RT_FontCollection, "FontCollection")
            RECORDNAME_BY_TYPE(RT_FontCollection10, "FontCollection10")
            RECORDNAME_BY_TYPE(RT_BookmarkCollection, "BookmarkCollection")
            RECORDNAME_BY_TYPE(RT_SoundCollection, "SoundCollection")
            RECORDNAME_BY_TYPE(RT_SoundCollectionAtom, "SoundCollectionAtom")
            RECORDNAME_BY_TYPE(RT_Sound, "Sound")
            RECORDNAME_BY_TYPE(RT_SoundDataBlob, "SoundDataBlob")
            RECORDNAME_BY_TYPE(RT_BookmarkSeedAtom, "BookmarkSeedAtom")
            RECORDNAME_BY_TYPE(RT_ColorSchemeAtom, "ColorSchemeAtom")
            RECORDNAME_BY_TYPE(RT_BlipCollection9, "BlipCollection9")
            RECORDNAME_BY_TYPE(RT_BlipEntity9Atom, "BlipEntity9Atom")
            RECORDNAME_BY_TYPE(RT_ExternalObjectRefAtom, "ExternalObjectRefAtom")
            RECORDNAME_BY_TYPE(RT_PlaceholderAtom, "PlaceholderAtom")
            RECORDNAME_BY_TYPE(RT_ShapeAtom, "ShapeAtom")
            RECORDNAME_BY_TYPE(RT_ShapeFlags10Atom, "ShapeFlags10Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripNewPlaceholderId12Atom, "RoundTripNewPlaceholderId12Atom")
            RECORDNAME_BY_TYPE(RT_OutlineTextRefAtom, "OutlineTextRefAtom")
            RECORDNAME_BY_TYPE(RT_TextHeaderAtom, "TextHeaderAtom")
            RECORDNAME_BY_TYPE(RT_TextCharsAtom, "TextCharsAtom")
            RECORDNAME_BY_TYPE(RT_StyleTextPropAtom, "StyleTextPropAtom")
            RECORDNAME_BY_TYPE(RT_MasterTextPropAtom, "MasterTextPropAtom")
            RECORDNAME_BY_TYPE(RT_TextMasterStyleAtom, "TextMasterStyleAtom")
            RECORDNAME_BY_TYPE(RT_TextCharFormatExceptionAtom, "TextCharFormatExceptionAtom")
            RECORDNAME_BY_TYPE(RT_TextParagraphFormatExceptionAtom, "TextParagraphFormatExceptionAtom")
            RECORDNAME_BY_TYPE(RT_TextRulerAtom, "TextRulerAtom")
            RECORDNAME_BY_TYPE(RT_TextBookmarkAtom, "TextBookmarkAtom")
            RECORDNAME_BY_TYPE(RT_TextBytesAtom, "TextBytesAtom")
            RECORDNAME_BY_TYPE(RT_TextSpecialInfoDefaultAtom, "TextSpecialInfoDefaultAtom")
            RECORDNAME_BY_TYPE(RT_TextSpecialInfoAtom, "TextSpecialInfoAtom")
            RECORDNAME_BY_TYPE(RT_DefaultRulerAtom, "DefaultRulerAtom")
            RECORDNAME_BY_TYPE(RT_StyleTextProp9Atom, "StyleTextProp9Atom")
            RECORDNAME_BY_TYPE(RT_TextMasterStyle9Atom, "TextMasterStyle9Atom")
            RECORDNAME_BY_TYPE(RT_OutlineTextProps9, "OutlineTextProps9")
            RECORDNAME_BY_TYPE(RT_OutlineTextPropsHeader9Atom, "OutlineTextPropsHeader9Atom")
            RECORDNAME_BY_TYPE(RT_TextDefaults9Atom, "TextDefaults9Atom")
            RECORDNAME_BY_TYPE(RT_StyleTextProp10Atom, "StyleTextProp10Atom")
            RECORDNAME_BY_TYPE(RT_TextMasterStyle10Atom, "TextMasterStyle10Atom")
            RECORDNAME_BY_TYPE(RT_OutlineTextProps10, "OutlineTextProps10")
            RECORDNAME_BY_TYPE(RT_TextDefaults10Atom, "TextDefaults10Atom")
            RECORDNAME_BY_TYPE(RT_OutlineTextProps11, "OutlineTextProps11")
            RECORDNAME_BY_TYPE(RT_StyleTextProp11Atom, "StyleTextProp11Atom")
            RECORDNAME_BY_TYPE(RT_FontEntityAtom, "FontEntityAtom")
            RECORDNAME_BY_TYPE(RT_FontEmbedDataBlob, "FontEmbedDataBlob")
            RECORDNAME_BY_TYPE(RT_CString, "CString")
            RECORDNAME_BY_TYPE(RT_MetaFile, "MetaFile")
            RECORDNAME_BY_TYPE(RT_ExternalOleObjectAtom, "ExternalOleObjectAtom")
            RECORDNAME_BY_TYPE(RT_Kinsoku, "Kinsoku")
            RECORDNAME_BY_TYPE(RT_Handout, "Handout")
            RECORDNAME_BY_TYPE(RT_ExternalOleEmbed, "ExternalOleEmbed")
            RECORDNAME_BY_TYPE(RT_ExternalOleEmbedAtom, "ExternalOleEmbedAtom")
            RECORDNAME_BY_TYPE(RT_ExternalOleLink, "ExternalOleLink")
            RECORDNAME_BY_TYPE(RT_BookmarkEntityAtom, "BookmarkEntityAtom")
            RECORDNAME_BY_TYPE(RT_ExternalOleLinkAtom, "ExternalOleLinkAtom")
            RECORDNAME_BY_TYPE(RT_KinsokuAtom, "KinsokuAtom")
            RECORDNAME_BY_TYPE(RT_ExternalHyperlinkAtom, "ExternalHyperlinkAtom")
            RECORDNAME_BY_TYPE(RT_ExternalHyperlink, "ExternalHyperlink")
            RECORDNAME_BY_TYPE(RT_SlideNumberMetaCharAtom, "SlideNumberMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_HeadersFooters, "HeadersFooters")
            RECORDNAME_BY_TYPE(RT_HeadersFootersAtom, "HeadersFootersAtom")
            RECORDNAME_BY_TYPE(RT_TextInteractiveInfoAtom, "TextInteractiveInfoAtom")
            RECORDNAME_BY_TYPE(RT_ExternalHyperlink9, "ExternalHyperlink9")
            RECORDNAME_BY_TYPE(RT_RecolorInfoAtom, "RecolorInfoAtom")
            RECORDNAME_BY_TYPE(RT_ExternalOleControl, "ExternalOleControl")
            RECORDNAME_BY_TYPE(RT_SlideListWithText, "SlideListWithText")
            RECORDNAME_BY_TYPE(RT_AnimationInfoAtom, "AnimationInfoAtom")
            RECORDNAME_BY_TYPE(RT_InteractiveInfo, "InteractiveInfo")
            RECORDNAME_BY_TYPE(RT_InteractiveInfoAtom, "InteractiveInfoAtom")
            RECORDNAME_BY_TYPE(RT_UserEditAtom, "UserEditAtom")
            RECORDNAME_BY_TYPE(RT_CurrentUserAtom, "CurrentUserAtom")
            RECORDNAME_BY_TYPE(RT_DateTimeMetaCharAtom, "DateTimeMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_GenericDateMetaCharAtom, "GenericDateMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_HeaderMetaCharAtom, "HeaderMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_FooterMetaCharAtom, "FooterMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_ExternalOleControlAtom, "ExternalOleControlAtom")
            RECORDNAME_BY_TYPE(RT_ExternalMediaAtom, "ExternalMediaAtom")
            RECORDNAME_BY_TYPE(RT_ExternalVideo, "ExternalVideo")
            RECORDNAME_BY_TYPE(RT_ExternalAviMovie, "ExternalAviMovie")
            RECORDNAME_BY_TYPE(RT_ExternalMciMovie, "ExternalMciMovie")
            RECORDNAME_BY_TYPE(RT_ExternalMidiAudio, "ExternalMidiAudio")
            RECORDNAME_BY_TYPE(RT_ExternalCdAudio, "ExternalCdAudio")
            RECORDNAME_BY_TYPE(RT_ExternalWavAudioEmbedded, "ExternalWavAudioEmbedded")
            RECORDNAME_BY_TYPE(RT_ExternalWavAudioLink, "ExternalWavAudioLink")
            RECORDNAME_BY_TYPE(RT_ExternalOleObjectStg, "ExternalOleObjectStg")
            RECORDNAME_BY_TYPE(RT_ExternalCdAudioAtom, "ExternalCdAudioAtom")
            RECORDNAME_BY_TYPE(RT_ExternalWavAudioEmbeddedAtom, "ExternalWavAudioEmbeddedAtom")
            RECORDNAME_BY_TYPE(RT_AnimationInfo, "AnimationInfo")
            RECORDNAME_BY_TYPE(RT_RtfDateTimeMetaCharAtom, "RtfDateTimeMetaCharAtom")
            RECORDNAME_BY_TYPE(RT_ExternalHyperlinkFlagsAtom, "ExternalHyperlinkFlagsAtom")
            RECORDNAME_BY_TYPE(RT_ProgTags, "ProgTags")
            RECORDNAME_BY_TYPE(RT_ProgStringTag, "ProgStringTag")
            RECORDNAME_BY_TYPE(RT_ProgBinaryTag, "ProgBinaryTag")
            RECORDNAME_BY_TYPE(RT_BinaryTagDataBlob, "BinaryTagDataBlob")
            RECORDNAME_BY_TYPE(RT_PrintOptionsAtom, "PrintOptionsAtom")
            RECORDNAME_BY_TYPE(RT_PersistDirectoryAtom, "PersistDirectoryAtom")
            RECORDNAME_BY_TYPE(RT_PresentationAdvisorFlags9Atom, "PresentationAdvisorFlags9Atom")
            RECORDNAME_BY_TYPE(RT_HtmlDocInfo9Atom, "HtmlDocInfo9Atom")
            RECORDNAME_BY_TYPE(RT_HtmlPublishInfoAtom, "HtmlPublishInfoAtom")
            RECORDNAME_BY_TYPE(RT_HtmlPublishInfo9, "HtmlPublishInfo9")
            RECORDNAME_BY_TYPE(RT_BroadcastDocInfo9, "BroadcastDocInfo9")
            RECORDNAME_BY_TYPE(RT_BroadcastDocInfo9Atom, "BroadcastDocInfo9Atom")
            RECORDNAME_BY_TYPE(RT_EnvelopeFlags9Atom, "EnvelopeFlags9Atom")
            RECORDNAME_BY_TYPE(RT_EnvelopeData9Atom, "EnvelopeData9Atom")
            RECORDNAME_BY_TYPE(RT_VisualShapeAtom, "VisualShapeAtom")
            RECORDNAME_BY_TYPE(RT_HashCodeAtom, "HashCodeAtom")
            RECORDNAME_BY_TYPE(RT_VisualPageAtom, "VisualPageAtom")
            RECORDNAME_BY_TYPE(RT_BuildList, "BuildList")
            RECORDNAME_BY_TYPE(RT_BuildAtom, "BuildAtom")
            RECORDNAME_BY_TYPE(RT_ChartBuild, "ChartBuild")
            RECORDNAME_BY_TYPE(RT_ChartBuildAtom, "ChartBuildAtom")
            RECORDNAME_BY_TYPE(RT_DiagramBuild, "DiagramBuild")
            RECORDNAME_BY_TYPE(RT_DiagramBuildAtom, "DiagramBuildAtom")
            RECORDNAME_BY_TYPE(RT_ParaBuild, "ParaBuild")
            RECORDNAME_BY_TYPE(RT_ParaBuildAtom, "ParaBuildAtom")
            RECORDNAME_BY_TYPE(RT_LevelInfoAtom, "LevelInfoAtom")
            RECORDNAME_BY_TYPE(RT_RoundTripAnimationAtom12Atom, "RoundTripAnimationAtom12Atom")
            RECORDNAME_BY_TYPE(RT_RoundTripAnimationHashAtom12Atom, "RoundTripAnimationHashAtom12Atom")
            RECORDNAME_BY_TYPE(RT_Comment10, "Comment10")
            RECORDNAME_BY_TYPE(RT_Comment10Atom, "Comment10Atom")
            RECORDNAME_BY_TYPE(RT_CommentIndex10, "CommentIndex10")
            RECORDNAME_BY_TYPE(RT_CommentIndex10Atom, "CommentIndex10Atom")
            RECORDNAME_BY_TYPE(RT_LinkedShape10Atom, "LinkedShape10Atom")
            RECORDNAME_BY_TYPE(RT_LinkedSlide10Atom, "LinkedSlide10Atom")
            RECORDNAME_BY_TYPE(RT_SlideFlags10Atom, "SlideFlags10Atom")
            RECORDNAME_BY_TYPE(RT_SlideTime10Atom, "SlideTime10Atom")
            RECORDNAME_BY_TYPE(RT_DiffTree10, "DiffTree10")
            RECORDNAME_BY_TYPE(RT_Diff10, "Diff10")
            RECORDNAME_BY_TYPE(RT_Diff10Atom, "Diff10Atom")
            RECORDNAME_BY_TYPE(RT_SlideListTableSize10Atom, "SlideListTableSize10Atom")
            RECORDNAME_BY_TYPE(RT_SlideListEntry10Atom, "SlideListEntry10Atom")
            RECORDNAME_BY_TYPE(RT_SlideListTable10, "SlideListTable10")
            RECORDNAME_BY_TYPE(RT_CryptSession10Container, "CryptSession10Container")
            RECORDNAME_BY_TYPE(RT_FontEmbedFlags10Atom, "FontEmbedFlags10Atom")
            RECORDNAME_BY_TYPE(RT_FilterPrivacyFlags10Atom, "FilterPrivacyFlags10Atom")
            RECORDNAME_BY_TYPE(RT_DocToolbarStates10Atom, "DocToolbarStates10Atom")
            RECORDNAME_BY_TYPE(RT_PhotoAlbumInfo10Atom, "PhotoAlbumInfo10Atom")
            RECORDNAME_BY_TYPE(RT_SmartTagStore11Container, "SmartTagStore11Container")
            RECORDNAME_BY_TYPE(RT_RoundTripSlideSyncInfo12, "RoundTripSlideSyncInfo12")
            RECORDNAME_BY_TYPE(RT_RoundTripSlideSyncInfoAtom12, "RoundTripSlideSyncInfoAtom12")
            RECORDNAME_BY_TYPE(RT_TimeConditionContainer, "TimeConditionContainer")
            RECORDNAME_BY_TYPE(RT_TimeNode, "TimeNode")
            RECORDNAME_BY_TYPE(RT_TimeCondition, "TimeCondition")
            RECORDNAME_BY_TYPE(RT_TimeModifier, "TimeModifier")
            RECORDNAME_BY_TYPE(RT_TimeBehaviorContainer, "TimeBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeAnimateBehaviorContainer, "TimeAnimateBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeColorBehaviorContainer, "TimeColorBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeEffectBehaviorContainer, "TimeEffectBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeMotionBehaviorContainer, "TimeMotionBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeRotationBehaviorContainer, "TimeRotationBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeScaleBehaviorContainer, "TimeScaleBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeSetBehaviorContainer, "TimeSetBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeCommandBehaviorContainer, "TimeCommandBehaviorContainer")
            RECORDNAME_BY_TYPE(RT_TimeBehavior, "TimeBehavior")
            RECORDNAME_BY_TYPE(RT_TimeAnimateBehavior, "TimeAnimateBehavior")
            RECORDNAME_BY_TYPE(RT_TimeColorBehavior, "TimeColorBehavior")
            RECORDNAME_BY_TYPE(RT_TimeEffectBehavior, "TimeEffectBehavior")
            RECORDNAME_BY_TYPE(RT_TimeMotionBehavior, "TimeMotionBehavior")
            RECORDNAME_BY_TYPE(RT_TimeRotationBehavior, "TimeRotationBehavior")
            RECORDNAME_BY_TYPE(RT_TimeScaleBehavior, "TimeScaleBehavior")
            RECORDNAME_BY_TYPE(RT_TimeSetBehavior, "TimeSetBehavior")
            RECORDNAME_BY_TYPE(RT_TimeCommandBehavior, "TimeCommandBehavior")
            RECORDNAME_BY_TYPE(RT_TimeClientVisualElement, "TimeClientVisualElement")
            RECORDNAME_BY_TYPE(RT_TimePropertyList, "TimePropertyList")
            RECORDNAME_BY_TYPE(RT_TimeVariantList, "TimeVariantList")
            RECORDNAME_BY_TYPE(RT_TimeAnimationValueList, "TimeAnimationValueList")
            RECORDNAME_BY_TYPE(RT_TimeIterateData, "TimeIterateData")
            RECORDNAME_BY_TYPE(RT_TimeSequenceData, "TimeSequenceData")
            RECORDNAME_BY_TYPE(RT_TimeVariant, "TimeVariant")
            RECORDNAME_BY_TYPE(RT_TimeAnimationValue, "TimeAnimationValue")
            RECORDNAME_BY_TYPE(RT_TimeExtTimeNodeContainer, "TimeExtTimeNodeContainer")
            RECORDNAME_BY_TYPE(RT_TimeSubEffectContainer, "TimeSubEffectContainer")

            RECORDNAME_BY_TYPE(RTE_DGG_CONTAINER, "DGG_CONTAINER")
            RECORDNAME_BY_TYPE(RTE_BSTORE_CONTAINER, "BSTORE_CONTAINER")
            RECORDNAME_BY_TYPE(RTE_DG_CONTAINER, "DG_CONTAINER")
            RECORDNAME_BY_TYPE(RTE_SPGR_CONTAINER, "SPGR_CONTAINER")
            RECORDNAME_BY_TYPE(RTE_SP_CONTAINER, "SP_CONTAINER")
            RECORDNAME_BY_TYPE(RTE_SOLVERCONTAINER, "SOLVERCONTAINER")
            RECORDNAME_BY_TYPE(RTE_DGG, "DGG")
            RECORDNAME_BY_TYPE(RTE_BSE, "BSE")
            RECORDNAME_BY_TYPE(RTE_DG, "DG")
            RECORDNAME_BY_TYPE(RTE_SPGR, "SPGR")
            RECORDNAME_BY_TYPE(RTE_SP, "SP")
            RECORDNAME_BY_TYPE(RTE_OPT, "OPT")
            RECORDNAME_BY_TYPE(RTE_TEXTBOX, "TEXTBOX")
            RECORDNAME_BY_TYPE(RTE_CLIENTTEXTBOX, "CLIENTTEXTBOX")
            RECORDNAME_BY_TYPE(RTE_ANCHOR, "ANCHOR")
            RECORDNAME_BY_TYPE(RTE_CHILDANCHOR, "CHILDANCHOR")
            RECORDNAME_BY_TYPE(RTE_CLIENTANCHOR, "CLIENTANCHOR")
            RECORDNAME_BY_TYPE(RTE_CLIENTDATA, "CLIENTDATA")
            RECORDNAME_BY_TYPE(RTE_CONNECTORRULE, "CONNECTORRULE")
            RECORDNAME_BY_TYPE(RTE_ALIGNRULE, "ALIGNRULE")
            RECORDNAME_BY_TYPE(RTE_ARCRULE, "ARCRULE")
            RECORDNAME_BY_TYPE(RTE_CLIENTRULE, "CLIENTRULE")
            RECORDNAME_BY_TYPE(RTE_CLSID, "CLSID")
            RECORDNAME_BY_TYPE(RTE_CALLOUTRULE, "CALLOUTRULE")
            RECORDNAME_BY_TYPE(RTE_BLIP_START, "BLIP_START")

            RECORDNAME_BY_TYPE(RTE_BLIP_EMF, "BLIP_EMF")
            RECORDNAME_BY_TYPE(RTE_BLIP_WMF, "BLIP_WMF")
            RECORDNAME_BY_TYPE(RTE_BLIP_PICT, "BLIP_PICT")
            RECORDNAME_BY_TYPE(RTE_BLIP_JPEG, "BLIP_JPEG")
            RECORDNAME_BY_TYPE(RTE_BLIP_PNG, "BLIP_PNG")
            RECORDNAME_BY_TYPE(RTE_BLIP_DIB, "BLIP_DIB")

            RECORDNAME_BY_TYPE(RTE_BLIP_END, "BLIP_END")
            RECORDNAME_BY_TYPE(RTE_REGROUPITEMS, "REGROUPITEMS")
            RECORDNAME_BY_TYPE(RTE_SELECTION, "SELECTION")

            RECORDNAME_BY_TYPE(RTE_COLORMRU, "COLORMRU")
            RECORDNAME_BY_TYPE(RTE_DELETEDPSPL, "DELETEDPSPL")
            RECORDNAME_BY_TYPE(RTE_SPLITMENUCOLORS, "SPLITMENUCOLORS")
            RECORDNAME_BY_TYPE(RTE_OLEOBJECT, "OLEOBJECT")
            RECORDNAME_BY_TYPE(RTE_COLORSCHEME, "COLORSCHEME")
            RECORDNAME_BY_TYPE(RTE_SECONDARY_OPT, "SECONDARY_OPT")
            RECORDNAME_BY_TYPE(RTE_TETRIARY_OPT, "TETRIARY_OPT")
            RECORDNAME_BY_TYPE(RTE_BLIP_TIFF, "BLIP_TIFF")
            default:
        name = "UnknownRecord!";
    }
    return name;
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
            //    CREATE_BY_TYPE(RT_VisualShapeAtom, CRecordVisualSoundAtom)
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

            CREATE_BY_TYPE(RT_SlideListWithText, CRecordSlideListWithText)
            CREATE_BY_TYPE(RT_DocumentAtom, CRecordDocumentAtom)
            CREATE_BY_TYPE(RT_EndDocumentAtom, CRecordEndDocument)
            //CREATE_BY_TYPE(RECORD_TYPE_SSLIDE_LAYOUT_ATOM			, CRecordSSlideLayoutAtom)
            CREATE_BY_TYPE(RT_SlideAtom, CRecordSlideAtom)
            CREATE_BY_TYPE(RT_NotesAtom, CRecordNotesAtom)
            CREATE_BY_TYPE(RT_SlidePersistAtom, CRecordSlidePersistAtom)
            CREATE_BY_TYPE(RT_Environment, CRecordDocumentTextInfo)
            CREATE_BY_TYPE(RT_FontCollection, CRecordFontCollection)
            CREATE_BY_TYPE(RT_FontCollection10, CRecordFontCollection)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMasterPersistAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordNotesPersistAtom)
            CREATE_BY_TYPE(RT_SlideShowSlideInfoAtom, CSlideShowSlideInfoAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_GUIDE_ATOM					, CRecordGuideAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordZoomViewInfoAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_VIEWINFO_ATOM				, CRecordNoZoomViewInfoAtom)
            CREATE_BY_TYPE(RT_ViewInfoAtom, CRecordViewInfoAtom)
            CREATE_BY_TYPE(RT_SlideViewInfoAtom, CRecordSlideViewInfoAtom)
            CREATE_BY_TYPE(RT_VbaInfoAtom, CRecordVBAInfoAtom)
            CREATE_BY_TYPE(RT_SlideShowDocInfoAtom, CRecordSSDocInfoAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_DOC_ROUTING_SLIP			, CRecordDocRoutingSlipAtom)
            CREATE_BY_TYPE(RT_ExternalObjectListAtom, CRecordExObjListAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_NAMEDSHOW_SLIDES			, CRecordNamedShowSlidesAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_SHEET_PROPERTIES			, CRecordNormalViewSetInfoAtom)
            //    CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordGridSpacing10Atom)

            CREATE_BY_TYPE(RT_ColorSchemeAtom, CRecordColorSchemeAtom)

            CREATE_BY_TYPE(RT_RoundTripTheme12Atom, RoundTripTheme12Atom)
            CREATE_BY_TYPE(RT_RoundTripColorMapping12Atom, RoundTripColorMapping12Atom)
            CREATE_BY_TYPE(RT_RoundTripOriginalMainMasterId12Atom, RoundTripOriginalMainMasterId12Atom)
            CREATE_BY_TYPE(RT_RoundTripCompositeMasterId12Atom, RoundTripCompositeMasterId12Atom)
            CREATE_BY_TYPE(RT_RoundTripContentMasterInfo12Atom, RoundTripContentMasterInfo12Atom)			// ECMA-376 дубль
            CREATE_BY_TYPE(RT_RoundTripShapeId12Atom, RoundTripShapeId12Atom)
            CREATE_BY_TYPE(RT_RoundTripContentMasterId12Atom, RoundTripContentMasterId12Atom)
            CREATE_BY_TYPE(RT_RoundTripOArtTextStyles12Atom, RoundTripOArtTextStyles12Atom)
            CREATE_BY_TYPE(RT_RoundTripHeaderFooterDefaults12Atom, RoundTripHeaderFooterDefaults12Atom)
            CREATE_BY_TYPE(RT_RoundTripDocFlags12Atom, RoundTripDocFlags12Atom)
            CREATE_BY_TYPE(RT_RoundTripShapeCheckSumForCL12Atom, RoundTripShapeCheckSumForCL12Atom)
            CREATE_BY_TYPE(RT_RoundTripNotesMasterTextStyles12Atom, RoundTripNotesMasterTextStyles12Atom)
            CREATE_BY_TYPE(RT_RoundTripCustomTableStyles12Atom, RoundTripCustomTableStyles12Atom)
            CREATE_BY_TYPE(RT_RoundTripNewPlaceholderId12Atom, RoundTripNewPlaceholderId12Atom)
            CREATE_BY_TYPE(RT_RoundTripAnimationAtom12Atom, RoundTripAnimationAtom12Atom)
            CREATE_BY_TYPE(RT_RoundTripAnimationHashAtom12Atom, RoundTripAnimationHashAtom12Atom)
            CREATE_BY_TYPE(RT_RoundTripSlideSyncInfo12, RoundTripSlideSyncInfo12Container)
            CREATE_BY_TYPE(RT_RoundTripSlideSyncInfoAtom12, RoundTripSlideSyncInfoAtom12)

            //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPDOCFLAGS12						, CRecordRoundTripDocFlags12Atom)
            //CREATE_BY_TYPE(RECORD_TYPE_ROUNDTRIPSHAPECHECKSUMFORCUSTOMLAYOUTS12	, CRecordRoundTripShapeCheckSumForCustomLayouts12Atom)

            //CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_SEED_ATOM			, CRecordBookmarkSeedAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordBlipEntityAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordExObjRefAtom)
            CREATE_BY_TYPE(RT_PlaceholderAtom, CRecordPlaceHolderAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlagsAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN, CRecordShapeFlags10Atom)

            CREATE_BY_TYPE(RT_OutlineTextRefAtom, CRecordOutlineTextRefAtom)
            CREATE_BY_TYPE(RT_TextHeaderAtom, CRecordTextHeaderAtom)
            CREATE_BY_TYPE(RT_TextCharsAtom, CRecordTextCharsAtom)
            CREATE_BY_TYPE(RT_StyleTextPropAtom, CRecordStyleTextPropAtom)
            CREATE_BY_TYPE(RT_TextSpecialInfoAtom, CRecordTextSpecInfoAtom)
            CREATE_BY_TYPE(RT_MasterTextPropAtom, CRecordMasterTextPropAtom)
            CREATE_BY_TYPE(RT_TextMasterStyleAtom, CRecordTextMasterStyleAtom)

            CREATE_BY_TYPE(RT_TextCharFormatExceptionAtom, CRecordTextCFExceptionAtom)
            CREATE_BY_TYPE(RT_TextParagraphFormatExceptionAtom, CRecordTextPFExceptionAtom)
            CREATE_BY_TYPE(RT_TextSpecialInfoDefaultAtom, CRecordTextSIExceptionAtom)

            CREATE_BY_TYPE(RT_TextRulerAtom, CRecordTextRulerAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_TEXTBOOKMARK_ATOM			, CRecordTextBookmarkAtom)
            CREATE_BY_TYPE(RT_TextBytesAtom, CRecordTextBytesAtom)
            CREATE_BY_TYPE(RT_OutlineTextPropsHeader9Atom, CRecordOutlineTextPropsHeaderExAtom)

            CREATE_BY_TYPE(RT_GuideAtom, CRecordGuideAtom)

            CREATE_BY_TYPE(RT_FontEntityAtom, CRecordFontEntityAtom)
            CREATE_BY_TYPE(RT_FontEmbedDataBlob, CRecordFontEmbedDataBlob)
            //CREATE_BY_TYPE(RECORD_TYPE_METAFILE					, CRecordMetafileBlob)
            CREATE_BY_TYPE(RT_CString, CRecordCString)
            CREATE_BY_TYPE(RT_SoundCollectionAtom, CRecordSoundCollAtom)
            CREATE_BY_TYPE(RT_ExternalOleObjectAtom, CRecordExOleObjAtom)
            CREATE_BY_TYPE(RT_ExternalOleEmbedAtom, CRecordExOleEmbedAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_BOOKMARK_ENTITY_ATOM		, CRecordBookmarkEntityAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_EXLINK_ATOM				, CRecordExOleLinkAtom)

            CREATE_BY_TYPE(RT_KinsokuAtom, CRecordKinsokuAtom)
            CREATE_BY_TYPE(RT_ExternalHyperlink, CRecordExHyperlinkContainer)
            CREATE_BY_TYPE(RT_ExternalHyperlinkAtom, CRecordExHyperlinkAtom)

            CREATE_BY_TYPE(RT_HeadersFooters, CRecordHeadersFootersContainer)
            CREATE_BY_TYPE(RT_HeadersFootersAtom, CRecordHeadersFootersAtom)

            CREATE_BY_TYPE(RT_SlideNumberMetaCharAtom, CRecordSlideNumberMetaAtom)
            CREATE_BY_TYPE(RT_DateTimeMetaCharAtom, CRecordDateTimeMetaAtom)
            CREATE_BY_TYPE(RT_HeaderMetaCharAtom, CRecordHeaderMetaAtom)
            CREATE_BY_TYPE(RT_FooterMetaCharAtom, CRecordFooterMetaAtom)
            CREATE_BY_TYPE(RT_GenericDateMetaCharAtom, CRecordGenericDateMetaAtom)
            CREATE_BY_TYPE(RT_RtfDateTimeMetaCharAtom, CRecordRTFDateTimeMetaAtom)

            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordMouseTextInteractiveInfoAtom)
            //CREATE_BY_TYPE(RECORD_TYPE_UNKNOWN					, CRecordRecolorInfoAtom)

            CREATE_BY_TYPE(RT_ExternalVideo, CRecordExVideoContainer)
            CREATE_BY_TYPE(RT_ExternalObjectRefAtom, CRecordExObjRefAtom)
            CREATE_BY_TYPE(RT_ExternalMediaAtom, CRecordExMediaAtom)
            CREATE_BY_TYPE(RT_ExternalOleControlAtom, CRecordExControlAtom)

            CREATE_BY_TYPE(RTE_BSTORE_CONTAINER, CRecordBlipStoreContainer)
            CREATE_BY_TYPE(RTE_BSE, CRecordBlipStoreEntry)

            CREATE_BY_TYPE(RTE_DG_CONTAINER, CRecordDrawingContainer)
            CREATE_BY_TYPE(RTE_DGG, CRecordDrawingGroup)
            CREATE_BY_TYPE(RTE_DG, CRecordDrawingRecord)
            CREATE_BY_TYPE(RTE_SPGR, CRecordGroupShape)
            CREATE_BY_TYPE(RTE_SPGR_CONTAINER, CRecordGroupShapeContainer)
            CREATE_BY_TYPE(RTE_SP_CONTAINER, CRecordShapeContainer)
            CREATE_BY_TYPE(RTE_SP, CRecordShape)
            CREATE_BY_TYPE(RTE_CLIENTANCHOR, CRecordClientAnchor)
            CREATE_BY_TYPE(RTE_CHILDANCHOR, CRecordChildAnchor)
            CREATE_BY_TYPE(RTE_OPT, CRecordShapeProperties)
            CREATE_BY_TYPE(RTE_SECONDARY_OPT, CRecordShapeProperties)
            CREATE_BY_TYPE(RTE_TETRIARY_OPT, CRecordShapeProperties)
            CREATE_BY_TYPE(RTE_REGROUPITEMS, CRecordRegGroupItems)
            CREATE_BY_TYPE(RTE_SPLITMENUCOLORS, CRecordSplitMenuColorRecord)
            CREATE_BY_TYPE(RTE_COLORMRU, CRecordMostRecentlyUserColors)
            CREATE_BY_TYPE(RTE_CLIENTDATA, CRecordOfficeArtClientData)
            CREATE_BY_TYPE(RTE_CLIENTTEXTBOX, CRecordOfficeArtClientTextbox)


            CREATE_BY_TYPE(RT_ExternalCdAudio, CRecordExCDAudioContainer)
            CREATE_BY_TYPE(RT_ExternalWavAudioLink, CRecordWAVAudioLinkContainer)
            CREATE_BY_TYPE(RT_ExternalWavAudioEmbedded, CRecordWAVAudioEmbeddedContainer)
            CREATE_BY_TYPE(RT_ExternalMidiAudio, CRecordExMIDIAudioContainer)

            CREATE_BY_TYPE(RT_List, CRecordDocInfoListContainer)

            // интерактив
            CREATE_BY_TYPE(RT_InteractiveInfoAtom, CRecordInteractiveInfoAtom)
            CREATE_BY_TYPE(RT_TextInteractiveInfoAtom, CRecordTextInteractiveInfoAtom)
            CREATE_BY_TYPE(RT_InteractiveInfo, CRecordMouseInteractiveInfoContainer)

            // читаем для анимации

            CREATE_BY_TYPE(RT_SoundCollection, CRecordSoundCollectionContainer)
            CREATE_BY_TYPE(RT_ExternalObjectList, CRecordExObjListContainer)

            CREATE_BY_TYPE(RT_Sound, CRecordSoundContainer)
            CREATE_BY_TYPE(RT_SoundDataBlob, CRecordSoundDataBlob)
            CREATE_BY_TYPE(RT_MetaFile, CRecordMetafileBlob)

            default:
    {
        //            std::cout << GetRecordName(oHeader.RecType) << ": " << oHeader.RecLen << "\n";
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
            char str[1024] = {};
            sprintf(str, "Unknown record type: %x\t", oHeader.RecType);
            std::cout << str << GetRecordName(oHeader.RecType) << "\n";
#endif
        }
    }
    else
    {
    }
    return pRecord;
}

void SRecordHeader::Clear()
{
    RecVersion = 0;
    RecInstance = 0;
    RecType = RT_NONE;
    RecLen = 0;

    bBadHeader = false;
}

SRecordHeader::SRecordHeader()
{
    Clear();
}

bool SRecordHeader::ReadFromStream(const XLS::CFStreamPtr &pStream)
{
    Clear();

    if (pStream->isEOF()) return FALSE;
    POLE::uint64 nRd = 0;

    unsigned short rec = 0;
    pStream->read((unsigned char*)&(rec), 2);

    RecInstance = rec >> 4;
    RecVersion = rec - (RecInstance << 4);

    *pStream >> RecType >> RecLen;

    unsigned long sz = pStream->getStreamSize() - pStream->getStreamPointer();

    if (RecLen > sz)
    {
        RecLen = (UINT)sz;
        bBadHeader = true; // GZoabli_PhD.ppt ... RecLen & 0xffff ????
    }

    return true;
}

bool SRecordHeader::ReadFromStream(POLE::Stream *pStream)
{
    Clear();
    if (!pStream) return false;

    POLE::uint64 nRd = 0;

    unsigned short rec = 0;
    nRd = pStream->read((unsigned char*)&(rec), 2);

    if (nRd != 2) return false;

    RecInstance = rec >> 4;
    RecVersion = rec - (RecInstance << 4);

    nRd = pStream->read((unsigned char*)&(RecType), 2);

    nRd = pStream->read((unsigned char*)&(RecLen), 4);

    POLE::uint64 sz = pStream->size() - pStream->tell();

    if (RecLen > sz)
    {
        RecLen = (UINT)sz;
        bBadHeader = true; // GZoabli_PhD.ppt ... RecLen & 0xffff ????
    }

    //        void** backTraceData = (void**)(new char*[40]);
    //        int backTraceSize = backtrace(backTraceData, 40);

    //        std::ofstream file("/home/ivaz28/pp/dia/ppt/pptRecords.txt", std::ios::out | std::ios::app);

    //        file << std::string(backTraceSize - 11, ' ')
    //             << "0x" << std::setw(4) << std::setfill('0') << std::hex << (int)RecType
    //             << " " << std::setw(40) << std::setfill(' ') << std::left << GetRecordName(RecType)
    //             << " " << std::setw(5) << std::dec << RecLen
    //             << " " << backTraceSize << std::endl;

    //        delete [] backTraceData;
    //        file.close();

    return true;
}

bool SRecordHeader::IsContainer()
{
    /*if ((RecVersion == PSFLAG_CONTAINER) || ((RecVersion & 0x0F) == 0x0F))
            {
                return TRUE;
            }*/
    if (1064 == RecType)
        return false;

    if (RecVersion == 0x0F)
    {
        return true;
    }
    return false;
}

SRecordHeader &SRecordHeader::operator =(const SRecordHeader &oSrc)
{
    RecVersion = oSrc.RecVersion;
    RecInstance = oSrc.RecInstance;
    RecType = oSrc.RecType;
    RecLen = oSrc.RecLen;
    return (*this);
}

IRecord::~IRecord() {}

}
