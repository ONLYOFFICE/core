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
    II_NextSlideII_PreviousSlideII_FirstSlideII_LastSlideII_LastSlideViewedII_EndShow
};
