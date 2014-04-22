#pragma once

//#include "NamespaceOwn.h"
#include "../XML/XmlUtils.h"
#include "atlstr.h"

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
		et_a_Paragraph,
		et_a_Run,
		et_a_Text,
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

		et_PictureNonVisual,
		et_NonVisualDrawingProperies,
		et_NonVisualPictureDrawingProps,
		et_ConnectionNonVisualShapeProps,
		et_ShapeProperties,
		et_ShapeNonVisual,
		et_ShapeStyle,
		et_Shape,
		et_ConnShape,
		et_ConnShapeNonVisual,
		et_ConnectionNonVisualConnShapeProps


	};
	class WritingElement
	{
	public:
		WritingElement(){}
		virtual ~WritingElement() {}

		virtual void			toXML(XmlUtils::CStringWriter& writer) const	= 0;
        virtual CString			toXML() const									= 0;
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
			for ( int nIndex = 0; nIndex < m_arrItems.GetSize(); nIndex++ )
			{
				if ( m_arrItems[nIndex] )
					delete m_arrItems[nIndex];

				m_arrItems[nIndex] = NULL;
			}

			m_arrItems.RemoveAll();
		}
		CSimpleArray<ElemType *>         m_arrItems;
	};
}
}
