#pragma once

#include <boost_filesystem_version.h>
#include <boost/lexical_cast.hpp>
#include <cpdoccore/CPOptional.h>

#include <atlcoll.h>

#include "..\progressCallback.h"

static std::wstring string2std_string(CString val)
{
	return std::wstring(val.GetBuffer());
}
namespace cpdoccore
{
	namespace odf 
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

		class color;
		class length;
		class text_align;
		class font_size;
		class length_or_percent;

	}
}
namespace OOX
{
	class WritingElement;
	class CTheme;
	class IFileContainer;

	namespace Drawing
	{
		class CPicture;
		class CChart;
		class CGraphic;
		class CInline;
		class CAnchor;
		class CNonVisualDrawingProps;
		class CShapeProperties;
		class CGroupShapeProperties;
		class CCustomGeometry2D;
		class CPresetGeometry2D;
		class CBlipFillProperties;
		class CGradientFillProperties;
		class CPatternFillProperties;
		class CSolidColorFillProperties;
		class CPath2D;
		class CPath2DLineTo;
		class CPath2DMoveTo;
		class CPath2DArcTo;
		class CPath2DQuadBezierTo;
		class CPath2DCubicBezierTo;
		class CPath2DClose;
		class CHslColor;
		class CSchemeColor;
		class CScRgbColor;
		class CSRgbColor;
		class CSystemColor;
		class CPresetColor;
		class CLineProperties;
		class CTextBodyProperties;
		class CParagraph;
		class CParagraphProperty;
		class CRun;
		class CRunProperty;
		class CLineSpacing;
		class CColor;
		class CShapeStyle;
		class CStyleMatrixReference;
		class CEffectList;
		class COuterShadowEffect;
		class CInnerShadowEffect;

		namespace Colors
		{
			class CColorTransform;
		}
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
		class CRichText;
		class CTextProperties;
	}


	namespace Vml
	{
		class CVmlShapeElements;
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

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual void convertDocument() = 0;
		virtual void write(const std::wstring & path) = 0;
		
		OoxConverter(const ProgressCallback* CallBack = NULL){	oox_current_child_document_spreadsheet = NULL; 
																oox_current_child_document = NULL; 
																
																pCallBack = CallBack;
																bUserStopConvert = 0;}
		
		const ProgressCallback* pCallBack;
		short bUserStopConvert;
		BOOL UpdateProgress(long nComplete);
	
//.......................................................................................................................
		virtual cpdoccore::odf::odf_conversion_context		*odf_context() = 0;
		virtual OOX::CTheme									*oox_theme() = 0;
		virtual CString										 find_link_by_id(CString sId, int t) = 0;
		
		OOX::Spreadsheet::IFileContainer					*oox_current_child_document_spreadsheet;
		OOX::IFileContainer									*oox_current_child_document;
//.......................................................................................................................
		void convert(OOX::WritingElement  *oox_unknown);
		void convert(double oox_font_size,			cpdoccore::_CP_OPT(cpdoccore::odf::font_size) & odf_font_size);
	
//.drawing......................................................................................................................
		void convert(OOX::Drawing::CNonVisualDrawingProps		*oox_cnvPr);
		void convert(OOX::Drawing::CShapeProperties				*oox_spPr, OOX::Drawing::CShapeStyle* oox_sp_style = NULL);
		void convert(OOX::Drawing::CGroupShapeProperties		*oox_groupSpPr);
		void convert(OOX::Drawing::CTextBodyProperties			*oox_bodyPr);
		
		void convert(OOX::Drawing::CCustomGeometry2D			*oox_cust_geom);
		void convert(OOX::Drawing::CPresetGeometry2D			*oox_prst_geom);

		void convert(OOX::Drawing::CLineProperties				*oox_line_prop,		CString *change_sheme_color = NULL);
		
		void convert(OOX::Drawing::CBlipFillProperties			*oox_bitmap_fill,	CString *change_sheme_color = NULL);
		void convert(OOX::Drawing::CGradientFillProperties		*oox_grad_fill	,	CString *change_sheme_color = NULL);
		void convert(OOX::Drawing::CPatternFillProperties		*oox_pattern_fill,	CString *change_sheme_color = NULL);
		void convert(OOX::Drawing::CSolidColorFillProperties	*oox_solid_fill	,	CString *change_sheme_color = NULL);

		void convert(OOX::Drawing::CEffectList					*oox_effect_list,	CString *change_sheme_color = NULL);
		void convert(OOX::Drawing::COuterShadowEffect			*oox_shadow,		CString *change_sheme_color = NULL);
		void convert(OOX::Drawing::CInnerShadowEffect			*oox_shadow,		CString *change_sheme_color = NULL);

		void convert(OOX::Drawing::CStyleMatrixReference		*style_matrix_ref);
		void convert(OOX::Drawing::CPath2D						*oox_geom_path);
		void convert(OOX::Drawing::CPath2DLineTo				*oox_geom_path);
		void convert(OOX::Drawing::CPath2DMoveTo				*oox_geom_path);
		void convert(OOX::Drawing::CPath2DArcTo					*oox_geom_path);
		void convert(OOX::Drawing::CPath2DQuadBezierTo			*oox_geom_path);
		void convert(OOX::Drawing::CPath2DCubicBezierTo			*oox_geom_path);
		void convert(OOX::Drawing::CPath2DClose					*oox_geom_path);
		void convert(OOX::Drawing::CColor						*oox_color,		std::wstring & hexColor , cpdoccore::_CP_OPT(double) &opacity);
		void convert(OOX::Drawing::CSchemeColor					*oox_ShemeClr,	std::wstring & hexString, cpdoccore::_CP_OPT(double) &opacity);
		void convert(OOX::Drawing::Colors::CColorTransform      *oox_ScrgbClr,	std::wstring & hexString, cpdoccore::_CP_OPT(double) &opacity);
		void convert(OOX::Drawing::CSolidColorFillProperties	*oox_solid_fill,std::wstring & hexColor , cpdoccore::_CP_OPT(double) &opacity);

		void convert(OOX::Drawing::CParagraph					*oox_paragraph);
		void convert(OOX::Drawing::CParagraphProperty			*oox_paragraph_pr, cpdoccore::odf::style_paragraph_properties * paragraph_properties);
		void convert(OOX::Drawing::CRun							*oox_run);
		void convert(OOX::Drawing::CRunProperty					*oox_run_pr, cpdoccore::odf::style_text_properties	* text_properties);
		void convert(OOX::Drawing::CLineSpacing					*oox_spacing, cpdoccore::odf::length_or_percent & length_or_percent);
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
		void convert(OOX::Spreadsheet::CRichText				*rich);
		void convert(OOX::Spreadsheet::CTextProperties			*txPr);
		void convert(OOX::Spreadsheet::CT_Tx					*ct_tx);
		void convert(OOX::Spreadsheet::CT_Layout				*ct_layout);
		void convert(OOX::Spreadsheet::CT_ManualLayout			*ct_layout);
		void convert(OOX::Spreadsheet::CT_ChartLines			*grid, int type);
		void convert(OOX::Spreadsheet::CT_SerTx					*ser_tx);
		void convert(OOX::Spreadsheet::CT_DLbls					*ser_lbls);
		void convert(OOX::Spreadsheet::CT_Marker				*marker);
		void convert(OOX::Spreadsheet::CT_Marker				*marker, CAtlArray<OOX::Spreadsheet::CT_DPt*> & dPt);
		void convert(OOX::Spreadsheet::CT_ExternalData			*external_data);
		void convert(OOX::Spreadsheet::CT_NumData				*num_data);
		void convert(OOX::Spreadsheet::CT_StrData				*str_data, bool label = false);


//.vml............................................................................................................................
		void convert(OOX::Vml::CVmlShapeElements		*vml);

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
	};

	class Converter
	{

	public:
		Converter(const std::wstring & path, std::wstring  type, const ProgressCallback* ffCallBack);
        virtual ~Converter();

	public:
		void convert();
     
        void write(const std::wstring & path) const;

		OoxConverter * get_ooxConverter() { return impl_; }

	private:
		OoxConverter* impl_;
	
	};
} // namespace Oox2Odf:Convert
