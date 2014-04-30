#pragma once

#include "Converter.h"

namespace OOX 
{
	class CDocx;
}
namespace cpdoccore 
{
	namespace odf 
	{
		namespace package 
		{		
			class odf_document;
		}
		class ods_conversion_context;
		class odf_conversion_context;
	}
}
using namespace cpdoccore;

namespace Oox2Odf
{
	class DocxConverter : public OoxConverter
	{
	public:
		DocxConverter(const std::wstring & path);

		virtual void convertDocument();
		virtual void write(const std::wstring & path);
		
		virtual odf::odf_conversion_context		*odf_context();
		virtual OOX::CTheme						*oox_theme();
		virtual CString							 find_link_by_id (CString sId, int t) {return L"";}

    private:
		OOX::CDocx								*docx_document;
		cpdoccore::odf::package::odf_document	*output_document;

	///////////////////////////////
	};
}