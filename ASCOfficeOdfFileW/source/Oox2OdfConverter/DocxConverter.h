#pragma once

#include "Converter.h"

#include <boost/foreach.hpp>
#include <cpdoccore/CPSharedPtr.h>
#include <cpdoccore/CPOptional.h>

#include <Common/SimpleTypes_Word.h>

namespace OOX 
{
	class CDocx;
	class CTheme;
	class CDocDefaults;
	class CStyle;

	namespace Logic
	{
		class CParagraph;
		class CParagraphProperty;
		class CRun;
		class CRunProperty;
		class CText;
		class CAlternateContent;
		class CDrawing;	
		class CGroupShape;
		class CShape;
		class CPBdr;
		class CHyperlink;
		class CFldChar;
		class CInstrText;
	}

}
namespace ComplexTypes
{
	namespace Word
	{
		class CColor;
		class CJc;
		class CBorder;
		class CShading;
	}
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
		class odt_conversion_context;
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
		virtual CString							 find_link_by_id (CString sId, int t);

    private:
		OOX::CDocx								*docx_document;
		cpdoccore::odf::package::odf_document	*output_document;
		
		odf::odt_conversion_context				*odt_context;
		OOX::Logic::CDrawing					*docx_current_drawing; //пока сюда .. потом покрасивше, для внешних ссылок

		void convert_document();
		void convert_styles();

		void convert(OOX::WritingElement				*oox_unknown);

		void convert(OOX::Logic::CParagraph				*oox_paragraph);
		void convert(OOX::Logic::CRun					*oox_run);
		void convert(OOX::Logic::CParagraphProperty		*oox_paragraph_prop,	odf::style_paragraph_properties *paragraph_properties);
		void convert(OOX::Logic::CRunProperty			*oox_run_prop,			odf::style_text_properties		*text_properties);
		void convert(OOX::Logic::CPBdr					*oox_border,			odf::style_paragraph_properties *paragraph_properties);
		void convert(ComplexTypes::Word::CShading		*oox_shading,			odf::style_text_properties		*text_properties );
		void convert(OOX::Logic::CFldChar				*oox_fld);
		void convert(OOX::Logic::CInstrText				*oox_instr);
		void convert(OOX::Logic::CText					*oox_text);
		void convert(OOX::Logic::CHyperlink				*oox_hyperlink);
		void convert(OOX::Logic::CAlternateContent		*oox_alt_content);
		void convert(OOX::Logic::CDrawing				*oox_drawing);
		void convert(OOX::Drawing::CAnchor				*oox_anchor);
		void convert(OOX::Drawing::CInline				*oox_inline);
		void convert(OOX::Drawing::CGraphic				*oox_graphic);
		void convert(OOX::Logic::CGroupShape			*oox_group_shape);
		void convert(OOX::Logic::CShape					*oox_shape);
		void convert(OOX::Drawing::CChart				*oox_chart);
		void convert(OOX::Drawing::CPicture				*oox_picture);
		void convert(ComplexTypes::Word::CColor			*color,			_CP_OPT(odf::color)				& odf_color);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf::length)			& odf_size);
		void convert(SimpleTypes::CUniversalMeasure		*oox_size,		_CP_OPT(odf::length_or_percent) & odf_size);
		void convert(ComplexTypes::Word::CJc			*oox_jc,		_CP_OPT(odf::text_align)		& align);
		void convert(ComplexTypes::Word::CBorder		*borderProp,	std::wstring & odf_border_prop);
		

		void convert(SimpleTypes::CHexColor<>			*color, SimpleTypes::CThemeColor<>	*theme_color, 
														SimpleTypes::CUcharHexNumber<>* theme_tint,
														SimpleTypes::CUcharHexNumber<>* theme_shade, _CP_OPT(odf::color) & odf_color);
		void convert(OOX::CDocDefaults					*def_style);
		void convert(OOX::CStyle						*style);
	};
}