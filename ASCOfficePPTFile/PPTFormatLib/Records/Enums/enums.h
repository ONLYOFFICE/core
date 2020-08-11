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
#pragma once
#include "../../../DesktopEditor/common/Types.h"


enum AnimAfterEffectEnum : BYTE
{
    AI_NoAfterEffect,
    AI_Dim,
    AI_Hide,
    AI_HideImmediately
};


enum AnimBuildTypeEnum : BYTE
{
    BT_NoBuild,
    BT_OneBuild,
    BT_Level1Build,
    BT_Level2Build,
    BT_Level3Build,
    BT_Level4Build,
    BT_Level5Build,
    BT_GraphBySeries,
    BT_GraphByCategory,
    BT_GraphByElementInSeries,
    BT_GraphByElementInCategory,
    BT_FollowMaster = 0xFE
};


enum BuildTypeEnum
{
    TL_BuildParagraph,
    TL_BuildChart,
    TL_BuildDiagram
};


enum ChartBuildEnum
{
    TLCB_AsOneObject,
    TLCB_BySeries,
    TLCB_ByCategory,
    TLCB_ByElementInSeries,
    TLCB_ByElementInCategory,
    TLCB_Custom
};


enum ColorModeEnum : BYTE
{
    CM_BlackAndWhite,
    CM_Grayscale,
    CM_Color
};

// TODO add different tupes for enum
enum ConditionEnum
{
    TL_CT_None,
    TL_CT_Begin,
    TL_CT_End,
    TL_CT_Next,
    TL_CT_Previous,
    TL_CT_EndSync
};


enum DiagramBuildEnum
{
    TLDB_AsOneObject,
    TLDB_DepthByNode,
    TLDB_DepthByBranch,
    TLDB_BreadthByNode,
    TLDB_BreadthByLevel,
    TLDB_ClockWise,
    TLDB_ClockWiseIn,
    TLDB_ClockWiseOut,
    TLDB_CounterClockWise,
    TLDB_CounterClockWiseIn,
    TLDB_CounterClockWiseOut,
    TLDB_InByRing,
    TLDB_OutByRing,
    TLDB_Up,
    TLDB_Down,
    TLDB_AllAtOnce,
    TLDB_Custom
};


enum DiffTypeEnum
{
    Diff_DocDiff,
    Diff_SlideDiff          = 0x2,
    Diff_MainMasterDiff,
    Diff_SlideListDiff,
    Diff_MasterListDiff,
    Diff_ShapeListDiff,
    Diff_ShapeDiff,
    Diff_TextDiff           = 0x9,
    Diff_NotesDiff,
    Diff_SlideShowDiff,
    Diff_HeaderFooterDiff,
    Diff_NamedShowDiff      = 0xE,
    Diff_NamedShowListDiff,
    Diff_RecolorInfoDiff    = 0x12,
    Diff_ExternalObjectDiff,
    Diff_TableListDiff      = 0x15,
    Diff_TableDiff,
    Diff_InteractiveInfoDiff
};


enum ExColorFollowEnum
{
    ExColor_FollowNone,
    ExColor_FollowScheme,
    ExColor_FollowTextAndBackground
};


enum ExOleObjSubTypeEnum
{
    ExOleSub_Default,
    ExOleSub_Clipart,
    ExOleSub_WordDoc,
    ExOleSub_Excel,
    ExOleSub_Graph,
    ExOleSub_OrgChart,
    ExOleSub_Equation,
    ExOleSub_WordArt,
    ExOleSub_Sound,
    ExOleSub_Project        = 0xC,
    ExOleSub_NoteIt,
    ExOleSub_ExcelChart,
    ExOleSub_MediaPlayer,
    ExOleSub_WordPad,
    ExOleSub_Visio,
    ExOleSub_WordODF,
    ExOleSub_ExcelODF,
    ExOleSub_PPTODF
};


enum ExOleObjTypeEnum
{
    ExOle_Embedded,
    ExOle_Link,
    ExOle_Control
};


enum InteractiveInfoActionEnum
{
    II_NoAction,
    II_MacroAction,
    II_RunProgramAction,
    II_JumpAction,
    II_HyperlinkAction,
    II_OLEAction,
    II_MediaAction,
    II_CustomShowAction
};


enum InteractiveInfoJumpEnum
{
    II_NoJump,
    II_NextSlide,
    II_PreviousSlide,
    II_FirstSlide,
    II_LastSlide,
    II_LastSlideViewed,
    II_EndShow
};


enum LinkToEnum
{
    LT_NextSlide,
    LT_PreviousSlide,
    LT_FirstSlide,
    LT_LastSlide,
    LT_CustomShow           = 0x6,
    LT_SlideNumber,
    LT_Url,
    LT_OtherPresentation,
    LT_OtherFile,
    LT_Nil                  = 0xFF
};


enum NormalViewSetBarStates
{
    BS_Minimized,
    BS_Restored,
    BS_Maximized
};


enum OLEVerbEnum
{
    OV_Primary,
    OV_Secondary,
    OV_Tertiary
};


enum ParaBuildEnum
{
    TLPB_AllAtOnce,
    TLPB_BuildByNthLevel,
    TLPB_CustomBuild,
    TLPB_AsAWhole
};


enum PhotoAlbumFrameShapeEnum
{
    PA_Rectangle,
    PA_RoundedRectangle,
    PA_Beveled,
    PA_Oval,
    PA_Octagon,
    PA_Cross,
    PA_Plaque
};


enum PhotoAlbumLayoutEnum
{
    PA_FitToSlide,
    PA_OnePicture,
    PA_TwoPictures,
    PA_FourPictures,
    PA_OnePictureAndTitle,
    PA_TwoPicturesAndTitle,
    PA_FourPicturesAndTitle
};


enum PlaceholderEnum
{
    PT_None,
    PT_MasterTitle,
    PT_MasterBody,
    PT_MasterCenterTitle,
    PT_MasterSubTitle,
    PT_MasterNotesSlideImage,
    PT_MasterNotesBody,
    PT_MasterDate,
    PT_MasterSlideNumber,
    PT_MasterFooter,
    PT_MasterHeader,
    PT_NotesSlideImage,
    PT_NotesBody,
    PT_Title,
    PT_Body,
    PT_CenterTitle,
    PT_SubTitle,
    PT_VerticalTitle,
    PT_VerticalBody,
    PT_Object,
    PT_Graph,
    PT_Table,
    PT_ClipArt,
    PT_OrgChart,
    PT_Media,
    PT_VerticalObject,
    PT_Picture
};


enum PlaceholderSize
{
    PS_Full,
    PS_Half,
    PS_Quarter
};


enum PrintWhatEnum
{
    PW_Slides,
    PW_BuildSlides,
    PW_Handouts2,
    PW_Handouts3,
    PW_Handouts6,
    PW_Notes,
    PW_Outline,
    PW_Handouts4,
    PW_Handouts9,
    PW_Handouts1
};


enum RecordType
{
    RT_Document                             = 0x03E8,
    RT_DocumentAtom,
    RT_EndDocumentAtom,
    RT_Slide                                = 0x03EE,
    RT_SlideAtom,
    RT_Notes,
    RT_NotesAtom,
    RT_Environment,
    RT_SlidePersistAtom,
    RT_MainMaster                           = 0x03F8,
    RT_SlideShowSlideInfoAtom,
    RT_SlideViewInfo,
    RT_GuideAtom,
    RT_ViewInfoAtom                         = 0x03FD,
    RT_SlideViewInfoAtom,
    RT_VbaInfo,
    RT_VbaInfoAtom,
    RT_SlideShowDocInfoAtom,
    RT_Summary,
    RT_DocRoutingSlipAtom                   = 0x0406,
    RT_OutlineViewInfo,
    RT_SorterViewInfo,
    RT_ExternalObjectList,
    RT_ExternalObjectListAtom,
    RT_DrawingGroup,
    RT_Drawing,
    RT_GridSpacing10Atom,
    RT_RoundTripTheme12Atom,
    RT_RoundTripColorMapping12Atom,
    RT_NamedShows,
    RT_NamedShow,
    RT_NamedShowSlidesAtom,
    RT_NotesTextViewInfo9,
    RT_NormalViewSetInfo9,
    RT_NormalViewSetInfo9Atom,
    RT_RoundTripOriginalMainMasterId12Atom  = 0x041C,
    RT_RoundTripCompositeMasterId12Atom,
    RT_RoundTripContentMasterInfo12Atom,
    RT_RoundTripShapeId12Atom,
    RT_RoundTripHFPlaceholder12Atom,
    RT_RoundTripContentMasterId12Atom       = 0x0422,
    RT_RoundTripOArtTextStyles12Atom,
    RT_RoundTripHeaderFooterDefaults12Atom,
    RT_RoundTripDocFlags12Atom,
    RT_RoundTripShapeCheckSumForCL12Atom,
    RT_RoundTripNotesMasterTextStyles12Atom,
    RT_RoundTripCustomTableStyles12Atom,
    RT_List                                 = 0x07D0,
    RT_FontCollection                       = 0x07D5,
    RT_FontCollection10,
    RT_BookmarkCollection                   = 0x07E3,
    RT_SoundCollection,
    RT_SoundCollectionAtom,
    RT_Sound,
    RT_SoundDataBlob,
    RT_BookmarkSeedAtom                     = 0x07E9,
    RT_ColorSchemeAtom                      = 0x07F0,
    RT_BlipCollection9                      = 0x07F8,
    RT_BlipEntity9Atom,
    RT_ExternalObjectRefAtom                = 0x0BC1,
    RT_PlaceholderAtom                      = 0x0BC3,
    RT_ShapeAtom                            = 0x0BDB,
    RT_ShapeFlags10Atom,
    RT_RoundTripNewPlaceholderId12Atom,
    RT_OutlineTextRefAtom                   = 0x0F9E,
    RT_TextHeaderAtom,
    RT_TextCharsAtom,
    RT_StyleTextPropAtom,
    RT_MasterTextPropAtom,
    RT_TextMasterStyleAtom,
    RT_TextCharFormatExceptionAtom,
    RT_TextParagraphFormatExceptionAtom,
    RT_TextRulerAtom,
    RT_TextBookmarkAtom,
    RT_TextBytesAtom,
    RT_TextSpecialInfoDefaultAtom,
    RT_TextSpecialInfoAtom,
    RT_DefaultRulerAtom,
    RT_StyleTextProp9Atom,
    RT_TextMasterStyle9Atom,
    RT_OutlineTextProps9,
    RT_OutlineTextPropsHeader9Atom,
    RT_TextDefaults9Atom,
    RT_StyleTextProp10Atom,
    RT_TextMasterStyle10Atom,
    RT_OutlineTextProps10,
    RT_TextDefaults10Atom,
    RT_OutlineTextProps11,
    RT_StyleTextProp11Atom,
    RT_FontEntityAtom,
    RT_FontEmbedDataBlob,
    RT_CString                              = 0x0FBA,
    RT_MetaFile                             = 0x0FC1,
    RT_ExternalOleObjectAtom                = 0x0FC3,
    RT_Kinsoku                              = 0x0FC8,
    RT_Handout,
    RT_ExternalOleEmbed                     = 0x0FCC,
    RT_ExternalOleEmbedAtom,
    RT_ExternalOleLink,
    RT_BookmarkEntityAtom                   = 0x0FD0,
    RT_ExternalOleLinkAtom,
    RT_KinsokuAtom,
    RT_ExternalHyperlinkAtom,
    RT_ExternalHyperlink                    = 0x0FD7,
    RT_SlideNumberMetaCharAtom,
    RT_HeadersFooters,
    RT_HeadersFootersAtom,
    RT_TextInteractiveInfoAtom              = 0x0FDF,
    RT_ExternalHyperlink9                   = 0x0FE4,
    RT_RecolorInfoAtom                      = 0x0FE7,
    RT_ExternalOleControl                   = 0x0FEE,
    RT_SlideListWithText                    = 0x0FF0,
    RT_AnimationInfoAtom,
    RT_InteractiveInfo,
    RT_InteractiveInfoAtom,
    RT_UserEditAtom,
    RT_CurrentUserAtom,
    RT_DateTimeMetaCharAtom,
    RT_GenericDateMetaCharAtom,
    RT_HeaderMetaCharAtom,
    RT_FooterMetaCharAtom,
    RT_ExternalOleControlAtom,
    RT_ExternalMediaAtom                    = 0x1004,
    RT_ExternalVideo,
    RT_ExternalAviMovie,
    RT_ExternalMciMovie,
    RT_ExternalMidiAudio                    = 0x100D,
    RT_ExternalCdAudio,
    RT_ExternalWavAudioEmbedded,
    RT_ExternalWavAudioLink,
    RT_ExternalOleObjectStg,
    RT_ExternalCdAudioAtom,
    RT_ExternalWavAudioEmbeddedAtom,
    RT_AnimationInfo,
    RT_RtfDateTimeMetaCharAtom,
    RT_ExternalHyperlinkFlagsAtom           = 0x1018,
    RT_ProgTags                             = 0x1388,
    RT_ProgStringTag,
    RT_ProgBinaryTag,
    RT_BinaryTagDataBlob,
    RT_PrintOptionsAtom                     = 0x1770,
    RT_PersistDirectoryAtom                 = 0x1772,
    RT_PresentationAdvisorFlags9Atom        = 0x177A,
    RT_HtmlDocInfo9Atom,
    RT_HtmlPublishInfoAtom,
    RT_HtmlPublishInfo9,
    RT_BroadcastDocInfo9,
    RT_BroadcastDocInfo9Atom,
    RT_EnvelopeFlags9Atom                   = 0x1784,
    RT_EnvelopeData9Atom,
    RT_VisualShapeAtom                      = 0x2AFB,
    RT_HashCodeAtom                         = 0x2B00,
    RT_VisualPageAtom,
    RT_BuildList,
    RT_BuildAtom,
    RT_ChartBuild,
    RT_ChartBuildAtom,
    RT_DiagramBuild,
    RT_DiagramBuildAtom,
    RT_ParaBuild,
    RT_LevelInfoAtom,
    RT_RoundTripAnimationAtom12Atom,
    RT_RoundTripAnimationHashAtom12Atom     = 0x2B0D,
    RT_Comment10                            = 0x2EE0,
    RT_Comment10Atom,
    RT_CommentIndex10                       = 0x2EE4,
    RT_CommentIndex10Atom,
    RT_LinkedShape10Atom,
    RT_LinkedSlide10Atom,
    RT_SlideFlags10Atom                     = 0x2EEA,
    RT_SlideTime10Atom,
    RT_DiffTree10,
    RT_Diff10,
    RT_Diff10Atom,
    RT_SlideListTableSize10Atom,
    RT_SlideListEntry10Atom,
    RT_SlideListTable10,
    RT_CryptSession10Container              = 0x2F14,
    RT_FontEmbedFlags10Atom                 = 0x32C8,
    RT_FilterPrivacyFlags10Atom             = 0x36B0,
    RT_DocToolbarStates10Atom,
    RT_PhotoAlbumInfo10Atom,
    RT_SmartTagStore11Container,
    RT_RoundTripSlideSyncInfo12             = 0x3714,
    RT_RoundTripSlideSyncInfoAtom12,
    RT_TimeConditionContainer               = 0xF125,
    RT_TimeNode                             = 0xF127,
    RT_TimeCondition,
    RT_TimeModifier,
    RT_TimeBehaviorContainer,
    RT_TimeAnimateBehaviorContainer,
    RT_TimeColorBehaviorContainer,
    RT_TimeEffectBehaviorContainer,
    RT_TimeMotionBehaviorContainer,
    RT_TimeRotationBehaviorContainer,
    RT_TimeScaleBehaviorContainer,
    RT_TimeSetBehaviorContainer,
    RT_TimeCommandBehaviorContainer,
    RT_TimeBehavior,
    RT_TimeAnimateBehavior,
    RT_TimeColorBehavior,
    RT_TimeEffectBehavior,
    RT_TimeMotionBehavior,
    RT_TimeRotationBehavior,
    RT_TimeScaleBehavior,
    RT_TimeSetBehavior,
    RT_TimeCommandBehavior,
    RT_TimeClientVisualElement,
    RT_TimePropertyList,
    RT_TimeVariantList,
    RT_TimeAnimationValueList,
    RT_TimeIterateData,
    RT_TimeSequenceData,
    RT_TimeVariant,
    RT_TimeAnimationValue,
    RT_TimeExtTimeNodeContainer,
    RT_TimeSubEffectContainer
};


enum SlideLayoutType
{
    SL_TitleSlide,
    SL_TitleBody,
    SL_MasterTitle,
    SL_TitleOnly        = 0x7,
    SL_TwoColumns,
    SL_TwoRows,
    SL_ColumnTwoRows,
    SL_TwoRowsColumn,
    SL_TwoColumnsRow    = 0xD,
    SL_FourObjects,
    SL_BigObject,
    SL_Blank,
    SL_VerticalTitleBody,
    SL_VerticalTwoRows
};


enum SlideSizeEnum
{
    SS_Screen,
    SS_LetterPaper,
    SS_A4Paper,
    SS_35mm,
    SS_Overhead,
    SS_Banner,
    SS_Custom
};


enum TextAlignmentEnum
{
    Tx_ALIGNLeft,
    Tx_ALIGNCenter,
    Tx_ALIGNRight,
    Tx_ALIGNJustify,
    Tx_ALIGNDistributed,
    Tx_ALIGNThaiDistributed,
    Tx_ALIGNJustifyLow
};


enum TextAutoNumberSchemeEnum
{
    ANM_AlphaLcPeriod,
    ANM_AlphaUcPeriod,
    ANM_ArabicParenRight,
    ANM_ArabicPeriod,
    ANM_RomanLcParenBoth,
    ANM_RomanLcParenRight,
    ANM_RomanLcPeriod,
    ANM_RomanUcPeriod,
    ANM_AlphaLcParenBoth,
    ANM_AlphaLcParenRight,
    ANM_AlphaUcParenBoth,
    ANM_AlphaUcParenRight,
    ANM_ArabicParenBoth,
    ANM_ArabicPlain,
    ANM_RomanUcParenBoth,
    ANM_RomanUcParenRight,
    ANM_ChsPlain,
    ANM_ChsPeriod,
    ANM_CircleNumDBPlain,
    ANM_CircleNumWDBWhitePlain,
    ANM_CircleNumWDBBlackPlain,
    ANM_ChtPlain,
    ANM_ChtPeriod,
    ANM_Arabic1Minus,
    ANM_Arabic2Minus,
    ANM_Hebrew2Minus,
    ANM_JpnKorPlain,
    ANM_JpnKorPeriod,
    ANM_ArabicDbPlain,
    ANM_ArabicDbPeriod,
    ANM_ThaiAlphaPeriod,
    ANM_ThaiAlphaParenRight,
    ANM_ThaiAlphaParenBoth,
    ANM_ThaiNumPeriod,
    ANM_ThaiNumParenRight,
    ANM_ThaiNumParenBoth,
    ANM_HindiAlphaPeriod,
    ANM_HindiNumPeriod,
    ANM_JpnChsDBPeriod,
    ANM_HindiNumParenRight,
    ANM_HindiAlpha1Period
};


enum TextBuildSubEffectEnum
{
    TXB_BuildByNone,
    TXB_BuildByWord,
    TXB_BuildByCharacter
};


enum TextDirectionEnum
{
    LeftToRight,
    RightToLeft
};


enum TextFontAlignmentEnum
{
    Tx_ALIGNFONTRoman,
    Tx_ALIGNFONTHanging,
    Tx_ALIGNFONTCenter,
    Tx_ALIGNFONTUpholdFixed
};


enum TextTabTypeEnum
{
    Tx_TABLeft,
    Tx_TABCenter,
    Tx_TABRight,
    Tx_TABDecimal
};


enum TextTypeEnum
{
    Tx_TYPE_TITLE,
    Tx_TYPE_BODY,
    Tx_TYPE_NOTES,
    Tx_TYPE_OTHER       = 0x4,
    Tx_TYPE_CENTERBODY,
    Tx_TYPE_CENTERTITLE,
    Tx_TYPE_HALFBODY,
    Tx_TYPE_QUARTERBODY
};


enum TimeAnimateBehaviorValueTypeEnum
{
    TL_TABVT_String,
    TL_TABVT_Number,
    TL_TABVT_Color
};


enum TimeCommandBehaviorTypeEnum
{
    TL_TCBT_Eventv,
    TL_TCBT_Call,
    TL_TCBT_OleVerb
};


enum TimeNodeTypeEnum
{
    TL_TNT_Parallel,
    TL_TNT_Sequential,
    TL_TNT_Behavior = 0x3,
    TL_TNT_Media
};


enum TimePropertyID4TimeBehavior
{
    TL_TBPID_UnknownPropertyList    = 0x1,
    TL_TBPID_RuntimeContext,
    TL_TBPID_MotionPathEditRelative,
    TL_TBPID_ColorColorModel,
    TL_TBPID_ColorDirection,
    TL_TBPID_Override,
    TL_TBPID_PathEditRotationAngle,
    TL_TBPID_PathEditRotationX,
    TL_TBPID_PathEditRotationY,
    TL_TBPID_PointsTypes
};


enum TimePropertyID4TimeNode
{
    TL_TPID_Display             = 0x02,
    TL_TPID_MasterPos           = 0x05,
    TL_TPID_SubType,
    TL_TPID_EffectID            = 0x09,
    TL_TPID_EffectDir,
    TL_TPID_EffectType,
    TL_TPID_AfterEffect         = 0x0D,
    TL_TPID_SlideCount          = 0x0F,
    TL_TPID_TimeFilter,
    TL_TPID_EventFilter,
    TL_TPID_HideWhenStopped,
    TL_TPID_GroupID,
    TL_TPID_EffectNodeType,
    TL_TPID_PlaceholderNode,
    TL_TPID_MediaVolume,
    TL_TPID_MediaMute,
    TL_TPID_ZoomToFullScreen    = 0x1A
};


enum TimeVariantTypeEnum
{
    TL_TVT_Bool,
    TL_TVT_Int,
    TL_TVT_Float,
    TL_TVT_String
};


enum TimeVisualElementEnum
{
    TL_TVET_Shape,
    TL_TVET_Page,
    TL_TVET_TextRange,
    TL_TVET_Audio,
    TL_TVET_Video,
    TL_TVET_ChartElement,
    TL_TVET_ShapeOnly,
    TL_TVET_AllTextRange    = 0x8
};


enum TriggerObjectEnum
{
    TL_TOT_None,
    TL_TOT_VisualElement,
    TL_TOT_TimeNode,
    TL_TOT_RuntimeNodeRef
};


enum ViewTypeEnum
{
    V_Slide,
    V_SlideMaster,
    V_Notes,
    V_Handout,
    V_NotesMaster,
    V_OutlineMaster,
    V_Outline,
    V_SlideSorter,
    V_VisualBasic,
    V_TitleMaster,
    V_SlideShow,
    V_SlideShowFullScreen,
    V_NotesText,
    V_PrintPreview,
    V_Thumbnails,
    V_MasterThumbnails,
    V_PodiumSlideView,
    V_PodiumNotesView
};


enum WebFrameColorsEnum
{
    MSOWOPTBrowserColors,
    MSOWOPTPresentationSchemeTextColor,
    MSOWOPTPresentationSchemeAccentColor,
    MSOWOPTWhiteTextOnBlack,
    MSOWOPTBlackTextOnWhite
};


enum WebOutputEnum
{
    HTML_EXPORTVersion3 = 0x1,
    HTML_EXPORTVersion4,
    HTML_EXPORTDual     = 0x4
};
