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
		OOX::Spreadsheet::CXlsx					*xlsx_document;
		odf::package::odf_document	*output_document;

		odf::ods_conversion_context	*ods_context;

		void convert_sheets();
		void convert_styles();
		
		void convert(OOX::Spreadsheet::CWorksheet *oox_sheet);

		void convert(OOX::Spreadsheet::CCol *oox_column);
		void convert(OOX::Spreadsheet::CRow *oox_row);
		void convert(OOX::Spreadsheet::CCell *oox_cell);

		void convert(OOX::Spreadsheet::CSheetFormatPr *oox_sheet_format);
		//void convert(OOX::Spreadsheet::CSheetPr *oox_sheet_format);
		
		void convert(OOX::Spreadsheet::CFill * fill, odf::office_element_ptr  & odf_style_);
		void convert(OOX::Spreadsheet::CFont * font, odf::office_element_ptr  & odf_style_);
		void convert(OOX::Spreadsheet::CNumFmt *numFmt, odf::office_element_ptr  & odf_style_);
		void convert(OOX::Spreadsheet::CBorder *border, odf::office_element_ptr  & odf_style_);
		
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color);
		void convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::background_color) & odf_bckgrd_color);
		
		void convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size);
		void convert(double oox_size,  _CP_OPT(odf::length) & odf_size);

		void convert(OOX::Spreadsheet::CXfs *		cell_style, int oox_id, bool automatic=true, bool root = false);
		void convert(OOX::Spreadsheet::CCellStyle *	cell_style, int oox_id);
	};
}