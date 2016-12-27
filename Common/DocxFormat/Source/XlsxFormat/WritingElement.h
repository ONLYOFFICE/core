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
#pragma once

//#include "NamespaceOwn.h"
//#include "../XML/XmlUtils.h"

#include "../../../DesktopEditor/common/StringBuilder.h"

namespace OOX
{
namespace Spreadsheet
{
#define WritingElementSpreadsheet_AdditionConstructors(Class) \
	Class(XmlUtils::CXmlLiteReader& oReader)\
	{\
		fromXML( oReader );\
	}\
	const Class& operator =(const XmlUtils::CXmlLiteReader& oReader)\
	{\
		fromXML( (XmlUtils::CXmlLiteReader&)oReader );\
		return *this;\
	}

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
		writer.WriteString(OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(val)); \
		writer.WriteString(end);


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
#define WritingStringNullableAttrInt(name, elem, val) \
		WritingStringNullableAttr(name, AddInt, elem, val)
#define WritingStringNullableAttrInt64(name, elem, val) \
		WritingStringNullableAttr(name, AddInt64, elem, val)
#define WritingStringNullableAttrDouble(name, elem, val) \
		if(elem.IsInit()) \
		{ \
			WritingStringAttrDouble(name, val) \
		}
#define WritingStringNullableAttrBool(name, elem) \
		WritingStringNullableAttrString(name, elem, elem->ToString3(SimpleTypes::onofftostring1))


	const double c_ag_Inch_to_MM	= 25.4;
	const double c_ag_1pxWidth		= 25.4 / 96;

	enum EElementType
	{
		et_Unknown,

		//Generated code
		et_ct_extlst,
		et_ct_chartspace,
		et_ct_boolean,
		et_ct_relid,
		et_ct_pagesetup,
		et_ct_pagemargins,
		et_ct_headerfooter,
		et_ct_printsettings,
		et_ct_externaldata,
		et_ct_dispblanksas,
		et_ct_legendentry,
		et_ct_unsignedint,
		et_ct_extension,
		et_ct_legendpos,
		et_ct_legend,
		et_ct_layout,
		et_ct_manuallayout,
		et_ct_layouttarget,
		et_ct_layoutmode,
		et_ct_double,
		et_ct_dtable,
		et_ct_serax,
		et_ct_scaling,
		et_ct_logbase,
		et_ct_orientation,
		et_ct_axpos,
		et_ct_chartlines,
		et_ct_title,
		et_ct_tx,
		et_ct_strref,
		et_ct_strdata,
		et_ct_strval,
		et_ct_numfmt,
		et_ct_tickmark,
		et_ct_ticklblpos,
		et_ct_crosses,
		et_ct_skip,
		et_ct_timeunit,
		et_ct_dateax,
		et_ct_lbloffset,
		et_ct_axisunit,
		et_ct_lblalgn,
		et_ct_catax,
		et_ct_dispunitslbl,
		et_ct_builtinunit,
		et_ct_dispunits,
		et_ct_crossbetween,
		et_ct_valax,
		et_ct_sizerepresents,
		et_ct_bubblescale,
		et_ct_bubbleser,
		et_ct_sertx,
		et_ct_dpt,
		et_ct_marker,
		et_ct_markerstyle,
		et_ct_markersize,
		et_ct_pictureoptions,
		et_ct_pictureformat,
		et_ct_picturestackunit,
		et_ct_dlbls,
		et_ct_dlbl,
		et_ct_dlblpos,
		et_ct_trendline,
		et_ct_trendlinetype,
		et_ct_order,
		et_ct_period,
		et_ct_trendlinelbl,
		et_ct_errbars,
		et_ct_errdir,
		et_ct_errbartype,
		et_ct_errvaltype,
		et_ct_numdatasource,
		et_ct_numdata,
		et_ct_numval,
		et_ct_numref,
		et_ct_axdatasource,
		et_ct_multilvlstrref,
		et_ct_lvl,
		et_ct_multilvlstrdata,
		et_ct_bubblechart,
		et_ct_bandfmts,
		et_ct_surface3dchart,
		et_ct_surfaceser,
		et_ct_bandfmt,
		et_ct_surfacechart,
		et_ct_secondpiesize,
		et_ct_splittype,
		et_ct_ofpietype,
		et_ct_custsplit,
		et_ct_ofpiechart,
		et_ct_pieser,
		et_ct_gapamount,
		et_ct_bar3dchart,
		et_ct_bardir,
		et_ct_bargrouping,
		et_ct_barser,
		et_ct_shape,
		et_ct_overlap,
		et_ct_barchart,
		et_ct_holesize,
		et_ct_doughnutchart,
		et_ct_firstsliceang,
		et_ct_pie3dchart,
		et_ct_piechart,
		et_ct_scatterser,
		et_ct_scatterstyle,
		et_ct_scatterchart,
		et_ct_radarser,
		et_ct_radarstyle,
		et_ct_radarchart,
		et_ct_stockchart,
		et_ct_lineser,
		et_ct_updownbars,
		et_ct_updownbar,
		et_ct_line3dchart,
		et_ct_grouping,
		et_ct_linechart,
		et_ct_area3dchart,
		et_ct_areaser,
		et_ct_areachart,
		et_ct_plotarea,
		et_ct_thickness,
		et_ct_surface,
		et_ct_perspective,
		et_ct_depthpercent,
		et_ct_roty,
		et_ct_hpercent,
		et_ct_rotx,
		et_ct_view3d,
		et_ct_pivotfmt,
		et_ct_pivotfmts,
		et_ct_chart,
		et_ct_protection,
		et_ct_pivotsource,
		et_ct_style1,
		et_ct_style,
		et_ct_textlanguageid,
		et_alternatecontent,
		et_alternatecontentchoice,
		et_alternatecontentfallback,

		et_BookViews, // <bookViews>
		et_WorkbookPr,
		et_WorkbookView, // <workbookView>
		et_DefinedNames, // <definedNames>
		et_DefinedName, // <definedName>
		et_Sheets, // <sheets>
		et_Sheet, // <sheet>
		et_ExternalReference,
		et_ExternalReferences,
		et_Si, // <si>
		et_PhoneticPr, // <phoneticPr>
		et_r, // <r>
		et_rPr, // <rPr>
		et_rPh, // <rPh>
		et_t, // <t>
		et_Borders, // <borders>
		et_Border,
		et_BorderProp,
		et_CellStyles,
		et_CellStyle,
		et_CellStyleXfs,
		et_CellXfs,
		et_Xfs,
		et_Aligment,
		et_Protection,
		et_Colors,
		et_Color,
		et_RgbColor,
		et_IndexedColors,
		et_MruColors,
		et_Dxfs,
		et_Dxf,
		et_Fills,
		et_Fill,
		et_GradientFill,
		et_GradientStop,
		et_PatternFill,
		et_BgColor,
		et_FgColor,
		et_Fonts,
		et_Font,
		et_NumFmts,
		et_NumFmt,
		et_TableStyles,
		et_TableStyle,
		et_TableStyleElement,
		et_SheetData,
		et_Row,
		et_Cell,
		et_Formula,
		et_Cols,
		et_Col,
		et_Hyperlinks,
		et_Hyperlink,
		et_PageMargins,
		et_PageSetup,
		et_PrintOptions,
		et_MergeCells,
		et_MergeCell,
		et_Dimension,
		et_SheetFormatPr,
		et_CellAnchor,
		et_Pic,
		et_BlipFill,
		et_Blip,
		et_FromTo,
		et_Pos,
		et_Ext,
		et_CalcCell,
		et_SheetViews,
		et_SheetView,
		et_c_Chart,
		et_c_ChartStyle,
		et_c_Title,
		et_c_Tx,
		et_c_Rich,
		et_c_Legend,
		et_c_Overlay,
		et_c_LegendPos,
		et_c_LegendEntry,
		et_c_Layout,
		et_c_ManualLayout,
		et_c_PlotArea,
		et_c_CatAx,
		et_c_ValAx,
		et_c_CatAy,
		et_c_ValAy,
		et_c_BasicChart,
		et_c_Series,
		et_c_NumPoint,
		et_c_NumCache,
		et_c_NumCacheRef,
		et_c_NumCacheValues,
		et_c_SeriesCat,
		et_c_StrCacheRef,
		et_c_SeriesTx,
		et_c_SeriesMarker,
		et_c_SeriesDataLabels,
		et_c_SeriesShapeProperties,
		et_c_SeriesShapeIndex,
		et_c_SeriesShapeOrder,
		et_c_SeriesShapeOutline,
		et_xdr_GraphicFrame,
		et_xdr_GraphicData,
		et_OleObjects,
		et_OleObject,
		et_OleObjectPr,
		et_OleObjectAnchor,
		et_TableParts,
		et_TablePart,
		et_Table,
		et_TableColumns,
		et_TableColumn,
		et_TableStyleInfo,
		et_SortState,
		et_SortCondition,
		et_Autofilter,
		et_FilterColumn,
		et_ColorFilter,
		et_DynamicFilter,
		et_CustomFilters,
		et_Filters,
		et_Filter,
		et_DateGroupItem,
		et_Authors,
		et_CommentList,
		et_Comment,
		et_ConditionalFormatting,
		et_ConditionalFormattingRule,
		et_ColorScale,
		et_DataBar,
		et_FormulaCF,
		et_IconSet,
		et_ConditionalFormatValueObject,
		et_SheetPr,
		et_Pane,
		et_ExternalBook,
		et_Selection,
		et_LegacyDrawingWorksheet,
		et_LegacyDrawingHFWorksheet,
		et_PictureWorksheet,
		et_HeaderFooterWorksheet,
		et_HeaderFooterElementWorksheet,

		et_PictureNonVisual,
		et_NonVisualDrawingProperties,
		et_NonVisualPictureDrawingProps,
		et_ConnectionNonVisualShapeProps,
		et_ShapeProperties,
		et_ShapeNonVisual,
		et_ShapeStyle,
		et_GroupShape,
		et_Shape,
		et_ConnShape,
		et_ShapeTextBody,
		et_ConnShapeNonVisual,
		et_GroupShapeNonVisual,
		et_NonVisualGroupShapeDrawingProps,
		et_ConnectionNonVisualConnShapeProps,
		et_GraphicFrameNonVisual,
		et_ConnectionNonVisualGraphicFrameProps,
		et_GraphicChart,
		et_ChartTextProperties,
		et_ChartRichText,

		et_SparklineGroups,
		et_SparklineGroup,
		et_Sparklines,
		et_Sparkline

	};
	class WritingElement
	{
	public:
		WritingElement(){}
		virtual ~WritingElement() {}

		virtual void			toXML(NSStringUtils::CStringBuilder& writer) const	= 0;
        //virtual CString			toXML() const									= 0;
		virtual EElementType	getType() const
		{
			return et_Unknown;
		}
		virtual void         fromXML(XmlUtils::CXmlLiteReader& oReader) {}
	};

	template<typename ElemType = WritingElement>
	class WritingElementWithChilds : public WritingElement
	{
	public:
		WritingElementWithChilds(){}
		virtual ~WritingElementWithChilds() {ClearItems();}
		virtual void ClearItems()
		{
			for ( unsigned int nIndex = 0; nIndex < m_arrItems.size(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )delete m_arrItems[nIndex];
				m_arrItems[nIndex] = NULL;
			}

			m_arrItems.clear();
		}
		std::vector<ElemType *>         m_arrItems;
	};
}
}
