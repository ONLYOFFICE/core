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


namespace PPT
{
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


enum ElementTypeEnum
{
    TL_ET_ShapeType = 1,
    TL_ET_SoundType
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


enum PlaceholderEnum : BYTE
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
    PT_Picture,
    PT_Body_Empty =  0xFF
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


enum TextAutoNumberSchemeEnum : USHORT
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


enum TextTypeEnum : unsigned
{
    Tx_TYPE_TITLE,
    Tx_TYPE_BODY,
    Tx_TYPE_NOTES,
    Tx_TYPE_OTHER       = 0x4,
    Tx_TYPE_CENTERBODY,
    Tx_TYPE_CENTERTITLE,
    Tx_TYPE_HALFBODY,
    Tx_TYPE_QUARTERBODY,

    NoPresent           = 0xFF
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
}
