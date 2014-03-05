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
	}
}

namespace Oox2Odf
{
	class DocxConverter : public OoxConverter
	{
	public:
		DocxConverter(const std::wstring & path);

		virtual void convert();
		virtual void write(const std::wstring & path);

    private:
		OOX::CDocx								*docx_document;
		cpdoccore::odf::package::odf_document	*output_document;

	///////////////////////////////
	};
}