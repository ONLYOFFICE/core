#pragma once

#include "Converter.h"
#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
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
	}
}

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
		cpdoccore::odf::package::odf_document	*output_document;

		cpdoccore::odf::ods_conversion_context	*ods_context;

		void convert_sheets();
		void convert_styles();
		
		void convert(OOX::Spreadsheet::CWorksheet *oox_sheet);
		
		void convert(OOX::Spreadsheet::CFill * fill, cpdoccore::odf::office_element_ptr  & odf_style_);
		void convert(OOX::Spreadsheet::CFont * font, cpdoccore::odf::office_element_ptr  & odf_style_);
	};
}