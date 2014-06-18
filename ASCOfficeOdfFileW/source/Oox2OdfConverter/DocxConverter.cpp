#pragma once

#include "stdAfx.h"

#include "DocxConverter.h"

#include <DocxFormat\Docx.h>

#include <boost/foreach.hpp>

#include "odf_conversion_context.h"
#include "odt_conversion_context.h"

#include "odf_text_context.h"
#include "odf_drawing_context.h"

#include "styles.h"

#include "style_table_properties.h"
#include "style_text_properties.h"
#include "style_paragraph_properties.h"
#include "style_graphic_properties.h"


using namespace cpdoccore;

namespace Oox2Odf
{
DocxConverter::DocxConverter(const std::wstring & path)
{
	output_document = new  odf::package::odf_document(L"text");

	const OOX::CPath oox_path(CString(path.c_str()));	
	docx_document = new OOX::CDocx(oox_path);	
}
void DocxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);

}
odf::odf_conversion_context* DocxConverter::odf_context()
{
	return odt_context;
}
OOX::CTheme* DocxConverter::oox_theme()
{
	return NULL;
}
CString	DocxConverter::find_link_by_id (CString sId, int type)
{
	CString ref;
	//if (type==1)
	//{
	//	if (docx_current_drawing)
	//	{
	//		smart_ptr<OOX::File> oFile = docx_current_drawing->Find(sId);
	//		if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
	//		{
	//			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

	//			ref = pImage->filename().GetPath();
	//		}
	//	}
	//	if (ref.GetLength() < 1 && oox_current_chart)
	//	{
	//		smart_ptr<OOX::File> oFile = oox_current_chart->Find(sId);
	//		if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
	//		{
	//			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

	//			ref = pImage->filename().GetPath();
	//		}
	//	}
	//}
	return ref;
}

void DocxConverter::convertDocument()
{
	if (!docx_document)return;

	odt_context = new odf::odt_conversion_context(output_document);

	if (!odt_context)return;
		
	odt_context->start_document();

	//convert_styles();
	convert_document();

	//удалим уже ненужный документ docx 
	delete docx_document; docx_document = NULL;

	odt_context->end_document();

}

void DocxConverter::convert_document()
{
	const OOX::CDocument* document = docx_document->GetDocument();
	if (!document)return;

	for ( int nIndex = 0; nIndex < document->m_arrItems.GetSize(); nIndex++ )
	{
		convert(document->m_arrItems[nIndex]);
	}
}
void DocxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_w_r:
		{
			OOX::Logic::CRun* pRun= static_cast<OOX::Logic::CRun*>(oox_unknown);
			convert(pRun);
		}break;
		case OOX::et_w_p:
		{
			OOX::Logic::CParagraph* pP= static_cast<OOX::Logic::CParagraph*>(oox_unknown);
			convert(pP);
		}break;
		case OOX::et_w_pPr:
		{
			OOX::Logic::CParagraphProperty* pPProp= static_cast<OOX::Logic::CParagraphProperty*>(oox_unknown);
			convert(pPProp);
		}break;
		case OOX::et_w_rPr:
		{
			OOX::Logic::CRunProperty* pRProp= static_cast<OOX::Logic::CRunProperty*>(oox_unknown);
			convert(pRProp);
		}break;
		case OOX::et_w_t:
		{
			OOX::Logic::CText* pText= static_cast<OOX::Logic::CText*>(oox_unknown);
			convert(pText);
		}break;
		case OOX::et_mc_alternateContent:
		{
			OOX::Logic::CAlternateContent* pAltCont= static_cast<OOX::Logic::CAlternateContent*>(oox_unknown);
			convert(pAltCont);
		}break;
		case OOX::et_w_drawing:
		{
			OOX::Logic::CDrawing* pDrawing= static_cast<OOX::Logic::CDrawing*>(oox_unknown);
			convert(pDrawing);
		}
		default:
		{
			OoxConverter::convert(oox_unknown);
		}
	}
}
void DocxConverter::convert(OOX::Logic::CParagraph	*oox_paragraph)
{
	if (oox_paragraph == NULL) return;

	odt_context->start_paragraph();
	for ( int nIndex = 0; nIndex < oox_paragraph->m_arrItems.GetSize(); nIndex++ )
	{
		convert(oox_paragraph->m_arrItems[nIndex]);
	}
	odt_context->end_paragraph();
}
void DocxConverter::convert(OOX::Logic::CRun	*oox_run)
{
	if (oox_run == NULL) return;
	
	odt_context->start_run();	
	for(int i = 0; i < oox_run->m_arrItems.GetSize(); ++i)
	{
		convert(oox_run->m_arrItems[i]);
	}
	odt_context->end_run();

}
void DocxConverter::convert(OOX::Logic::CParagraphProperty	*oox_paragraph_prop)
{
	if (oox_paragraph_prop == NULL) return;

}
void DocxConverter::convert(OOX::Logic::CRunProperty *oox_run_prop)
{
	if (oox_run_prop == NULL) return;

}
void DocxConverter::convert(OOX::Logic::CText *oox_text)
{
	if (oox_text == NULL) return;

	odt_context->add_text_content(string2std_string(oox_text->m_sText));
}
void DocxConverter::convert(OOX::Logic::CAlternateContent *oox_alt_content)
{
	if (oox_alt_content == NULL)return;

	for(int i = 0; i < oox_alt_content->m_arrChoiceItems.GetSize(); ++i)
	{
		convert(oox_alt_content->m_arrChoiceItems[i]);
	}
	for(int i = 0; i < oox_alt_content->m_arrFallbackItems.GetSize(); ++i)
	{
		convert(oox_alt_content->m_arrFallbackItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CDrawing *oox_drawing)
{
	if (oox_drawing == NULL) return;

	convert(oox_drawing->m_oAnchor.GetPointer());
	convert(oox_drawing->m_oInline.GetPointer());
}
void DocxConverter::convert(OOX::Drawing::CAnchor *oox_anchor)
{
	if (oox_anchor == NULL)return;

}
void DocxConverter::convert(OOX::Drawing::CInline *oox_inline)
{
	if (oox_inline == NULL)return;

}
} 