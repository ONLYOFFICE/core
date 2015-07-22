#include "DocxConverter.h"
#include "../utils.h"

#include "../../../Common/DocxFormat/Source/DocxFormat/Docx.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/External/HyperLink.h"
#include "../../../Common/DocxFormat/Source/XlsxFormat/Chart/Chart.h"
#include "../../../Common/DocxFormat/Source/DocxFormat/Diagram/DiagramDrawing.h"

#include "VmlShapeTypes2Oox.h"

#include <boost/foreach.hpp>
#include <boost/lexical_cast.hpp>

#include "../OdfFormat/odt_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/styles.h"

#include "../OdfFormat/style_table_properties.h"
#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"
#include "../OdfFormat/style_graphic_properties.h"
#include "../OdfFormat/styles_list.h"


using namespace cpdoccore;

namespace Oox2Odf
{
DocxConverter::DocxConverter(const std::wstring & path, const ProgressCallback* CallBack)
{
    const OOX::CPath oox_path(CString(path.c_str()));

    docx_document   = new OOX::CDocx(oox_path);

    output_document = new odf_writer::package::odf_document(L"text");
    odt_context     = new odf_writer::odt_conversion_context(output_document);

	pCallBack = CallBack;

//set flags to default
    last_section_properties = NULL;
	
	if (UpdateProgress(290000))return;
}
DocxConverter::~DocxConverter()
{
	if (odt_context)		delete odt_context;		odt_context		= NULL;
	if (docx_document)		delete docx_document;	docx_document	= NULL;
	if (output_document)	delete output_document;	output_document = NULL;
}
void DocxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);
		
	if (UpdateProgress(1000000))return;
}
odf_writer::odf_conversion_context* DocxConverter::odf_context()
{
	return odt_context;
}
OOX::CTheme* DocxConverter::oox_theme()
{
	if (docx_document)
		return docx_document->GetTheme();
	else
		return NULL;
}
CString	DocxConverter::find_link_by_id (CString sId, int type)
{
	OOX::CDocument  *oox_doc = docx_document->GetDocument();

	if (oox_doc == NULL)return L"";
		
	CString ref;

	if (ref.GetLength() < 1 && oox_current_child_document_spreadsheet)
	{
		smart_ptr<OOX::File> oFile = oox_current_child_document_spreadsheet->Find(sId);
		if (oFile.IsInit())
		{		
			if (type==1 && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
			if (type==2 && oFile.IsInit() && OOX::FileTypes::HyperLink == oFile->type())
			{
				OOX::HyperLink* pHyperlink = (OOX::HyperLink*)oFile.operator->();

				ref = pHyperlink->Uri().GetPath();
			}
		}
	}
	if (ref.GetLength() < 1 && oox_current_child_document)
	{
		smart_ptr<OOX::File> oFile = oox_current_child_document->Find(sId);
		if (oFile.IsInit())
		{		
			if (type==1 && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
			if (type==2 && oFile.IsInit() && OOX::FileTypes::HyperLink == oFile->type())
			{
				OOX::HyperLink* pHyperlink = (OOX::HyperLink*)oFile.operator->();

				ref = pHyperlink->Uri().GetPath();
			}
		}
	}

	smart_ptr<OOX::File> oFile = docx_document->GetDocument()->Find(sId);
	if (ref.GetLength() < 1 && oFile.IsInit())
	{
		if (type==1 && OOX::FileTypes::Image == oFile->type())
		{
			OOX::Image* pImage = (OOX::Image*)oFile.operator->();

			ref = pImage->filename().GetPath();
		}
		if (type == 2 && OOX::FileTypes::HyperLink == oFile->type())
		{
			OOX::HyperLink* pHyperlink = (OOX::HyperLink*)oFile.operator->();

			ref = pHyperlink->Uri().GetPath();
		}
	}

	return ref;
}

void DocxConverter::convertDocument()
{
    if (!docx_document) return;
    if (!odt_context)   return;
		
	odt_context->start_document();

	convert_lists_styles();
	convert_styles();

	if (UpdateProgress(300000))return;

	convert_settings(); 
	
	convert_document();

	if (UpdateProgress(800000))return;
	//удалим уже ненужный документ docx 
	delete docx_document; docx_document = NULL;

	odt_context->end_document();
 	
	if (UpdateProgress(850000))return;
}

void DocxConverter::convert_document()
{
	const OOX::CDocument* document = docx_document->GetDocument();
	if (!document)return;

	odt_context->text_context()->clear_params();

	for (unsigned int nIndex = 0; nIndex < document->m_arrItems.size(); nIndex++ )
	{
		convert(document->m_arrItems[nIndex]);
	}

	convert(document->m_oSectPr.GetPointer(),true);
}
void DocxConverter::convert(OOX::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_w_sdt:
		{
			OOX::Logic::CSdt* pP= static_cast<OOX::Logic::CSdt*>(oox_unknown);
			convert(pP);
		}break;
		case OOX::et_w_fldSimple:	
		{
			OOX::Logic::CFldSimple* pFldS= static_cast<OOX::Logic::CFldSimple*>(oox_unknown);
			convert(pFldS);
		}break;
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
		case OOX::et_w_ins:
		{
			OOX::Logic::CIns* pIns= static_cast<OOX::Logic::CIns*>(oox_unknown);
			convert(pIns);
		}break;
		case OOX::et_w_smartTag:
		{
			OOX::Logic::CSmartTag* pTag= static_cast<OOX::Logic::CSmartTag*>(oox_unknown);
			convert(pTag);
		}break;
		case OOX::et_w_pPr:
		{
			OOX::Logic::CParagraphProperty* pPProp= static_cast<OOX::Logic::CParagraphProperty*>(oox_unknown);
			convert(pPProp);
		}break;
		case OOX::et_w_hyperlink:
		{
			OOX::Logic::CHyperlink* pH= static_cast<OOX::Logic::CHyperlink*>(oox_unknown);
			convert(pH);
		}break;
		case OOX::et_w_rPr:
		{
			OOX::Logic::CRunProperty* pRProp= static_cast<OOX::Logic::CRunProperty*>(oox_unknown);
			convert(pRProp);
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
		}break;
		case OOX::et_c_chart:
		{
			OOX::Drawing::CChart* pChart = static_cast<OOX::Drawing::CChart*>(oox_unknown);
			convert(pChart);
		}break;
		case OOX::et_w_Shape:
		{
			OOX::Logic::CShape* pShape = static_cast<OOX::Logic::CShape*>(oox_unknown);
			convert(pShape);
		}break;
		case OOX::et_w_pict:
		{
			OOX::Logic::CPicture* pPic = static_cast<OOX::Logic::CPicture*>(oox_unknown);
			convert(pPic);
		}break;
		case OOX::et_w_object:
		{
			OOX::Logic::CObject* pObj = static_cast<OOX::Logic::CObject*>(oox_unknown);
			convert(pObj);
		}break;
		case OOX::et_pic_pic:
		{
			OOX::Drawing::CPicture* pPic = static_cast<OOX::Drawing::CPicture*>(oox_unknown);
			convert(pPic);
		}break;
		case OOX::et_w_GroupShape:
		{
			OOX::Logic::CGroupShape* pGroupShape= static_cast<OOX::Logic::CGroupShape*>(oox_unknown);
			convert(pGroupShape);
		}break;
		case OOX::et_w_LockedCanvas:
		{
			OOX::Logic::CLockedCanvas* pLockedCanvas= static_cast<OOX::Logic::CLockedCanvas*>(oox_unknown);
			convert(pLockedCanvas);
		}break;
		case OOX::et_dgm_DiagrammParts:
		{
			OOX::Drawing::CDiagrammParts* pDiagrammParts = static_cast<OOX::Drawing::CDiagrammParts*>(oox_unknown);
			convert(pDiagrammParts);
		}break;
		case OOX::et_w_commentRangeEnd:
		{
			OOX::Logic::CCommentRangeEnd* pCommEnd = static_cast<OOX::Logic::CCommentRangeEnd*>(oox_unknown);
			convert(pCommEnd);
		}break;
		case OOX::et_w_commentRangeStart:
		{
			OOX::Logic::CCommentRangeStart* pCommStart = static_cast<OOX::Logic::CCommentRangeStart*>(oox_unknown);
			convert(pCommStart);
		}break;
		case OOX::et_w_commentReference:
		{
			OOX::Logic::CCommentReference* pCommRef = static_cast<OOX::Logic::CCommentReference*>(oox_unknown);
			convert(pCommRef);		//если нет Start - означает начало с предыдущего Run
		}break;
		case OOX::et_w_tbl:
		{
			OOX::Logic::CTbl* pTable= static_cast<OOX::Logic::CTbl*>(oox_unknown);
			convert(pTable);
		}break;
		case OOX::et_w_tr:
		{
			OOX::Logic::CTr* pRow= static_cast<OOX::Logic::CTr*>(oox_unknown);
			convert(pRow);
		}break;
		case OOX::et_w_tc:
		{
			OOX::Logic::CTc* pCell= static_cast<OOX::Logic::CTc*>(oox_unknown);
			convert(pCell);
		}break;
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}
void DocxConverter::convert(OOX::Logic::CSdt *oox_sdt)
{
	if (oox_sdt == NULL) return;
	//nullable<OOX::Logic::CSdtEndPr  > m_oSdtEndPr;
	//nullable<OOX::Logic::CSdtPr     > m_oSdtPr;

	if (oox_sdt->m_oSdtContent.IsInit())
	{
		for (unsigned int i=0; i< oox_sdt->m_oSdtContent->m_arrItems.size(); i++)
		{
			convert(oox_sdt->m_oSdtContent->m_arrItems[i]);
		}
	}
}
void DocxConverter::convert(OOX::Logic::CParagraph *oox_paragraph)
{
	if (oox_paragraph == NULL) return;

	bool bStyled = false;

	bool bStartNewParagraph = !odt_context->text_context()->get_KeepNextParagraph();
	
	bool		 list_present = false;
	std::wstring list_style_name;

	int			 list_level = -1;
	int			 list_style_id = -1;

	if(oox_paragraph->m_oParagraphProperty && odt_context->text_context()->get_list_item_state() == false)
	{
		if (oox_paragraph->m_oParagraphProperty->m_oPStyle.IsInit() && oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal.IsInit())
		{
			std::wstring style_name = string2std_string(*oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal);

			odf_writer::odf_style_state_ptr style_state;
			if (odt_context->styles_context()->find_odf_style_state(style_name,odf_types::style_family::Paragraph, style_state) && style_state)
			{
				list_present	= style_state->get_list_style_exist();
				list_level		= style_state->get_list_style_level();
				list_style_id	= style_state->get_list_style_id();
			}
		}
		if (oox_paragraph->m_oParagraphProperty->m_oNumPr.IsInit())
		{
			list_present = true;
		
			if (oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl.IsInit() && oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal.IsInit())
				list_level = oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oIlvl->m_oVal->GetValue();		
			
			if (oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID.IsInit() && oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal.IsInit())
				list_style_id = oox_paragraph->m_oParagraphProperty->m_oNumPr->m_oNumID->m_oVal->GetValue();
		}
	}
	if (oox_paragraph->m_oParagraphProperty || odt_context->is_empty_section())
	{
		bStyled = true;
		odf_writer::style_paragraph_properties	*paragraph_properties = NULL;

		if (odt_context->text_context()->get_KeepNextParagraph())
		{
			odf_writer::odf_style_state_ptr state =  odt_context->styles_context()->last_state(odf_types::style_family::Paragraph);
			if (state)
			{
				paragraph_properties = state->get_paragraph_properties();

				if (oox_paragraph->m_oParagraphProperty->m_oPStyle.IsInit() && oox_paragraph->m_oParagraphProperty->m_oPStyle->m_sVal.IsInit())
				{
					//перезатираем все свойства ... наложение не катит -- ваще то надо чистить после буквицы (Nadpis.docx) .. проверить надобность с остальными случами
					paragraph_properties->content().clear();
				}
			}
		}
		else
		{
			odt_context->styles_context()->create_style(L"",odf_types::style_family::Paragraph, true, false, -1);					
			paragraph_properties = odt_context->styles_context()->last_state()->get_paragraph_properties();
			
			if(list_present && list_style_id >= 0)
			{
				list_style_name = odt_context->styles_context()->lists_styles().get_style_name(list_style_id); 
				odt_context->styles_context()->last_state()->set_list_style_name(list_style_name);
			}
		}
		convert(oox_paragraph->m_oParagraphProperty, paragraph_properties); 

	}
	else 
	{
		odt_context->text_context()->set_KeepNextParagraph(false); //НУЖНО ПРИВЯЗАТЬ к УРОВНЮ на котором хранить m_bKeepNextParagraph !!! todooo
	//пока - если следующий не параграф - скидываем !!!
	}

	if(list_present)
	{
		odt_context->start_list_item(list_level, list_style_name);
	}

	if ((list_present = odt_context->text_context()->get_list_item_state()) == false) odt_context->set_no_list();

	if (bStartNewParagraph) odt_context->start_paragraph(bStyled);
	
	for (unsigned int nIndex = 0; nIndex < oox_paragraph->m_arrItems.size(); nIndex++ )
	{
		//те элементы которые тока для Paragraph - здесь - остальные в общей куче		
		switch(oox_paragraph->m_arrItems[nIndex]->getType())
		{
			case OOX::et_w_pPr:
			{
				// пропускаем .. 
			}break;		
			default:
				convert(oox_paragraph->m_arrItems[nIndex]);
				break;
		}
	}
	if (odt_context->text_context()->get_KeepNextParagraph()) odt_context->end_drop_cap();

	if (!odt_context->text_context()->get_KeepNextParagraph())  odt_context->end_paragraph();

	if(list_present && !odt_context->text_context()->get_KeepNextParagraph())
	{
		odt_context->end_list_item();
	}
}
void DocxConverter::convert(OOX::Logic::CRun *oox_run)//wordprocessing 22.1.2.87 math 17.3.2.25
{
	if (oox_run == NULL) return;
	
	//test for break - 2 first element ЭТОТ элемент НУЖНО вытащить отдельно !!!
    for(unsigned int i = 0; i < (std::min) ( (size_t)2, oox_run->m_arrItems.size()); ++i)
	{
		if (oox_run->m_arrItems[i]->getType() == OOX::et_w_lastRenderedPageBreak)
		{
			odt_context->add_page_break();
		}
	}	
	bool styled = false;	
	if (oox_run->m_oRunProperty) 
	{
		styled = true;

		odt_context->styles_context()->create_style(L"",odf_types::style_family::Text, true, false, -1);					
		odf_writer::style_text_properties	* text_properties = odt_context->styles_context()->last_state()->get_text_properties();

		convert(oox_run->m_oRunProperty, text_properties);
	}
	
	odt_context->start_run(styled);
	
	for(unsigned int i = 0; i < oox_run->m_arrItems.size(); ++i)
	{
		//те элементы которые тока для Run - здесь - остальные в общей куче		
		switch(oox_run->m_arrItems[i]->getType())
		{
			case OOX::et_w_fldChar:	
			{
				OOX::Logic::CFldChar* pFldChar= static_cast<OOX::Logic::CFldChar*>(oox_run->m_arrItems[i]);
				convert(pFldChar);
			}break;
			case OOX::et_w_instrText:
			{
				OOX::Logic::CInstrText* pInstrText= static_cast<OOX::Logic::CInstrText*>(oox_run->m_arrItems[i]);
				convert(pInstrText);
			}break;
			case OOX::et_w_rPr:
			{
				// пропускаем .. 
			}break;		
			case OOX::et_w_lastRenderedPageBreak:
			{
				//odt_context->text_context()->add_page_break(); выше
			}break;
			case OOX::et_w_br:
			{
				OOX::Logic::CBr* pBr= static_cast<OOX::Logic::CBr*>(oox_run->m_arrItems[i]);
				if (pBr)odt_context->text_context()->set_type_break(pBr->m_oType.GetValue(), pBr->m_oClear.GetValue());
			}break;
			case OOX::et_w_t:
			{
				OOX::Logic::CText* pText= static_cast<OOX::Logic::CText*>(oox_run->m_arrItems[i]);
				convert(pText);
			}break;
			case OOX::et_w_sym:
			{
				OOX::Logic::CSym* pSym= static_cast<OOX::Logic::CSym*>(oox_run->m_arrItems[i]);
				convert(pSym);
			}break;
			case OOX::et_w_tab:
			{
				OOX::Logic::CTab* pTab= static_cast<OOX::Logic::CTab*>(oox_run->m_arrItems[i]);
				odt_context->text_context()->add_tab();
			}break;
			case OOX::et_w_delText:
			{
			}break;
			case OOX::et_w_separator:
			case OOX::et_w_continuationSeparator:
			{
			}break;
			//annotationRef
			//endnoteRef
			//endnoteReference
			//footnoteRef
			//footnoteReference
			//commentReference
			//separator			
			//contentPart
			//cr
			//dayLong, dayShort, monthLong, monthShort, yearLong, yearShort
			//delText
			//noBreakHyphen
			//pgNum
			//ptab
			//ruby
			//softHyphen
			//delInstrText
			default:
				convert(oox_run->m_arrItems[i]);
		}
	}
	odt_context->end_run();
}

void DocxConverter::convert(OOX::Logic::CSym	*oox_sym)
{
	if (oox_sym == NULL) return;
	if (oox_sym->m_oChar.IsInit() == false) return;
	
	if (oox_sym->m_oFont.IsInit())
		odt_context->text_context()->set_symbol_font(string2std_string(*oox_sym->m_oFont));
		
	odt_context->text_context()->set_symbol_text(oox_sym->m_oChar->GetValue());
	//odt_context->text_context()->add_text_content(std::wstring(L"/") + string2std_string(oox_sym->m_oChar->ToString()));
}
void DocxConverter::convert(OOX::Logic::CFldChar	*oox_fld)
{
	if (oox_fld == NULL) return;

		//nullable<SimpleTypes::COnOff<>       > m_oDirty;
		//nullable<SimpleTypes::COnOff<>       > m_oFldLock;
		//nullable<OOX::Logic::CFFData         > m_oFFData;
	if (oox_fld->m_oFldCharType.IsInit())
	{
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeBegin)	odt_context->start_field(false);
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeEnd)		odt_context->end_field();
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeSeparate){}
	}

}
void DocxConverter::convert(OOX::Logic::CFldSimple	*oox_fld)
{
	if (oox_fld == NULL) return;

	//SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oDirty;
	//SimpleTypes::COnOff<SimpleTypes::onoffFalse> m_oFldLock;

	odt_context->start_field(true);
		if (oox_fld->m_sInstr.IsInit())	odt_context->set_field_instr(string2std_string(oox_fld->m_sInstr.get2()));

		for (unsigned int i=0; i< oox_fld->m_arrItems.size(); i++)
		{
			convert(oox_fld->m_arrItems[i]);
		}
	odt_context->end_field();
}
void DocxConverter::convert(OOX::Logic::CInstrText	*oox_instr)
{
	if (oox_instr == NULL) return;

	odt_context->set_field_instr(string2std_string(oox_instr->m_sText));

}
void DocxConverter::convert(OOX::Logic::CIns *oox_ins)
{
	if (oox_ins == NULL) return;
	//текст-вставка авторский
	//todooo сделать должную организацию text:change-start -> text:change-end c трейсером Abortion.docx

	for (unsigned int i=0; i< oox_ins->m_arrItems.size(); i++)
	{
		convert(oox_ins->m_arrItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CSmartTag *oox_tag)
{
	if (oox_tag == NULL) return;
	//текст-вставка авторский
	//todooo сделать должную организацию text:change-start -> text:change-end c трейсером Abortion.docx

	for (unsigned int i=0; i< oox_tag->m_arrItems.size(); i++)
	{
		convert(oox_tag->m_arrItems[i]);
	}
}
void DocxConverter::convert(OOX::Logic::CParagraphProperty	*oox_paragraph_pr, cpdoccore::odf_writer::style_paragraph_properties * paragraph_properties)
{
	odt_context->text_context()->set_KeepNextParagraph(false);
	
	if (!oox_paragraph_pr)		return;
	if (!paragraph_properties)	return;

	int outline_level = 0;

	if (oox_paragraph_pr->m_oPStyle.IsInit() && oox_paragraph_pr->m_oPStyle->m_sVal.IsInit())
	{
		std::wstring style_name = string2std_string(*oox_paragraph_pr->m_oPStyle->m_sVal);
		odt_context->styles_context()->last_state()->set_parent_style_name(style_name);
		/////////////////////////find parent properties 

		cpdoccore::odf_writer::style_paragraph_properties  parent_paragraph_properties;
		odt_context->styles_context()->calc_paragraph_properties(style_name,odf_types::style_family::Paragraph, &parent_paragraph_properties.content());
		
		if (parent_paragraph_properties.content().outline_level_)
		{
			outline_level = *parent_paragraph_properties.content().outline_level_;
		}
		//список тож явно ??? угу :( - выше + велосипед для хранения
	}

	if (oox_paragraph_pr->m_oSpacing.IsInit())
	{
		SimpleTypes::ELineSpacingRule rule = SimpleTypes::linespacingruleAtLeast;
		if (oox_paragraph_pr->m_oSpacing->m_oLineRule.IsInit())rule = oox_paragraph_pr->m_oSpacing->m_oLineRule->GetValue();

		if (oox_paragraph_pr->m_oSpacing->m_oLine.IsInit())
		{
			_CP_OPT(odf_types::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oLine.GetPointer()), length);
			if (length && rule == SimpleTypes::linespacingruleExact)
				paragraph_properties->content().fo_line_height_ = odf_types::line_width(*length);
			else if (length)
				paragraph_properties->content().style_line_height_at_least_= length;
		}
		if (oox_paragraph_pr->m_oSpacing->m_oAfter.IsInit())
		{
 			_CP_OPT(odf_types::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oAfter.GetPointer()), length);
			paragraph_properties->content().fo_margin_bottom_ = length;
		}
		if (oox_paragraph_pr->m_oSpacing->m_oBefore.IsInit())
		{
 			_CP_OPT(odf_types::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oBefore.GetPointer()), length);
			paragraph_properties->content().fo_margin_top_ = length;
		}
			//nullable<SimpleTypes::COnOff<>            > m_oAfterAutospacing;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oAfterLines;
			//nullable<SimpleTypes::COnOff<>            > m_oBeforeAutospacing;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oBeforeLines;
	}
	if (oox_paragraph_pr->m_oContextualSpacing.IsInit())
		paragraph_properties->content().style_contextual_spacing_ = true;

	if (oox_paragraph_pr->m_oInd.IsInit())
	{
		if (oox_paragraph_pr->m_oInd->m_oStart.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oStart.GetPointer()), length);
			paragraph_properties->content().fo_margin_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
			//в случае списка тута надо выдумать indent (взять из стиля списка)
		}
		if (oox_paragraph_pr->m_oInd->m_oEnd.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oEnd.GetPointer()), length);
			paragraph_properties->content().fo_margin_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
		if (oox_paragraph_pr->m_oInd->m_oFirstLine.IsInit())
		{
 			_CP_OPT(odf_types::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oFirstLine.GetPointer()), length);
			paragraph_properties->content().fo_text_indent_ = odf_types::length(length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
		if (oox_paragraph_pr->m_oInd->m_oHanging.IsInit())
		{//first line ignored
 			_CP_OPT(odf_types::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oHanging.GetPointer()), length);
			if (length) paragraph_properties->content().fo_text_indent_ = odf_types::length(-length->get_value_unit(odf_types::length::cm), odf_types::length::cm);
		}
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oEndChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oFirstLineChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oHangingChars;
			//nullable<SimpleTypes::CDecimalNumber<>    > m_oStartChars;
	}
	//if (oox_paragraph_pr->m_oRtl.IsInit())
	//{
	//}	

	convert(oox_paragraph_pr->m_oJc.GetPointer(), paragraph_properties->content().fo_text_align_);

	if (oox_paragraph_pr->m_oTextAlignment.IsInit() && oox_paragraph_pr->m_oTextAlignment->m_oVal.IsInit())
	{
		switch(oox_paragraph_pr->m_oTextAlignment->m_oVal->GetValue())
		{
		//case SimpleTypes::textalignAuto     :
		//case SimpleTypes::textalignBaseLine :
		//case SimpleTypes::textalignBottom   :
		//case SimpleTypes::textalignCenter   :
		//case SimpleTypes::textalignTop      :
		}
	}

	//if (oox_paragraph_pr->m_oWordWrap.IsInit())	odt_context->set_word_wrap(oox_paragraph_pr->m_oWordWrap->ToBool());

	convert(oox_paragraph_pr->m_oPBdr.GetPointer(), paragraph_properties);

	if (oox_paragraph_pr->m_oRPr.IsInit())
	{
		odf_writer::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		if (text_properties) 
			convert(oox_paragraph_pr->m_oRPr.GetPointer(), text_properties);
	}
	if (oox_paragraph_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_paragraph_pr->m_oShd->m_oFill.GetPointer(), oox_paragraph_pr->m_oShd->m_oThemeFill.GetPointer(),
			oox_paragraph_pr->m_oShd->m_oThemeFillTint.GetPointer(), oox_paragraph_pr->m_oShd->m_oThemeShade.GetPointer(), odf_color);
		if (odf_color)
			paragraph_properties->content().fo_background_color_ = *odf_color;
	}
	if (oox_paragraph_pr->m_oTextDirection.IsInit() && oox_paragraph_pr->m_oTextDirection->m_oVal.IsInit())
	{
		switch(oox_paragraph_pr->m_oTextDirection->m_oVal->GetValue())
		{
		case SimpleTypes::textdirectionLr   :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Lr); break;
		case SimpleTypes::textdirectionLrV  :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::LrTb); break;
		case SimpleTypes::textdirectionRl   :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Rl); break;
		case SimpleTypes::textdirectionRlV  :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::RlTb); break;
		case SimpleTypes::textdirectionTb   :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::Tb); break;
		case SimpleTypes::textdirectionTbV  :
			paragraph_properties->content().style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::TbLr); break;
		}
	}
	if (oox_paragraph_pr->m_oOutlineLvl.IsInit())
	{
		outline_level = 0;
		if (oox_paragraph_pr->m_oOutlineLvl->m_oVal.IsInit())
			outline_level = oox_paragraph_pr->m_oOutlineLvl->m_oVal->GetValue();
		if (outline_level > 0) paragraph_properties->content().outline_level_ =  outline_level /*+1*/;
		odt_context->text_context()->set_outline_level (outline_level);
	}
	if (oox_paragraph_pr->m_oPageBreakBefore.IsInit() && oox_paragraph_pr->m_oPageBreakBefore->m_oVal.ToBool())
	{
		paragraph_properties->content().fo_break_before_ = odf_types::fo_break(odf_types::fo_break::Page);
		odt_context->text_context()->set_type_break(1, 0); //page, clear_all
	}

	if (oox_paragraph_pr->m_oKeepNext.IsInit() && odt_context->table_context()->empty()/* && !oox_paragraph_pr->m_oFramePr.IsInit()*/)
	{
		odt_context->text_context()->set_KeepNextParagraph(true);
	}

	convert(oox_paragraph_pr->m_oFramePr.GetPointer(), paragraph_properties);		//буквица или фрейм

	if (oox_paragraph_pr->m_oSectPr.IsInit())
	{
		if (oox_paragraph_pr->m_oSectPr->m_oPgNumType.IsInit())
		{
			if (oox_paragraph_pr->m_oSectPr->m_oPgNumType->m_oStart.IsInit())
				paragraph_properties->content().style_page_number_ = oox_paragraph_pr->m_oSectPr->m_oPgNumType->m_oStart->GetValue();

			//paragraph//style:page-number=
		}	
		convert(oox_paragraph_pr->m_oSectPr.GetPointer());
	}

	if (oox_paragraph_pr->m_oTabs.IsInit())
	{
		paragraph_properties->add_child_element(odf_context()->start_tabs());
		for (unsigned int i = 0; i < oox_paragraph_pr->m_oTabs->m_arrTabs.size(); i++)
		{
			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i] == NULL) continue;
			_CP_OPT(int) type;
			_CP_OPT(int) leader;
			_CP_OPT(odf_types::length) length;

			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oVal.IsInit())
				type = oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oVal->GetValue();
			if (type == 2) continue; //clear tab

			if (oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oLeader.IsInit())
				leader = oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oLeader->GetValue();

			convert(oox_paragraph_pr->m_oTabs->m_arrTabs[i]->m_oPos.GetPointer(), length );

			odf_context()->add_tab(type, length, leader);
		}
		odf_context()->end_tabs();
	}
}

void DocxConverter::apply_from(OOX::Logic::CSectionProperty *props, OOX::Logic::CSectionProperty *other)
{
	if (props == NULL || other== NULL)return;

	props->m_bSectPrChange = other->m_bSectPrChange;

	// Attributes
	if (other->m_oRsidDel.IsInit())	props->m_oRsidDel		= other->m_oRsidDel;
	if (other->m_oRsidR.IsInit())	props->m_oRsidR			= other->m_oRsidR;
	if (other->m_oRsidRPr.IsInit())	props->m_oRsidRPr		= other->m_oRsidRPr;
	if (other->m_oRsidSect.IsInit())props->m_oRsidSect		= other->m_oRsidSect;

	// Child Elements
	if (other->m_oBidi.IsInit())		props->m_oBidi		= other->m_oBidi;
	if (other->m_oDocGrid.IsInit())		props->m_oDocGrid	= other->m_oDocGrid;
	if (other->m_oEndnotePr.IsInit())	props->m_oEndnotePr	= other->m_oEndnotePr;
	if (other->m_oRsidSect.IsInit())	props->m_oRsidSect	= other->m_oRsidSect;

	if (other->m_oFootnotePr.IsInit())	props->m_oFootnotePr= other->m_oFootnotePr;
	if (other->m_oFormProt.IsInit())	props->m_oFormProt	= other->m_oFormProt;
	if (other->m_oLnNumType.IsInit())	props->m_oLnNumType	= other->m_oLnNumType;
	if (other->m_oNoEndnote.IsInit())	props->m_oNoEndnote	= other->m_oNoEndnote;

	if (other->m_oPaperSrc.IsInit())	props->m_oPaperSrc	= other->m_oPaperSrc;
	if (other->m_oPgBorders.IsInit())	props->m_oPgBorders	= other->m_oPgBorders;
	if (other->m_oPgMar.IsInit())		props->m_oPgMar		= other->m_oPgMar;
	if (other->m_oPgNumType.IsInit())	props->m_oPgNumType	= other->m_oPgNumType;
	if (other->m_oPgSz.IsInit())		props->m_oPgSz		= other->m_oPgSz;
	if (other->m_oPrinterSettings.IsInit())	props->m_oPrinterSettings = other->m_oPrinterSettings;

	if (other->m_oRtlGutter.IsInit())		props->m_oRtlGutter		= other->m_oRtlGutter;
	if (other->m_oSectPrChange.IsInit())	props->m_oSectPrChange	= other->m_oSectPrChange;
	if (other->m_oTextDirection.IsInit())	props->m_oTextDirection	= other->m_oTextDirection;
	if (other->m_oTitlePg.IsInit())			props->m_oTitlePg		= other->m_oTitlePg;
	if (other->m_oType.IsInit())			props->m_oType			= other->m_oType;
	if (other->m_oVAlign.IsInit())			props->m_oVAlign		= other->m_oVAlign;
	if (other->m_oTitlePg.IsInit())			props->m_oTitlePg		= other->m_oTitlePg;

	if (other->m_oCols.IsInit())
	{
		props->m_oCols.Init();
		props->m_oCols->m_oEqualWidth	= other->m_oCols->m_oEqualWidth;
		props->m_oCols->m_oNum			= other->m_oCols->m_oNum;
		props->m_oCols->m_oSep			= other->m_oCols->m_oSep;
		props->m_oCols->m_oSpace		= other->m_oCols->m_oSpace;
		for (unsigned int i =0; i < other->m_oCols->m_arrColumns.size(); i++)
		{	
			if (other->m_oCols->m_arrColumns[i] == NULL)continue;

			ComplexTypes::Word::CColumn* col = new ComplexTypes::Word::CColumn();
		
			if (other->m_oCols->m_arrColumns[i]->m_oW.IsInit())		col->m_oW		= new SimpleTypes::CTwipsMeasure(*other->m_oCols->m_arrColumns[i]->m_oW.GetPointer());
			if (other->m_oCols->m_arrColumns[i]->m_oSpace.IsInit()) col->m_oSpace	= new SimpleTypes::CTwipsMeasure(*other->m_oCols->m_arrColumns[i]->m_oSpace.GetPointer());

			props->m_oCols->m_arrColumns.push_back(col);
		}
	}

	if (other->m_arrFooterReference.size() > 0)
	{
		for (unsigned int i =0 ; i < props->m_arrFooterReference.size() ; i++)
		{
			if (props->m_arrFooterReference[i]) delete props->m_arrFooterReference[i];
			props->m_arrFooterReference[i] = NULL;
		}
		props->m_arrFooterReference.clear();

		for (unsigned int i =0 ; i < other->m_arrFooterReference.size() ; i++)
		{
			ComplexTypes::Word::CHdrFtrRef* ref = new ComplexTypes::Word::CHdrFtrRef();

			ref->m_oId= other->m_arrFooterReference[i]->m_oId;
			ref->m_oType= other->m_arrFooterReference[i]->m_oType;

			props->m_arrFooterReference.push_back(ref);
		}
		
	}
	if (other->m_arrHeaderReference.size() > 0)
	{
		for (unsigned int i =0 ; i < props->m_arrHeaderReference.size() ; i++)
		{
			if (props->m_arrHeaderReference[i]) delete props->m_arrHeaderReference[i];
			props->m_arrHeaderReference[i] = NULL;
		}
		props->m_arrHeaderReference.clear();
		
		for (unsigned int i =0 ; i < other->m_arrHeaderReference.size() ; i++)
		{
			ComplexTypes::Word::CHdrFtrRef* ref = new ComplexTypes::Word::CHdrFtrRef();
			
			ref->m_oId= other->m_arrHeaderReference[i]->m_oId;
			ref->m_oType= other->m_arrHeaderReference[i]->m_oType;

			props->m_arrHeaderReference.push_back(ref);
		}
	}
}
void DocxConverter::convert(OOX::Logic::CSectionProperty *oox_section_pr, bool root)
{
	if (oox_section_pr == NULL) return;

	odt_context->text_context()->set_type_break(-1,0);
 
	bool continuous	= false;

	if (oox_section_pr->m_oType.IsInit() && oox_section_pr->m_oType->m_oVal.IsInit())
	{
		switch(oox_section_pr->m_oType->m_oVal->GetValue())
		{		
		case SimpleTypes::sectionmarkContinious :
			continuous = true; 
			break;
		case SimpleTypes::sectionmarkNextColumn :
		case SimpleTypes::sectionmarkEvenPage   :
		case SimpleTypes::sectionmarkNextPage   :
		case SimpleTypes::sectionmarkOddPage    :
			// возможен разрыв
			break;
		}
	}

    if (continuous && last_section_properties)
	{	// нужно убрать автоматический разрыв.на следующую страницу
		// + 
		//нужно текущие совйства накотить на предыдущие !! .. и так пока continues далее повторяется 
        apply_from(last_section_properties,oox_section_pr);
	}
	else
	{
	}

    //oox_section_pr = last_section_properties;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	odt_context->page_layout_context()->start_master_page(root ? L"Standard" : L"");
	
    odt_context->set_master_page_name(odt_context->page_layout_context()->last_master() ?
                                          odt_context->page_layout_context()->last_master()->get_name() : L"");
	
	bool present_header = false;
	bool present_footer = false;

	if (oox_section_pr->m_oPgMar.IsInit())
	{
		_CP_OPT(odf_types::length) top, left, right, bottom, other;
		convert(oox_section_pr->m_oPgMar->m_oBottom.GetPointer(), bottom);
		convert(oox_section_pr->m_oPgMar->m_oLeft.GetPointer()	, left);
		convert(oox_section_pr->m_oPgMar->m_oRight.GetPointer()	, right);
		convert(oox_section_pr->m_oPgMar->m_oTop.GetPointer()	, top);
		
		odt_context->page_layout_context()->set_page_margin(top,left,bottom, right);
		
		convert(oox_section_pr->m_oPgMar->m_oGutter.GetPointer(), other);
		odt_context->page_layout_context()->set_page_gutter(other);

		if (oox_section_pr->m_oPgMar->m_oFooter.IsInit())
		{
			convert(oox_section_pr->m_oPgMar->m_oFooter.GetPointer(), other);
			odt_context->page_layout_context()->set_footer_size(other);
			present_footer = true;
		}
		
		if (oox_section_pr->m_oPgMar->m_oHeader.IsInit())
		{
			convert(oox_section_pr->m_oPgMar->m_oHeader.GetPointer(), other);
			odt_context->page_layout_context()->set_header_size(other);
			present_header =true;
		}
	}
	if (oox_section_pr->m_oPgBorders.IsInit())
	{
		std::wstring top, left, right, bottom;
		convert(oox_section_pr->m_oPgBorders->m_oBottom.GetPointer(), bottom);
		convert(oox_section_pr->m_oPgBorders->m_oLeft.GetPointer()	, left);
		convert(oox_section_pr->m_oPgBorders->m_oRight.GetPointer()	, right);
		convert(oox_section_pr->m_oPgBorders->m_oTop.GetPointer()	, top);
		
		odt_context->page_layout_context()->set_page_border(top,left,bottom, right);

		int border_offset = 1;
		
		if (oox_section_pr->m_oPgBorders->m_oOffsetFrom.IsInit() && 
			(oox_section_pr->m_oPgBorders->m_oOffsetFrom->GetValue() == SimpleTypes::pageborderoffsetPage))border_offset = 2;

		if (oox_section_pr->m_oPgBorders->m_oBottom.IsInit() && oox_section_pr->m_oPgBorders->m_oBottom->m_oSpace.IsInit())
			odt_context->page_layout_context()->set_page_border_padding_bottom(border_offset,oox_section_pr->m_oPgBorders->m_oBottom->m_oSpace->ToPoints());
		if (oox_section_pr->m_oPgBorders->m_oTop.IsInit() && oox_section_pr->m_oPgBorders->m_oTop->m_oSpace.IsInit())
			odt_context->page_layout_context()->set_page_border_padding_top(border_offset,oox_section_pr->m_oPgBorders->m_oTop->m_oSpace->ToPoints());
		if (oox_section_pr->m_oPgBorders->m_oLeft.IsInit() && oox_section_pr->m_oPgBorders->m_oLeft->m_oSpace.IsInit())
			odt_context->page_layout_context()->set_page_border_padding_left(border_offset,oox_section_pr->m_oPgBorders->m_oLeft->m_oSpace->ToPoints());
		if (oox_section_pr->m_oPgBorders->m_oRight.IsInit() && oox_section_pr->m_oPgBorders->m_oRight->m_oSpace.IsInit())
			odt_context->page_layout_context()->set_page_border_padding_right(border_offset,oox_section_pr->m_oPgBorders->m_oRight->m_oSpace->ToPoints());


		bool shadow = false;
		if (oox_section_pr->m_oPgBorders->m_oBottom.IsInit() && oox_section_pr->m_oPgBorders->m_oBottom->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oBottom->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oTop.IsInit() && oox_section_pr->m_oPgBorders->m_oTop->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oTop->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oLeft.IsInit() && oox_section_pr->m_oPgBorders->m_oLeft->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oLeft->m_oShadow->ToBool()) shadow = true;
		if (oox_section_pr->m_oPgBorders->m_oRight.IsInit() && oox_section_pr->m_oPgBorders->m_oRight->m_oShadow.IsInit() && oox_section_pr->m_oPgBorders->m_oRight->m_oShadow->ToBool()) shadow = true;
		
		if (shadow) odt_context->page_layout_context()->set_page_border_shadow(true);
		
		if (oox_section_pr->m_oPgBorders->m_oDisplay.IsInit())
		{
			// todooo
			//pageborderdisplayAllPages     = 0,
			//pageborderdisplayFirstPage    = 1,
			//pageborderdisplayNotFirstPage = 2		
			//нужно разделить + сделать новые (без этих свойств) стили страниц - действительно для раздела 
		}
		//nullable<SimpleTypes::CPageBorderOffset<>  > m_oOffsetFrom;
		//nullable<SimpleTypes::CPageBorderZOrder<>  > m_oZOrder;

	}
	if (oox_section_pr->m_oPgSz.IsInit())
	{
		if (oox_section_pr->m_oPgSz->m_oOrient.IsInit())
			odt_context->page_layout_context()->set_page_orientation(oox_section_pr->m_oPgSz->m_oOrient->GetValue());

		_CP_OPT(odf_types::length) width, height;

		convert(oox_section_pr->m_oPgSz->m_oW.GetPointer(), width);
		convert(oox_section_pr->m_oPgSz->m_oH.GetPointer(), height);

		odt_context->page_layout_context()->set_page_size(width, height);

		//nullable<SimpleTypes::CDecimalNumber<>   > m_oCode;
	}
	convert(docx_document->GetDocument()->m_oBackground.GetPointer(), 1);//подложка - вот в таком она месте :(, причём одна на все разделы, не как в оо
			//nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oRtlGutter;
			//nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;

			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oTitlePg;	
	if (oox_section_pr->m_oPgNumType.IsInit())
	{
		_CP_OPT(int) format;
		_CP_OPT(int) start;

		if (oox_section_pr->m_oPgNumType->m_oFmt.IsInit())	format = oox_section_pr->m_oPgNumType->m_oFmt->GetValue();
		if (oox_section_pr->m_oPgNumType->m_oStart.IsInit())start  = oox_section_pr->m_oPgNumType->m_oStart->GetValue();

		odt_context->page_layout_context()->set_page_number_format(	format, start);
			//nullable<SimpleTypes::CChapterSep<>    > m_oChapSep;
			//nullable<SimpleTypes::CDecimalNumber<> > m_oChapStyle;
	}
	OOX::Logic::CSectionProperty * s = oox_section_pr;
    if (present_header && s->m_arrHeaderReference.size() <1 && last_section_properties)
        s = last_section_properties;

	for (unsigned int i=0; i< s->m_arrHeaderReference.size(); i++)
	{
		if (s->m_arrHeaderReference[i] == NULL) continue;

		int type =s->m_arrHeaderReference[i]->m_oType.IsInit() ? s->m_arrHeaderReference[i]->m_oType->GetValue() :0 ;
		if (odt_context->start_header(type))
		{
			if (s->m_arrHeaderReference[i]->m_oId.IsInit())
			{
				convert_hdr_ftr(s->m_arrHeaderReference[i]->m_oId->GetValue());
				convert(docx_document->GetDocument()->m_oBackground.GetPointer(), 2);
			}

			odt_context->end_header_footer();
		}
	}
	s = oox_section_pr;
    if (present_footer && s->m_arrFooterReference.size() <1 && last_section_properties)
        s = last_section_properties; // нужно хранить ссылки на ВСЕ !!!

	for (unsigned int i=0; i< s->m_arrFooterReference.size(); i++)
	{
		if (s->m_arrFooterReference[i] == NULL) continue;

		int type =s->m_arrFooterReference[i]->m_oType.IsInit() ? s->m_arrFooterReference[i]->m_oType->GetValue() :0 ;
		if (odt_context->start_footer(type))
		{
			if (s->m_arrFooterReference[i]->m_oId.IsInit())
			{
				convert_hdr_ftr(s->m_arrFooterReference[i]->m_oId->GetValue());
				convert(docx_document->GetDocument()->m_oBackground.GetPointer(), 3);
			}

			odt_context->end_header_footer();	
		}
	}
//--------------------------------------------------------------------------------------------------------------------------------------------		
	// то что относится собственно к секциям-разделам
	if (!root)odt_context->add_section(continuous);

			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBidi;
			//nullable<ComplexTypes::Word::CDocGrid                        > m_oDocGrid;
			//nullable<OOX::Logic::CEdnProps                               > m_oEndnotePr;
			//nullable<OOX::Logic::CFtnProps                               > m_oFootnotePr;
			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oFormProt;

			//nullable<ComplexTypes::Word::CLineNumber                     > m_oLnNumType;
			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oNoEndnote;
			//nullable<ComplexTypes::Word::CPaperSource                    > m_oPaperSrc;

			//nullable<ComplexTypes::Word::CRel                            > m_oPrinterSettings;
			//nullable<OOX::Logic::CSectPrChange                           > m_oSectPrChange;
//--------------------------------------------------------------------------------------------------------------------------------------------		

	if ((oox_section_pr->m_oCols.IsInit()) && (oox_section_pr->m_oCols->m_oNum.IsInit()) && (oox_section_pr->m_oCols->m_oNum->GetValue() > 1))//колонки
	{
		if (root)odt_context->add_section(continuous);
		
		unsigned int count = oox_section_pr->m_oCols->m_oNum->GetValue();
		
		double default_space_pt = -1;
		if (oox_section_pr->m_oCols->m_oSpace.IsInit())	default_space_pt = oox_section_pr->m_oCols->m_oSpace->ToPoints();
		
		bool separator = oox_section_pr->m_oCols->m_oSep.IsInit() && oox_section_pr->m_oCols->m_oSep->ToBool();
		
		odt_context->add_section_columns(count, default_space_pt, separator );

		std::vector<std::pair<double,double>> width_space;
		
		for (unsigned int i =0; i< oox_section_pr->m_oCols->m_arrColumns.size(); i++)
		{
			if (oox_section_pr->m_oCols->m_arrColumns[i] == NULL) continue;
			double space = default_space_pt;
			if (oox_section_pr->m_oCols->m_arrColumns[i]->m_oSpace.IsInit())
				space = oox_section_pr->m_oCols->m_arrColumns[i]->m_oSpace->ToPoints();
		
			double w = -1; 
			if (oox_section_pr->m_oCols->m_arrColumns[i]->m_oW.IsInit())
				w = oox_section_pr->m_oCols->m_arrColumns[i]->m_oW->ToPoints();
			
			width_space.push_back(std::pair<double,double>(w,space));
		}
		for (unsigned int i= oox_section_pr->m_oCols->m_arrColumns.size(); i< count; i ++)
		{
			width_space.push_back(std::pair<double,double>(-1, default_space_pt));
		}
		odt_context->add_section_column(width_space);

		if (root) odt_context->flush_section();
	}

	odt_context->page_layout_context()->end_master_page(); // для добавления автогенераций
	if (root)odt_context->page_layout_context()->set_current_master_page_base();

    last_section_properties = oox_section_pr;
}
void DocxConverter::convert(OOX::Logic::CBackground *oox_background, int type)
{
	if (oox_background == NULL) return;

	if (oox_background->m_oDrawing.IsInit())
	{
		//подложка
	}
	else
	{
		//цветовая подложка
		_CP_OPT(odf_types::color) color;
		convert (	oox_background->m_oColor.GetPointer(), 
					oox_background->m_oThemeColor.GetPointer(), 
					oox_background->m_oThemeTint.GetPointer(), 
					oox_background->m_oThemeShade.GetPointer(), color);

		odt_context->set_background(color, type);
	}
}

void DocxConverter::convert(ComplexTypes::Word::CFramePr *oox_frame_pr, odf_writer::style_paragraph_properties * paragraph_properties)
{
	if (oox_frame_pr == NULL) return;
	if (paragraph_properties == NULL) return;

	if (oox_frame_pr->m_oDropCap.IsInit() || oox_frame_pr->m_oLines.IsInit())
	{
		//буквица
		odt_context->start_drop_cap(paragraph_properties);

		if (oox_frame_pr->m_oDropCap.IsInit() && oox_frame_pr->m_oDropCap->GetValue() == SimpleTypes::dropcapMargin)
			odt_context->set_drop_cap_margin(true);
		if (oox_frame_pr->m_oLines.IsInit())
			odt_context->set_drop_cap_lines(oox_frame_pr->m_oLines->GetValue());
		//??
	//nullable<SimpleTypes::CTwipsMeasure       > m_oHSpace;
	//nullable<SimpleTypes::CTwipsMeasure       > m_oVSpace;
	}
	else
	{
		//фрейм текста .. ваще то его никто не формирует .. странно todoo ??
		if (oox_frame_pr->m_oXAlign.IsInit())
		{
			switch(oox_frame_pr->m_oXAlign->GetValue())
			{
			case SimpleTypes::xalignCenter  :	paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Center); break;
			case SimpleTypes::xalignInside  :	paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Start ); break;
			case SimpleTypes::xalignLeft    :	paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Left ); break;
			case SimpleTypes::xalignOutside :	paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::End ); break;
			case SimpleTypes::xalignRight   :	paragraph_properties->content().fo_text_align_ = odf_types::text_align(odf_types::text_align::Right); break;
			}
		}
		if (oox_frame_pr->m_oYAlign.IsInit())
		{
			switch(oox_frame_pr->m_oYAlign->GetValue())
			{
			case SimpleTypes::yalignBottom  :	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
			case SimpleTypes::yalignCenter  :	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle ); break;
			case SimpleTypes::yalignInline  :	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Baseline ); break;
			case SimpleTypes::yalignInside	:	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Auto ); break;
			case SimpleTypes::yalignOutside :	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Auto); break;
			case SimpleTypes::yalignTop		:	paragraph_properties->content().style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
			}
		}
	}
	//nullable<SimpleTypes::CHeightRule<>       > m_oHRule;

//имеют отношение только в drawing
	//nullable<SimpleTypes::CTwipsMeasure       > m_oW;
	//nullable<SimpleTypes::CWrap<>             > m_oWrap;
	//nullable<SimpleTypes::CTwipsMeasure       > m_oH;
	//nullable<SimpleTypes::CHAnchor<>          > m_oHAnchor;
	//nullable<SimpleTypes::CVAnchor<>          > m_oVAnchor;
	//nullable<SimpleTypes::CSignedTwipsMeasure > m_oX;
	//nullable<SimpleTypes::CSignedTwipsMeasure > m_oY;
}
void DocxConverter::convert(OOX::Logic::CTblBorders	*oox_border, odf_writer::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oStart.GetPointer()	, left);
	convert(oox_border->m_oEnd.GetPointer()		, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_	= 
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_		= 
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_		= 
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_	= table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_;

		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_		= right;
	}
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
}
void DocxConverter::convert(OOX::Logic::CTcBorders 	*oox_border, odf_writer::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oStart.GetPointer()	, left);
	convert(oox_border->m_oEnd.GetPointer()		, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_ = left;

		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ =
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = 
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = 
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = boost::none;
	}
	else
	{
		if (table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_)
		{
			table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_	= 
			table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_		= 
			table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_		= 
			table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_		= table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_;
		}
		table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0  )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_		= right;
	}
	convert(oox_border->m_oTL2BR.GetPointer()	, other);
	if (other.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.style_diagonal_tl_br_ = other;
	
	convert(oox_border->m_oTR2BL.GetPointer()	, other);
	if (other.length() >0 )
	{
		table_cell_properties->style_table_cell_properties_attlist_.style_diagonal_bl_tr_ = other;
	}
	
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
}

void DocxConverter::convert(OOX::Logic::CPBdr *oox_border, odf_writer::style_paragraph_properties *paragraph_properties)
{
	if (oox_border == NULL) return;
	if (paragraph_properties == NULL) return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oLeft.GetPointer()	, left);
	convert(oox_border->m_oRight.GetPointer()	, right);
	
	if (bottom == top && top == left && left== right && bottom.length() > 0)
	{
		paragraph_properties->content().common_border_attlist_.fo_border_ = left;
	}
	else
	{
		paragraph_properties->content().common_border_attlist_.fo_border_bottom_	= 
		paragraph_properties->content().common_border_attlist_.fo_border_top_		= 
		paragraph_properties->content().common_border_attlist_.fo_border_left_		= 
		paragraph_properties->content().common_border_attlist_.fo_border_right_	= paragraph_properties->content().common_border_attlist_.fo_border_;

		paragraph_properties->content().common_border_attlist_.fo_border_ = boost::none;

		if (bottom.length() >0 )paragraph_properties->content().common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	paragraph_properties->content().common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	paragraph_properties->content().common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) paragraph_properties->content().common_border_attlist_.fo_border_right_		= right;
	}
	bool shadow = false;
	if (oox_border->m_oBottom.IsInit() && oox_border->m_oBottom->m_oShadow.IsInit() && oox_border->m_oBottom->m_oShadow->ToBool()) shadow = true;
	if (oox_border->m_oTop.IsInit() && oox_border->m_oTop->m_oShadow.IsInit() && oox_border->m_oTop->m_oShadow->ToBool()) shadow = true;
	if (oox_border->m_oLeft.IsInit() && oox_border->m_oLeft->m_oShadow.IsInit() && oox_border->m_oLeft->m_oShadow->ToBool()) shadow = true;
	if (oox_border->m_oRight.IsInit() && oox_border->m_oRight->m_oShadow.IsInit() && oox_border->m_oRight->m_oShadow->ToBool()) shadow = true;
	
	if (shadow)
	{
		paragraph_properties->content().style_shadow_ = L"#000000 0.159cm 0.159cm";
	}
	//if (oox_border->m_oTL2BR.IsInit())
	//{
	//	convert(oox_border->m_oTL2BR.GetPointer(), odf_border);
	//	if (odf_border.length() >0 ) paragraph_properties->content().style_diagonal_tl_br_ = odf_border;
	//}

}
void DocxConverter::convert(ComplexTypes::Word::CPageBorder *borderProp, std::wstring & odf_border_prop)
{
	odf_border_prop = L"";
	if (!borderProp)return;
			
	//nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oFrame;
	//nullable<SimpleTypes::CPointMeasure<>                 > m_oSpace;

	std::wstringstream border_style;
	std::wstring border_color;

	odf_types::length length;
	length = odf_types::length(borderProp->m_oSz.IsInit() ? borderProp->m_oSz->ToPoints() : 1 ,  odf_types::length::pt);

	length = odf_types::length( length.get_value_unit(odf_types::length::cm), odf_types::length::cm);
	border_style << length;
	if (borderProp->m_oVal.IsInit())//266 styles  oO
	{
		switch(borderProp->m_oVal->GetValue())
		{
			case SimpleTypes::bordervalueDotDash:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDashed:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDotted:
				border_style << L" dotted";
			break;
			case SimpleTypes::bordervalueDouble:
				border_style << L" double";
			break;
			case SimpleTypes::bordervalueSingle:
				border_style << L" solid";
			break;
			case SimpleTypes::bordervalueNone:
			case SimpleTypes::bordervalueNil:
				odf_border_prop = L"none";
				return;
			break;
			default:
				border_style << L" solid";
		}
	}else border_style << L" solid";
///////////////////////////////////////////////////////////////////
	_CP_OPT(odf_types::color) color;

	if (borderProp->m_oColor.IsInit())
	{
		if (borderProp->m_oColor->GetValue() != SimpleTypes::hexcolorAuto)
			convert(borderProp->m_oColor.GetPointer(), borderProp->m_oThemeColor.GetPointer(), borderProp->m_oThemeTint.GetPointer(),borderProp->m_oThemeShade.GetPointer(), color);
	}
	if (color) border_color = color->get_hex_value();
	else border_color = L"000000";
	
	odf_border_prop = border_style.str() + L" #" + border_color;
}
void DocxConverter::convert(ComplexTypes::Word::CBorder *borderProp, std::wstring & odf_border_prop)
{
	odf_border_prop = L"";
	if (!borderProp)
		return;
			//nullable<SimpleTypes::COnOff<>              > m_oFrame;
			//nullable<SimpleTypes::COnOff<>              > m_oShadow;
			//nullable<SimpleTypes::CPointMeasure<>       > m_oSpace;
			//nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeShade;

	std::wstringstream border_style;
	std::wstring border_color;

	odf_types::length length;
	length = odf_types::length(borderProp->m_oSz.IsInit() ? borderProp->m_oSz->ToPoints() : 1 ,  odf_types::length::pt);

	length = odf_types::length( length.get_value_unit(odf_types::length::cm), odf_types::length::cm);
	border_style << length;
	if (borderProp->m_oVal.IsInit())//266 styles  oO
	{
		switch(borderProp->m_oVal->GetValue())
		{
			case SimpleTypes::bordervalueDotDash:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDashed:
				border_style << L" dashed";
			break;
			case SimpleTypes::bordervalueDotted:
				border_style << L" dotted";
			break;
			case SimpleTypes::bordervalueDouble:
				border_style << L" double";
			break;
			case SimpleTypes::bordervalueSingle:
				border_style << L" solid";
			break;
			case SimpleTypes::bordervalueNone:
			case SimpleTypes::bordervalueNil:
				odf_border_prop = L"none";
				return;
			break;
			default:
				border_style << L" solid";
		}
	}else border_style << L" solid";
///////////////////////////////////////////////////////////////////
	_CP_OPT(odf_types::color) color;

	if (borderProp->m_oColor.IsInit())
	{
		if (borderProp->m_oColor->GetValue() != SimpleTypes::hexcolorAuto)
			convert(borderProp->m_oColor.GetPointer(), borderProp->m_oThemeColor.GetPointer(), borderProp->m_oThemeTint.GetPointer(),borderProp->m_oThemeShade.GetPointer(), color);
	}
	if (color) border_color = color->get_hex_value();
	else border_color = L"000000";
	
	odf_border_prop = border_style.str() + L" #" + border_color;
}
void DocxConverter::convert(ComplexTypes::Word::CJc * oox_jc,  _CP_OPT(odf_types::text_align) & align)
{
	if (oox_jc == NULL) return;
	if (oox_jc->m_oVal.IsInit() == false) return;

	switch(oox_jc->m_oVal->GetValue())
	{
		case SimpleTypes::jcBoth            : align = odf_types::text_align(odf_types::text_align::Justify);break;
		case SimpleTypes::jcCenter          : align = odf_types::text_align(odf_types::text_align::Center);	break;
		case SimpleTypes::jcThaiDistribute  :	
		case SimpleTypes::jcDistribute      : align = odf_types::text_align(odf_types::text_align::Justify);break;
		case SimpleTypes::jcEnd             : align = odf_types::text_align(odf_types::text_align::End);	break;
		case SimpleTypes::jcHighKashida     :	break;
		case SimpleTypes::jcLowKashida      :	break;
		case SimpleTypes::jcMediumKashida   :	break;
		case SimpleTypes::jcNumTab          :	break;
		case SimpleTypes::jcStart           : align = odf_types::text_align(odf_types::text_align::Start);	break;
		case SimpleTypes::jcLeft            : align = odf_types::text_align(odf_types::text_align::Left);	break;
		case SimpleTypes::jcRight           : align = odf_types::text_align(odf_types::text_align::Right);	break;
	}
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure *oox_size, _CP_OPT(odf_types::length) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

	odf_size = odf_types::length(oox_size->ToPoints(), odf_types::length::pt);
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure* oox_size, _CP_OPT(odf_types::length_or_percent) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

		//odf_size = odf_types::percent(oox_size->ToPoints(());
	odf_size = odf_types::length(oox_size->ToPoints(), odf_types::length::pt);

}
void DocxConverter::convert(ComplexTypes::Word::CTblWidth *oox_size, _CP_OPT(odf_types::length) & odf_size)
{
	odf_size = boost::none;
	if (oox_size == NULL)return;

	if (oox_size->m_oW.IsInit() == false) return;

	if (oox_size->m_oType.IsInit() && oox_size->m_oType->GetValue() == SimpleTypes::tblwidthDxa)
	{
		odf_size = odf_types::length(oox_size->m_oW->GetValue() / 20., odf_types::length::pt);
	}

		//tblwidthAuto = 0,
		//tblwidthDxa  = 1,
		//tblwidthNil  = 2,
		//tblwidthPct  = 3
}
void DocxConverter::convert(OOX::Logic::CRunProperty *oox_run_pr, odf_writer::style_text_properties * text_properties)
{
	if (oox_run_pr == NULL) return;
	if (text_properties == NULL)return;
	
	if (oox_run_pr->m_oRStyle.IsInit() && oox_run_pr->m_oRStyle->m_sVal.IsInit())
	{
		odt_context->styles_context()->last_state()->set_parent_style_name(string2std_string(*oox_run_pr->m_oRStyle->m_sVal));
	}
	if (oox_run_pr->m_oBold.IsInit())
	{
		if (oox_run_pr->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);
	}
	if (oox_run_pr->m_oColor.IsInit())
	{
		if(oox_run_pr->m_oColor->m_oVal.IsInit() && oox_run_pr->m_oColor->m_oVal->GetValue() == SimpleTypes::hexcolorAuto)
			text_properties->content().fo_color_ = odf_types::color(L"#000000");
		else
		   convert(oox_run_pr->m_oColor.GetPointer(),text_properties->content().fo_color_);
	}

    text_properties->content().style_text_underline_type_= odf_types::line_type(odf_types::line_type::Non);
	if (oox_run_pr->m_oU.IsInit())
	{
		text_properties->content().style_text_underline_style_	= odf_types::line_style(odf_types::line_style::Solid);
		text_properties->content().style_text_underline_type_	= odf_types::line_type(odf_types::line_type::Single);

		_CP_OPT(odf_types::color) color;
		convert(oox_run_pr->m_oU->m_oColor.GetPointer(), oox_run_pr->m_oU->m_oThemeColor.GetPointer(),
			oox_run_pr->m_oU->m_oThemeTint.GetPointer(), oox_run_pr->m_oU->m_oThemeShade.GetPointer(), color);
		
		if (color) text_properties->content().style_text_underline_color_ = color;
		else text_properties->content().style_text_underline_color_ = odf_types::underline_color(odf_types::underline_color::FontColor);
		
		
		if (oox_run_pr->m_oU->m_oVal.IsInit())
		{
			SimpleTypes::EUnderline 	type = oox_run_pr->m_oU->m_oVal->GetValue();

			switch(type)
			{
			case SimpleTypes::underlineDouble		:
					text_properties->content().style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);break;
			case SimpleTypes::underlineNone			:
				{
                    text_properties->content().style_text_underline_type_	= odf_types::line_type(odf_types::line_type::Non);
					text_properties->content().style_text_underline_style_	= boost::none;
					text_properties->content().style_text_underline_color_  = boost::none;
				}break;
			case SimpleTypes::underlineDash            :
			case SimpleTypes::underlineDashedHeavy     :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::Dash);break;
			case SimpleTypes::underlineDashLong        :
			case SimpleTypes::underlineDashLongHeavy   :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::LongDash);break;
			case SimpleTypes::underlineDotDash         :
			case SimpleTypes::underlineDashDotHeavy    :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::DotDash);break;
			case SimpleTypes::underlineDotted          :
			case SimpleTypes::underlineDottedHeavy     :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::Dotted);break;
			case SimpleTypes::underlineDotDotDash      :
			case SimpleTypes::underlineDashDotDotHeavy :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::DotDotDash);break;
			case SimpleTypes::underlineThick           :
			case SimpleTypes::underlineWave            :
			case SimpleTypes::underlineWavyDouble      :
			case SimpleTypes::underlineWavyHeavy       :
					text_properties->content().style_text_underline_style_= odf_types::line_style(odf_types::line_style::Wave);break;
			}
		}
	}
	if (oox_run_pr->m_oItalic.IsInit())
	{
		if (oox_run_pr->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content().fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	}
	if (oox_run_pr->m_oSz.IsInit() && oox_run_pr->m_oSz->m_oVal.IsInit())
	{
		OoxConverter::convert(oox_run_pr->m_oSz->m_oVal->ToPoints(), text_properties->content().fo_font_size_);
	}
	if (oox_run_pr->m_oKern.IsInit() && oox_run_pr->m_oKern->m_oVal.IsInit())
	{
		//OoxConverter::convert(oox_run_pr->m_oSz->m_oVal->ToPoints(), text_properties->content().fo_font_size_);
		text_properties->content().style_letter_kerning_ = true;
	}
	if (oox_run_pr->m_oCaps.IsInit() && oox_run_pr->m_oCaps->m_oVal.ToBool())
		text_properties->content().fo_text_transform_ = odf_types::text_transform(odf_types::text_transform::Uppercase);
	
	if (oox_run_pr->m_oSmallCaps.IsInit() && oox_run_pr->m_oSmallCaps->m_oVal.ToBool())
		text_properties->content().fo_font_variant_ = odf_types::font_variant(odf_types::font_variant::SmallCaps);

	if (oox_run_pr->m_oRFonts.IsInit())
	{
		if (oox_run_pr->m_oRFonts->m_sAscii.IsInit())
			text_properties->content().fo_font_family_ = string2std_string(oox_run_pr->m_oRFonts->m_sAscii.get());
		else convert(oox_run_pr->m_oRFonts->m_oAsciiTheme.GetPointer(), text_properties->content().fo_font_family_);

		if (oox_run_pr->m_oRFonts->m_sCs.IsInit())
			text_properties->content().style_font_family_complex_ = string2std_string(oox_run_pr->m_oRFonts->m_sCs.get());
		else convert(oox_run_pr->m_oRFonts->m_oCsTheme.GetPointer(), text_properties->content().style_font_family_complex_);

		if (oox_run_pr->m_oRFonts->m_sEastAsia.IsInit())
			text_properties->content().style_font_family_asian_= string2std_string(oox_run_pr->m_oRFonts->m_sEastAsia.get());
		else convert(oox_run_pr->m_oRFonts->m_oEastAsiaTheme.GetPointer(), text_properties->content().style_font_family_asian_);

			//nullable<CString              > m_sHAnsi;
			//nullable<SimpleTypes::CTheme<>> m_oHAnsiTheme;
			//nullable<SimpleTypes::CHint<> > m_oHint;
	}
	if (oox_run_pr->m_oVertAlign.IsInit() && oox_run_pr->m_oVertAlign->m_oVal.IsInit())
	{ 
		switch(oox_run_pr->m_oVertAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticalalignrunSuperscript:
			//text_properties->content().style_text_position_ = odf_types::text_position(odf_types::text_position::Super); break;
			text_properties->content().style_text_position_ = odf_types::text_position(+33.); break;
		case SimpleTypes::verticalalignrunSubscript:
			//text_properties->content().style_text_position_ = odf_types::text_position(odf_types::text_position::Sub); break;
			text_properties->content().style_text_position_ = odf_types::text_position(-33.); break;//по умолчанию 58% - хуже выглядит
		}
	}
	if (oox_run_pr->m_oW.IsInit() && oox_run_pr->m_oW->m_oVal.IsInit())
	{
		text_properties->content().style_text_scale_ = odf_types::percent(oox_run_pr->m_oW->m_oVal->GetValue());
	}
	if (oox_run_pr->m_oStrike.IsInit() && oox_run_pr->m_oStrike->m_oVal.ToBool())
	{
		text_properties->content().style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Single);
	}

	if (oox_run_pr->m_oBdr.IsInit())
	{
		std::wstring odf_border;
		convert(oox_run_pr->m_oBdr.GetPointer(), odf_border);
		if (odf_border.length() > 0)
			text_properties->content().common_border_attlist_.fo_border_ = odf_border;
	}
	if (oox_run_pr->m_oHighlight.IsInit() && oox_run_pr->m_oHighlight->m_oVal.IsInit())
	{
		BYTE ucR = oox_run_pr->m_oHighlight->m_oVal->Get_R(); 
		BYTE ucB = oox_run_pr->m_oHighlight->m_oVal->Get_B(); 
		BYTE ucG = oox_run_pr->m_oHighlight->m_oVal->Get_G(); 
		SimpleTypes::CHexColor<> *oRgbColor = new SimpleTypes::CHexColor<>(ucR,ucG,ucB);
		if (oRgbColor)
		{		
			text_properties->content().fo_background_color_ = odf_types::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString().Right(6)));
			delete oRgbColor;
		}
	}
	if (oox_run_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_run_pr->m_oShd->m_oFill.GetPointer(), oox_run_pr->m_oShd->m_oThemeFill.GetPointer(),
			oox_run_pr->m_oShd->m_oThemeFillTint.GetPointer(), oox_run_pr->m_oShd->m_oThemeShade.GetPointer(), odf_color);
		if (odf_color)
			text_properties->content().fo_background_color_ = *odf_color;
	}
	if (oox_run_pr->m_oOutline.IsInit())
		text_properties->content().style_text_outline_ = true; //контур

	if (oox_run_pr->m_oVanish.IsInit())
		text_properties->content().text_display_ = odf_types::text_display(odf_types::text_display::None);
}

void DocxConverter::convert(SimpleTypes::CTheme<>* oox_font_theme, _CP_OPT(std::wstring) & odf_font_name)
{
	if (oox_font_theme == NULL) return;

	OOX::CTheme * docx_theme= docx_document->GetTheme();
	if (docx_theme == NULL) return;

	std::wstring font;

	switch(oox_font_theme->GetValue())
	{
	case SimpleTypes::themeMajorAscii:
	case SimpleTypes::themeMajorHAnsi :
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oLatin.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	case SimpleTypes::themeMajorBidi:
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oCs.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oCs.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	case SimpleTypes::themeMajorEastAsia:
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oEa.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMajorFont.m_oEa.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	case SimpleTypes::themeMinorAscii:
	case SimpleTypes::themeMinorHAnsi:
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oLatin.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	case SimpleTypes::themeMinorBidi:
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oCs.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oCs.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	case SimpleTypes::themeMinorEastAsia:
		if (docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oEa.m_oTypeFace.IsInit())
		{
			font = string2std_string(docx_theme->m_oThemeElements.m_oFontScheme.m_oMinorFont.m_oEa.m_oTypeFace->GetValue());
			if (font.length() > 0) odf_font_name = font;
		}
		break;
	}


}

void DocxConverter::convert(OOX::Logic::CText *oox_text)
{
	if (oox_text == NULL) return;

	if (oox_text->m_oSpace.IsInit())
	{
		if (oox_text->m_oSpace->GetValue() == SimpleTypes::xmlspacePreserve)
		{
		}
			//odt_context->text_context()->add_text_space(26);
	}
	odt_context->add_text_content(string2std_string(oox_text->m_sText));
}
void DocxConverter::convert(OOX::Logic::CAlternateContent *oox_alt_content)
{
	if (oox_alt_content == NULL)return;

	for(unsigned int i = 0; i < oox_alt_content->m_arrChoiceItems.size(); ++i) // правильный выбор
	{
		convert(oox_alt_content->m_arrChoiceItems[i]);
	}
	
	if (oox_alt_content->m_arrChoiceItems.size() > 0) return;  //чтоб не было дубляжа
	for(unsigned int i = 0; i < oox_alt_content->m_arrFallbackItems.size(); ++i) // альтернативный 
	// todooo нужно сверять по инддексно что нормально сконвертилось ... или делать ВСЕ по choice ( это правильнее)
	{
		convert(oox_alt_content->m_arrFallbackItems[i]);
	}
}
///////////////////////////////////////
//old formating vml
void DocxConverter::convert(OOX::Logic::CPicture* oox_pic)
{
	if (oox_pic == NULL) return;

	odt_context->start_drawings();
			
	if (odt_context->table_context()->empty())
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::AsChar);//default
	else
	{
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::Paragraph);
		odf_context()->drawing_context()->set_object_background(true);
	}
		
	int type = 1; // shape .. default
	if (oox_pic->m_oShapeGroup.IsInit())
	{
		OoxConverter::convert(oox_pic->m_oShapeGroup.GetPointer());
	}
	else
	{
		if (oox_pic->m_oShape.IsInit())
			OoxConverter::convert(oox_pic->m_oShape->m_oStyle.GetPointer());
		
		odf_context()->drawing_context()->start_drawing();
		
		if (oox_pic->m_oShapeOval.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Oval");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeEllipse);
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer()); 		
			OoxConverter::convert(oox_pic->m_oShapeOval.GetPointer());
			
			odf_context()->drawing_context()->corrected_line_fill();
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapeCurve.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Curve");
			odf_context()->drawing_context()->start_shape(1000);
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer()); 	
			OoxConverter::convert(oox_pic->m_oShapeCurve.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapeLine.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Line");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);

			odf_context()->drawing_context()->set_line_width(1.);
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer());
			OoxConverter::convert(oox_pic->m_oShapeLine.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapePolyLine.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Polyline");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);
			
			odf_context()->drawing_context()->set_line_width(1.);

			OoxConverter::convert(oox_pic->m_oShape.GetPointer());	
			OoxConverter::convert(oox_pic->m_oShapePolyLine.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapeArc.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Arc");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeLine);
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer());		
			OoxConverter::convert(oox_pic->m_oShapeArc.GetPointer());
			
			odf_context()->drawing_context()->corrected_line_fill();
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapeRect.IsInit())
		{
			if ((oox_pic->m_oShapeRect->m_oFilled.IsInit() && oox_pic->m_oShapeRect->m_oFilled->GetValue() == SimpleTypes::booleanFalse) && 
				(oox_pic->m_oShapeRect->m_oStroked.IsInit() && oox_pic->m_oShapeRect->m_oStroked->GetValue() == SimpleTypes::booleanFalse))
			{
				odf_context()->drawing_context()->start_text_box();
				
				OoxConverter::convert(oox_pic->m_oShape.GetPointer());
				OoxConverter::convert(oox_pic->m_oShapeRect.GetPointer());
				
				odf_context()->drawing_context()->end_text_box();
			}
			else
			{
				odf_context()->drawing_context()->set_name(L"Rect");
				odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);
				
				OoxConverter::convert(oox_pic->m_oShape.GetPointer());
				OoxConverter::convert(oox_pic->m_oShapeRect.GetPointer());
				
				odf_context()->drawing_context()->corrected_line_fill();
				odf_context()->drawing_context()->end_shape(); 
			}
		}
		else if (oox_pic->m_oShapeRoundRect.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"RoundRect");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRoundRect);
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer());
			OoxConverter::convert(oox_pic->m_oShapeRoundRect.GetPointer());
			
			odf_context()->drawing_context()->corrected_line_fill();
			odf_context()->drawing_context()->end_shape(); 
		}
		else if (oox_pic->m_oShapeType.IsInit())
		{
			OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(oox_pic->m_oShapeType->m_oSpt.GetValue());

			odf_context()->drawing_context()->set_name(std::wstring (L"Custom") + boost::lexical_cast<std::wstring>(sptType));
			odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));
			
			OoxConverter::convert(oox_pic->m_oShape.GetPointer());	
			OoxConverter::convert(oox_pic->m_oShapeType.GetPointer());
			
			odf_context()->drawing_context()->end_shape(); 
		}
		else // and oox_pic->m_oShapeRect
		{
			bool bSet = false;
			if (oox_pic->m_oShape.IsInit())
			{
				OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(oox_pic->m_oShape->m_oSpt.GetValue());
				if (sptType != OOX::Vml::SptType::sptNotPrimitive)
				{
					odf_context()->drawing_context()->set_name(std::wstring (L"Custom") + boost::lexical_cast<std::wstring>(sptType));
					odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));
					bSet = true;
				}
				else if (oox_pic->m_oShape->m_oConnectorType.GetValue() != SimpleTypes::connectortypeNone)
				{
					odf_context()->drawing_context()->set_name(L"Connector");
					odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeStraightConnector1);
					odf_context()->drawing_context()->set_line_width(1.);
					bSet = true;
				}
				else if (oox_pic->m_oShape->m_oPath.IsInit())
				{
					odf_context()->drawing_context()->set_name(L"Path");
					odf_context()->drawing_context()->start_shape(1001);
					odf_context()->drawing_context()->set_line_width(1.);
					bSet = true;
				}
			}
			if (!bSet)
			{
				odf_context()->drawing_context()->set_name(L"Rect");
				odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);			
			}
			OoxConverter::convert(oox_pic->m_oShape.GetPointer()); 			
			odf_context()->drawing_context()->end_shape(); 
		}
		odf_context()->drawing_context()->end_drawing();
	}

	odt_context->end_drawings();
}
void DocxConverter::convert(OOX::Logic::CObject* oox_obj)
{
	if (oox_obj == NULL) return;

	odt_context->start_drawings();
			
	if (odt_context->table_context()->empty())
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::AsChar);//default
	else
	{
		odf_context()->drawing_context()->set_anchor(odf_types::anchor_type::Paragraph);
		odf_context()->drawing_context()->set_object_background(true);
	}

	if (oox_obj->m_oShape.IsInit())
		OoxConverter::convert(oox_obj->m_oShape->m_oStyle.GetPointer());
		
	odf_context()->drawing_context()->start_drawing();
		
	bool bSet = false;
	if (oox_obj->m_oShape.IsInit())
	{
		OOX::Vml::SptType sptType = static_cast<OOX::Vml::SptType>(oox_obj->m_oShape->m_oSpt.GetValue());
		if (sptType != OOX::Vml::SptType::sptNotPrimitive)
		{
			odf_context()->drawing_context()->set_name(std::wstring (L"Custom") + boost::lexical_cast<std::wstring>(sptType));
			odf_context()->drawing_context()->start_shape(OOX::Spt2ShapeType(sptType));
			bSet = true;
		}
		else if (oox_obj->m_oShape->m_oConnectorType.GetValue() != SimpleTypes::connectortypeNone)
		{
			odf_context()->drawing_context()->set_name(L"Connector");
			odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeStraightConnector1);
			odf_context()->drawing_context()->set_line_width(1.);
			bSet = true;
		}
		else if (oox_obj->m_oShape->m_oPath.IsInit())
		{
			odf_context()->drawing_context()->set_name(L"Path");
			odf_context()->drawing_context()->start_shape(1001);
			odf_context()->drawing_context()->set_line_width(1.);
			bSet = true;
		}
	}
	if (!bSet)
	{
		odf_context()->drawing_context()->set_name(L"Rect");
		odf_context()->drawing_context()->start_shape(SimpleTypes::shapetypeRect);			
	}
	OoxConverter::convert(oox_obj->m_oShape.GetPointer()); 			
	odf_context()->drawing_context()->end_shape(); 

	odf_context()->drawing_context()->end_drawing();

	odt_context->end_drawings();
}
///////////////////////////////////////////////
void DocxConverter::convert(OOX::Logic::CDrawing *oox_drawing)
{
	if (oox_drawing == NULL) return;

	odt_context->start_drawings();
		convert(oox_drawing->m_oAnchor.GetPointer());
		convert(oox_drawing->m_oInline.GetPointer());
	odt_context->end_drawings();
}
void DocxConverter::convert(OOX::Drawing::CAnchor *oox_anchor) 
{
	if (oox_anchor == NULL)return;

	_CP_OPT(double) width, height;
	_CP_OPT(double) x, y;

	if (oox_anchor->m_oExtent.IsInit()) //size
	{
		width = oox_anchor->m_oExtent->m_oCx.ToPoints();
		height = oox_anchor->m_oExtent->m_oCy.ToPoints();
	}
	if (oox_anchor->m_oDistL.IsInit())odt_context->drawing_context()->set_margin_left(oox_anchor->m_oDistL->ToPoints());
	if (oox_anchor->m_oDistT.IsInit())odt_context->drawing_context()->set_margin_top(oox_anchor->m_oDistT->ToPoints());
	if (oox_anchor->m_oDistR.IsInit())odt_context->drawing_context()->set_margin_right(oox_anchor->m_oDistR->ToPoints());
	if (oox_anchor->m_oDistB.IsInit())odt_context->drawing_context()->set_margin_bottom(oox_anchor->m_oDistB->ToPoints());

	odt_context->drawing_context()->set_drawings_rect(x, y, width, height);

	if (oox_anchor->m_oPositionV.IsInit() && oox_anchor->m_oPositionV->m_oRelativeFrom.IsInit())
	{
		odt_context->drawing_context()->set_vertical_rel(oox_anchor->m_oPositionV->m_oRelativeFrom->GetValue());

		if ( oox_anchor->m_oPositionV->m_oAlign.IsInit())
			odt_context->drawing_context()->set_vertical_pos(oox_anchor->m_oPositionV->m_oAlign->GetValue());

		else if(oox_anchor->m_oPositionV->m_oPosOffset.IsInit())
			odt_context->drawing_context()->set_vertical_pos(oox_anchor->m_oPositionV->m_oPosOffset->ToPoints());
		else
			odt_context->drawing_context()->set_vertical_pos(SimpleTypes::alignvTop);
	}
	if (oox_anchor->m_oPositionH.IsInit() && oox_anchor->m_oPositionH->m_oRelativeFrom.IsInit())
	{
		odt_context->drawing_context()->set_horizontal_rel(oox_anchor->m_oPositionH->m_oRelativeFrom->GetValue());
		
		if (oox_anchor->m_oPositionH->m_oAlign.IsInit())
			odt_context->drawing_context()->set_horizontal_pos(oox_anchor->m_oPositionH->m_oAlign->GetValue());
		
		else if(oox_anchor->m_oPositionH->m_oPosOffset.IsInit())
			odt_context->drawing_context()->set_horizontal_pos(oox_anchor->m_oPositionH->m_oPosOffset->ToPoints());
		else
			odt_context->drawing_context()->set_horizontal_pos(SimpleTypes::alignhLeft);
	}

	bool wrap_set = false;
	if (oox_anchor->m_oWrapSquare.IsInit())
	{
		if (oox_anchor->m_oWrapSquare->m_oWrapText.IsInit() && oox_anchor->m_oWrapSquare->m_oWrapText->GetValue() == SimpleTypes::wraptextLargest)
			odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);
		else	
			odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);

		wrap_set = true;
	}	
	else if (oox_anchor->m_oWrapThrough.IsInit())//style:wrap="run-through" draw:wrap-influence-on-position style:wrap-contour
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::RunThrough);
	}
	else if (oox_anchor->m_oWrapTight.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
		wrap_set = true;
	}
	else if (oox_anchor->m_oWrapTopAndBottom.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Parallel);
		wrap_set = true;
	}
	else if (oox_anchor->m_oAllowOverlap.IsInit())
	{
		odt_context->drawing_context()->set_overlap(oox_anchor->m_oAllowOverlap->ToBool());
	}
	if (oox_anchor->m_oBehindDoc.IsInit() && !wrap_set)
	{
		if (oox_anchor->m_oBehindDoc->ToBool())
		{
			odt_context->drawing_context()->set_object_background(true);
			//odt_context->drawing_context()->set_object_foreground(true);
		}
		else
		{
			//z - order foreground
			odt_context->drawing_context()->set_object_foreground(true);
		}
	}
	if (oox_anchor->m_oRelativeHeight.IsInit())
	{
		int id = oox_anchor->m_oRelativeHeight->GetValue();
		odf_context()->drawing_context()->set_z_order(id);
	}
	OoxConverter::convert(oox_anchor->m_oDocPr.GetPointer());
	convert(oox_anchor->m_oGraphic.GetPointer());
}
void DocxConverter::convert(OOX::Drawing::CInline *oox_inline)
{
	if (oox_inline == NULL)return;

	_CP_OPT(double) width, height;
	_CP_OPT(double) x = 0., y = 0.;
	
	if (oox_inline->m_oExtent.IsInit()) //size
	{
		width = oox_inline->m_oExtent->m_oCx.ToPoints();
		height = oox_inline->m_oExtent->m_oCy.ToPoints();
	}

	odt_context->drawing_context()->set_drawings_rect(x, y, width, height);

	odt_context->drawing_context()->set_anchor(odf_types::anchor_type::AsChar); 
	
	if (oox_inline->m_oDistL.IsInit())odt_context->drawing_context()->set_margin_left(oox_inline->m_oDistL->ToPoints());
	if (oox_inline->m_oDistT.IsInit())odt_context->drawing_context()->set_margin_top(oox_inline->m_oDistT->ToPoints());
	if (oox_inline->m_oDistR.IsInit())odt_context->drawing_context()->set_margin_right(oox_inline->m_oDistR->ToPoints());
	if (oox_inline->m_oDistB.IsInit())odt_context->drawing_context()->set_margin_bottom(oox_inline->m_oDistB->ToPoints());

	//вертикальное выравнивание относительно строки поставим в середину (иначе по нижнему краю почемуто)

	odt_context->drawing_context()->set_vertical_rel(2);//line
	odt_context->drawing_context()->set_vertical_pos(1);//middle

	OoxConverter::convert(oox_inline->m_oDocPr.GetPointer());
	convert(oox_inline->m_oGraphic.GetPointer());
}

void DocxConverter::convert(OOX::Drawing::CGraphic *oox_graphic)
{
	if (oox_graphic == NULL)return;

	for (unsigned int i = 0 ; i < oox_graphic->m_arrItems.size(); i++)
	{
		convert(oox_graphic->m_arrItems[i]);
	}

}
void DocxConverter::convert(OOX::Drawing::CPicture * oox_picture)
{
	if (!oox_picture)return;

	odt_context->drawing_context()->start_drawing();

	CString pathImage;
	double Width=0, Height = 0;

	if (oox_picture->m_oBlipFill.m_oBlip.IsInit())
	{
		CString sID = oox_picture->m_oBlipFill.m_oBlip->m_oEmbed.GetValue();		
		pathImage = find_link_by_id(sID,1);
		
		if (pathImage.GetLength() < 1)
		{
			sID = oox_picture->m_oBlipFill.m_oBlip->m_oLink.GetValue();	
			//???
		}
        _graphics_utils_::GetResolution(pathImage, Width, Height);
	}
	odt_context->start_image(string2std_string(pathImage));
	{
		if (oox_picture->m_oBlipFill.m_oTile.IsInit()) 
		{
			odt_context->drawing_context()->set_image_style_repeat(2);
		}
		if (oox_picture->m_oBlipFill.m_oStretch.IsInit())
		{
			odt_context->drawing_context()->set_image_style_repeat(1);
		}
		if (oox_picture->m_oBlipFill.m_oSrcRect.IsInit() && Width >0 && Height >0)
		{
			odt_context->drawing_context()->set_image_client_rect_inch(
					oox_picture->m_oBlipFill.m_oSrcRect->m_oL.GetValue() * Width/100. /currentSystemDPI,
					oox_picture->m_oBlipFill.m_oSrcRect->m_oT.GetValue() * Height/100./currentSystemDPI,
					oox_picture->m_oBlipFill.m_oSrcRect->m_oR.GetValue() * Width/100. /currentSystemDPI, 
					oox_picture->m_oBlipFill.m_oSrcRect->m_oB.GetValue() * Height/100./currentSystemDPI);
		}		

		OoxConverter::convert(&oox_picture->m_oNvPicPr.m_oCNvPr);		

		//oox_picture->m_oNvPicPr.m_oCNvPicPr
		//oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks
		{
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoChangeAspect)
			//{
			//}
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoCrop))
			//{
			//}
			//if (oox_picture->m_oNvPicPr.m_oCNvPicPr.m_oPicLocks->m_oNoResize)
			//{
			//}
		}	
		//m_oExtLst


		OoxConverter::convert(&oox_picture->m_oSpPr, NULL);

	}
	odt_context->drawing_context()->end_image();
	odt_context->drawing_context()->end_drawing();
}
void DocxConverter::convert(OOX::Drawing::CDiagrammParts	*oox_diagramm)
{
	if (oox_diagramm == NULL)return;
	if (oox_diagramm->m_oRId_Data.IsInit() == false) return;

	smart_ptr<OOX::Image> pDiagData;
	
	if (oox_current_child_document)
		pDiagData = docx_document->GetDocument()->GetImage(oox_diagramm->m_oRId_Data->GetValue());
	else
		pDiagData = docx_document->GetDocument()->GetImage(oox_diagramm->m_oRId_Data->GetValue());
	
	NSCommon::nullable<OOX::RId> id_drawing;

	if (pDiagData.IsInit())
	{
		CString strDiagDataPath = pDiagData->filename().m_strFilename;
		
		XmlUtils::CXmlNode oNodeDiagData;
		if (oNodeDiagData.FromXmlFile2(strDiagDataPath))
		{
			XmlUtils::CXmlNode oNode2 = oNodeDiagData.ReadNode(_T("dgm:extLst"));
			if (oNode2.IsValid())
			{
				XmlUtils::CXmlNode oNode3 = oNode2.ReadNode(_T("a:ext"));
				if (oNode3.IsValid())
				{
					XmlUtils::CXmlNode oNode4 = oNode3.ReadNode(_T("dsp:dataModelExt"));
					if (oNode4.IsValid())
					{
						oNode4.ReadAttributeBase(L"relId", id_drawing);
					}
				}
			}
		}
	}
	if (id_drawing.is_init() == false) return;

	smart_ptr<OOX::File> oFile = docx_document->GetDocument()->Find( *id_drawing );

	if (oFile.is_init() && OOX::FileTypes::DiagDrawing == oFile->type())
	{
		OOX::CDiagramDrawing * diag_drawing = (OOX::CDiagramDrawing*)oFile.operator->();

		oox_current_child_document = diag_drawing;

		for (unsigned int i=0; i < diag_drawing->m_arrShapeTree.size(); i++)
		{
			OoxConverter::convert(diag_drawing->m_arrShapeTree[i]);
		}
		oox_current_child_document = NULL;
	}
}

void DocxConverter::convert(OOX::Drawing::CChart * oox_chart)
{
	if (oox_chart == NULL)return;

	if (oox_chart->m_oRId.IsInit())
	{
		smart_ptr<OOX::File> oFile;
		
		if (oox_current_child_document)
			oFile = oox_current_child_document->Find(oox_chart->m_oRId->GetValue());
		else
			oFile = docx_document->GetDocument()->Find(oox_chart->m_oRId->GetValue());
		
		if (oFile.IsInit() && OOX::FileTypes::Chart == oFile->type())
		{
			OOX::Spreadsheet::CChartSpace* pChart = (OOX::Spreadsheet::CChartSpace*)oFile.operator->();
			
			if (pChart)
			{
				odt_context->drawing_context()->start_drawing();				
				odt_context->drawing_context()->start_object(odf_context()->get_next_name_object());

					double width =0, height =0;
					odt_context->drawing_context()->get_size(width, height);

					OoxConverter::convert(pChart->m_oChartSpace.m_oSpPr.GetPointer());
					
					oox_current_child_document_spreadsheet = dynamic_cast<OOX::Spreadsheet::IFileContainer*>(pChart);
					
					odf_context()->start_chart();
						odf_context()->chart_context()->set_chart_size(width, height);
						OoxConverter::convert(&pChart->m_oChartSpace);
					odf_context()->end_chart();
					
					oox_current_child_document_spreadsheet = NULL; 

				odt_context->drawing_context()->end_object();	
				odt_context->drawing_context()->end_drawing();
			}
		}
	}

}

void DocxConverter::convert(OOX::Logic::CGroupShape	 *oox_group_shape)
{
	if (oox_group_shape == NULL)return;
	if (oox_group_shape->m_arrItems.size() < 1) return;

	odt_context->drawing_context()->start_group();
		if (oox_group_shape->m_oCNvPr.IsInit())
		{	
			if (oox_group_shape->m_oCNvPr->m_sName.IsInit())
				odt_context->drawing_context()->set_group_name(string2std_string(*oox_group_shape->m_oCNvPr->m_sName));
			if (oox_group_shape->m_oCNvPr->m_oId.IsInit())
				odt_context->drawing_context()->set_group_z_order(oox_group_shape->m_oCNvPr->m_oId->GetValue());
		}
		OoxConverter::convert(oox_group_shape->m_oGroupSpPr.GetPointer());

		for (unsigned int i=0; i < oox_group_shape->m_arrItems.size(); i++)
		{
			convert(oox_group_shape->m_arrItems[i]);
		}
	odt_context->drawing_context()->end_group();
}

void DocxConverter::convert(OOX::Logic::CLockedCanvas	 *oox_canvas)
{
	if (oox_canvas== NULL)return;

	odf_context()->drawing_context()->start_group();
	
	_CP_OPT(double) x, y , ch_x , ch_y ;

	odf_context()->drawing_context()->set_group_position(x, y, ch_x, ch_y);

	for (unsigned int i=0; i < oox_canvas->m_arrItems.size(); i++)
	{
		convert(oox_canvas->m_arrItems[i]);
	}

	odf_context()->drawing_context()->end_group();
}

void DocxConverter::convert(OOX::Logic::CShape	 *oox_shape)
{
	if (oox_shape == NULL)return;
	if (!oox_shape->m_oSpPr.IsInit()) return;

	odt_context->drawing_context()->start_drawing();
	
		int type = -1;
		if (oox_shape->m_oSpPr->m_oCustGeom.IsInit())
		{
			type = 1000;//6???
		}
		if (oox_shape->m_oSpPr->m_oPrstGeom.IsInit())
		{
			OOX::Drawing::CPresetGeometry2D * geometry = oox_shape->m_oSpPr->m_oPrstGeom.GetPointer();
			type =(geometry->m_oPrst.GetValue());
		}
		
		if (oox_shape->m_oCNvSpPr.IsInit() && oox_shape->m_oCNvSpPr->m_otxBox.GetValue() == 1) type = 2000; //textBox
				
		if (type == SimpleTypes::shapetypeRect && oox_shape->m_oTxBody.IsInit() && oox_shape->m_oTxBodyProperties.IsInit() /*&&
			oox_shape->m_oTxBodyProperties->m_eAutoFitType == OOX::Drawing::textautofitShape*/)
		{	// ваще то тут обычный прямоугольник, но в него не вставишь таблицы, ...
			//второй вариант таблицы ВСЕГДА оборачивать фреймами.
			//надо тогда хотя бы сделать определялку где мы находимся - в drawing_context или нет - причем пофиг на уровень вложенности
			//todoo
		
			type = 2000;
		}
		if ((type == 2000 || type == SimpleTypes::shapetypeRect )	&& oox_shape->m_oTxBodyProperties.IsInit() 
																	&& oox_shape->m_oTxBodyProperties->m_oPrstTxWrap.IsInit())
		{
			if (oox_shape->m_oTxBodyProperties->m_oFromWordArt.ToBool())
			{
				int wordart_type = OoxConverter::convert(oox_shape->m_oTxBodyProperties->m_oPrstTxWrap.GetPointer());
				if (wordart_type >=0)type = wordart_type;
			}
		}
		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		odt_context->drawing_context()->start_shape(type);
		
		OoxConverter::convert(oox_shape->m_oSpPr.GetPointer(), oox_shape->m_oShapeStyle.GetPointer());
	//имя, описалово, номер ...		
		OoxConverter::convert(oox_shape->m_oCNvPr.GetPointer());	
	//заблокированности 
		OoxConverter::convert(oox_shape->m_oCNvSpPr.GetPointer());	
		
		if (oox_shape->m_oCNvConnSpPr.IsInit())
		{
			OoxConverter::convert(oox_shape->m_oCNvConnSpPr.GetPointer());
			//частенько приплывает из стиля заполенение объекта .. а он то одномерный :)
			odf_context()->drawing_context()->start_area_properties();
				odf_context()->drawing_context()->set_no_fill();
			odf_context()->drawing_context()->end_area_properties();
		}
		
		if (oox_shape->m_oTxBody.IsInit() && oox_shape->m_oTxBody->m_oTxtbxContent.IsInit())
		{
			odt_context->start_text_context();
				for (unsigned int i=0 ; i < oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems.size();i++)
				{
					convert(oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems[i]);
				}
				odt_context->drawing_context()->set_text( odt_context->text_context());
				
				//наложим внешние настройки для текста
				OoxConverter::convert(oox_shape->m_oTxBodyProperties.GetPointer());
					
				if (oox_shape->m_oShapeStyle.IsInit() && oox_shape->m_oShapeStyle->m_oFontRef.getType() == OOX::et_a_fontRef)
				{
					OoxConverter::convert(&oox_shape->m_oShapeStyle->m_oFontRef);
				}
			odt_context->end_text_context();	
		}

	odt_context->drawing_context()->end_shape();
	
	odt_context->drawing_context()->end_drawing();
}

void DocxConverter::convert(SimpleTypes::CHexColor<>		*color, 
							SimpleTypes::CThemeColor<>		*theme_color, 
							SimpleTypes::CUcharHexNumber<>	*theme_tint,
							SimpleTypes::CUcharHexNumber<>	*theme_shade, _CP_OPT(odf_types::color) & odf_color)
{
	odf_color = boost::none;

	unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
	bool result = false;	

	if(color && color->GetValue() == SimpleTypes::hexcolorRGB)//easy, faster,realy  !!
	{
		ucR = color->Get_R(); 
		ucB = color->Get_B(); 
		ucG = color->Get_G(); 
		ucA = color->Get_A(); 
		result = true;
	}
	if(theme_color && result == false)
	{
		OOX::CTheme * docx_theme= docx_document->GetTheme();
		int theme_ind = theme_color->GetValue();
		switch(theme_ind)
		{
			case SimpleTypes::themecolorLight1:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oLt1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorLight2:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oLt2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorDark1:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oDk1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorDark2:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oDk2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent1:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent2:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent3:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent3.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent4:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent4.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent5:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent5.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorAccent6:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oAccent6.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorFollowedHyperlink:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oFolHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::themecolorHyperlink:
				result = docx_theme->m_oThemeElements.m_oClrScheme.m_oHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
		}
		if (result == true && theme_tint)
		{
			OOX::Drawing::CHslColor col;
			col.SetRGBA(ucR, ucG, ucB);
			double dH,  dS,  dL;
			col.GetHSL(dH, dS,dL);
			dL = dL * theme_tint->GetValue()/255. + (1 - theme_tint->GetValue()/255.); 
			col.SetHSL(dH, dS,dL);
			col.GetRGBA(ucR, ucG, ucB,ucA);
		}
	}
	if (result == true)
	{
		SimpleTypes::CHexColor<> *oRgbColor = new SimpleTypes::CHexColor<>(ucR,ucG,ucB);
		if (oRgbColor)
		{		
			odf_color = odf_types::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString().Right(6)));
			delete oRgbColor;
		}
	}
}
void DocxConverter::convert(ComplexTypes::Word::CColor *color, _CP_OPT(odf_types::color) & odf_color)
{
	if (!color)return;
	convert(color->m_oVal.GetPointer(), color->m_oThemeColor.GetPointer(),color->m_oThemeTint.GetPointer(),color->m_oThemeShade.GetPointer(), odf_color);
}
void DocxConverter::convert_settings()
{
	if (!odt_context) return;
	OOX::CSettings * docx_settings = docx_document->GetSettings();

	if (!docx_settings) return;

	if (docx_settings->m_oZoom.IsInit())
	{
	}
	if (docx_settings->m_oMirrorMargins.IsInit())
	{
		odt_context->page_layout_context()->set_pages_mirrored(true);
	}

	if (docx_settings->m_oEvenAndOddHeaders.IsInit())odt_context->page_layout_context()->set_even_and_left_headers(true);
	else odt_context->page_layout_context()->set_even_and_left_headers(false);

	if (docx_settings->m_oPrintTwoOnOne.IsInit())
	{
		if (docx_settings->m_oGutterAtTop.IsInit()){} //portrait
		else {}//landscape
	}

	if (docx_settings->m_oDefaultTabStop.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(docx_settings->m_oDefaultTabStop->m_oVal.GetPointer(), length);
		
		odf_writer::odf_style_state_ptr state;
		if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, state) && state)
		{
			odf_writer::style_paragraph_properties *props =	state->get_paragraph_properties();
			if (props)
				props->content().style_tab_stop_distance_ = length;
		}

	}
//nullable<ComplexTypes::Word::CLanguage>                       m_oThemeFontLang;
//nullable<OOX::Settings::CEdnDocProps>                         m_oEndnotePr;
//nullable<OOX::Settings::CCaptions>                            m_oCaptions;
//m_oFootnotePr;
}
void DocxConverter::convert_lists_styles()
{
	if (!odt_context) return;

	OOX::CNumbering * lists_styles = docx_document->GetNumbering();
	
	if (!lists_styles)return;

	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(lists_styles);
//базовые
	for (unsigned int i=0; i < lists_styles->m_arrAbstractNum.size(); i++)
	{
		convert(lists_styles->m_arrAbstractNum[i]);
	}

//используемые в документе, используют базовые + могут поменяться - Override	
	for (unsigned int i=0; i < lists_styles->m_arrNum.size(); i++)
	{
		if (lists_styles->m_arrNum[i] == NULL) continue;

		if (lists_styles->m_arrNum[i]->m_oNumId.IsInit() == false) continue;
		if (lists_styles->m_arrNum[i]->m_arrLvlOverride.size() >0)
		{
			//parent ??? 
			//create_new_style (in automatic main document??? )
		}
		{
			int abstr_num = 0;
			if (lists_styles->m_arrNum[i]->m_oAbstractNumId.IsInit() &&  lists_styles->m_arrNum[i]->m_oAbstractNumId->m_oVal.IsInit())
				abstr_num = lists_styles->m_arrNum[i]->m_oAbstractNumId->m_oVal->GetValue();
			
			odt_context->styles_context()->lists_styles().add_style( lists_styles->m_arrNum[i]->m_oNumId->GetValue(), abstr_num);
		}

		convert(lists_styles->m_arrNum[i]);//for override ???
	}
	oox_current_child_document = NULL;
//nullable<ComplexTypes::Word::CDecimalNumber > m_oNumIdMacAtCleanup;
}
void DocxConverter::convert_styles()
{
	if (!odt_context) return;

	OOX::CStyles * docx_styles = docx_document->GetStyles();
	
	if (!docx_styles)return;

	//nullable<OOX::CLatentStyles > m_oLatentStyles;

	convert(docx_styles->m_oDocDefaults.GetPointer());

	for (unsigned int i=0; i< docx_styles->m_arrStyle.size(); i++)
	{
		if (docx_styles->m_arrStyle[i] == NULL) continue;

		convert(docx_styles->m_arrStyle[i]);
	
		if (i == 0 && docx_styles->m_arrStyle[i]->m_oDefault.IsInit() && docx_styles->m_arrStyle[i]->m_oDefault->ToBool())
		{
			//NADIE_COMO_TU.docx тут дефолтовый стиль не прописан явно, берем тот что Normal
			odf_writer::odf_style_state_ptr def_style_state;

			odf_writer::style_paragraph_properties	* def_para_properties = NULL;
			odf_writer::style_text_properties		* def_text_properties = NULL;
			
			if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
			{
				def_para_properties = def_style_state->get_paragraph_properties();
				def_text_properties = def_style_state->get_text_properties();

				odf_writer::style_paragraph_properties	* para_properties = odt_context->styles_context()->last_state()->get_paragraph_properties();
				odf_writer::style_text_properties		* text_properties = odt_context->styles_context()->last_state()->get_text_properties();

				def_para_properties->apply_from(para_properties);
				def_text_properties->apply_from(text_properties);
			}
		}
	}
}

void DocxConverter::convert(OOX::Logic::CHyperlink *oox_hyperlink)
{
	if (oox_hyperlink == NULL)return;
	if (oox_hyperlink->m_oId.IsInit()) //гиперлинк
	{
		std::wstring ref = find_link_by_id(oox_hyperlink->m_oId->GetValue(),2);

		odt_context->start_hyperlink(ref);
		
		for (unsigned int i=0; i< oox_hyperlink->m_arrItems.size(); i++)
		{
			convert(oox_hyperlink->m_arrItems[i]);
		}
		odt_context->end_hyperlink();
	}
	else
	{//ссылка внутри дока
		//anchor todooo
		for (unsigned int i=0; i< oox_hyperlink->m_arrItems.size(); i++)
		{
			convert(oox_hyperlink->m_arrItems[i]);
		}
	}
	//nullable<CString                                      > m_sAnchor;
	//nullable<CString                                      > m_sDocLocation;
	//nullable<SimpleTypes::COnOff<SimpleTypes::onoffFalse> > m_oHistory;
	//nullable<CString                                      > m_sTgtFrame;
	//nullable<CString                                      > m_sTooltip;
}

void DocxConverter::convert(OOX::CDocDefaults *def_style)
{
	if (def_style == NULL)return;

	if (def_style->m_oParPr.IsInit())
	{
		odt_context->styles_context()->create_default_style(odf_types::style_family::Paragraph);					
		
		odf_writer::style_paragraph_properties	* paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
		odf_writer::style_text_properties		* text_properties		= NULL;

		convert(def_style->m_oParPr.GetPointer(), paragraph_properties); 
		
		if (def_style->m_oParPr->m_oRPr.IsInit())
		{
			text_properties = odt_context->styles_context()->last_state()->get_text_properties();
			convert(def_style->m_oParPr->m_oRPr.GetPointer(), text_properties);
		}
		else if (def_style->m_oRunPr.IsInit())
		{
			text_properties = odt_context->styles_context()->last_state()->get_text_properties();
			convert(def_style->m_oRunPr.GetPointer(), text_properties);
		}
		if (text_properties && !text_properties->content().fo_font_size_)
		{
			text_properties->content().fo_font_size_ = odf_types::font_size(odf_types::length(10,odf_types::length::pt));
		}
	}
	
	if (def_style->m_oRunPr.IsInit())
	{
		odt_context->styles_context()->create_default_style(odf_types::style_family::Text);					
		odf_writer::style_text_properties	* text_properties = odt_context->styles_context()->last_state()->get_text_properties();

		convert(def_style->m_oRunPr.GetPointer(), text_properties);

	///////на дефолтовый параграф - дефолтовые настройки шрифта
		odf_writer::odf_style_state_ptr def_style_state;

		odf_writer::style_text_properties		* para_text_properties = NULL;
		if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
		{
			para_text_properties = def_style_state->get_text_properties();
			para_text_properties->apply_from(text_properties);
		}
		else
		{
			odt_context->styles_context()->create_default_style(odf_types::style_family::Paragraph);					
			para_text_properties = odt_context->styles_context()->last_state()->get_text_properties();

			para_text_properties->apply_from(text_properties);
		}
		if (para_text_properties && !para_text_properties->content().fo_font_size_)
		{
			para_text_properties->content().fo_font_size_ = odf_types::font_size(odf_types::length(10,odf_types::length::pt));
		}
	}

///////////////////////////////////////////////////////////////////////////

	odt_context->styles_context()->create_default_style(odf_types::style_family::Table);					
	odf_writer::style_table_properties	* table_properties	= odt_context->styles_context()->last_state()->get_table_properties();
	//для красивой отрисовки в редакторах - разрешим объеденить стили пересекающихся обрамлений 
	table_properties->table_format_properties_.table_border_model_ = odf_types::border_model(odf_types::border_model::Collapsing);

	odt_context->styles_context()->create_default_style(odf_types::style_family::TableRow);					
	odf_writer::style_table_row_properties	* row_properties	= odt_context->styles_context()->last_state()->get_table_row_properties();
	row_properties->style_table_row_properties_attlist_.fo_keep_together_ = odf_types::keep_together(odf_types::keep_together::Auto);

/////////////////////////////////////////////////////////////////////////////////////////////////
	//зачемто ?! для OpenOffice для врезок/фреймов нужен базовый стиль - без него другой тип геометрии oO !!!
	odt_context->styles_context()->create_style(L"Frame", odf_types::style_family::Graphic,false,true);		
	odf_writer::style_graphic_properties	* graphic_properties	= odt_context->styles_context()->last_state()->get_graphic_properties();
	if (graphic_properties)graphic_properties->content().common_background_color_attlist_.fo_background_color_ = odf_types::background_color(odf_types::background_color::Transparent);

}

void DocxConverter::convert(OOX::Numbering::CAbstractNum* oox_num_style)
{
	if (oox_num_style == NULL) return;

	if (oox_num_style->m_oAbstractNumId.IsInit() == false) return;


	odt_context->styles_context()->lists_styles().start_style(oox_num_style->m_oAbstractNumId->GetValue());
	//// Childs
	//std::vector<OOX::Numbering::CLvl                            >  m_arrLvl;
	//nullable<ComplexTypes::Word::CMultiLevelType                 > m_oMultiLevelType;
	//nullable<ComplexTypes::Word::CString_                        > m_oName;
	//nullable<ComplexTypes::Word::CLongHexNumber                  > m_oNsid;
	//nullable<ComplexTypes::Word::CString_                        > m_oNumStyleLink;
	//nullable<ComplexTypes::Word::CString_                        > m_oStyleLink;
	//nullable<ComplexTypes::Word::CLongHexNumber                  > m_oTmpl;

	for (unsigned int i=0; i < oox_num_style->m_arrLvl.size(); i++)
	{
		convert(oox_num_style->m_arrLvl[i]);
	}

	odt_context->styles_context()->lists_styles().end_style();
}
void DocxConverter::convert(OOX::Numbering::CLvl* oox_num_lvl)
{
	if (oox_num_lvl == NULL) return;
	if (oox_num_lvl->m_oIlvl.IsInit() == false) return; //???

	if (oox_num_lvl->m_oNumFmt.IsInit() == false) return; //???

	if (oox_num_lvl->m_oNumFmt->m_oVal.IsInit()== false) return; //???

	int oox_type_list = oox_num_lvl->m_oNumFmt->m_oVal->GetValue();
	if (oox_num_lvl->m_oLvlPicBulletId.IsInit()) oox_type_list = 1000;

	int type_list = odt_context->styles_context()->lists_styles().start_style_level(oox_num_lvl->m_oIlvl->GetValue(), oox_type_list );
	if (type_list < 0) return;

	odf_writer::style_list_level_properties		* level_props		= odt_context->styles_context()->lists_styles().get_list_level_properties();
	odf_writer::style_list_level_label_alignment	* aligment_props	= odt_context->styles_context()->lists_styles().get_list_level_alignment_properties();
	
	if (level_props		== NULL)return;
	if (aligment_props	== NULL)return;

	//nullable<SimpleTypes::COnOff<>         > m_oTentative;
	//nullable<SimpleTypes::CLongHexNumber<> > m_oTplc;

	//// Childs
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oIsLgl;
	//nullable<ComplexTypes::Word::CLvlLegacy                      > m_oLegacy;
	//nullable<ComplexTypes::Word::CString_                        > m_oPStyle;

	double  size_bullet_number_marker = 0;
	if (oox_num_lvl->m_oLvlJc.IsInit())
	{
	}
	if (oox_num_lvl->m_oPPr.IsInit())
	{
		if (oox_num_lvl->m_oPPr->m_oTabs.IsInit())
		{
			if ((oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs.size() >0) && (oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs[0]))
			{
				_CP_OPT(odf_types::length) length;
				convert(oox_num_lvl->m_oPPr->m_oTabs->m_arrTabs[0]->m_oPos.GetPointer(), length);
				if (length) aligment_props->text_list_tab_stop_position_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}
		}
		if (oox_num_lvl->m_oPPr->m_oInd.IsInit())
		{
			_CP_OPT(odf_types::length) length_indent;
			
			convert(oox_num_lvl->m_oPPr->m_oInd->m_oHanging.GetPointer(), length_indent);	
			
			if (length_indent) aligment_props->fo_text_indent_ = odf_types::length(-length_indent->get_value_unit(odf_types::length::cm),odf_types::length::cm);//text_min_label_width_

			_CP_OPT(odf_types::length) length_margin;
			if (oox_num_lvl->m_oPPr->m_oInd->m_oStart.IsInit())
			{
				if (oox_num_lvl->m_oPPr->m_oInd->m_oStart.IsInit()) convert(oox_num_lvl->m_oPPr->m_oInd->m_oStart.GetPointer(), length_margin);
				
				if (length_margin) aligment_props->fo_margin_left_ = odf_types::length(length_margin->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}
			if (oox_num_lvl->m_oPPr->m_oInd->m_oEnd.IsInit())
			{
				convert(oox_num_lvl->m_oPPr->m_oInd->m_oEnd.GetPointer(), length_margin);
			
				if (length_margin) aligment_props->fo_margin_right_  = odf_types::length(length_margin->get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}	
		}else
		{
			aligment_props->fo_text_indent_ = odf_types::length(0,odf_types::length::cm);
			aligment_props->fo_margin_left_ = odf_types::length(0,odf_types::length::cm);

		}
	}
	if (oox_num_lvl->m_oRPr.IsInit())//для обозначений списка
	{
		odf_writer::odf_style_context* styles_context = odf_context()->page_layout_context()->get_local_styles_context();
		
		odf_writer::style_text_properties *text_props = odt_context->styles_context()->lists_styles().get_text_properties();
		convert(oox_num_lvl->m_oRPr.GetPointer(), text_props);

		//create text style for symbols list НА ЛОКАЛЬНОМ контексте - иначе пересечение имен стилей (todoo вытащить генерацию имен в общую часть)
		styles_context->create_style(L"",odf_types::style_family::Text, false, true, -1);					
		odf_writer::odf_style_state_ptr style_state = styles_context->last_state(odf_types::style_family::Text);
		if (style_state)
		{
			odt_context->styles_context()->lists_styles().set_text_style_name( style_state->get_name());		
			
			odf_writer::style_text_properties	* text_props_2 = style_state->get_text_properties();			
			if (text_props_2)text_props_2->apply_from(text_props);
		}

		if ((text_props) && (text_props->content().fo_font_size_))
		{
			size_bullet_number_marker = text_props->content().fo_font_size_->get_length().get_value();
		}
	}
	if (size_bullet_number_marker < 0.01 && (type_list == 2 || type_list == 3))
	{
		//выдернем из дефолтного
		odf_writer::odf_style_state_ptr state;
		if ((odf_context()->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph,  state)) && (state))
		{
			odf_writer::style_text_properties *text_props = state->get_text_properties();
			if ((text_props) && (text_props->content().fo_font_size_))
			{
				size_bullet_number_marker = text_props->content().fo_font_size_->get_length().get_value();
			}
		}
	}
	if ((oox_num_lvl->m_oStart.IsInit()) && (oox_num_lvl->m_oStart->m_oVal.IsInit()))
	{
		int start_value = oox_num_lvl->m_oStart->m_oVal->GetValue();
		if (start_value > 1)
			odt_context->styles_context()->lists_styles().set_start_number(start_value);
	}
	if (oox_num_lvl->m_oSuffix.IsInit())
	{
	}
	if (oox_num_lvl->m_oLvlText.IsInit() && oox_num_lvl->m_oLvlText->m_sVal.IsInit())
	{
		if (type_list == 2)
			odt_context->styles_context()->lists_styles().set_bullet_char(string2std_string(oox_num_lvl->m_oLvlText->m_sVal.get()));
		else if (type_list == 1)
			odt_context->styles_context()->lists_styles().set_numeric_format(string2std_string(oox_num_lvl->m_oLvlText->m_sVal.get()));
	}
	if (oox_num_lvl->m_oLvlPicBulletId.IsInit() && oox_num_lvl->m_oLvlPicBulletId->m_oVal.IsInit())
	{
		int id = oox_num_lvl->m_oLvlPicBulletId->m_oVal->GetValue();
		OOX::CNumbering * lists_styles = docx_document->GetNumbering();

		for (unsigned int i = 0; (lists_styles) && (i< lists_styles->m_arrNumPicBullet.size()); i++)
		{
			if ((lists_styles->m_arrNumPicBullet[i]) && (lists_styles->m_arrNumPicBullet[i]->m_oNumPicBulletId.GetValue() == id))
			{
				if (lists_styles->m_arrNumPicBullet[i]->m_oDrawing.IsInit())
				{
					convert(lists_styles->m_arrNumPicBullet[i]->m_oDrawing.GetPointer());
				}
				else
				{
					convert(lists_styles->m_arrNumPicBullet[i]->m_oVmlDrawing.GetPointer());
				}
				if (odt_context->mediaitems()->items().size() > 0)
				{
					odt_context->styles_context()->lists_styles().set_bullet_image(odt_context->mediaitems()->items().back().odf_ref);
				}
			}
		}

	}
	if (type_list == 2 || type_list == 3)
		odt_context->styles_context()->lists_styles().set_bullet_image_size(size_bullet_number_marker);

	if (oox_num_lvl->m_oLvlRestart.IsInit() && oox_num_lvl->m_oLvlRestart->m_oVal.IsInit() && type_list == 1)
	{
		 odt_context->styles_context()->lists_styles().set_start_number(oox_num_lvl->m_oLvlRestart->m_oVal->GetValue());
	}

	odt_context->styles_context()->lists_styles().end_style_level();

}
void DocxConverter::convert_table_style(OOX::CStyle *oox_style)
{
	if (oox_style == NULL)return;

	std::wstring oox_name = oox_style->m_sStyleId.IsInit() ? string2std_string(*oox_style->m_sStyleId) : L"";

	odt_context->styles_context()->table_styles().start_style(oox_name);
//общие

	if (oox_style->m_oTblPr.IsInit())
	{
		odf_writer::style_table_properties	* table_properties = odt_context->styles_context()->table_styles().get_table_properties();
		convert(oox_style->m_oTblPr.GetPointer(), table_properties);
		
		//нужно проверить стоит ли отнаследоваться от base_on 

		
		if (oox_style->m_oTblPr->m_oTblBorders.IsInit())
		{
			odf_writer::style_table_cell_properties * table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
			convert(oox_style->m_oTblPr->m_oTblBorders.GetPointer(), table_cell_properties);

			std::wstring inside_border;
			if (oox_style->m_oTblPr->m_oTblBorders->m_oInsideH.IsInit()) 
			{
				convert(oox_style->m_oTblPr->m_oTblBorders->m_oInsideH.GetPointer(), inside_border);
				odt_context->styles_context()->table_styles().set_table_insideH(inside_border);
				
			}
			if (oox_style->m_oTblPr->m_oTblBorders->m_oInsideV.IsInit()) 
			{
				convert(oox_style->m_oTblPr->m_oTblBorders->m_oInsideV.GetPointer(), inside_border);
				odt_context->styles_context()->table_styles().set_table_insideV(inside_border);
				
			}
		}
	}
	if (oox_style->m_oRunPr.IsInit())
	{
		odf_writer::style_text_properties	* text_properties = odt_context->styles_context()->table_styles().get_text_properties();
		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf_writer::style_paragraph_properties	* paragraph_properties = odt_context->styles_context()->table_styles().get_paragraph_properties();
		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);
	}

	if (oox_style->m_oTcPr.IsInit())
	{
		odf_writer::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
		convert(oox_style->m_oTcPr.GetPointer(), table_cell_properties);
	}	
	//if (oox_style->m_oTrPr.IsInit())
	//{
	//	odf_writer::style_table_row_properties	* table_row_properties = odt_context->styles_context()->table_styles().get_table_row_properties();
	//	convert(oox_style->m_oTrPr.GetPointer(), table_row_properties);
	//}
//отдельные
	for (unsigned int i = 0 ; i <oox_style->m_arrTblStylePr.size() ; i++)
	{
		if (oox_style->m_arrTblStylePr[i] == NULL) continue;

		if (oox_style->m_arrTblStylePr[i]->m_oType.IsInit() == false) continue;
		switch (oox_style->m_arrTblStylePr[i]->m_oType->GetValue())
		{
		case SimpleTypes::tblstyleoverridetypeBand1Horz : odt_context->styles_context()->table_styles().add_band1Horz();	break;
		case SimpleTypes::tblstyleoverridetypeBand1Vert : odt_context->styles_context()->table_styles().add_band1Vert();	break;
		case SimpleTypes::tblstyleoverridetypeBand2Horz : odt_context->styles_context()->table_styles().add_band2Horz();	break;
		case SimpleTypes::tblstyleoverridetypeBand2Vert : odt_context->styles_context()->table_styles().add_band2Vert();	break;
		case SimpleTypes::tblstyleoverridetypeFirstCol	: odt_context->styles_context()->table_styles().add_firstCol();		break;
		case SimpleTypes::tblstyleoverridetypeFirstRow	: odt_context->styles_context()->table_styles().add_firstRow();		break;
		case SimpleTypes::tblstyleoverridetypeLastCol	: odt_context->styles_context()->table_styles().add_lastCol();		break;
		case SimpleTypes::tblstyleoverridetypeLastRow	: odt_context->styles_context()->table_styles().add_lastRow();		break;
		case SimpleTypes::tblstyleoverridetypeNeCell	: odt_context->styles_context()->table_styles().add_neCell();		break;
		case SimpleTypes::tblstyleoverridetypeNwCell	: odt_context->styles_context()->table_styles().add_nwCell();		break;
		case SimpleTypes::tblstyleoverridetypeSeCell	: odt_context->styles_context()->table_styles().add_seCell();		break;
		case SimpleTypes::tblstyleoverridetypeSwCell	: odt_context->styles_context()->table_styles().add_swCell();		break;
		case SimpleTypes::tblstyleoverridetypeWholeTable : odt_context->styles_context()->table_styles().add_wholeTable();	break;
		}
		//сначела отнаследоваться от общих настроек???
		convert(oox_style->m_arrTblStylePr[i]->m_oTcPr.GetPointer(), odt_context->styles_context()->table_styles().get_table_cell_properties());
		convert(oox_style->m_arrTblStylePr[i]->m_oRunPr.GetPointer(),odt_context->styles_context()->table_styles().get_text_properties());
		convert(oox_style->m_arrTblStylePr[i]->m_oParPr.GetPointer(),odt_context->styles_context()->table_styles().get_paragraph_properties());

			//nullable<OOX::Logic::CTableProperty      >      m_oTblPr;
			//nullable<OOX::Logic::CTableRowProperties >      m_oTrPr;
	}

	odt_context->styles_context()->table_styles().end_style();

}
void DocxConverter::convert(OOX::CStyle	*oox_style)
{
	if (oox_style == NULL)return;

	if (oox_style->m_oType.IsInit() == false)return;
	odf_types::style_family::type family = odf_types::style_family::None;

	if ( SimpleTypes::styletypeNumbering == oox_style->m_oType->GetValue())
	{
		//????
		return;
	}
	
	if ( SimpleTypes::styletypeTable == oox_style->m_oType->GetValue())
	{
		convert_table_style(oox_style);
		return;
	}
	

	switch(oox_style->m_oType->GetValue())
	{
		case SimpleTypes::styletypeCharacter : family = odf_types::style_family::Text;		break;
		case SimpleTypes::styletypeParagraph : family = odf_types::style_family::Paragraph;	break;
		default:  
			return;
	}

	std::wstring oox_name = oox_style->m_sStyleId.IsInit() ? string2std_string(*oox_style->m_sStyleId) : L"";

	odt_context->styles_context()->create_style(oox_name,family, false, true, -1); 


	if (oox_style->m_oName.IsInit() && oox_style->m_oName->m_sVal.IsInit()) 
		odt_context->styles_context()->last_state()->set_display_name(string2std_string(*oox_style->m_oName->m_sVal));

	if (oox_style->m_oRunPr.IsInit())
	{
		odf_writer::style_text_properties	* text_properties = odt_context->styles_context()->last_state()->get_text_properties();
	
		if (oox_style->m_oDefault.IsInit() && oox_style->m_oDefault->ToBool())
		{
			//основан на дефолтовом - накатить
			odf_writer::odf_style_state_ptr def_style_state;
			if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
			{
				odf_writer::style_text_properties * props = def_style_state->get_text_properties();
				text_properties->apply_from(props);
			}
		}

		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf_writer::style_paragraph_properties	* paragraph_properties = odt_context->styles_context()->last_state()->get_paragraph_properties();
		if (oox_style->m_oDefault.IsInit() && oox_style->m_oDefault->ToBool())
		{
			//основан на дефолтовом - накатить
			odf_writer::odf_style_state_ptr def_style_state;
			if (odt_context->styles_context()->find_odf_default_style_state(odf_types::style_family::Paragraph, def_style_state) && def_style_state)
			{
				odf_writer::style_paragraph_properties * props = def_style_state->get_paragraph_properties();
				paragraph_properties->apply_from(props);
			}
		}

		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);

		if (oox_style->m_oParPr->m_oNumPr.IsInit())
		{
			int level = (oox_style->m_oParPr->m_oNumPr->m_oIlvl.IsInit() && oox_style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal.IsInit()) ? 
				oox_style->m_oParPr->m_oNumPr->m_oIlvl->m_oVal->GetValue() : -1;
			int id	  = (oox_style->m_oParPr->m_oNumPr->m_oNumID.IsInit() && oox_style->m_oParPr->m_oNumPr->m_oNumID->m_oVal.IsInit()) ? 
				oox_style->m_oParPr->m_oNumPr->m_oNumID->m_oVal->GetValue() : -1;

			if (level<0 && id >=0) level =0;
			if (level >=0 && id >=0)
			{
				odt_context->styles_context()->last_state()->set_list_style_level(level);
				odt_context->styles_context()->last_state()->set_list_style_id(id);
				odt_context->styles_context()->last_state()->set_list_style_exist(true);
		
				std::wstring list_style_name = odt_context->styles_context()->lists_styles().get_style_name(id); 
				odt_context->styles_context()->last_state()->set_list_style_name(list_style_name);			}
		}
	}
	if (oox_style->m_oBasedOn.IsInit() && oox_style->m_oBasedOn->m_sVal.IsInit())
		odt_context->styles_context()->last_state()->set_parent_style_name(string2std_string(*oox_style->m_oBasedOn->m_sVal));

		//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue>> m_oQFormat;
		//nullable<ComplexTypes::Word::CString_                       > m_oAliases;

}

void DocxConverter::convert(OOX::Logic::CCommentRangeStart* oox_comm_start)
{
	if(oox_comm_start == NULL)return;
	if (oox_comm_start->m_oId.IsInit() == false) return;

	int oox_comm_id = oox_comm_start->m_oId->GetValue();

	bool added = odt_context->start_comment(oox_comm_id);

	if (added==false)
	{
		convert_comment(oox_comm_id);
	}
}

void DocxConverter::convert(OOX::Logic::CCommentRangeEnd* oox_comm_end)
{
	if(oox_comm_end == NULL)return;
	if (oox_comm_end->m_oId.IsInit() == false) return;

	int oox_comm_id = oox_comm_end->m_oId->GetValue();

	 odt_context->end_comment(oox_comm_id);
}
void DocxConverter::convert(OOX::Logic::CCommentReference* oox_comm_ref)
{
	if(oox_comm_ref == NULL)return;
	if (oox_comm_ref->m_oId.IsInit() == false) return;

	int oox_comm_id = oox_comm_ref->m_oId->GetValue();

	bool added = odt_context->start_comment(oox_comm_id);

	if (added == false) // типо тока стартанул
	{
		//значит старт тута а не по RangeStart
		convert_comment(oox_comm_id);
	}

}
void DocxConverter::convert_comment(int oox_comm_id)
{
	OOX::CComments * docx_comments = docx_document->GetComments();
	if (!docx_comments)return;

	for (unsigned int comm =0 ; comm < docx_comments->m_arrComments.size(); comm++)
	{
		OOX::CComment* oox_comment = docx_comments->m_arrComments[comm];
		if (oox_comment == NULL) continue;
		if (oox_comment->m_oId.IsInit() == false) continue;
		
		if (oox_comment->m_oId->GetValue() == oox_comm_id)
		{
			odt_context->start_comment_content();
			{
				if (oox_comment->m_oAuthor.IsInit())odt_context->comment_context()->set_author	(string2std_string(*oox_comment->m_oAuthor));
				if (oox_comment->m_oDate.IsInit())	odt_context->comment_context()->set_date	(string2std_string(oox_comment->m_oDate->GetValue()));
				if (oox_comment->m_oInitials.IsInit()){}

				for (unsigned int i=0; i <oox_comment->m_arrItems.size(); i++)
				{
					convert(oox_comment->m_arrItems[i]);
				}
			}
			odt_context->end_comment_content();
		}
	}
}
void DocxConverter::convert_hdr_ftr	(CString sId)
{
	OOX::CHdrFtr * oox_hdr_ftr = docx_document->GetHeaderOrFooter(sId);
	if (oox_hdr_ftr == NULL ) return;

	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(oox_hdr_ftr);

	for ( unsigned int nIndex = 0; nIndex < oox_hdr_ftr->m_arrItems.size(); nIndex++ )
	{
		convert(oox_hdr_ftr->m_arrItems[nIndex]);
	}
	oox_current_child_document  = NULL;
}
void DocxConverter::convert(OOX::Logic::CTbl *oox_table)
{
	if (oox_table == NULL) return;

	if (odt_context->text_context()->get_KeepNextParagraph())
	{
		odt_context->end_paragraph();
		odt_context->text_context()->set_KeepNextParagraph(false);
	}

	//if (in_list, but not in paragraph)
	odt_context->set_no_list();

	bool styled_table		= false;
	bool in_frame			= false;
	int  in_frame_anchor	= odf_types::anchor_type::Paragraph;
	
	if (oox_table->m_oTableProperties && (oox_table->m_oTableProperties->m_oTblStyle.IsInit() && oox_table->m_oTableProperties->m_oTblStyle->m_sVal.IsInit()))
	{//настройка предустановленного стиля
		std::wstring base_style_name = string2std_string(*oox_table->m_oTableProperties->m_oTblStyle->m_sVal);

		styled_table = odt_context->styles_context()->table_styles().start_table(base_style_name);
	}

	convert(oox_table->m_oTableProperties, styled_table );	
	
	if(oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblpPr.IsInit())
	{
		if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX.IsInit()) 
		{
			if (oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor.IsInit() && 
				oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor->GetValue() == SimpleTypes::hanchorText)
				in_frame = false;
			else 
				in_frame = true;
		}
		if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY.IsInit())
		{
			in_frame = true;
			if (oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor.IsInit() && 
				oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor->GetValue() == SimpleTypes::vanchorText)
			{
				//in_frame_anchor	= odf_types::anchor_type::Char;
			}
		}
	}

	if (in_frame)
	{
		odt_context->start_paragraph();
			odt_context->start_drawings();
				_CP_OPT(double) width, height, x, y ;
				
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX.IsInit())
					x = oox_table->m_oTableProperties->m_oTblpPr->m_oTblpX->ToPoints();
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY.IsInit())
					y = oox_table->m_oTableProperties->m_oTblpPr->m_oTblpY->ToPoints();

				//if (oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText.IsInit())
				//{
				//	if (!x) x = oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText->ToPoints();
				//	else x = -*x + oox_table->m_oTableProperties->m_oTblpPr->m_oLeftFromText->ToPoints();
				//}

				odt_context->drawing_context()->set_drawings_rect(x, y, width, height);	
				
				odt_context->drawing_context()->set_anchor(in_frame_anchor); 
				odt_context->drawing_context()->set_wrap_style(odf_types::style_wrap::Dynamic);

				if (oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor.IsInit())
				{
					switch(oox_table->m_oTableProperties->m_oTblpPr->m_oVertAnchor->GetValue())
					{
						case SimpleTypes::vanchorMargin: odt_context->drawing_context()->set_vertical_rel(6); break;//1???
						case SimpleTypes::vanchorPage:	 odt_context->drawing_context()->set_vertical_rel(5); break;
					}
				}
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor.IsInit())
				{
					switch(oox_table->m_oTableProperties->m_oTblpPr->m_oHorzAnchor->GetValue())
					{
						case SimpleTypes::hanchorMargin:	odt_context->drawing_context()->set_horizontal_rel(8); break;
						case SimpleTypes::hanchorPage:		odt_context->drawing_context()->set_horizontal_rel(6); break;
					}
				}		

				if (oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText.IsInit())
				{
					if (!x) //x = *x + oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText->ToPoints();
						x = oox_table->m_oTableProperties->m_oTblpPr->m_oRightFromText->ToPoints();

					odt_context->drawing_context()->set_horizontal_pos(*x);
				}
				if (oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText.IsInit())
				{
					if (!y) //y = *y + oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText->ToPoints();
						y = oox_table->m_oTableProperties->m_oTblpPr->m_oTopFromText->ToPoints();
					
					odt_context->drawing_context()->set_vertical_pos(*y);
				}
				odt_context->drawing_context()->start_drawing();	

				odt_context->drawing_context()->start_text_box();
					odt_context->drawing_context()->set_text_box_min_size(0, 1.);
					odt_context->drawing_context()->set_z_order(0x7fffffff-1);
					odt_context->drawing_context()->set_text_box_parent_style(L"Frame");
					odt_context->drawing_context()->set_name(L"TableFrame");
				odt_context->start_text_context();
	}

	odt_context->start_table(true); 

	_CP_OPT(std::wstring) border_inside_h,border_inside_v;

	if (styled_table)
	{
		odt_context->table_context()->set_table_styled(styled_table);
		
		if (oox_table->m_oTableProperties->m_oTblLook.IsInit() && oox_table->m_oTableProperties->m_oTblLook->m_oVal.IsInit())
			odt_context->styles_context()->table_styles().set_flags(oox_table->m_oTableProperties->m_oTblLook->m_oVal->GetValue());

		
		border_inside_h = odt_context->styles_context()->table_styles().get_table_insideH();
		border_inside_v = odt_context->styles_context()->table_styles().get_table_insideV();
	}
	if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders.IsInit())
	{
		std::wstring ins_b;
		if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders->m_oInsideH.IsInit())
		{
			convert(oox_table->m_oTableProperties->m_oTblBorders->m_oInsideH.GetPointer(), ins_b);
			border_inside_h = ins_b;
		}
		if (oox_table->m_oTableProperties && oox_table->m_oTableProperties->m_oTblBorders->m_oInsideV.IsInit())
		{
			convert(oox_table->m_oTableProperties->m_oTblBorders->m_oInsideV.GetPointer(), ins_b);
			border_inside_v = ins_b;
		}
	}
	odt_context->table_context()->set_table_inside_v(border_inside_v);
	odt_context->table_context()->set_table_inside_h(border_inside_h);

	int count_rows = oox_table->m_nCountRow;
	int count_columns = 0;
	if (oox_table->m_oTblGrid.IsInit())count_columns = oox_table->m_oTblGrid->m_arrGridCol.size();
	
	odt_context->styles_context()->table_styles().set_current_dimension(count_columns, count_rows);
	odt_context->table_context()->count_rows(count_rows);
//------ колонки
	convert(oox_table->m_oTblGrid.GetPointer());

//------ строки
	for (unsigned int i =0 ; i < oox_table->m_arrItems.size(); i++)
	{
		switch(oox_table->m_arrItems[i]->getType())
		{
			case OOX::et_w_tblPr:	
			{
				//skip 
			}break;
			default:				
				convert(oox_table->m_arrItems[i]);
		}
	}
	if (in_frame)
	{
		_CP_OPT(double) width = odt_context->table_context()->get_table_width();
		_CP_OPT(double) height;
		odt_context->drawing_context()->set_size(width, height);

		odt_context->drawing_context()->set_text_box_min_size(-1,0);
	}
	
	odt_context->end_table();

	if (odt_context->table_context()->is_styled())
		odt_context->styles_context()->table_styles().end_table();

	if (in_frame)
	{
		odt_context->drawing_context()->set_text( odt_context->text_context());
		odt_context->end_text_context();
		
		odt_context->drawing_context()->end_text_box();
		odt_context->drawing_context()->end_drawing();
		odt_context->end_drawings();
		
		odt_context->end_paragraph();
	}	
}
void DocxConverter::convert(OOX::Logic::CTblGrid	*oox_table_grid)
{
	if (oox_table_grid == NULL) return;

	odt_context->start_table_columns();
	//nullable<OOX::Logic::CTblGridChange          > m_oTblGridChange;
	for (int i =0 ; i < oox_table_grid->m_arrGridCol.size(); i++)
	{
		if (oox_table_grid->m_arrGridCol[i] == NULL) continue;
		double width = -1;

		if (oox_table_grid->m_arrGridCol[i]->m_oW.IsInit())
		{
			width = oox_table_grid->m_arrGridCol[i]->m_oW->ToPoints();
		}

		odt_context->add_table_column(width);

		if (width > 0 && width < 5) //2222010_53102Reader final.docx  - настройка через ячейки
		{
			//odt_context->table_context()->set_column_optimal(true);
			//2222010_53102Reader final.docx 
		}
			
	}	
	odt_context->end_table_columns();
}
void DocxConverter::convert(OOX::Logic::CTr	*oox_table_row)
{
	if (oox_table_row == NULL) return;

	bool styled = oox_table_row->m_oTableRowProperties ? true : false;
	bool is_header = false;

	if (styled && oox_table_row->m_oTableRowProperties->m_oTblHeader.IsInit()
		&& oox_table_row->m_oTableRowProperties->m_oTblHeader->m_oVal.ToBool() )is_header = true;

	if (is_header)odt_context->start_table_header_rows();

	odt_context->start_table_row(styled);
	
	convert(oox_table_row->m_oTableRowProperties);

	for (unsigned int i =0 ; i < oox_table_row->m_arrItems.size(); i++)
	{
		switch(oox_table_row->m_arrItems[i]->getType())
		{
			case OOX::et_w_trPr:	
			{
				//skip
			}break;
			default:
				convert(oox_table_row->m_arrItems[i]);
		}
	}	
	odt_context->end_table_row();

	if (is_header)odt_context->end_table_header_rows();
}
void DocxConverter::convert(OOX::Logic::CTc	*oox_table_cell)
{
	if (oox_table_cell == NULL) return;

	bool covered = false;

	if (oox_table_cell->m_oTableCellProperties)
	{
		if (oox_table_cell->m_oTableCellProperties->m_oVMerge.IsInit())
		{
			if (!(oox_table_cell->m_oTableCellProperties->m_oVMerge->m_oVal.IsInit() && 
					oox_table_cell->m_oTableCellProperties->m_oVMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart))
				covered = true; 
		}
	}

	odt_context->start_table_cell( oox_table_cell->m_nNumCol,covered, convert(oox_table_cell->m_oTableCellProperties, oox_table_cell->m_nNumCol+1));
	
	if (oox_table_cell->m_oTableCellProperties)
	{
		if (oox_table_cell->m_oTableCellProperties->m_oVMerge.IsInit())
		{
			if (oox_table_cell->m_oTableCellProperties->m_oVMerge->m_oVal.IsInit() && 
					oox_table_cell->m_oTableCellProperties->m_oVMerge->m_oVal->GetValue() == SimpleTypes::mergeRestart)
				odt_context->table_context()->set_cell_row_span_restart();
			else 
				odt_context->table_context()->set_cell_row_span();
		}
		if (oox_table_cell->m_oTableCellProperties->m_oGridSpan.IsInit() && oox_table_cell->m_oTableCellProperties->m_oGridSpan->m_oVal.IsInit())
			odt_context->table_context()->set_cell_column_span(oox_table_cell->m_oTableCellProperties->m_oGridSpan->m_oVal->GetValue());
	}

	for (unsigned int i =0 ; i < oox_table_cell->m_arrItems.size(); i++)
	{
		switch(oox_table_cell->m_arrItems[i]->getType())
		{
			case OOX::et_w_tcPr:	
			{
				//skip
			}break;
			default:
				convert(oox_table_cell->m_arrItems[i]);
		}
	}		
	odt_context->end_table_cell();
}
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, odf_writer::style_table_properties	* table_properties )
{
	if (oox_table_pr == NULL) return false;
	if (table_properties == NULL) return false;

	if (oox_table_pr->m_oTblW.IsInit())
	{
		if ((oox_table_pr->m_oTblW->m_oType.IsInit() && oox_table_pr->m_oTblW->m_oType->GetValue() == SimpleTypes::tblwidthDxa) &&
			(oox_table_pr->m_oTblW->m_oW.IsInit() && oox_table_pr->m_oTblW->m_oW->GetValue() >0))
		{
			if (oox_table_pr->m_oTblW->m_oW->IsPercent() == false)
				odt_context->table_context()->set_default_column_width(oox_table_pr->m_oTblW->m_oW->GetValue()/20.);
		}
	}

	if (oox_table_pr->m_oTblStyleColBandSize.IsInit())
	{
	}
	if (oox_table_pr->m_oTblStyleRowBandSize.IsInit())
	{
	}
	
	if (oox_table_pr->m_oTblInd.IsInit())//отступ слева - обтекания нет
	{
		_CP_OPT(odf_types::length) length;

		convert(oox_table_pr->m_oTblInd.GetPointer(), length);
        if (length)
        {
            table_properties->table_format_properties_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
        }
		table_properties->table_format_properties_.table_align_ = odf_types::table_align(odf_types::table_align::Left);
	}
	else if(oox_table_pr->m_oTblpPr.IsInit()) //отступы, обтекание есть 
	{
		table_properties->table_format_properties_.table_align_ = odf_types::table_align(odf_types::table_align::Left);

		if (oox_table_pr->m_oTblpPr->m_oTblpX.IsInit() && oox_table_pr->m_oTblpPr->m_oTblpY.IsInit()){}//floating position 
		else
		{
            _CP_OPT(odf_types::length) length_, length_left, length_right;
			
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oLeftFromText.GetPointer()), length_left);
            if (length_left)
                table_properties->table_format_properties_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(length_left->get_value_unit(odf_types::length::cm),odf_types::length::cm);
				
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oRightFromText.GetPointer()), length_right);
            if (length_right)
                table_properties->table_format_properties_.common_horizontal_margin_attlist_.fo_margin_right_= odf_types::length(length_right->get_value_unit(odf_types::length::cm),odf_types::length::cm);

            convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oTopFromText.GetPointer()), length_);
            if (length_)
                table_properties->table_format_properties_.common_vertical_margin_attlist_.fo_margin_top_= odf_types::length(length_->get_value_unit(odf_types::length::cm),odf_types::length::cm);

            convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_pr->m_oTblpPr->m_oBottomFromText.GetPointer()),	length_);
            if (length_)
                table_properties->table_format_properties_.common_vertical_margin_attlist_.fo_margin_bottom_= odf_types::length(length_->get_value_unit(odf_types::length::cm),odf_types::length::cm);

			if ((length_left && length_right ) && (*length_left == *length_right))
				table_properties->table_format_properties_.table_align_ = odf_types::table_align(odf_types::table_align::Center);
		}
	}
	else if (oox_table_pr->m_oTblLayout.IsInit() && oox_table_pr->m_oTblLayout->m_oType.IsInit())
	{
		table_properties->table_format_properties_.common_horizontal_margin_attlist_.fo_margin_left_ = odf_types::length(0,odf_types::length::cm);
		
		table_properties->table_format_properties_.table_align_ = odf_types::table_align(odf_types::table_align::Left);
	}
	//if(oox_table_pr->m_oJc.IsInit() && oox_table_pr->m_oJc->m_oVal.IsInit())
	//{
	//}
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBidiVisual;
	//nullable<ComplexTypes::Word::CShading                        > m_oShade;
	//nullable<ComplexTypes::Word::CString_                        > m_oTblCaption;
	//nullable<OOX::Logic::CTblCellMar                             > m_oTblCellMar;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
	//nullable<ComplexTypes::Word::CString_                        > m_oTblDescription;
	//nullable<ComplexTypes::Word::CTblLayoutType                  > m_oTblLayout;
	//nullable<ComplexTypes::Word::CTblOverlap                     > m_oTblOverlap;
	//nullable<OOX::Logic::CTblPrChange                            > m_oTblPrChange;


	return true;
}
void DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, odf_writer::style_table_cell_properties	* table_cell_properties)
{
	if (oox_table_pr == NULL || oox_table_pr == NULL) return;

	convert(oox_table_pr->m_oTblBorders.GetPointer(), table_cell_properties);

	if (oox_table_pr->m_oTblCellMar.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(oox_table_pr->m_oTblCellMar->m_oStart.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_pr->m_oTblCellMar->m_oEnd.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_pr->m_oTblCellMar->m_oTop.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_top_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
		
		convert(oox_table_pr->m_oTblCellMar->m_oBottom.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_bottom_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
}
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, bool base_styled)
{
	if (oox_table_pr && oox_table_pr->m_oTblBorders.IsInit())
	{//напрямую задать cell_prop на саму таблицу низя - тока как default-cell-style-name на columns & row

		//общие свойства ячеек
		odt_context->styles_context()->create_style(L"",odf_types::style_family::TableCell, true, false, -1); //ради нормального задания дефолтовых свойств на cells
		odt_context->styles_context()->last_state()->set_dont_write(true);
		odf_writer::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->last_state()->get_table_cell_properties();
		
		if (base_styled)// накатим свойства из стиля
		{			
			odt_context->styles_context()->table_styles().get_table_cell_properties(table_cell_properties);
		}		
		convert(oox_table_pr, table_cell_properties);

		odt_context->table_context()->set_default_cell_properties(odt_context->styles_context()->last_state()->get_name());
	}
//стиль создаем всегда	
	odt_context->styles_context()->create_style(L"",odf_types::style_family::Table, true, false, -1); 
	
	odf_writer::style_table_properties	* table_properties = odt_context->styles_context()->last_state()->get_table_properties();
	if (base_styled)// накатим свойства - они не наследуются :(
	{		
		odt_context->styles_context()->table_styles().get_table_properties(table_properties);
	}
	convert(oox_table_pr, table_properties);
	
	return true;
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr, odf_writer::style_table_row_properties	* table_row_properties)
{
	if (oox_table_row_pr == NULL) return;
	if (table_row_properties == NULL) return;

	if (oox_table_row_pr->m_oTblHeight.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_row_pr->m_oTblHeight->m_oVal.GetPointer()), length);

		if (oox_table_row_pr->m_oTblHeight->m_oHRule.IsInit())
		{
			switch(oox_table_row_pr->m_oTblHeight->m_oHRule->GetValue())
			{
				case SimpleTypes::heightruleAtLeast:
					table_row_properties->style_table_row_properties_attlist_.style_min_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm); break;
				case SimpleTypes::heightruleExact:
					table_row_properties->style_table_row_properties_attlist_.style_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm); break;
				case SimpleTypes::heightruleAuto:
					table_row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; break;
			}
		}
		else
			table_row_properties->style_table_row_properties_attlist_.style_min_row_height_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr)
{
	if (oox_table_row_pr == NULL) return;

	odf_writer::style_table_row_properties	* table_row_properties = odt_context->styles_context()->last_state()->get_table_row_properties();

	if (oox_table_row_pr->m_oCnfStyle.IsInit())
	{
	}

	convert(oox_table_row_pr, table_row_properties);

	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oCantSplit;
	//nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oDel;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oDivId;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridAfter;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oGridBefore;
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oHidden;
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oIns;
	//nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
	//nullable<OOX::Logic::CTrPrChange                             > m_oTrPrChange;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oWAfter;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oWBefore;

}
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr,odf_writer::style_table_cell_properties	* table_cell_properties)
{
	if (oox_table_cell_pr == NULL) return false;
	if (table_cell_properties == NULL) return false;

	if (oox_table_cell_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_table_cell_pr->m_oShd->m_oFill.GetPointer(), oox_table_cell_pr->m_oShd->m_oThemeFill.GetPointer(),
			oox_table_cell_pr->m_oShd->m_oThemeFillTint.GetPointer(), oox_table_cell_pr->m_oShd->m_oThemeShade.GetPointer(), odf_color);
		if (odf_color)
			table_cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_ = *odf_color;

		if (oox_table_cell_pr->m_oShd->m_oVal.IsInit())
		{
			//паттерная заливка ячейки :(
		}
	}

	if (oox_table_cell_pr->m_oTextDirection.IsInit() && oox_table_cell_pr->m_oTextDirection->m_oVal.IsInit())
	{
		switch(oox_table_cell_pr->m_oTextDirection->m_oVal->GetValue())
		{
		case SimpleTypes::textdirectionLr  :
			table_cell_properties->style_table_cell_properties_attlist_.style_direction_ = odf_types::direction(odf_types::direction::Ltr);break;
		case SimpleTypes::textdirectionLrV :
		case SimpleTypes::textdirectionTb  ://повернутость буковок
		case SimpleTypes::textdirectionTbV :
		case SimpleTypes::textdirectionRlV :
			table_cell_properties->style_table_cell_properties_attlist_.style_direction_ = odf_types::direction(odf_types::direction::Ttb);break;
		case SimpleTypes::textdirectionRl  ://rtl
			break;
		}
	}
	convert(oox_table_cell_pr->m_oTcBorders.GetPointer() , table_cell_properties);

	if (oox_table_cell_pr->m_oTcMar.IsInit())
	{
		_CP_OPT(odf_types::length) length;
		convert(oox_table_cell_pr->m_oTcMar->m_oStart.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_left_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_cell_pr->m_oTcMar->m_oEnd.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_right_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

		convert(oox_table_cell_pr->m_oTcMar->m_oTop.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_top_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);
		
		convert(oox_table_cell_pr->m_oTcMar->m_oBottom.GetPointer(), length);
		if (length)table_cell_properties->style_table_cell_properties_attlist_.common_padding_attlist_.fo_padding_bottom_ = odf_types::length(length->get_value_unit(odf_types::length::cm),odf_types::length::cm);

	}
	if (oox_table_cell_pr->m_oVAlign.IsInit() && oox_table_cell_pr->m_oVAlign->m_oVal.IsInit())
	{
		switch(oox_table_cell_pr->m_oVAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticaljcBoth   : //??????
			table_cell_properties->style_table_cell_properties_attlist_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Justify); break;
		case SimpleTypes::verticaljcBottom :
			table_cell_properties->style_table_cell_properties_attlist_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
		case SimpleTypes::verticaljcCenter :
			table_cell_properties->style_table_cell_properties_attlist_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Middle); break;
		case SimpleTypes::verticaljcTop    :
			table_cell_properties->style_table_cell_properties_attlist_.style_vertical_align_ = odf_types::vertical_align(odf_types::vertical_align::Top); break;
		}
	}

	if (oox_table_cell_pr->m_oTcFitText.IsInit() )
	{
		table_cell_properties->style_table_cell_properties_attlist_.style_shrink_to_fit_ = oox_table_cell_pr->m_oTcFitText->m_oVal.ToBool();
	}

	if (oox_table_cell_pr->m_oNoWrap.IsInit())
	{
		if (oox_table_cell_pr->m_oNoWrap->m_oVal.ToBool() == true)
			table_cell_properties->style_table_cell_properties_attlist_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::NoWrap);
		else
			table_cell_properties->style_table_cell_properties_attlist_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::Wrap);
	}
	if (oox_table_cell_pr->m_oTcW.IsInit() && oox_table_cell_pr->m_oTcW->m_oW.IsInit() &&
		oox_table_cell_pr->m_oTcW->m_oType.IsInit() && oox_table_cell_pr->m_oTcW->m_oType->GetValue() == SimpleTypes::tblwidthDxa)
	{
		//кастомная ширина ячейки :( //2222010_53102Reader final.docx - все равно кривоватенько
		odt_context->table_context()->change_current_column_width(oox_table_cell_pr->m_oTcW->m_oW->GetValue()/ 20.);//dxa type
	}
	
	
	//nullable<OOX::Logic::CHeaders                                > m_oHeaders;
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oHideMark;
	return true;
}
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr, int col)
{
	bool is_base_styled = odt_context->table_context()->is_styled();
	
	if (col < 0)col = odt_context->table_context()->current_column()+1;
	int			row = odt_context->table_context()->current_row();
		
	odf_writer::style_table_cell_properties * parent_cell_properties = NULL;

	std::wstring parent_name = odt_context->table_context()->get_default_cell_properties();
	if (parent_name.length() > 0) 
	{
		odf_writer::style * style_ = NULL;		
		if (odt_context->styles_context()->find_odf_style(parent_name,odf_types::style_family::TableCell,style_))
		{
			parent_cell_properties = style_->style_content_.get_style_table_cell_properties();
		}
	}
	
	if (oox_table_cell_pr == NULL && is_base_styled == false && parent_cell_properties == NULL) return false;
	
	odt_context->styles_context()->create_style(L"",odf_types::style_family::TableCell, true, false, -1); 	
	odf_writer::style_table_cell_properties *cell_properties = odt_context->styles_context()->last_state()->get_table_cell_properties();

	if (cell_properties == NULL) return false;

	if (is_base_styled)
	{
		odf_writer::style_text_properties		* text_properties		= odt_context->styles_context()->last_state()->get_text_properties();
		odf_writer::style_paragraph_properties	* paragraph_properties	= odt_context->styles_context()->last_state()->get_paragraph_properties();
		
		if (col < 0) col=odt_context->table_context()->current_column()+1;
		int row=odt_context->table_context()->current_row();
		
		odt_context->styles_context()->table_styles().get_table_cell_properties (col, row, cell_properties);
		odt_context->styles_context()->table_styles().get_text_properties		(col, row, text_properties);
		odt_context->styles_context()->table_styles().get_paragraph_properties	(col, row, paragraph_properties);
	}
	cell_properties->apply_from(parent_cell_properties);

//check for inside cell or not

	_CP_OPT(std::wstring) border_inside_v = odt_context->table_context()->get_table_inside_v();
	_CP_OPT(std::wstring) border_inside_h = odt_context->table_context()->get_table_inside_h();
	
	if ((border_inside_v || border_inside_h))
	{
		if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_)//раскидаем по сторонам
		{
			if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_->is_none() == false)
			{
				cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = 
				cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ =
				cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = 
				cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ =
												cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_;
			}		
			cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_ = boost::none;
		}	
		//если нет убрать, если да - добавить
		if (border_inside_h)
		{
			int del_border = border_inside_h->find(L"none");
			if (row != 1)
			{
				if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ && del_border>=0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = *border_inside_h;
			}
			if (row != odt_context->table_context()->count_rows())
			{
				if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ && del_border>=0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = *border_inside_h;
			}
		}
		if (border_inside_v)
		{
			int del_border = border_inside_v->find(L"none");
			if (col != 1)
			{
				if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ && del_border>=0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = *border_inside_h;
			}
			if (col != odt_context->table_context()->count_columns())
			{
				if (cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ && del_border>=0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = boost::none;

				else if (border_inside_h && del_border<0)
					cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = *border_inside_h;
			}
		}
	}
	bool res = convert(oox_table_cell_pr, cell_properties);

	return true;
}

} 

