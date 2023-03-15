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

#include <CPOptional.h>
#include "../../../OOXML/Base/SmartPtr.h"
#include "../../../OOXML/DocxFormat/Math/oMathContent.h"

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
		class graphic_format_properties;
		namespace package
		{
			class odf_document;
		}
	}
}
namespace OOX
{
	class WritingElement;
	class IFileContainer;
	class File;
	class JsaProject;

	namespace Drawing
	{
		//class CDiagrammParts;
		//class CPresetTextShape;
		class CInline;
		class CAnchor;
		class COfficeArtExtensionList;
		class COfficeArtExtension;
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

		namespace ChartEx
		{
			class CChartSpace;
			class CChart;
			class CChartData;
			class CData;
			class CDimension;
			class CPlotArea;
			class CLegend;
			class CAxis;
			class CTickMarks;
			class CGridlines;
			class CValScaling;
			class CCatScaling;
			class CSeries;
			class CDataLabels;
			class CTitle;

		}
	}
	class CSizeAnchor;
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
		class CFormulas;
	}
	namespace VmlWord
	{	
		class CWrap;
	}
	namespace Logic
	{
		class CDeg;
		class CDen;
		class CElement;
		class CFName;
		class CLim;
		class CNum;
		class COMath;
		class CSub;
		class CSup;
		class CCtrlPr;
		class CAcc;
		class CAccPr;
		class CArgPr;
		class CBar;
		class CBarPr;
		class CBorderBox;
		class CBorderBoxPr;
		class CBox;
		class CBoxPr;
		class CBrk;
		class CDelimiter;
		class CDelimiterPr;
		class CEqArr;
		class CEqArrPr;
		class CFPr;
		class CFraction;
		class CFunc;
		class CFuncPr;
		class CGroupChr;
		class CGroupChrPr;
		class CLimLow;
		class CLimLowPr;
		class CLimUpp;
		class CLimUppPr;
		class CMathFont;
		class CMathPr;
		class CMatrix;
		class CMc;
		class CMcPr;
		class CMcs;
		class CMDel;
		class CMIns;
		class CMPr;
		class CMr;
		class CMRPr;
		class CMRun;
		class CMText;
		class CNary;
		class CNaryPr;
		class COMathPara;
		class COMathParaPr;
		class CPhant;
		class CPhantPr;
		class CRad;
		class CRadPr;
		class CSPre;
		class CSPrePr;
		class CSSub;
		class CSSubPr;
		class CSSubSup;
		class CSSubSupPr;
		class CSSup;
		class CSSupPr;		
		//class CType;
		//class CSupHide;
		//class CSubHide;
		//class CBegChr;
		//class CEndChr;
		//class CDiff;
		//class CDegHide;
		//class CChr;
		//class CArgSz;
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
		class Bullet;
		class ClrMap;
		class SpTreeElem;
		class GraphicFrame;
		class SpTree;
		class Shape;
		class CxnSp;
		class Pic;
		class SmartArt;
		class ChartRec;
		class SpPr;
		class GrpSpPr;
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
		class EffectDag;
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
		class CNvCxnSpPr;
		class CNvGrpSpPr;
		class NvCxnSpPr;
		class NvGrpSpPr;
		class NvPr;
		class Paragraph;
		class TxBody;
		class TextListStyle;
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
		class AhXY;
		class AhPolar;
		class EffectStyle;
		class InnerShdw;
		class OuterShdw;
		class PrstShdw;
		class AlphaModFix;
		class Blur;
		class FillOverlay;
		class Glow;
		class Reflection;
		class SoftEdge;
		class Grayscl;
		class Duotone;
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual bool convertDocument() = 0;
		
		bool write(const std::wstring & out_path, const std::wstring & temp_path, const std::wstring & password, const std::wstring & documentID);
		
		OoxConverter() : output_document(NULL), oox_current_child_document(NULL)
		{
		}
		virtual ~OoxConverter()
		{
			m_mapVmlShapeTypes.clear();
		}

        void set_fonts_directory (const std::wstring & fontsPath);
		
		bool encrypt_document (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath);
		bool encrypt_file (const std::wstring &password, const std::wstring & srcPath, const std::wstring & dstPath, std::wstring &encrypt_info, int &size);

//.......................................................................................................................
		virtual OOX::IFileContainer								*current_document() = 0;
		virtual cpdoccore::odf_writer::odf_conversion_context	*odf_context()	= 0;		
		virtual PPTX::Theme										*oox_theme()	= 0;
		virtual PPTX::Logic::ClrMap								*oox_clrMap()	{return NULL;}
       
		cpdoccore::odf_writer::package::odf_document		*output_document;

				std::wstring						find_link_by (NSCommon::smart_ptr<OOX::File> & oFile, int type, bool & bExternal);
		virtual std::wstring						find_link_by_id(const std::wstring & sId, int type, bool & bExternal) = 0;
		virtual NSCommon::smart_ptr<OOX::File>		find_file_by_id(const std::wstring & sId) = 0;
		
		OOX::IFileContainer							*oox_current_child_document;
		
		void convert (OOX::JsaProject *jsaProject);	
		void convert (double oox_font_size, _CP_OPT(cpdoccore::odf_types::font_size) & odf_font_size);	
		bool convert (std::wstring sSchemeColor, DWORD & argb);
		void convert_font(PPTX::Theme *theme, std::wstring & font);
//.......................................................................................................................
		void convert(OOX::WritingElement  *oox_unknown);
//drawingML & pptx................................................................................................................................
		void convert(PPTX::Logic::SpTreeElem					*oox_element);
		void convert(PPTX::Logic::GraphicFrame					*oox_graphicFrame);
		void convert(PPTX::Logic::SpTree						*oox_shape_tree);
		void convert(PPTX::Logic::Shape							*oox_shape);
		void convert(PPTX::Logic::CxnSp							*oox_connect);
		void convert(PPTX::Logic::Pic							*oox_pic);
		void convert(PPTX::Logic::GrpSpPr						*oox_grpSpPr);
		void convert(PPTX::Logic::SpPr							*oox_spPr, PPTX::Logic::ShapeStyle* oox_sp_style = NULL);
		void convert(PPTX::Logic::TextSpacing					*oox_spacing, cpdoccore::odf_types::length_or_percent & length_or_percent);
		void convert(PPTX::Logic::Xfrm							*oox_xfrm);
		void convert(PPTX::Logic::Xfrm							*oox_txbx, PPTX::Logic::Xfrm *oox_xfrm);
		int	 convert(PPTX::Logic::PrstTxWarp					*oox_text_preset);
		void convert(PPTX::Logic::PrstGeom						*oox_geom);
		void convert(PPTX::Logic::CustGeom						*oox_geom);
		void convert(PPTX::Logic::BlipFill						*oox_fill);
        void convert(PPTX::Logic::GradFill						*oox_fill, DWORD ARGB = 0);
        void convert(PPTX::Logic::SolidFill						*oox_fill, DWORD ARGB = 0);
        void convert(PPTX::Logic::PattFill						*oox_fill, DWORD ARGB = 0);
		void convert(PPTX::Logic::Ln							*oox_line, DWORD ARGB = 0, PPTX::Logic::ShapeStyle* oox_sp_style = NULL);
		void convert(PPTX::Logic::FontRef						*oox_fontRef);
		void convert(PPTX::Logic::StyleRef						*oox_styleRef, int type);
		void convert(PPTX::Logic::Path2D						*oox_path2D);
		void convert(PPTX::Logic::PathBase						*oox_path);
        void convert(PPTX::Logic::UniFill						*oox_fill, DWORD ARGB = 0);
		void convert(PPTX::Logic::UniColor						*color, DWORD & nARGB);
        void convert(PPTX::Logic::UniColor						*color, std::wstring & hexString, _CP_OPT(double) & opacity, DWORD ARGB = 0);
		void convert(PPTX::Logic::NvSpPr						*oox_nvSpPr);
		void convert(PPTX::Logic::CNvPr							*oox_cnvPr);
		void convert(PPTX::Logic::CNvCxnSpPr					*oox_cnvPr);
		void convert(PPTX::Logic::CNvSpPr						*oox_cnvSpPr);
		void convert(PPTX::Logic::CNvGrpSpPr					*oox_cnvGrpSpPr);
		void convert(PPTX::Logic::NvCxnSpPr						*oox_nvSpPr);
		void convert(PPTX::Logic::NvGrpSpPr						*oox_nvSpPr);
		void convert(PPTX::Logic::NvPr							*oox_nvPr);
		void convert(PPTX::Logic::Paragraph						*oox_para, PPTX::Logic::TextListStyle *oox_list_style = NULL);		
		void convert(PPTX::Logic::TextListStyle					*oox_list_style);
		
		void convert_list_level	(PPTX::Logic::TextParagraphPr	*oox_para_props, int level);

		void convert(PPTX::Logic::TextListStyle					*oox_list_style, int level, cpdoccore::odf_writer::style_paragraph_properties	*paragraph_properties,
																							cpdoccore::odf_writer::style_text_properties		*text_properties);
		void convert(PPTX::Logic::TextParagraphPr				*oox_para_props,			cpdoccore::odf_writer::style_paragraph_properties	*paragraph_properties, 
																							cpdoccore::odf_writer::style_text_properties		*text_properties);
		void convert(PPTX::Logic::RunProperties					*oox_run_props,				cpdoccore::odf_writer::style_text_properties		*text_properties);
		void convert(PPTX::Logic::Run							*oox_run);
		void convert(PPTX::Logic::Fld							*oox_fld);
		void convert(PPTX::Logic::Br							*oox_br);
		
		void convert(PPTX::Logic::TxBody						*oox_txBody, PPTX::Logic::ShapeStyle* oox_style = NULL);
		void convert_chart_text(PPTX::Logic::TxBody				*oox_txBody, bool only_properties = false);
		void convert(PPTX::Logic::BodyPr						*oox_bodyPr);
		void convert_chart_text(PPTX::Logic::BodyPr				*oox_bodyPr);
		
		void convert(PPTX::Logic::MathParaWrapper				*oox_math);
		void convert(PPTX::Logic::NvGraphicFramePr				*oox_framePr);
		void convert(PPTX::Logic::ChartRec						*oox_chart);
		void convert(PPTX::Logic::SmartArt						*oox_smart_art);
		void convert(PPTX::Logic::LineTo						*oox_geom_path);
		void convert(PPTX::Logic::MoveTo						*oox_geom_path);
		void convert(PPTX::Logic::ArcTo							*oox_geom_path);
		void convert(PPTX::Logic::QuadBezTo						*oox_geom_path);
		void convert(PPTX::Logic::CubicBezTo					*oox_geom_path);
		void convert(PPTX::Logic::Close							*oox_geom_path);
		void convert(PPTX::Logic::AhXY							*oox_handle);
		void convert(PPTX::Logic::AhPolar						*oox_handle);
		
		void convert(PPTX::Logic::EffectStyle					*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::EffectLst						*oox_effect_list, DWORD ARGB = 0);
		void convert(PPTX::Logic::EffectDag						*oox_effect_dag, DWORD ARGB = 0);
		void convert(PPTX::Logic::InnerShdw						*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::OuterShdw						*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::PrstShdw						*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::AlphaModFix					*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::Blur							*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::FillOverlay					*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::Glow							*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::Reflection					*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::SoftEdge						*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::Grayscl						*oox_effect, DWORD ARGB = 0);
		void convert(PPTX::Logic::Duotone						*oox_effect, DWORD ARGB = 0);
//.chart............................................................................................................................
		void convert(OOX::Spreadsheet::CT_ChartSpace			*oox_chart);
		void convert(OOX::Spreadsheet::CT_View3D				*oox_view3D);
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
		void convert_before(OOX::Spreadsheet::CT_Bar3DChart		*chart);
		void convert(OOX::Spreadsheet::CT_BarChart				*chart);
		void convert_before(OOX::Spreadsheet::CT_BarChart		*chart);
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
		
		void convert(OOX::CSizeAnchor *sz_anchor, double x0, double y0, double width, double height);
//.chart.ex...........................................................................................................................
		void convert(OOX::Spreadsheet::ChartEx::CChartSpace		*oox_chart);
		void convert(OOX::Spreadsheet::ChartEx::CTitle			*ct_title);
		void convert(OOX::Spreadsheet::ChartEx::CChart			*oox_chart);
		void convert(OOX::Spreadsheet::ChartEx::CChartData		*oox_chart_data);
		void convert(OOX::Spreadsheet::ChartEx::CPlotArea		*oox_plot_area);
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
		void convert(SimpleTypes::Vml::CCssStyle		*vml_style);
		void convert(OOX::VmlWord::CWrap				*vml_wrap);
		void convert(OOX::Vml::CGroup					*vml_group);
		void convert(OOX::Vml::CVmlCommonElements		*vml_attr);
		void convert(OOX::Vml::CFormulas				*vml_formulas);

		void convert(OOX::Drawing::COfficeArtExtensionList		*ext_list);
		void convert(OOX::Drawing::COfficeArtExtension			*art_ext);

		std::wstring GetImageIdFromVmlShape(OOX::Vml::CVmlCommonElements* pShape);
//math............................................................................................................................
		std::vector<std::vector<std::wstring>>& brackets();
		int& lvl_of_me();
		std::vector<int>& end_counter();
		std::wstring& annotation();
		bool& annotation_flag();
		void mrow();
		void endOfMrow();
		void resizeBrackets();
		void convert(OOX::Logic::COMath					*oox_math);
		void convert(OOX::Logic::CMathPr				*oox_math_pr);
		bool convert(OOX::Logic::CCtrlPr				*oox_ctrl_pr);
		void convert(OOX::Logic::CAcc					*oox_acc);
		void convert(OOX::Logic::CAccPr					*oox_acc_pr);
		void convert(OOX::Logic::CArgPr					*oox_arg_pr);
		void convert(OOX::Logic::CBar					*oox_bar);
		bool convert(OOX::Logic::CBarPr					*oox_bar_pr);
		void convert(OOX::Logic::CBorderBox				*oox_border_box);
		void convert(OOX::Logic::CBorderBoxPr			*oox_border_box_pr);
		void convert(OOX::Logic::CBox					*oox_box);
		bool convert(OOX::Logic::CBoxPr					*oox_box_pr);
		void convert(OOX::Logic::CBrk					*oox_brk);
		void convert(OOX::Logic::CDelimiter				*oox_del);
		std::pair<std::wstring, std::wstring> convert(OOX::Logic::CDelimiterPr			*oox_del_pr);
		void convert(OOX::Logic::CEqArr					*oox_eq_arr);
		void convert(OOX::Logic::CEqArrPr				*oox_eq_arr_pr);
		void convert(OOX::Logic::CBaseJc				*oox_base_jc);
		void convert(OOX::Logic::CMaxDist				*oox_max_dist);
		void convert(OOX::Logic::CObjDist				*oox_obj_dist);
		void convert(OOX::Logic::CRSp					*oox_r_sp);
		void convert(OOX::Logic::CRSpRule				*oox_r_sp_rule);
		std::wstring convert(OOX::Logic::CFPr			*oox_f_pr);
		void convert(OOX::Logic::CFraction				*oox_fraction);
		void convert(OOX::Logic::CFunc					*oox_func);
		void convert(OOX::Logic::CFuncPr				*oox_func_pr);
		void convert(OOX::Logic::CGroupChr				*oox_group_ch);
		bool convert(OOX::Logic::CGroupChrPr			*oox_group_ch_pr);
		void convert(OOX::Logic::CLimLow				*oox_lim_low);
		void convert(OOX::Logic::CLimLowPr				*oox_lim_low_pr);
		void convert(OOX::Logic::CLimUpp				*oox_lim_upp);
		void convert(OOX::Logic::CLimUppPr				*oox_lim_upp_pr);
		void convert(OOX::Logic::CMathFont				*oox_math_font);
		void convert(OOX::Logic::CMatrix				*oox_matrix);
		void convert(OOX::Logic::CMc					*oox_mc);
		void convert(OOX::Logic::CMcPr					*oox_mc_pr);
		void convert(OOX::Logic::CMcs					*oox_mcs);
		void convert(OOX::Logic::CMDel					*oox_m_del);
		void convert(OOX::Logic::CMIns					*oox_m_ins);
		void convert(OOX::Logic::CMPr					*oox_m_pr);
		void convert(OOX::Logic::CMr					*oox_mr);
		void convert(OOX::Logic::CMRun					*oox_run);
		void convert(OOX::Logic::CMText					*oox_text);
		void convert(OOX::Logic::CNary					*oox_nary);
		std::vector<bool> convert(OOX::Logic::CNaryPr *oox_nary_pr);
		void convert(OOX::Logic::CGrow					*oox_grow);
		void convert(OOX::Logic::COMathPara				*oox_math_para);
		void convert(OOX::Logic::COMathParaPr			*oox_math_para_pr);
		void convert(OOX::Logic::CPhant					*oox_phant);
		void convert(OOX::Logic::CPhantPr				*oox_phant_pr);
		void convert(OOX::Logic::CRad					*oox_rad);
		bool convert(OOX::Logic::CRadPr					*oox_rad_pr);
		void convert(OOX::Logic::CSPre					*oox_s_pre);
		void convert(OOX::Logic::CSPrePr				*oox_s_pre_pr);
		void convert(OOX::Logic::CSSub					*oox_ssub);
		void convert(OOX::Logic::CSSubPr				*oox_ssub_pr);
		void convert(OOX::Logic::CSSubSup				*oox_ssub_sup);
		void convert(OOX::Logic::CSSubSupPr				*oox_ssub_sup_pr);
		void convert(OOX::Logic::CAlnScr				*oox_aln_scr);
		void convert(OOX::Logic::CSSup					*oox_ssup);
		void convert(OOX::Logic::CSSupPr				*oox_ssup_pr);
		void convert(OOX::Logic::CNum					*oox_num);
		void convert(OOX::Logic::CDen					*oox_den);
		std::wstring convert(OOX::Logic::CBegChr				*oox_beg_chr);
		std::wstring convert(OOX::Logic::CEndChr				*oox_end_chr);
		void convert(OOX::Logic::CElement				*oox_elm);
		bool convert(OOX::Logic::CDegHide				*oox_deg_hide);
		void convert(OOX::Logic::CDeg *oox_deg, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CSup *oox_sup, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CSub *oox_sub, OOX::Logic::CElement *oox_elm);
		void convert(OOX::Logic::CFName					*oox_fname);
		void convert(OOX::Logic::CLim					*oox_lim);
		bool convert(OOX::Logic::CChr					*oox_chr);
		void convert(OOX::Logic::CSup					*oox_csup);
		void convert(OOX::Logic::CSub					*oox_csub);
		bool convert(OOX::Logic::CSubHide				*oox_subHide);
		bool convert(OOX::Logic::CSupHide				*oox_supHide);
		std::wstring convert(OOX::Logic::CType			*oox_type);
		void convert(OOX::Logic::CDiff					*oox_diff);
		void convert(OOX::Logic::CArgSz					*oox_arg_sz);
		bool convert(OOX::Logic::COpEmu					*oox_op_emu);
		bool convert(OOX::Logic::CPos					*oox_pos);
		void convert(OOX::Logic::CVertJc				*oox_vert_jc);
		void convert(OOX::Logic::CNoBreak				*oox_no_break);
		void convert(OOX::Logic::CCount					*oox_count);
		void convert(OOX::Logic::CMcJc					*oox_mc_jc);
		void convert(OOX::Logic::CPlcHide				*oox_plc_hide);
		bool convert(OOX::Logic::CRunProperty			*oox_r_pr);

//-----------------------------------
		void RGB2HSL(DWORD argb, double& dH, double& dS, double& dL);
		DWORD HSL2RGB(double dH, double dS, double dL);
	private:
		std::map<std::wstring, OOX::Vml::CShapeType*> m_mapVmlShapeTypes;

	};

} // namespace Oox2Odf
