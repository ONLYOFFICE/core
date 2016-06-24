/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "CFRecordType.h"

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string.hpp>

namespace XLS
{

struct CFRecordTypeValue
{
	CFRecordType::TypeString name;
	CFRecordType::TypeId id;
}
CFRecordTypeValues[] = 
{
	{"Formula",					rt_Formula},
	{"EOF",						rt_EOF},
	{"CalcCount",				rt_CalcCount},
	{"CalcMode",				rt_CalcMode},
	{"CalcPrecision",			rt_CalcPrecision},
	{"CalcRefMode",				rt_CalcRefMode},
	{"CalcDelta",				rt_CalcDelta},
	{"CalcIter",				rt_CalcIter},
	{"Protect",					rt_Protect},
	{"Password",				rt_Password},
	{"Header",					rt_Header},
	{"Footer",					rt_Footer},
	{"ExternSheet",				rt_ExternSheet},
	{"Lbl",						rt_Lbl},
	{"WinProtect",				rt_WinProtect},
	{"VerticalPageBreaks",		rt_VerticalPageBreaks},
	{"HorizontalPageBreaks",	rt_HorizontalPageBreaks},
	{"Note",					rt_Note},
	{"Selection",				rt_Selection},
	{"Date1904",				rt_Date1904},
	{"ExternName",				rt_ExternName},
	{"LeftMargin",				rt_LeftMargin},
	{"RightMargin",				rt_RightMargin},
	{"TopMargin",				rt_TopMargin},
	{"BottomMargin",			rt_BottomMargin},
	{"PrintRowCol",				rt_PrintRowCol},
	{"PrintGrid",				rt_PrintGrid},
	{"FilePass",				rt_FilePass},
	{"Font",					rt_Font},
	{"PrintSize",				rt_PrintSize},
	{"Continue",				rt_Continue},
	{"Window1",					rt_Window1},
	{"Backup",					rt_Backup},
	{"Pane",					rt_Pane},
	{"CodePage",				rt_CodePage},
	{"Pls",						rt_Pls},
	{"DCon",					rt_DCon},
	{"DConRef",					rt_DConRef},
	{"DConName",				rt_DConName},
	{"DefColWidth",				rt_DefColWidth},
	{"XCT",						rt_XCT},
	{"CRN",						rt_CRN},
	{"FileSharing",				rt_FileSharing},
	{"WriteAccess",				rt_WriteAccess},
	{"Obj",						rt_Obj},
	{"Uncalced",				rt_Uncalced},
	{"CalcSaveRecalc",			rt_CalcSaveRecalc},
	{"Template",				rt_Template},
	{"Intl",					rt_Intl},
	{"ObjProtect",				rt_ObjProtect},
	{"ColInfo",					rt_ColInfo},
	{"Guts",					rt_Guts},
	{"WsBool",					rt_WsBool},
	{"GridSet",					rt_GridSet},
	{"HCenter",					rt_HCenter},
	{"VCenter",					rt_VCenter},
	{"BoundSheet8",				rt_BoundSheet8},
	{"WriteProtect",			rt_WriteProtect},
	{"Country",					rt_Country},
	{"HideObj",					rt_HideObj},
	{"Sort",					rt_Sort},
	{"Palette",					rt_Palette},
	{"Sync",					rt_Sync},
	{"LPr",						rt_LPr},
	{"DxGCol",					rt_DxGCol},
	{"FnGroupName",				rt_FnGroupName},
	{"FilterMode",				rt_FilterMode},
	{"BuiltInFnGroupCount",		rt_BuiltInFnGroupCount},
	{"AutoFilterInfo",			rt_AutoFilterInfo},
	{"AutoFilter",				rt_AutoFilter},
	{"Scl",						rt_Scl},
	{"Setup",					rt_Setup},
	{"ScenMan",					rt_ScenMan},
	{"SCENARIO",				rt_SCENARIO},
	{"SxView",					rt_SxView},
	{"Sxvd",					rt_Sxvd},
	{"SXVI",					rt_SXVI},
	{"SxIvd",					rt_SxIvd},
	{"SXLI",					rt_SXLI},
	{"SXPI",					rt_SXPI},
	{"DocRoute",				rt_DocRoute},
	{"RecipName",				rt_RecipName},
	{"MulRk",					rt_MulRk},
	{"MulBlank",				rt_MulBlank},
	{"Mms",						rt_Mms},
	{"SXDI",					rt_SXDI},
	{"SXDB",					rt_SXDB},
	{"SXFDB",					rt_SXFDB},
	{"SXDBB",					rt_SXDBB},
	{"SXNum",					rt_SXNum},
	{"SxBool",					rt_SxBool},
	{"SxErr",					rt_SxErr},
	{"SXInt",					rt_SXInt},
	{"SXString",				rt_SXString},
	{"SXDtr",					rt_SXDtr},
	{"SxNil",					rt_SxNil},
	{"SXTbl",					rt_SXTbl},
	{"SXTBRGIITM",				rt_SXTBRGIITM},
	{"SxTbpg",					rt_SxTbpg},
	{"ObProj",					rt_ObProj},
	{"SXStreamID",				rt_SXStreamID},
	{"DBCell",					rt_DBCell},
	{"SXRng",					rt_SXRng},
	{"SxIsxoper",				rt_SxIsxoper},
	{"BookBool",				rt_BookBool},
	{"DbOrParamQry",			rt_DbOrParamQry},
	{"ScenarioProtect",			rt_ScenarioProtect},
	{"OleObjectSize",			rt_OleObjectSize},
	{"XF",						rt_XF},
	{"InterfaceHdr",			rt_InterfaceHdr},
	{"InterfaceEnd",			rt_InterfaceEnd},
	{"SXVS",					rt_SXVS},
	{"MergeCells",				rt_MergeCells},
	{"BkHim",					rt_BkHim},
	{"IMDATA",					rt_IMDATA},
	{"MsoDrawingGroup",			rt_MsoDrawingGroup},
	{"MsoDrawing",				rt_MsoDrawing},
	{"MsoDrawingSelection",		rt_MsoDrawingSelection},
	{"PhoneticInfo",			rt_PhoneticInfo},
	{"SxRule",					rt_SxRule},
	{"SXEx",					rt_SXEx},
	{"SxFilt",					rt_SxFilt},
	{"SxDXF",					rt_SxDXF},
	{"SxItm",					rt_SxItm},
	{"SxName",					rt_SxName},
	{"SxSelect",				rt_SxSelect},
	{"SXPair",					rt_SXPair},
	{"SxFmla",					rt_SxFmla},
	{"SxFormat",				rt_SxFormat},
	{"SST",						rt_SST},
	{"LabelSst",				rt_LabelSst},
	{"ExtSST",					rt_ExtSST},
	{"SXVDEx",					rt_SXVDEx},
	{"SXFormula",				rt_SXFormula},
	{"SXDBEx",					rt_SXDBEx},
	{"RRDInsDel",				rt_RRDInsDel},
	{"RRDHead",					rt_RRDHead},
	{"RRDChgCell",				rt_RRDChgCell},
	{"RRTabId",					rt_RRTabId},
	{"RRDRenSheet",				rt_RRDRenSheet},
	{"RRSort",					rt_RRSort},
	{"RRDMove",					rt_RRDMove},
	{"RRFormat",				rt_RRFormat},
	{"RRAutoFmt",				rt_RRAutoFmt},
	{"RRInsertSh",				rt_RRInsertSh},
	{"RRDMoveBegin",			rt_RRDMoveBegin},
	{"RRDMoveEnd",				rt_RRDMoveEnd},
	{"RRDInsDelBegin",			rt_RRDInsDelBegin},
	{"RRDInsDelEnd",			rt_RRDInsDelEnd},
	{"RRDConflict",				rt_RRDConflict},
	{"RRDDefName",				rt_RRDDefName},
	{"RRDRstEtxp",				rt_RRDRstEtxp},
	{"LRng",					rt_LRng},
	{"UsesELFs",				rt_UsesELFs},
	{"DSF",						rt_DSF},
	{"CUsr",					rt_CUsr},
	{"CbUsr",					rt_CbUsr},
	{"UsrInfo",					rt_UsrInfo},
	{"UsrExcl",					rt_UsrExcl},
	{"FileLock",				rt_FileLock},
	{"RRDInfo",					rt_RRDInfo},
	{"BCUsrs",					rt_BCUsrs},
	{"UsrChk",					rt_UsrChk},
	{"UserBView",				rt_UserBView},
	{"UserSViewBegin",			rt_UserSViewBegin},
	{"UserSViewBegin_Chart",	rt_UserSViewBegin_Chart},
	{"UserSViewEnd",			rt_UserSViewEnd},
	{"RRDUserView",				rt_RRDUserView},
	{"Qsi",						rt_Qsi},
	{"SupBook",					rt_SupBook},
	{"Prot4Rev",				rt_Prot4Rev},
	{"CondFmt",					rt_CondFmt},
	{"CF",						rt_CF},
	{"DVal",					rt_DVal},
	{"DConBin",					rt_DConBin},
	{"TxO",						rt_TxO},
	{"RefreshAll",				rt_RefreshAll},
	{"HLink",					rt_HLink},
	{"Lel",						rt_Lel},
	{"CodeName",				rt_CodeName},
	{"SXFDBType",				rt_SXFDBType},
	{"Prot4RevPass",			rt_Prot4RevPass},
	{"ObNoMacros",				rt_ObNoMacros},
	{"Dv",						rt_Dv},
	{"Excel9File",				rt_Excel9File},
	{"RecalcId",				rt_RecalcId},
	{"EntExU2",					rt_EntExU2},
	{"Dimensions",				rt_Dimensions},
	{"Blank",					rt_Blank},
	{"Number",					rt_Number},
	{"Label",					rt_Label},
	{"BoolErr",					rt_BoolErr},
	{"String",					rt_String},
	{"Row",						rt_Row},
	{"Index",					rt_Index},
	{"Array",					rt_Array},
	{"DefaultRowHeight",		rt_DefaultRowHeight},
	{"Table",					rt_Table},
	{"Window2",					rt_Window2},
	{"RK",						rt_RK},
	{"Style",					rt_Style},
	{"BigName",					rt_BigName},
	{"Format",					rt_Format},
	{"ContinueBigName",			rt_ContinueBigName},
	{"ShrFmla",					rt_ShrFmla},
	{"HLinkTooltip",			rt_HLinkTooltip},
	{"WebPub",					rt_WebPub},
	{"QsiSXTag",				rt_QsiSXTag},
	{"DBQueryExt",				rt_DBQueryExt},
	{"ExtString",				rt_ExtString},
	{"TxtQry",					rt_TxtQry},
	{"Qsir",					rt_Qsir},
	{"Qsif",					rt_Qsif},
	{"RRDTQSIF",				rt_RRDTQSIF},
	{"BOF",						rt_BOF},
	{"OleDbConn",				rt_OleDbConn},
	{"WOpt",					rt_WOpt},
	{"SXViewEx",				rt_SXViewEx},
	{"SXTH",					rt_SXTH},
	{"SXPIEx",					rt_SXPIEx},
	{"SXVDTEx",					rt_SXVDTEx},
	{"SXViewEx9",				rt_SXViewEx9},
	{"ContinueFrt",				rt_ContinueFrt},
	{"RealTimeData",			rt_RealTimeData},
	{"ChartFrtInfo",			rt_ChartFrtInfo},
	{"FrtWrapper",				rt_FrtWrapper},
	{"StartBlock",				rt_StartBlock},
	{"EndBlock",				rt_EndBlock},
	{"StartObject",				rt_StartObject},
	{"EndObject",				rt_EndObject},
	{"CatLab",					rt_CatLab},
	{"YMult",					rt_YMult},
	{"SXViewLink",				rt_SXViewLink},
	{"PivotChartBits",			rt_PivotChartBits},
	{"FrtFontList",				rt_FrtFontList},
	{"SheetExt",				rt_SheetExt},
	{"BookExt",					rt_BookExt},
	{"SXAddl",					rt_SXAddl},
	{"CrErr",					rt_CrErr},
	{"HFPicture",				rt_HFPicture},
	{"FeatHdr",					rt_FeatHdr},
	{"Feat",					rt_Feat},
	{"DataLabExt",				rt_DataLabExt},
	{"DataLabExtContents",		rt_DataLabExtContents},
	{"CellWatch",				rt_CellWatch},
	{"FeatHdr11",				rt_FeatHdr11},
	{"Feature11",				rt_Feature11},
	{"DropDownObjIds",			rt_DropDownObjIds},
	{"ContinueFrt11",			rt_ContinueFrt11},
	{"DConn",					rt_DConn},
	{"List12",					rt_List12},
	{"Feature12",				rt_Feature12},
	{"CondFmt12",				rt_CondFmt12},
	{"CF12",					rt_CF12},
	{"CFEx",					rt_CFEx},
	{"XFCRC",					rt_XFCRC},
	{"XFExt",					rt_XFExt},
	{"AutoFilter12",			rt_AutoFilter12},
	{"ContinueFrt12",			rt_ContinueFrt12},
	{"MDTInfo",					rt_MDTInfo},
	{"MDXStr",					rt_MDXStr},
	{"MDXTuple",				rt_MDXTuple},
	{"MDXSet",					rt_MDXSet},
	{"MDXProp",					rt_MDXProp},
	{"MDXKPI",					rt_MDXKPI},
	{"MDB",						rt_MDB},
	{"PLV",						rt_PLV},
	{"Compat12",				rt_Compat12},
	{"DXF",						rt_DXF},
	{"TableStyles",				rt_TableStyles},
	{"TableStyle",				rt_TableStyle},
	{"TableStyleElement",		rt_TableStyleElement},
	{"StyleExt",				rt_StyleExt},
	{"NamePublish",				rt_NamePublish},
	{"NameCmt",					rt_NameCmt},
	{"SortData",				rt_SortData},
	{"Theme",					rt_Theme},
	{"GUIDTypeLib",				rt_GUIDTypeLib},
	{"FnGrp12",					rt_FnGrp12},
	{"NameFnGrp12",				rt_NameFnGrp12},
	{"MTRSettings",				rt_MTRSettings},
	{"CompressPictures",		rt_CompressPictures},
	{"HeaderFooter",			rt_HeaderFooter},
	{"CrtLayout12",				rt_CrtLayout12},
	{"CrtMlFrt",				rt_CrtMlFrt},
	{"CrtMlFrtContinue",		rt_CrtMlFrtContinue},
	{"ForceFullCalculation",	rt_ForceFullCalculation},
	{"ShapePropsStream",		rt_ShapePropsStream},
	{"TextPropsStream",			rt_TextPropsStream},
	{"RichTextStream",			rt_RichTextStream},
	{"CrtLayout12A",			rt_CrtLayout12A},
	{"Units",					rt_Units},
	{"Chart",					rt_Chart},
	{"Series",					rt_Series},
	{"DataFormat",				rt_DataFormat},
	{"LineFormat",				rt_LineFormat},
	{"MarkerFormat",			rt_MarkerFormat},
	{"AreaFormat",				rt_AreaFormat},
	{"PieFormat",				rt_PieFormat},
	{"AttachedLabel",			rt_AttachedLabel},
	{"SeriesText",				rt_SeriesText},
	{"ChartFormat",				rt_ChartFormat},
	{"Legend",					rt_Legend},
	{"SeriesList",				rt_SeriesList},
	{"Bar",						rt_Bar},
	{"Line",					rt_Line},
	{"Pie",						rt_Pie},
	{"Area",					rt_Area},
	{"Scatter",					rt_Scatter},
	{"CrtLine",					rt_CrtLine},
	{"Axis",					rt_Axis},
	{"Tick",					rt_Tick},
	{"ValueRange",				rt_ValueRange},
	{"CatSerRange",				rt_CatSerRange},
	{"AxisLine",				rt_AxisLine},
	{"CrtLink",					rt_CrtLink},
	{"DefaultText",				rt_DefaultText},
	{"Text",					rt_Text},
	{"FontX",					rt_FontX},
	{"ObjectLink",				rt_ObjectLink},
	{"Frame",					rt_Frame},
	{"Begin",					rt_Begin},
	{"End",						rt_End},
	{"PlotArea",				rt_PlotArea},
	{"Chart3d",					rt_Chart3d},
	{"PicF",					rt_PicF},
	{"DropBar",					rt_DropBar},
	{"Radar",					rt_Radar},
	{"Surf",					rt_Surf},
	{"RadarArea",				rt_RadarArea},
	{"AxisParent",				rt_AxisParent},
	{"LegendException",			rt_LegendException},
	{"ShtProps",				rt_ShtProps},
	{"SerToCrt",				rt_SerToCrt},
	{"AxesUsed",				rt_AxesUsed},
	{"SBaseRef",				rt_SBaseRef},
	{"SerParent",				rt_SerParent},
	{"SerAuxTrend",				rt_SerAuxTrend},
	{"IFmtRecord",				rt_IFmtRecord},
	{"Pos",						rt_Pos},
	{"AlRuns",					rt_AlRuns},
	{"BRAI",					rt_BRAI},
	{"SerAuxErrBar",			rt_SerAuxErrBar},
	{"ClrtClient",				rt_ClrtClient},
	{"SerFmt",					rt_SerFmt},
	{"Chart3DBarShape",			rt_Chart3DBarShape},
	{"Fbi",						rt_Fbi},
	{"BopPop",					rt_BopPop},
	{"AxcExt",					rt_AxcExt},
	{"Dat",						rt_Dat},
	{"PlotGrowth",				rt_PlotGrowth},
	{"SIIndex",					rt_SIIndex},
	{"GelFrame",				rt_GelFrame},
	{"BopPopCustom",			rt_BopPopCustom},
	{"Fbi2",					rt_Fbi2},
	{"CommentText",				rt_CommentText},
	{"ANY_TYPE",				rt_ANY_TYPE}
};

// Called only once per process
CFRecordType::CFRecordType()
{
	for(int i = 0; i < sizeof(CFRecordTypeValues) / sizeof(CFRecordTypeValue); ++i)
	{
		CFRecordTypeValue val(CFRecordTypeValues[i]);
		all_types_id.insert(std::make_pair(val.id, val.name));
		all_types_string.insert(std::make_pair(val.name, val.id));
	}
}


CFRecordType& CFRecordType::getInst()
{
	static CFRecordType inst;
	return inst;
}


const CFRecordType::TypeId CFRecordType::getIdByString(const TypeString& str)
{
	return getInst().all_types_string[str];
}



const CFRecordType::TypeString& CFRecordType::getStringById(const TypeId id)
{
	return getInst().all_types_id[id];
}


// Whether the specified type is one of Continue records
const bool CFRecordType::isContinue(const TypeId type)
{
	return rt_Continue == type || rt_ContinueBigName == type || rt_ContinueFrt == type || 
		rt_ContinueFrt11 == type || rt_ContinueFrt12 == type;
}


} // namespace XLS

