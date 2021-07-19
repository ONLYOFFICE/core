﻿/*
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

#include "../DocxFormat/WritingElement.h"
#include "../../../../DesktopEditor/common/StringBuilder.h"

namespace OOX
{
namespace Spreadsheet
{

#define WritingStringCommon(start, func, val, end) \
		writer.WriteString(start); \
		writer.func(val); \
		writer.WriteString(end);
//#define WritingStringCommonDouble(start, val, end) \
//		writer.WriteString(start); \
//		writer.AddDouble(val, -1); \
//		writer.WriteString(end);
#define WritingStringCommonDouble(start, val, end) \
		writer.WriteString(start); \
		writer.WriteString(XmlUtils::DoubleToString(val)); \
		writer.WriteString(end);

#define WritingValNode(ns, name, val) \
		writer.StartNodeWithNS(ns, name); \
		writer.StartAttributes(); \
		writer.WriteAttribute(L"val", val); \
		writer.EndAttributesAndNode();
#define WritingValNodeEncodeXml(ns, name, val) \
		writer.StartNodeWithNS(ns, name); \
		writer.StartAttributes(); \
		writer.WriteAttributeEncodeXml(L"val", val); \
		writer.EndAttributesAndNode();
#define WritingValNodeIf(ns, name, cond, val) \
		writer.StartNodeWithNS(ns, name); \
		writer.StartAttributes(); \
		if (cond) \
		{ \
			writer.WriteAttribute(L"val", val); \
		} \
		writer.EndAttributesAndNode();

#define WritingStringValAttr(name, func, val) \
		WritingStringCommon(L"<" name L" val=\"", func, val, L"\"/>")
#define WritingStringValAttrString(name, val) \
		WritingStringValAttr(name, WriteString, val)
#define WritingStringValAttrEncodeXmlString(name, val) \
		WritingStringValAttr(name, WriteEncodeXmlString, val)
#define WritingStringValAttrInt(name, val) \
		WritingStringValAttr(name, AddInt, val)
#define WritingStringValAttrDouble(name, val) \
		WritingStringCommonDouble(L"<" name L" val=\"", val, L"\"/>")

#define WritingStringVal(name, func, val) \
		WritingStringCommon(L"<" name L">", func, val, L"</" name L">")
#define WritingStringValString(name, val) \
		WritingStringVal(name, WriteString, val)
#define WritingStringValEncodeXmlString(name, val) \
		WritingStringVal(name, WriteEncodeXmlString, val)
#define WritingStringValInt(name, val) \
		WritingStringVal(name, AddInt, val)
#define WritingStringValInt64(name, val) \
		WritingStringVal(name, AddInt64, val)
#define WritingStringValDouble(name, val) \
		WritingStringCommonDouble(L"<" name L">", val, L"</" name L">")

#define WritingStringAttr(name, func, val) \
		WritingStringCommon(L" " name L"=\"", func, val, L"\"")
#define WritingStringAttrString(name, val) \
		WritingStringAttr(name, WriteString, val)
#define WritingStringAttrEncodeXmlString(name, val) \
		WritingStringAttr(name, WriteEncodeXmlString, val)
#define WritingStringAttrInt(name, val) \
		WritingStringAttr(name, AddInt, val)
#define WritingStringAttrDouble(name, val) \
		WritingStringCommonDouble(L" " name L"=\"", val, L"\"")

#define WritingStringNullableAttr(name, func, elem, val) \
		if(elem.IsInit()) \
		{ \
			WritingStringAttr(name, func, val) \
		}
#define WritingStringNullableAttrString(name, elem, val) \
		WritingStringNullableAttr(name, WriteString, elem, val)
#define WritingStringNullableAttrEncodeXmlString(name, elem, val) \
		WritingStringNullableAttr(name, WriteEncodeXmlString, elem, val)
#define WritingStringNullableAttrEncodeXmlStringHHHH(name, elem, val) \
		WritingStringNullableAttr(name, WriteEncodeXmlStringHHHH, elem, val)
#define WritingStringNullableAttrInt(name, elem, val) \
		WritingStringNullableAttr(name, AddInt, elem, val)
#define WritingStringNullableAttrUInt(name, elem, val) \
		WritingStringNullableAttr(name, AddUInt, elem, val)
#define WritingStringNullableAttrInt64(name, elem, val) \
		WritingStringNullableAttr(name, AddInt64, elem, val)
#define WritingStringNullableAttrDouble(name, elem, val) \
		if(elem.IsInit()) \
		{ \
			WritingStringAttrDouble(name, val) \
		}
#define WritingStringNullableAttrBool(name, elem) \
		WritingStringNullableAttrString(name, elem, elem->ToString3(SimpleTypes::onofftostring1))

#define WritingStringNullableAttrBool2(name, elem) \
		WritingStringNullableAttrString(name, elem, *elem ? L"1": L"0")

#define WritingStringNullableAttrInt2(name, elem) \
		WritingStringNullableAttrInt(name, elem, *elem)

#define WritingStringNullableAttrDouble2(name, elem) \
		WritingStringNullableAttrDouble(name, elem, *elem)

#define WritingStringNullableAttrEncodeXmlString2(name, elem) \
		WritingStringNullableAttrEncodeXmlString(name, elem, *elem)

#define WritingNullable(elem, action) \
		if(elem.IsInit()) \
		{ \
			action \
		}

	const double c_ag_Inch_to_MM	= 25.4;
	const double c_ag_1pxWidth		= 25.4 / 96;

	class WritingElement : public OOX::WritingElement
	{
	public:
		WritingElement(OOX::Document *pMain = NULL) : OOX::WritingElement(pMain) {}
        virtual EElementType    getType()   const
		{
			return OOX::et_x_Unknown;
		}
		virtual void toXML(NSStringUtils::CStringBuilder& writer) const = 0;
	};

	template<typename ElemType = WritingElement>
	class WritingElementWithChilds : public WritingElement
	{
	public:
		WritingElementWithChilds(OOX::Document *pMain = NULL) : WritingElement(pMain) {}
		
		virtual ~WritingElementWithChilds() 
		{
			ClearItems();
		}
		virtual void ClearItems()
		{
            for ( size_t i = 0; i < m_arrItems.size(); ++i)
            {
                if ( m_arrItems[i] ) delete m_arrItems[i];
            }
			m_arrItems.clear();
		}
        std::vector<ElemType *>  m_arrItems;
	};
}
}
namespace XLSB
{
	typedef enum CF_RECORD_TYPE
	{
		rt_ROW_HDR =								0,
		rt_CELL_BLANK =								1,
		rt_CELL_RK =								2,
		rt_CELL_ERROR =								3,
		rt_CELL_BOOL =								4,
		rt_CELL_REAL =								5,
		rt_CELL_ST =								6,
		rt_CELL_ISST =								7,
		rt_FMLA_STRING =							8,
		rt_FMLA_NUM =								9,
		rt_FMLA_BOOL =								10,
		rt_FMLA_ERROR =								11,
		rt_SST_ITEM =								19,
		rt_PCDI_MISSING =							20,
		rt_PCDI_NUMBER =							21,
		rt_PCDI_BOOLEAN =							22,
		rt_PCDI_ERROR =								23,
		rt_PCDI_STRING =							24,
		rt_PCDI_DATETIME =							25,
		rt_PCDI_INDEX =								26,
		rt_PCDIA_MISSING =							27,
		rt_PCDIA_NUMBER =							28,
		rt_PCDIA_BOOLEAN =							29,
		rt_PCDIA_ERROR =							30,
		rt_PCDIA_STRING =							31,
		rt_PCDIA_DATETIME =							32,
		rt_PCR_RECORD =								33,
		rt_PCR_RECORD_DT =							34,
		rt_FRT_BEGIN =								35,
		rt_FRT_END =								36,
		rt_AC_BEGIN =								37,
		rt_AC_END =									38,
		rt_NAME =									39,
		rt_INDEX_ROW_BLOCK =						40,
		rt_INDEX_BLOCK =							42,
		rt_FONT =									43,
		rt_FMT =									44,
		rt_FILL =									45,
		rt_BORDER =									46,
		rt_XF =										47,
		rt_STYLE =									48,
		rt_CELL_META =								49,
		rt_VALUE_META =								50,
		rt_MDB =									51,
		rt_BEGIN_FMD =								52,
		rt_END_FMD =								53,
		rt_BEGIN_MDX =								54,
		rt_END_MDX =								55,
		rt_BEGIN_MDX_TUPLE =						56,
		rt_END_MDX_TUPLE =							57,
		rt_MDX_MBR_ISTR =							58,
		rt_STR =									59,
		rt_COL_INFO =								60,
		rt_CELL_R_STRING =							62,
		rt_D_VAL =									64,
		rt_SXVCELL_NUM =							65,
		rt_SXVCELL_STR =							66,
		rt_SXVCELL_BOOL =							67,
		rt_SXVCELL_ERR =							68,
		rt_SXVCELL_DATE =							69,
		rt_SXVCELL_NIL =							70,
		rt_FILE_VERSION =							128,
		rt_BEGIN_SHEET =							129,
		rt_END_SHEET =								130,
		rt_BEGIN_BOOK =								131,
		rt_END_BOOK =								132,
		rt_BEGIN_WS_VIEWS =							133,
		rt_END_WS_VIEWS =							134,
		rt_BEGIN_BOOK_VIEWS =						135,
		rt_END_BOOK_VIEWS =							136,
		rt_BEGIN_WS_VIEW =							137,
		rt_END_WS_VIEW =							138,
		rt_BEGIN_CS_VIEWS =							139,
		rt_END_CS_VIEWS =							140,
		rt_BEGIN_CS_VIEW =							141,
		rt_END_CS_VIEW =							142,
		rt_BEGIN_BUNDLE_SHS =						143,
		rt_END_BUNDLE_SHS =							144,
		rt_BEGIN_SHEET_DATA =						145,
		rt_END_SHEET_DATA =							146,
		rt_WS_PROP =								147,
		rt_WS_DIM =									148,
		rt_PANE =									151,
		rt_SEL =									152,
		rt_WB_PROP =								153,
		rt_WB_FACTOID =								154,
		rt_FILE_RECOVER =							155,
		rt_BUNDLE_SH =								156,
		rt_CALC_PROP =								157,
		rt_BOOK_VIEW =								158,
		rt_BEGIN_SST =								159,
		rt_END_SST =								160,
		rt_BEGIN_A_FILTER =							161,
		rt_END_A_FILTER =							162,
		rt_BEGIN_FILTER_COLUMN =					163,
		rt_END_FILTER_COLUMN =						164,
		rt_BEGIN_FILTERS =							165,
		rt_END_FILTERS =							166,
		rt_FILTER =									167,
		rt_COLOR_FILTER =							168,
		rt_ICON_FILTER =							169,
		rt_TOP10_FILTER =							170,
		rt_DYNAMIC_FILTER =							171,
		rt_BEGIN_CUSTOM_FILTERS =					172,
		rt_END_CUSTOM_FILTERS =						173,
		rt_CUSTOM_FILTER =							174,
		rt_A_FILTER_DATE_GROUP_ITEM =				175,
		rt_MERGE_CELL =								176,
		rt_BEGIN_MERGE_CELLS =						177,
		rt_END_MERGE_CELLS =						178,
		rt_BEGIN_PIVOT_CACHE_DEF =					179,
		rt_END_PIVOT_CACHE_DEF =					180,
		rt_BEGIN_PCD_FIELDS =						181,
		rt_END_PCD_FIELDS =							182,
		rt_BEGIN_PCD_FIELD =						183,
		rt_END_PCD_FIELD =							184,
		rt_BEGIN_PCD_SOURCE =						185,
		rt_END_PCD_SOURCE =							186,
		rt_BEGIN_PCDS_RANGE =						187,
		rt_END_PCDS_RANGE =							188,
		rt_BEGIN_PCDF_ATBL =						189,
		rt_END_PCDF_ATBL =							190,
		rt_BEGIN_PCDI_RUN =							191,
		rt_END_PCDI_RUN =							192,
		rt_END_PIVOT_CACHE_RECORDS =				194,
		rt_BEGIN_PCD_HIERARCHIES =					195,
		rt_END_PCD_HIERARCHIES =					196,
		rt_BEGIN_PCD_HIERARCHY =					197,
		rt_END_PCD_HIERARCHY =						198,
		rt_BEGIN_PCDH_FIELDS_USAGE =				199,
		rt_END_PCDH_FIELDS_USAGE =					200,
		rt_BEGIN_EXT_CONNECTION =					201,
		rt_END_EXT_CONNECTION =						202,
		rt_BEGIN_EC_DB_PROPS =						203,
		rt_END_EC_DB_PROPS =						204,
		rt_BEGIN_EC_OLAP_PROPS =					205,
		rt_END_EC_OLAP_PROPS =						206,
		rt_BEGIN_PCDS_CONSOL =						207,
		rt_END_PCDS_CONSOL =						208,
		rt_BEGIN_PCDSC_PAGES =						209,
		rt_END_PCDSC_PAGES =						210,
		rt_BEGIN_PCDSC_PAGE =						211,
		rt_END_PCDSC_PAGE =							212,
		rt_BEGIN_PCDSCP_ITEM =						213,
		rt_END_PCDSCP_ITEM =						214,
		rt_BEGIN_PCDSC_SETS =						215,
		rt_END_PCDSC_SETS =							216,
		rt_BEGIN_PCDSC_SET =						217,
		rt_END_PCDSC_SET =							218,
		rt_BEGIN_PCDF_GROUP =						219,
		rt_END_PCDF_GROUP =							220,
		rt_BEGIN_PCDFG_ITEMS =						221,
		rt_END_PCDFG_ITEMS =						222,
		rt_BEGIN_PCDFG_RANGE =						223,
		rt_END_PCDFG_RANGE =						224,
		rt_BEGIN_PCDFG_DISCRETE =					225,
		rt_END_PCDFG_DISCRETE =						226,
		rt_END_PCDSD_TUPLE_CACHE =					228,
		rt_BEGIN_PCDSDTC_ENTRIES =					229,
		rt_END_PCDSDTC_ENTRIES =					230,
		rt_END_PCDSDTCE_MEMBER =					234,
		rt_BEGIN_PCDSDTC_QUERIES =					235,
		rt_END_PCDSDTC_QUERIES =					236,
		rt_BEGIN_PCDSDTC_QUERY =					237,
		rt_END_PCDSDTC_QUERY =						238,
		rt_BEGIN_PCDSDTC_SETS =						239,
		rt_END_PCDSDTC_SETS =						240,
		rt_BEGIN_PCDSDTC_SET =						241,
		rt_END_PCDSDTC_SET =						242,
		rt_BEGIN_PCD_CALC_ITEMS =					243,
		rt_END_PCD_CALC_ITEMS =						244,
		rt_BEGIN_PCD_CALC_ITEM =					245,
		rt_END_PCD_CALC_ITEM =						246,
		rt_BEGIN_P_RULE =							247,
		rt_END_P_RULE =								248,
		rt_BEGIN_PR_FILTERS =						249,
		rt_END_PR_FILTERS =							250,
		rt_BEGIN_PR_FILTER =						251,
		rt_END_PR_FILTER =							252,
		rt_BEGIN_P_NAMES =							253,
		rt_END_P_NAMES =							254,
		rt_BEGIN_P_NAME =							255,
		rt_END_P_NAME =								256,
		rt_BEGIN_PN_PAIRS =							257,
		rt_END_PN_PAIRS =							258,
		rt_BEGIN_PN_PAIR =							259,
		rt_END_PN_PAIR =							260,
		rt_BEGIN_EC_WEB_PROPS =						261,
		rt_END_EC_WEB_PROPS =						262,
		rt_BEGIN_EC_WP_TABLES =						263,
		rt_END_ECWP_TABLES =						264,
		rt_BEGIN_EC_PARAMS =						265,
		rt_END_EC_PARAMS =							266,
		rt_BEGIN_EC_PARAM =							267,
		rt_END_EC_PARAM =							268,
		rt_BEGIN_PCDKPIS =							269,
		rt_END_PCDKPIS =							270,
		rt_BEGIN_PCDKPI =							271,
		rt_END_PCDKPI =								272,
		rt_BEGIN_DIMS =								273,
		rt_END_DIMS =								274,
		rt_BEGIN_DIM =								275,
		rt_END_DIM =								276,
		rt_INDEX_PART_END =							277,
		rt_BEGIN_STYLE_SHEET =						278,
		rt_END_STYLE_SHEET =						279,
		rt_BEGIN_SX_VIEW =							280,
		rt_END_SXVI =								281,
		rt_BEGIN_SXVI =								282,
		rt_BEGIN_SXVIS =							283,
		rt_END_SXVIS =								284,
		rt_BEGIN_SXVD =								285,
		rt_END_SXVD =								286,
		rt_BEGIN_SXVDS =							287,
		rt_END_SXVDS =								288,
		rt_BEGIN_SXPI =								289,
		rt_END_SXPI =								290,
		rt_BEGIN_SXPIS =							291,
		rt_END_SXPIS =								292,
		rt_BEGIN_SXDI =								293,
		rt_END_SXDI =								294,
		rt_BEGIN_SXDIS =							295,
		rt_END_SXDIS =								296,
		rt_BEGIN_SXLI =								297,
		rt_END_SXLI =								298,
		rt_BEGIN_SXLI_RWS =							299,
		rt_END_SXLI_RWS =							300,
		rt_BEGIN_SXLI_COLS =						301,
		rt_END_SXLI_COLS =							302,
		rt_BEGIN_SX_FORMAT =						303,
		rt_END_SX_FORMAT =							304,
		rt_BEGIN_SX_FORMATS =						305,
		rt_END_SX_FORMATS =							306,
		rt_BEGIN_SX_SELECT =						307,
		rt_END_SX_SELECT =							308,
		rt_BEGIN_ISXVD_RWS =						309,
		rt_END_ISXVD_RWS =							310,
		rt_BEGIN_ISXVD_COLS =						311,
		rt_END_ISXVD_COLS =							312,
		rt_END_SX_LOCATION =						313,
		rt_BEGIN_SX_LOCATION =						314,
		rt_END_SX_VIEW =							315,
		rt_BEGIN_SXTHS =							316,
		rt_END_SXTHS =								317,
		rt_BEGIN_SXTH =								318,
		rt_END_SXTH =								319,
		rt_BEGIN_ISXTH_RWS =						320,
		rt_END_ISXTH_RWS =							321,
		rt_BEGIN_ISXTH_COLS =						322,
		rt_END_ISXTH_COLS =							323,
		rt_BEGIN_SXTDMPS =							324,
		rt_END_SXTDMPS =							325,
		rt_BEGIN_SXTDMP =							326,
		rt_END_SXTDMP =								327,
		rt_BEGIN_SXTH_ITEMS =						328,
		rt_END_SXTH_ITEMS =							329,
		rt_BEGIN_SXTH_ITEM =						330,
		rt_END_SXTH_ITEM =							331,
		rt_BEGIN_METADATA =							332,
		rt_END_METADATA =							333,
		rt_BEGIN_ESMDTINFO =						334,
		rt_MDTINFO =								335,
		rt_END_ESMDTINFO =							336,
		rt_BEGIN_ESMDB =							337,
		rt_END_ESMDB =								338,
		rt_BEGIN_ESFMD =							339,
		rt_END_ESFMD =								340,
		rt_BEGIN_SINGLE_CELLS =						341,
		rt_END_SINGLE_CELLS =						342,
		rt_BEGIN_LIST =								343,
		rt_END_LIST =								344,
		rt_BEGIN_LIST_COLS =						345,
		rt_END_LIST_COLS =							346,
		rt_BEGIN_LIST_COL =							347,
		rt_END_LIST_COL =							348,
		rt_BEGIN_LIST_XML_C_PR =					349,
		rt_END_LIST_XML_C_PR =						350,
		rt_LIST_CC_FMLA =							351,
		rt_LIST_TR_FMLA =							352,
		rt_BEGIN_EXTERNALS =						353,
		rt_END_EXTERNALS =							354,
		rt_SUP_BOOK_SRC =							355,
		rt_SUP_SELF =								357,
		rt_SUP_SAME =								358,
		rt_SUP_TABS =								359,
		rt_BEGIN_SUP_BOOK =							360,
		rt_PLACEHOLDER_NAME =						361,
		rt_EXTERN_SHEET =							362,
		rt_EXTERN_TABLE_START =						363,
		rt_EXTERN_TABLE_END =						364,
		rt_EXTERN_ROW_HDR =							366,
		rt_EXTERN_CELL_BLANK =						367,
		rt_EXTERN_CELL_REAL =						368,
		rt_EXTERN_CELL_BOOL =						369,
		rt_EXTERN_CELL_ERROR =						370,
		rt_EXTERN_CELL_STRING =						371,
		rt_BEGIN_ESMDX =							372,
		rt_END_ESMDX =								373,
		rt_BEGIN_MDX_SET =							374,
		rt_END_MDX_SET =							375,
		rt_BEGIN_MDX_MBR_PROP =						376,
		rt_END_MDX_MBR_PROP =						377,
		rt_BEGIN_MDX_KPI =							378,
		rt_END_MDX_KPI =							379,
		rt_BEGIN_ESSTR =							380,
		rt_END_ESSTR =								381,
		rt_BEGIN_PRF_ITEM =							382,
		rt_END_PRF_ITEM =							383,
		rt_BEGIN_PIVOT_CACHE_IDS =					384,
		rt_END_PIVOT_CACHE_IDS =					385,
		rt_BEGIN_PIVOT_CACHE_ID =					386,
		rt_END_PIVOT_CACHE_ID =						387,
		rt_BEGIN_ISXVIS =							388,
		rt_END_ISXVIS =								389,
		rt_BEGIN_COL_INFOS =						390,
		rt_END_COL_INFOS =							391,
		rt_BEGIN_RW_BRK =							392,
		rt_END_RW_BRK =								393,
		rt_BEGIN_COL_BRK =							394,
		rt_END_COL_BRK =							395,
		rt_BRK =									396,
		rt_USER_BOOK_VIEW =							397,
		rt_INFO =									398,
		rt_C_USR =									399,
		rt_USR =									400,
		rt_BEGIN_USERS =							401,
		rt_EOF =									403,
		rt_UCR =									404,
		rt_RR_INS_DEL =								405,
		rt_RR_END_INS_DEL =							406,
		rt_RR_MOVE =								407,
		rt_RR_END_MOVE =							408,
		rt_RR_CHG_CELL =							409,
		rt_RR_END_CHG_CELL =						410,
		rt_RR_HEADER =								411,
		rt_RR_USER_VIEW =							412,
		rt_RR_REN_SHEET =							413,
		rt_RR_INSERT_SH =							414,
		rt_RR_DEF_NAME =							415,
		rt_RR_NOTE =								416,
		rt_RR_CONFLICT =							417,
		rt_RRTQSIF =								418,
		rt_RR_FORMAT =								419,
		rt_RR_END_FORMAT =							420,
		rt_RR_AUTO_FMT =							421,
		rt_BEGIN_USER_SH_VIEWS =					422,
		rt_BEGIN_USER_SH_VIEW =						423,
		rt_END_USER_SH_VIEW =						424,
		rt_END_USER_SH_VIEWS =						425,
		rt_ARR_FMLA =								426,
		rt_SHR_FMLA =								427,
		rt_TABLE =									428,
		rt_BEGIN_EXT_CONNECTIONS =					429,
		rt_END_EXT_CONNECTIONS =					430,
		rt_BEGIN_PCD_CALC_MEMS =					431,
		rt_END_PCD_CALC_MEMS =						432,
		rt_BEGIN_PCD_CALC_MEM =						433,
		rt_END_PCD_CALC_MEM =						434,
		rt_BEGIN_PCDHG_LEVELS =						435,
		rt_END_PCDHG_LEVELS =						436,
		rt_BEGIN_PCDHG_LEVEL =						437,
		rt_END_PCDHG_LEVEL =						438,
		rt_BEGIN_PCDHGL_GROUPS =					439,
		rt_END_PCDHGL_GROUPS =						440,
		rt_BEGIN_PCDHGL_GROUP =						441,
		rt_END_PCDHGL_GROUP =						442,
		rt_END_PCDHGLG_MEMBERS =					444,
		rt_BEGIN_PCDHGLG_MEMBER =					445,
		rt_END_PCDHGLG_MEMBER =						446,
		rt_BEGIN_QSI =								447,
		rt_END_QSI =								448,
		rt_BEGIN_QSIR =								449,
		rt_END_QSIR =								450,
		rt_BEGIN_DELETED_NAMES =					451,
		rt_END_DELETED_NAMES =						452,
		rt_BEGIN_DELETED_NAME =						453,
		rt_END_DELETED_NAME =						454,
		rt_BEGIN_QSIFS =							455,
		rt_END_QSIFS =								456,
		rt_BEGIN_QSIF =								457,
		rt_END_QSIF =								458,
		rt_BEGIN_AUTO_SORT_SCOPE =					459,
		rt_END_AUTO_SORT_SCOPE =					460,
		rt_BEGIN_CF_RULE =							463,
		rt_END_CF_RULE =							464,
		rt_BEGIN_ICON_SET =							465,
		rt_END_ICON_SET =							466,
		rt_BEGIN_DATABAR =							467,
		rt_END_DATABAR =							468,
		rt_BEGIN_COLOR_SCALE =						469,
		rt_END_COLOR_SCALE =						470,
		rt_CFVO =									471,
		rt_EXTERN_VALUE_META =						472,
		rt_BEGIN_COLOR_PALETTE =					473,
		rt_END_COLOR_PALETTE =						474,
		rt_INDEXED_COLOR =							475,
		rt_MARGINS =								476,
		rt_PRINT_OPTIONS =							477,
		rt_PAGE_SETUP =								478,
		rt_BEGIN_HEADER_FOOTER =					479,
		rt_END_HEADER_FOOTER =						480,
		rt_BEGIN_SX_CRT_FORMAT =					481,
		rt_END_SX_CRT_FORMAT =						482,
		rt_BEGIN_SX_CRT_FORMATS =					483,
		rt_END_SX_CRT_FORMATS =						484,
		rt_WS_FMT_INFO =							485,
		rt_BEGIN_MGS =								486,
		rt_END_MGS =								487,
		rt_BEGIN_MG_MAPS =							488,
		rt_END_MG_MAPS =							489,
		rt_BEGIN_MG =								490,
		rt_END_MG =									491,
		rt_BEGIN_MAP =								492,
		rt_END_MAP =								493,
		rt_H_LINK =									494,
		rt_BEGIN_D_CON =							495,
		rt_END_D_CON =								496,
		rt_BEGIN_D_REFS =							497,
		rt_END_D_REFS =								498,
		rt_D_REF =									499,
		rt_BEGIN_SCEN_MAN =							500,
		rt_END_SCEN_MAN =							501,
		rt_BEGIN_SCT =								502,
		rt_END_SCT =								503,
		rt_SLC =									504,
		rt_BEGIN_DXFS =								505,
		rt_END_DXFS =								506,
		rt_DXF =									507,
		rt_BEGIN_TABLE_STYLES =						508,
		rt_END_TABLE_STYLES =						509,
		rt_BEGIN_TABLE_STYLE =						510,
		rt_END_TABLE_STYLE =						511,
		rt_TABLE_STYLE_ELEMENT =					512,
		rt_TABLE_STYLE_CLIENT =						513,
		rt_BEGIN_VOL_DEPS =							514,
		rt_END_VOL_DEPS =							515,
		rt_BEGIN_VOL_TYPE =							516,
		rt_END_VOL_TYPE =							517,
		rt_BEGIN_VOL_MAIN =							518,
		rt_END_VOL_MAIN =							519,
		rt_BEGIN_VOL_TOPIC =						520,
		rt_END_VOL_TOPIC =							521,
		rt_VOL_SUBTOPIC =							522,
		rt_VOL_REF =								523,
		rt_VOL_NUM =								524,
		rt_VOL_ERR =								525,
		rt_VOL_STR =								526,
		rt_VOL_BOOL =								527,
		rt_BEGIN_SORT_STATE =						530,
		rt_END_SORT_STATE =							531,
		rt_BEGIN_SORT_COND =						532,
		rt_END_SORT_COND =							533,
		rt_BOOK_PROTECTION =						534,
		rt_SHEET_PROTECTION =						535,
		rt_RANGE_PROTECTION =						536,
		rt_PHONETIC_INFO =							537,
		rt_BEGIN_EC_TXT_WIZ =						538,
		rt_END_EC_TXT_WIZ =							539,
		rt_BEGIN_ECTW_FLD_INFO_LST =				540,
		rt_END_ECTW_FLD_INFO_LST =					541,
		rt_BEGIN_EC_TW_FLD_INFO =					542,
		rt_FILE_SHARING =							548,
		rt_OLE_SIZE =								549,
		rt_DRAWING =								550,
		rt_LEGACY_DRAWING =							551,
		rt_LEGACY_DRAWING_HF =						552,
		rt_WEB_OPT =								553,
		rt_BEGIN_WEB_PUB_ITEMS =					554,
		rt_END_WEB_PUB_ITEMS =						555,
		rt_BEGIN_WEB_PUB_ITEM =						556,
		rt_END_WEB_PUB_ITEM =						557,
		rt_BEGIN_SX_COND_FMT =						558,
		rt_END_SX_COND_FMT =						559,
		rt_BEGIN_SX_COND_FMTS =						560,
		rt_END_SX_COND_FMTS =						561,
		rt_BK_HIM =									562,
		rt_COLOR =									564,
		rt_BEGIN_INDEXED_COLORS =					565,
		rt_END_INDEXED_COLORS =						566,
		rt_BEGIN_MRU_COLORS =						569,
		rt_END_MRU_COLORS =							570,
		rt_MRU_COLOR =								572,
		rt_BEGIN_D_VALS =							573,
		rt_END_D_VALS =								574,
		rt_SUP_NAME_START =							577,
		rt_SUP_NAME_VALUE_START =					578,
		rt_SUP_NAME_VALUE_END =						579,
		rt_SUP_NAME_NUM =							580,
		rt_SUP_NAME_ERR =							581,
		rt_SUP_NAME_ST =							582,
		rt_SUP_NAME_NIL =							583,
		rt_SUP_NAME_BOOL =							584,
		rt_SUP_NAME_FMLA =							585,
		rt_SUP_NAME_BITS =							586,
		rt_SUP_NAME_END =							587,
		rt_END_SUP_BOOK =							588,
		rt_CELL_SMART_TAG_PROPERTY =				589,
		rt_BEGIN_CELL_SMART_TAG =					590,
		rt_END_CELL_SMART_TAG =						591,
		rt_BEGIN_CELL_SMART_TAGS =					592,
		rt_END_CELL_SMART_TAGS =					593,
		rt_BEGIN_SMART_TAGS =						594,
		rt_END_SMART_TAGS =							595,
		rt_SMART_TAG_TYPE =							596,
		rt_BEGIN_SMART_TAG_TYPES =					597,
		rt_END_SMART_TAG_TYPES =					598,
		rt_BEGIN_SX_FILTERS =						599,
		rt_END_SX_FILTERS =							600,
		rt_BEGIN_SXFILTER =							601,
		rt_END_SX_FILTER =							602,
		rt_BEGIN_FILLS =							603,
		rt_END_FILLS =								604,
		rt_BEGIN_CELL_WATCHES =						605,
		rt_END_CELL_WATCHES =						606,
		rt_CELL_WATCH =								607,
		rt_BEGIN_CR_ERRS =							608,
		rt_END_CR_ERRS =							609,
		rt_CRASH_REC_ERR =							610,
		rt_BEGIN_FONTS =							611,
		rt_END_FONTS =								612,
		rt_BEGIN_BORDERS =							613,
		rt_END_BORDERS =							614,
		rt_BEGIN_FMTS =								615,
		rt_END_FMTS =								616,
		rt_BEGIN_CELL_XFS =							617,
		rt_END_CELL_XFS =							618,
		rt_BEGIN_STYLES =							619,
		rt_END_STYLES =								620,
		rt_BIG_NAME =								625,
		rt_BEGIN_CELL_STYLE_XFS =					626,
		rt_END_CELL_STYLE_XFS =						627,
		rt_BEGIN_COMMENTS =							628,
		rt_END_COMMENTS =							629,
		rt_BEGIN_COMMENT_AUTHORS =					630,
		rt_END_COMMENT_AUTHORS =					631,
		rt_COMMENT_AUTHOR =							632,
		rt_BEGIN_COMMENT_LIST =						633,
		rt_END_COMMENT_LIST =						634,
		rt_BEGIN_COMMENT =							635,
		rt_END_COMMENT =							636,
		rt_COMMENT_TEXT =							637,
		rt_BEGIN_OLE_OBJECTS =						638,
		rt_OLE_OBJECT =								639,
		rt_END_OLE_OBJECTS =						640,
		rt_BEGIN_SXRULES =							641,
		rt_END_SX_RULES =							642,
		rt_BEGIN_ACTIVE_X_CONTROLS =				643,
		rt_ACTIVE_X =								644,
		rt_END_ACTIVE_X_CONTROLS =					645,
		rt_BEGIN_PCDSDTCE_MEMBERS_SORT_BY =			646,
		rt_BEGIN_CELL_IGNORE_ECS =					648,
		rt_CELL_IGNORE_EC =							649,
		rt_END_CELL_IGNORE_ECS =					650,
		rt_CS_PROP =								651,
		rt_CS_PAGE_SETUP =							652,
		rt_BEGIN_USER_CS_VIEWS =					653,
		rt_END_USER_CS_VIEWS =						654,
		rt_BEGIN_USER_CS_VIEW =						655,
		rt_END_USER_CS_VIEW =						656,
		rt_BEGIN_PCD_SFCI_ENTRIES =					657,
		rt_END_PCDSFCI_ENTRIES =					658,
		rt_PCDSFCI_ENTRY =							659,
		rt_BEGIN_LIST_PARTS =						660,
		rt_LIST_PART =								661,
		rt_END_LIST_PARTS =							662,
		rt_SHEET_CALC_PROP =						663,
		rt_BEGIN_FN_GROUP =							664,
		rt_FN_GROUP =								665,
		rt_END_FN_GROUP =							666,
		rt_SUP_ADDIN =								667,
		rt_SXTDMP_ORDER =							668,
		rt_CS_PROTECTION =							669,
		rt_BEGIN_WS_SORT_MAP =						671,
		rt_END_WS_SORT_MAP =						672,
		rt_BEGIN_RR_SORT =							673,
		rt_END_RR_SORT =							674,
		rt_RR_SORT_ITEM =							675,
		rt_FILE_SHARING_ISO =						676,
		rt_BOOK_PROTECTION_ISO =					677,
		rt_SHEET_PROTECTION_ISO =					678,
		rt_CS_PROTECTION_ISO =						679,
		rt_RANGE_PROTECTION_ISO =					680,
		rt_RW_DESCENT =								1024,
		rt_KNOWN_FONTS =							1025,
		rt_BEGIN_SX_TUPLE_SET =						1026,
		rt_END_SX_TUPLE_SET =						1027,
		rt_END_SX_TUPLE_SET_HEADER =				1029,
		rt_BEGIN_SX_TUPLE_SET_DATA =				1031,
		rt_END_SX_TUPLE_SET_DATA =					1032,
		rt_BEGIN_SX_TUPLE_SET_ROW =					1033,
		rt_END_SX_TUPLE_SET_ROW =					1034,
		rt_SX_TUPLE_SET_ROW_ITEM =					1035,
		rt_NAME_EXT =								1036,
		rt_PCDH14 =									1037,
		rt_BEGIN_PCD_CALC_MEM14 =					1038,
		rt_END_PCD_CALC_MEM14 =						1039,
		rt_SXTH14 =									1040,
		rt_BEGIN_SPARKLINE_GROUP =					1041,
		rt_END_SPARKLINE_GROUP =					1042,
		rt_SPARKLINE =								1043,
		rt_SXDI14 =									1044,
		rt_WS_FMT_INFO_EX14 =						1045,
		rt_BEGIN_CF_RULE14 =						1048,
		rt_END_CF_RULE14 =							1049,
		rt_CFVO14 =									1050,
		rt_BEGIN_DATABAR14 =						1051,
		rt_BEGIN_ICON_SET14 =						1052,
		rt_D_VAL14 =								1053,
		rt_BEGIN_D_VALS14 =							1054,
		rt_COLOR14 =								1055,
		rt_BEGIN_SPARKLINES =						1056,
		rt_END_SPARKLINES =							1057,
		rt_BEGIN_SPARKLINE_GROUPS =					1058,
		rt_END_SPARKLINE_GROUPS =					1059,
		rt_SXVD14 =									1061,
		rt_BEGIN_SX_VIEW14 =						1062,
		rt_END_SX_VIEW14 =							1063,
		rt_BEGIN_SX_VIEW16 =						1064,
		rt_END_SX_VIEW16 =							1065,
		rt_BEGIN_PCD14 =							1066,
		rt_END_PCD14 =								1067,
		rt_BEGIN_EXT_CONN14 =						1068,
		rt_END_EXT_CONN14 =							1069,
		rt_BEGIN_SLICER_CACHE_IDS =					1070,
		rt_END_SLICER_CACHE_IDS =					1071,
		rt_BEGIN_SLICER_CACHE_ID =					1072,
		rt_END_SLICER_CACHE_ID =					1073,
		rt_BEGIN_SLICER_CACHE =						1075,
		rt_END_SLICER_CACHE =						1076,
		rt_BEGIN_SLICER_CACHE_DEF =					1077,
		rt_END_SLICER_CACHE_DEF =					1078,
		rt_BEGIN_SLICERS_EX =						1079,
		rt_END_SLICERS_EX =							1080,
		rt_BEGIN_SLICER_EX =						1081,
		rt_END_SLICER_EX =							1082,
		rt_BEGIN_SLICER =							1083,
		rt_END_SLICER =								1084,
		rt_SLICER_CACHE_PIVOT_TABLES =				1085,
		rt_SLICER_CACHE_OLAP_ITEM =					1096,
		rt_SLICER_CACHE_SELECTION =					1098,
		rt_END_SLICER_CACHE_NATIVE =				1101,
		rt_SLICER_CACHE_NATIVE_ITEM =				1102,
		rt_RANGE_PROTECTION14 =						1103,
		rt_RANGE_PROTECTION_ISO14 =					1104,
		rt_CELL_IGNORE_EC14 =						1105,
		rt_LIST14 =									1111,
		rt_CF_ICON =								1112,
		rt_BEGIN_SLICER_CACHES_PIVOT_CACHE_IDS =	1113,
		rt_BEGIN_SLICERS =							1115,
		rt_END_SLICERS =							1116,
		rt_WB_PROP14 =								1117,
		rt_BEGIN_SX_EDIT =							1118,
		rt_END_SX_EDIT =							1119,
		rt_BEGIN_SX_EDITS =							1120,
		rt_END_SX_EDITS =							1121,
		rt_BEGIN_SX_CHANGE =						1122,
		rt_END_SX_CHANGE =							1123,
		rt_BEGIN_SX_CHANGES =						1124,
		rt_END_SX_CHANGES =							1125,
		rt_SX_TUPLE_ITEMS =							1126,
		rt_BEGIN_SLICER_STYLE =						1128,
		rt_END_SLICER_STYLE =						1129,
		rt_SLICER_STYLE_ELEMENT =					1130,
		rt_BEGIN_STYLE_SHEET_EXT14 =				1131,
		rt_END_STYLE_SHEET_EXT14 =					1132,
		rt_BEGIN_SLICER_CACHES_PIVOT_CACHE_ID =		1133,
		rt_BEGIN_PCD_CALC_MEM_EXT =					1137,
		rt_END_PCD_CALC_MEM_EXT =					1138,
		rt_BEGIN_PCD_CALC_MEMS_EXT =				1139,
		rt_END_PCD_CALC_MEMS_EXT =					1140,
		rt_PCD_FIELD14 =							1141,
		rt_BEGIN_SLICER_STYLES =					1142,
		rt_END_SLICER_STYLES =						1143,
		rt_CF_RULE_EXT =							1146,
		rt_BEGIN_SX_COND_FMT14 =					1147,
		rt_END_SX_COND_FMT14 =						1148,
		rt_BEGIN_SX_COND_FMTS14 =					1149,
		rt_END_SX_COND_FMTS14 =						1150,
		rt_BEGIN_SORT_COND14 =						1152,
		rt_END_SORT_COND14 =						1153,
		rt_END_D_VALS14 =							1154,
		rt_END_ICON_SET14 =							1155,
		rt_END_DATABAR14 =							1156,
		rt_BEGIN_COLOR_SCALE14 =					1157,
		rt_END_COLOR_SCALE14 =						1158,
		rt_BEGIN_SXRULES14 =						1159,
		rt_END_SXRULES14 =							1160,
		rt_BEGIN_P_RULE14 =							1161,
		rt_END_P_RULE14 =							1162,
		rt_BEGIN_PR_FILTERS14 =						1163,
		rt_END_PR_FILTERS14 =						1164,
		rt_BEGIN_PR_FILTER14 =						1165,
		rt_END_PR_FILTER14 =						1166,
		rt_BEGIN_PRF_ITEM14 =						1167,
		rt_END_PRF_ITEM14 =							1168,
		rt_BEGIN_CELL_IGNORE_ECS14 =				1169,
		rt_END_CELL_IGNORE_ECS14 =					1170,
		rt_DXF14 =									1171,
		rt_BEGIN_DX_F14S =							1172,
		rt_END_DXF14S =								1173,
		rt_FILTER14 =								1177,
		rt_BEGIN_CUSTOM_FILTERS14 =					1178,
		rt_CUSTOM_FILTER14 =						1180,
		rt_ICON_FILTER14 =							1181,
		rt_BEGIN_PIVOT_TABLE_REFS =					2051,
		rt_END_PIVOT_TABLE_REFS =					2052,
		rt_PIVOT_TABLE_REF =						2053,
		rt_BEGIN_SXVCELLS =							2055,
		rt_END_SXVCELLS =							2056,
		rt_BEGIN_SX_ROW =							2057,
		rt_END_SX_ROW =								2058,
		rt_PCD_CALC_MEM15 =							2060,
		rt_QSI15 =									2067,
		rt_BEGIN_WEB_EXTENSIONS =					2068,
		rt_END_WEB_EXTENSIONS =						2069,
		rt_WEB_EXTENSION =							2070,
		rt_ABS_PATH15 =								2071,
		rt_TABLE_SLICER_CACHE_IDS =					2075,
		rt_TABLE_SLICER_CACHE_ID =					2076,
		rt_BEGIN_TABLE_SLICER_CACHE =				2077,
		rt_END_TABLE_SLICER_CACHE =					2078,
		rt_SX_FILTER15 =							2079,
		rt_BEGIN_TIMELINE_CACHE_PIVOT_CACHE_IDS =	2080,
		rt_END_TIMELINE_CACHE_PIVOT_CACHE_IDS =		2081,
		rt_END_TIMELINE_CACHE_IDS =					2084,
		rt_BEGIN_TIMELINE_CACHE_ID =				2085,
		rt_END_TIMELINE_CACHE_ID =					2086,
		rt_BEGIN_TIMELINES_EX =						2087,
		rt_END_TIMELINES_EX =						2088,
		rt_BEGIN_TIMELINE_EX =						2089,
		rt_END_TIMELINE_EX =						2090,
		rt_WORK_BOOK_PR15 =							2091,
		rt_PCDH15 =									2092,
		rt_BEGIN_TIMELINE_STYLE =					2093,
		rt_END_TIMELINE_STYLE =						2094,
		rt_TIMELINE_STYLE_ELEMENT =					2095,
		rt_BEGIN_TIMELINE_STYLES =					2098,
		rt_END_TIMELINE_STYLES =					2099,
		rt_DXF15 =									2102,
		rt_BEGIN_DXFS15 =							2103,
		rt_END_DXFS15 =								2104,
		rt_SLICER_CACHE_HIDE_ITEMS_WITH_NO_DATA =	2105,
		rt_BEGIN_ITEM_UNIQUE_NAMES =				2106,
		rt_END_ITEM_UNIQUE_NAMES =					2107,
		rt_ITEM_UNIQUE_NAME =						2108,
		rt_BEGIN_EXT_CONN15 =						2109,
		rt_END_EXT_CONN15 =							2110,
		rt_BEGIN_OLEDB_PR15 =						2111,
		rt_END_OLEDB_PR15 =							2112,
		rt_BEGIN_DATA_FEED_PR15 =					2113,
		rt_END_DATA_FEED_PR15 =						2114,
		rt_TEXT_PR15 =								2115,
		rt_RANGE_PR15 =								2116,
		rt_DB_COMMAND15 =							2117,
		rt_BEGIN_DB_TABLES15 =						2118,
		rt_END_DB_TABLES15 =						2119,
		rt_DB_TABLE15 =								2120,
		rt_BEGIN_DATA_MODEL =						2121,
		rt_END_DATA_MODEL =							2122,
		rt_BEGIN_MODEL_TABLES =						2123,
		rt_END_MODEL_TABLES =						2124,
		rt_MODEL_TABLE =							2125,
		rt_END_MODEL_RELATIONSHIPS =				2127,
		rt_MODEL_RELATIONSHIP =						2128,
		rt_BEGIN_EC_TXT_WIZ15 =						2129,
		rt_END_EC_TXT_WIZ15 =						2130,
		rt_BEGIN_ECTW_FLD_INFO_LST15 =				2131,
		rt_END_ECTW_FLD_INFO_LST15 =				2132,
		rt_BEGIN_ECTW_FLD_INFO15 =					2133,
		rt_FIELD_LIST_ACTIVE_ITEM =					2134,
		rt_PIVOT_CACHE_ID_VERSION =					2135,
		rt_SXDI15 =									2136,
		rt_REVISION_PTR =							3073,
		rt_UNKNOWN =								0xFFFF
	} RecordType;
}
