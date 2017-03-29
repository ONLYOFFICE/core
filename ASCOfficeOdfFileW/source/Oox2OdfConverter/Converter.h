/*
 * (c) Copyright Ascensio System SIA 2010-2017
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

#include <cpdoccore/CPOptional.h>
#include "../../../Common/DocxFormat/Source/Base/SmartPtr.h"

#include "../progressCallback.h"

#include <vector>
#include <string>

namespace cpdoccore
{
	namespace odf_types
	{

		class color;
		class length;
		class text_align;
		class font_size;
		class length_or_percent;
	}
	namespace odf_writer 
	{
		class odf_conversion_context;

		class style_paragraph_properties;
		class style_text_properties;
		class style_graphic_properties;
		class style_section_properties;
		class style_table_properties;
		class style_table_cell_properties;
		class style_table_row_properties;
		class style_table_column_properties;
		class style_chart_properties;
		class style_drawing_page_properties;
	}
}
namespace OOX
{
	class WritingElement;
	class IFileContainer;
	class File;

	namespace Drawing
	{
		//class CDiagrammParts;
		//class CPresetTextShape;
		class CInline;
		class CAnchor;
	}
	namespace Spreadsheet
	{
		class IFileContainer;
		class CChartSpace;

		class CT_extLst;
		class CT_ChartSpace;
		class CT_Boolean;
		class CT_RelId;
		class CT_PageSetup;
		class CT_PageMargins;
		class CT_HeaderFooter;
		class CT_PrintSettings;
		class CT_ExternalData;
		class CT_DispBlanksAs;
		class CT_LegendEntry;
		class CT_UnsignedInt;
		class CT_Extension;
		class CT_LegendPos;
		class CT_Legend;
		class CT_Layout;
		class CT_ManualLayout;
		class CT_LayoutTarget;
		class CT_LayoutMode;
		class CT_Double;
		class CT_DTable;
		class CT_SerAx;
		class CT_Scaling;
		class CT_LogBase;
		class CT_Orientation;
		class CT_AxPos;
		class CT_ChartLines;
		class CT_Title;
		class CT_Tx;
		class CT_StrRef;
		class CT_StrData;
		class CT_StrVal;
		class CT_NumFmt;
		class CT_TickMark;
		class CT_TickLblPos;
		class CT_Crosses;
		class CT_Skip;
		class CT_TimeUnit;
		class CT_DateAx;
		class CT_LblOffset;
		class CT_AxisUnit;
		class CT_LblAlgn;
		class CT_CatAx;
		class CT_DispUnitsLbl;
		class CT_BuiltInUnit;
		class CT_DispUnits;
		class CT_CrossBetween;
		class CT_ValAx;
		class CT_SizeRepresents;
		class CT_BubbleScale;
		class CT_BubbleSer;
		class CT_SerTx;
		class CT_DPt;
		class CT_Marker;
		class CT_MarkerStyle;
		class CT_MarkerSize;
		class CT_PictureOptions;
		class CT_PictureFormat;
		class CT_PictureStackUnit;
		class CT_DLbls;
		class CT_DLbl;
		class CT_DLblPos;
		class CT_Trendline;
		class CT_TrendlineType;
		class CT_Order;
		class CT_Period;
		class CT_TrendlineLbl;
		class CT_ErrBars;
		class CT_ErrDir;
		class CT_ErrBarType;
		class CT_ErrValType;
		class CT_NumDataSource;
		class CT_NumData;
		class CT_NumVal;
		class CT_NumRef;
		class CT_AxDataSource;
		class CT_MultiLvlStrRef;
		class CT_lvl;
		class CT_MultiLvlStrData;
		class CT_BubbleChart;
		class CT_bandFmts;
		class CT_Surface3DChart;
		class CT_SurfaceSer;
		class CT_BandFmt;
		class CT_SurfaceChart;
		class CT_SecondPieSize;
		class CT_SplitType;
		class CT_OfPieType;
		class CT_custSplit;
		class CT_OfPieChart;
		class CT_PieSer;
		class CT_GapAmount;
		class CT_Bar3DChart;
		class CT_BarDir;
		class CT_BarGrouping;
		class CT_BarSer;
		class CT_Shape;
		class CT_Overlap;
		class CT_BarChart;
		class CT_HoleSize;
		class CT_DoughnutChart;
		class CT_FirstSliceAng;
		class CT_Pie3DChart;
		class CT_PieChart;
		class CT_ScatterSer;
		class CT_ScatterStyle;
		class CT_ScatterChart;
		class CT_RadarSer;
		class CT_RadarStyle;
		class CT_RadarChart;
		class CT_StockChart;
		class CT_LineSer;
		class CT_UpDownBars;
		class CT_UpDownBar;
		class CT_Line3DChart;
		class CT_Grouping;
		class CT_LineChart;
		class CT_Area3DChart;
		class CT_AreaSer;
		class CT_AreaChart;
		class CT_PlotArea;
		class CT_Thickness;
		class CT_Surface;
		class CT_Perspective;
		class CT_DepthPercent;
		class CT_RotY;
		class CT_HPercent;
		class CT_RotX;
		class CT_View3D;
		class CT_PivotFmt;
		class CT_pivotFmts;
		class CT_Chart;
		class CT_Protection;
		class CT_PivotSource;
		class CT_Style1;
		class CT_Style;
		class CT_TextLanguageID;
		class CTextProperties;
	}
	namespace Vml
	{
		class CShapeType;
		class CShape;
		class CImage;
		class CImageData;
		class CArc;
		class CBackground;
		class CCurve;
		class CFill;
		class CLine;
		class COval;
		class CPath	;
		class CPolyLine	;
		class CRect;
		class CRoundRect;
		class CShadow;
		class CStroke;
		class CTextbox;
		class CTextPath;
		class CGroup;
		class CVmlCommonElements;
	}
	namespace VmlWord
	{	
		class CWrap;
	}
}
namespace SimpleTypes
{
	namespace Vml
	{
		class CCssStyle;
	}
}
namespace PPTX
{
	class Theme;
	namespace Logic
	{
		class ClrMap;
		class SpTreeElem;
		class GraphicFrame;
		class SpTree;
		class Shape;
		class Pic;
		class Table;
		class SmartArt;
		class ChartRec;
		class SpPr;
		class Xfrm;
		class ShapeStyle;
		class PrstTxWarp;
		class PrstGeom;
		class CustGeom;
		class BlipFill;
		class GradFill;
		class SolidFill;
		class PattFill;
		class EffectLst;
		class FontRef;
		class StyleRef;
		class Ln;
		class Path2D;
		class PathBase;
		class TxBody;
		class BodyPr;
		class UniFill;
		class UniColor;
		class NvSpPr;
		class CNvPr;
		class CNvSpPr;
		class NvPr;
		class Paragraph;
		class TxBody;
		class TextParagraphPr;
		class TextSpacing;
		class RunProperties;
		class Run;
		class Fld;
		class Br;
		class MathParaWrapper;
		class NvGraphicFramePr;
		class LineTo;
		class MoveTo;
		class ArcTo;
		class QuadBezTo;
		class CubicBezTo;
		class Close;
	}
}

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual void convertDocument() = 0;
		virtual void write(const std::wstring & path) = 0;
		
		OoxConverter(const ProgressCallback* CallBack = NULL){
																oox_current_child_document = NULL; 
																
																pCallBack = CallBack;
																bUserStopConvert = 0;
																currentSystemDPI = getSystemDPI();
															 }

        void set_fonts_directory (const std::wstring & fontsPath);
		
		const	ProgressCallback* pCallBack;
		short	bUserStopConvert;
		bool	UpdateProgress(long nComplete);

		double	currentSystemDPI;
		double	getSystemDPI();
	
//.......................................................................................................................
		virtual cpdoccore::odf_writer::odf_conversion_context	*odf_context()	= 0;		
		virtual PPTX::Theme										*oox_theme()	= 0;
		virtual PPTX::Logic::ClrMap								*oox_clrMap()	{return NULL;}
        
		virtual std::wstring								find_link_by_id(std::wstring sId, int t) = 0;
		virtual NSCommon::smart_ptr<OOX::File>				find_file_by_id(std::wstring sId) = 0;
		
		OOX::IFileContainer									*oox_current_child_document;
		
		void convert (double oox_font_size, _CP_OPT(cpdoccore::odf_types::font_size) & odf_font_size);	
		bool convert (std::wstring sSchemeColor, DWORD & argb);
//.......................................................................................................................
		void convert(OOX::WritingElement  *oox_unknown);
//drawingML & pptx................................................................................................................................
		void convert(PPTX::Logic::SpTreeElem					*oox_element);
		void convert(PPTX::Logic::GraphicFrame					*oox_graphicFrame);
		void convert(PPTX::Logic::SpTree						*oox_shape_tree);
		void convert(PPTX::Logic::Shape							*oox_shape);
		void convert(PPTX::Logic::Pic							*oox_pic);
		void convert(PPTX::Logic::SpPr							*oox_spPr, PPTX::Logic::ShapeStyle* oox_sp_style = NULL);
		void convert(PPTX::Logic::TextSpacing					*oox_spacing, cpdoccore::odf_types::length_or_percent & length_or_percent);
		void convert(PPTX::Logic::Xfrm							*oox_xfrm);
		int	 convert(PPTX::Logic::PrstTxWarp					*oox_text_preset);
		void convert(PPTX::Logic::PrstGeom						*oox_geom);
		void convert(PPTX::Logic::CustGeom						*oox_geom);
		void convert(PPTX::Logic::BlipFill						*oox_fill);
		void convert(PPTX::Logic::GradFill						*oox_fill);
		void convert(PPTX::Logic::SolidFill						*oox_fill);
		void convert(PPTX::Logic::PattFill						*oox_fill);
		void convert(PPTX::Logic::EffectLst						*oox_effect_lst);
		void convert(PPTX::Logic::Ln							*oox_line);
		void convert(PPTX::Logic::FontRef						*oox_fontRef);
		void convert(PPTX::Logic::StyleRef						*oox_styleRef);
		void convert(PPTX::Logic::Path2D						*oox_path2D);
		void convert(PPTX::Logic::PathBase						*oox_path);
		void convert(PPTX::Logic::BodyPr						*oox_bodyPr);
		void convert(PPTX::Logic::UniFill						*oox_fill,	PPTX::Logic::ShapeStyle* oox_style = NULL);
		void convert(PPTX::Logic::UniColor						*color,		std::wstring & hexString, _CP_OPT(double) & opacity);
		void convert(PPTX::Logic::NvSpPr						*oox_nvSpPr);
		void convert(PPTX::Logic::CNvPr							*oox_cnvPr);
		void convert(PPTX::Logic::CNvSpPr						*oox_cnvSpPr);
		void convert(PPTX::Logic::NvPr							*oox_nvPr);
		void convert(PPTX::Logic::Paragraph						*oox_para);
		void convert(PPTX::Logic::TextParagraphPr				*oox_para_props, cpdoccore::odf_writer::style_paragraph_properties * paragraph_properties);
		void convert(PPTX::Logic::RunProperties					*oox_run_props, cpdoccore::odf_writer::style_text_properties * text_properties);
		void convert(PPTX::Logic::Run							*oox_run);
		void convert(PPTX::Logic::Fld							*oox_fld);
		void convert(PPTX::Logic::Br							*oox_br);
		void convert(PPTX::Logic::TxBody						*oox_txBody, PPTX::Logic::ShapeStyle* oox_style = NULL);
		void convert(PPTX::Logic::MathParaWrapper				*oox_math);
		void convert(PPTX::Logic::NvGraphicFramePr				*oox_framePr);
		void convert(PPTX::Logic::ChartRec						*oox_chart);
		void convert(PPTX::Logic::SmartArt						*oox_smart_art);
		void convert(PPTX::Logic::Table							*oox_table);
		void convert(PPTX::Logic::LineTo						*oox_geom_path);
		void convert(PPTX::Logic::MoveTo						*oox_geom_path);
		void convert(PPTX::Logic::ArcTo							*oox_geom_path);
		void convert(PPTX::Logic::QuadBezTo						*oox_geom_path);
		void convert(PPTX::Logic::CubicBezTo					*oox_geom_path);
		void convert(PPTX::Logic::Close							*oox_geom_path);
//.chart............................................................................................................................
		void convert(OOX::Spreadsheet::CT_ChartSpace			*oox_chart);
		void convert(OOX::Spreadsheet::CT_Title					*ct_title);
		void convert(OOX::Spreadsheet::CT_Legend				*ct_legend);
		void convert(OOX::Spreadsheet::CT_LegendEntry			*ct_legend);
		void convert(OOX::Spreadsheet::CT_PlotArea				*ct_plotArea);
		void convert(OOX::Spreadsheet::CT_Surface				*ct_surface, int type, bool chart3D);
		void convert(OOX::Spreadsheet::CT_CatAx					*ct_catAx);
		void convert(OOX::Spreadsheet::CT_SerAx					*ct_serAx);
		void convert(OOX::Spreadsheet::CT_DateAx				*ct_dateAx);
		void convert(OOX::Spreadsheet::CT_ValAx					*ct_valAx);
		void convert(OOX::Spreadsheet::CT_Area3DChart			*chart);
		void convert(OOX::Spreadsheet::CT_AreaChart				*chart);
		void convert(OOX::Spreadsheet::CT_Bar3DChart			*chart);
		void convert(OOX::Spreadsheet::CT_BarChart				*chart);
		void convert(OOX::Spreadsheet::CT_Line3DChart			*chart);
		void convert(OOX::Spreadsheet::CT_LineChart				*chart);
		void convert(OOX::Spreadsheet::CT_Pie3DChart			*chart);
		void convert(OOX::Spreadsheet::CT_PieChart				*chart);
		void convert(OOX::Spreadsheet::CT_Surface3DChart		*chart);
		void convert(OOX::Spreadsheet::CT_SurfaceChart			*chart);
		void convert(OOX::Spreadsheet::CT_BubbleChart			*chart);
		void convert(OOX::Spreadsheet::CT_DoughnutChart			*chart);
		void convert(OOX::Spreadsheet::CT_ScatterChart			*chart);
		void convert(OOX::Spreadsheet::CT_RadarChart			*chart);
		void convert(OOX::Spreadsheet::CT_StockChart			*chart);
		void convert(OOX::Spreadsheet::CT_OfPieChart			*chart);
		void convert(OOX::Spreadsheet::CT_AreaSer				*ser);
		void convert(OOX::Spreadsheet::CT_BubbleSer				*ser);
		void convert(OOX::Spreadsheet::CT_SurfaceSer			*ser);
		void convert(OOX::Spreadsheet::CT_PieSer				*ser);
		void convert(OOX::Spreadsheet::CT_BarSer				*ser);
		void convert(OOX::Spreadsheet::CT_ScatterSer			*ser);
		void convert(OOX::Spreadsheet::CT_RadarSer				*ser);
		void convert(OOX::Spreadsheet::CT_LineSer				*ser);
		void convert(OOX::Spreadsheet::CT_AxDataSource			*cat, int type);
		void convert(OOX::Spreadsheet::CT_NumDataSource			*val);
		//void convert(OOX::Spreadsheet::CRichText				*rich);
		//void convert(OOX::Spreadsheet::CTextProperties		*txPr);
		void convert(OOX::Spreadsheet::CT_Tx					*ct_tx);
		void convert(OOX::Spreadsheet::CT_Layout				*ct_layout);
		void convert(OOX::Spreadsheet::CT_ManualLayout			*ct_layout);
		void convert(OOX::Spreadsheet::CT_ChartLines			*grid, int type);
		void convert(OOX::Spreadsheet::CT_SerTx					*ser_tx);
		void convert(OOX::Spreadsheet::CT_DLbls					*ser_lbls);
		void convert(OOX::Spreadsheet::CT_Marker				*marker);
		void convert(OOX::Spreadsheet::CT_Marker				*marker, std::vector<OOX::Spreadsheet::CT_DPt*> & dPt);
		void convert(OOX::Spreadsheet::CT_ExternalData			*external_data);
		void convert(OOX::Spreadsheet::CT_NumData				*num_data, bool categories, bool label);
		void convert(OOX::Spreadsheet::CT_StrData				*str_data, bool categories, bool label);

//.vml............................................................................................................................
		void convert(OOX::Vml::CShapeType				*vml_shape_type);
		void convert(OOX::Vml::CShape					*vml_shape);
		void convert(OOX::Vml::CImage					*vml_image);
		void convert(OOX::Vml::CImageData				*vml_image_data);
		void convert(OOX::Vml::CArc						*vml_arc);
		void convert(OOX::Vml::CBackground				*vml_background);
		void convert(OOX::Vml::CCurve					*vml_curve);
		void convert(OOX::Vml::CFill					*vml_fill);
		void convert(OOX::Vml::CLine					*vml_line);
		void convert(OOX::Vml::COval					*vml_oval);
		void convert(OOX::Vml::CPath					*vml_path);
		void convert(OOX::Vml::CPolyLine				*vml_polyline);
		void convert(OOX::Vml::CRect					*vml_rect);
		void convert(OOX::Vml::CRoundRect				*vml_roundrect);
		void convert(OOX::Vml::CShadow					*vml_shadow);
		void convert(OOX::Vml::CStroke					*vml_stroke);
		void convert(OOX::Vml::CTextbox					*vml_textbox);
		void convert(OOX::Vml::CTextPath				*vml_textpath);
		void convert(SimpleTypes::Vml::CCssStyle		*vml_style, bool group = false);
		void convert(OOX::VmlWord::CWrap				*vml_wrap);
		void convert(OOX::Vml::CGroup					*vml_group);
		void convert(OOX::Vml::CVmlCommonElements		*vml_attr);
	};

} // namespace Oox2Odf
