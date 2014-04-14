#pragma once

#include <boost_filesystem_version.h>

static std::wstring string2std_string(CString val)
{
	return std::wstring(val.GetBuffer());
}
namespace cpdoccore
{
	namespace odf 
	{

		class odf_conversion_context;
	}
}
namespace OOX
{
	namespace Drawing
	{
		class CNonVisualDrawingProps;
		class CShapeProperties;
	}
}

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual void convert() = 0;
		virtual void write(const std::wstring & path) = 0;
		
		OoxConverter(){}

		virtual cpdoccore::odf::odf_conversion_context* odf_context() = 0;
	
		void convert_CNvPr(OOX::Drawing::CNonVisualDrawingProps* oox_cnvPr);
		void convert_SpPr(OOX::Drawing::CShapeProperties* oox_spPr);
	};

	class Converter
	{

	public:
		Converter(const std::wstring & path);
        virtual ~Converter();

	public:
		void convert();
     
        void write(const std::wstring & path) const;

		OoxConverter * get_ooxConverter() { return impl_; }

	private:
		OoxConverter* impl_;
	
	};
} // namespace Oox2Odf:Convert
