#pragma once

#include "Converter.h"
#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

namespace OOX 
{
	class CDocx;
	class CTheme;

	namespace Spreadsheet
	{
		class CXlsx;
		class WritingElement;

		class CWorksheet;
		class CDrawing;
		class CCol;
		class CRow;
		class CCell;
		class CRun;
		class CRPr;
		class CText;
		class CFormula;
		class CHyperlink;
		class CSheetFormatPr;
		class CSheetPr;		
		class CFont;
		class CBorder;
		class CFill;		
		class CColor;
		class CBorderProp;
		class CXfs;
		class CCellStyle;
		class CNumFmt;
		class CCellAnchor;
		class CDrawing;
		class CFromTo;
		class CPic;
		class CShape;
		class CConnShape;
	}
}

namespace cpdoccore
{
	struct oox_table_position;

namespace odf 
	{
		class office_element;
		typedef shared_ptr<office_element>::Type office_element_ptr;

		class style_text_properties;
		class style_paragraph_properties;
		class style_graphic_properties;
		class style_table_properties;
		class style_section_properties;
		class style_table_cell_properties;
		class style_table_row_properties;
		class style_table_column_properties;
		class style_chart_properties;
		class style_drawing_page_properties;

		namespace package 
		{		
			class odf_document;
		}
		class odf_conversion_context;
		class ods_conversion_context;
		class color;
		class background_color;
		class font_size;
		class length;
	}
}

using namespace cpdoccore;

namespace Oox2Odf
{

	class XlsxConverter : public OoxConverter
	{
	public:
		XlsxConverter(const std::wstring & path);

		virtual void convertDocument();
		virtual void write(const std::wstring & path);
		
		virtual odf::odf_conversion_context* odf_context();
		virtual OOX::CTheme* oox_theme();

    private:
		OOX::Spreadsheet::CXlsx		*xlsx_document;
		odf::package::odf_document	*output_document;
		OOX::Spreadsheet::CDrawing	*xlsx_current_drawing; //пока сюда .. потом покрасивше, для внешних ссылок

		odf::ods_conversion_context	*ods_context;

		void convert_sheets();
		void convert_styles();
		
		void convert(OOX::Spreadsheet::WritingElement  *oox_unknown);
		
		void convert(OOX::Spreadsheet::CWorksheet *oox_sheet);

		void convert(OOX::Spreadsheet::CCol *oox_column);
		void convert(OOX::Spreadsheet::CRow *oox_row);
		void convert(OOX::Spreadsheet::CCell *oox_cell);

		void convert(OOX::Spreadsheet::CRun *oox_text_run);
		void convert(OOX::Spreadsheet::CRPr *oox_text_pr);
		void convert(OOX::Spreadsheet::CText *oox_text);

		void convert(OOX::Spreadsheet::CFormula		*oox_formula);
		void convert(OOX::Spreadsheet::CHyperlink	*oox_hyperlink,	OOX::Spreadsheet::CWorksheet *oox_sheet);

		void convert(OOX::Spreadsheet::CSheetFormatPr	*oox_sheet_format_pr);
		void convert(OOX::Spreadsheet::CSheetPr			*oox_sheet_pr);
		
		void convert(OOX::Spreadsheet::CFont	*font,		odf::style_text_properties		*text_properties);		
		void convert(OOX::Spreadsheet::CBorder	*border,	odf::style_table_cell_properties *cell_properties);
		void convert(OOX::Spreadsheet::CFill	*fill,		odf::style_table_cell_properties *cell_properties);
		
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color);
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::background_color) & odf_bckgrd_color);
		void convert(OOX::Spreadsheet::CBorderProp *borderProp, std::wstring & odf_border_prop);
		
		void convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size);
		void convert(double oox_size,  _CP_OPT(odf::length) & odf_size);

		void convert(OOX::Spreadsheet::CXfs			*cell_style, int oox_id, bool automatic=true, bool root = false);
		void convert(OOX::Spreadsheet::CCellStyle	*cell_style, int oox_id);
		void convert(OOX::Spreadsheet::CNumFmt		*numFmt);

		void convert(OOX::Spreadsheet::CCellAnchor *oox_anchor);
		void convert(OOX::Spreadsheet::CDrawing *oox_drawing);

		void convert_sharing_string(int number);
		void convert(OOX::Spreadsheet::CFromTo* oox_from_to, oox_table_position * pos);

		void convert(OOX::Spreadsheet::CPic*	oox_picture);
		void convert(OOX::Spreadsheet::CShape*	oox_shape);
		void convert(OOX::Spreadsheet::CConnShape*	oox_conn_shape);
	};

}