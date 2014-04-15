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
		class CCustomGeometry2D;
		class CPresetGeometry2D;
		class CBlipFillProperties;
		class CGradientFillProperties;
		class CPatternFillProperties;
		class CSolidColorFillProperties;
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
		
		void convert(OOX::Drawing::CCustomGeometry2D *oox_cust_geom);
		void convert(OOX::Drawing::CPresetGeometry2D *oox_prst_geom);
		
		void convert(OOX::Drawing::CBlipFillProperties *oox_bitmap_fill);
		void convert(OOX::Drawing::CGradientFillProperties *oox_grad_fill);
		void convert(OOX::Drawing::CPatternFillProperties *oox_pattern_fill);
		void convert(OOX::Drawing::CSolidColorFillProperties *oox_solid_fill);
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
