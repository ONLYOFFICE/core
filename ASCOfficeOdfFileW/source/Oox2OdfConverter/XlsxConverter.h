#pragma once

#include "Converter.h"
#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>
#include <XlsxFormat\Xlsx.h>

namespace OOX 
{
	class CDocx;
}

namespace cpdoccore
{
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

		virtual void convert();
		virtual void write(const std::wstring & path);

    private:
		OOX::Spreadsheet::CXlsx		*xlsx_document;
		odf::package::odf_document	*output_document;

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

		void convert(OOX::Spreadsheet::CFormula *oox_formula);
		void convert(OOX::Spreadsheet::CHyperlink *oox_hyperlink,OOX::Spreadsheet::CWorksheet *oox_sheet);

		void convert(OOX::Spreadsheet::CSheetFormatPr	*oox_sheet_format_pr);
		void convert(OOX::Spreadsheet::CSheetPr			*oox_sheet_pr);
		
		void convert(OOX::Spreadsheet::CFont * font, odf::style_text_properties * text_properties);
		
		void convert(OOX::Spreadsheet::CNumFmt	*numFmt,	odf::style_table_cell_properties * cell_properties);
		void convert(OOX::Spreadsheet::CBorder	*border,	odf::style_table_cell_properties * cell_properties);
		void convert(OOX::Spreadsheet::CFill	*fill,		odf::style_table_cell_properties * cell_properties);
		
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color);
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::background_color) & odf_bckgrd_color);
		
		void convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size);
		void convert(double oox_size,  _CP_OPT(odf::length) & odf_size);

		void convert(OOX::Spreadsheet::CXfs *		cell_style, int oox_id, bool automatic=true, bool root = false);
		void convert(OOX::Spreadsheet::CCellStyle *	cell_style, int oox_id);

		void convert_sharing_string(int number);
	};
}