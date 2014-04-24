#pragma once

#include <boost_filesystem_version.h>
#include <cpdoccore/CPOptional.h>

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
	class WritingElement;
	class CTheme;

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
		//class ;

		namespace Colors
		{
			class CColorTransform;
		}
	}
}

namespace Oox2Odf
{
	class OoxConverter
	{
public:
		virtual void convertDocument() = 0;
		virtual void write(const std::wstring & path) = 0;
		
		OoxConverter(){}

		virtual cpdoccore::odf::odf_conversion_context* odf_context() = 0;
		virtual OOX::CTheme* oox_theme() = 0;

		void convert(OOX::WritingElement  *oox_unknown);
	
		void convert_CNvPr(OOX::Drawing::CNonVisualDrawingProps	* oox_cnvPr);
		void convert_SpPr(OOX::Drawing::CShapeProperties		* oox_spPr);
		
		void convert(OOX::Drawing::CCustomGeometry2D	*oox_cust_geom);
		void convert(OOX::Drawing::CPresetGeometry2D	*oox_prst_geom);

		void convert(OOX::Drawing::CLineProperties		*oox_line_prop);
		
		void convert(OOX::Drawing::CBlipFillProperties			*oox_bitmap_fill);
		void convert(OOX::Drawing::CGradientFillProperties		*oox_grad_fill);
		void convert(OOX::Drawing::CPatternFillProperties		*oox_pattern_fill);
		void convert(OOX::Drawing::CSolidColorFillProperties	*oox_solid_fill);
//////////////////////////////////////////////
		void convert(OOX::Drawing::CPath2D						*oox_geom_path);
		void convert(OOX::Drawing::CPath2DLineTo				*oox_geom_path);
		void convert(OOX::Drawing::CPath2DMoveTo				*oox_geom_path);
		void convert(OOX::Drawing::CPath2DArcTo					*oox_geom_path);
		void convert(OOX::Drawing::CPath2DQuadBezierTo			*oox_geom_path);
		void convert(OOX::Drawing::CPath2DCubicBezierTo			*oox_geom_path);
		void convert(OOX::Drawing::CPath2DClose					*oox_geom_path);
/////////////////////////////////////////
		void convert(OOX::Drawing::CSchemeColor					*oox_ShemeClr,	std::wstring & hexString, cpdoccore::_CP_OPT(double) &opacity);
		void convert(OOX::Drawing::Colors::CColorTransform      *oox_ScrgbClr,	std::wstring & hexString, cpdoccore::_CP_OPT(double) &opacity);

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
