#pragma once

#include "Converter.h"

namespace OOX 
{
	class CDocx;
	class CTheme;

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

		void convert(OOX::WritingElement				*oox_unknown);

		void convert(OOX::Logic::CParagraph				*oox_paragraph);
		void convert(OOX::Logic::CRun					*oox_run);
		void convert(OOX::Logic::CParagraphProperty		*oox_paragraph_prop);
		void convert(OOX::Logic::CRunProperty			*oox_run_prop);
		void convert(OOX::Logic::CText					*oox_text);
		void convert(OOX::Logic::CAlternateContent		*oox_alt_content);
		void convert(OOX::Logic::CDrawing				*oox_drawing);
		void convert(OOX::Drawing::CAnchor				*oox_anchor);
		void convert(OOX::Drawing::CInline				*oox_inline);
		void convert(OOX::Drawing::CGraphic				*oox_graphic);
		void convert(OOX::Logic::CGroupShape			*oox_group_shape);
		void convert(OOX::Logic::CShape					*oox_shape);
		void convert(OOX::Drawing::CChart				*oox_chart);
		void convert(OOX::Drawing::CPicture				*oox_chart);

	};
}