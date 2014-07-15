#pragma once

#include "stdAfx.h"

#include "DocxConverter.h"
#include "../utils.h"

#include <DocxFormat\Docx.h>
#include <DocxFormat\External\Hyperlink.h>
#include <XlsxFormat\Chart\Chart.h>

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
DocxConverter::DocxConverter(const std::wstring & path, const ProgressCallback* CallBack)
{
	output_document = new  odf::package::odf_document(L"text");

	pCallBack = CallBack;

	const OOX::CPath oox_path(CString(path.c_str()));	
	docx_document = new OOX::CDocx(oox_path);	

//set flags to default
	m_bKeepNextParagraph = false;
	
	if (UpdateProgress(400000))return;
}
void DocxConverter::write(const std::wstring & path)
{
	if (!output_document)return;

	output_document->write(path);
		
	if (UpdateProgress(1000000))return;
}
odf::odf_conversion_context* DocxConverter::odf_context()
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
	smart_ptr<OOX::File> oFile = docx_document->GetDocument()->Find(sId);

	if (oFile.IsInit())
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
	if (ref.GetLength() < 1 && oox_current_chart)
	{
		smart_ptr<OOX::File> oFile = oox_current_chart->Find(sId);
		if (oFile.IsInit())
		{		
			if (type==1 && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
			if (type==2 && oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
		}
	}
	return ref;
}

void DocxConverter::convertDocument()
{
	if (!docx_document)return;

	odt_context = new odf::odt_conversion_context(output_document);

	if (!odt_context)return;
		
	odt_context->start_document();

	convert_styles();
	if (UpdateProgress(500000))return;

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

	convert(document->m_oBackground.GetPointer());//подложка
	for ( int nIndex = 0; nIndex < document->m_arrItems.GetSize(); nIndex++ )
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
		case OOX::et_w_Shape:
		{
			OOX::Logic::CShape* pShape = static_cast<OOX::Logic::CShape*>(oox_unknown);
			convert(pShape);
		}break;
		case OOX::et_w_GroupShape:
		{
			OOX::Logic::CGroupShape* pGroupShape= static_cast<OOX::Logic::CGroupShape*>(oox_unknown);
			convert(pGroupShape);
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
		case OOX::et_w_proofErr:
		case OOX::et_w_proofState:
		{
			//б€ка
		}break;
		default:
		{
			OoxConverter::convert(oox_unknown);
		}break;
	}
}
void DocxConverter::convert(OOX::Logic::CParagraph	*oox_paragraph)
{
	if (oox_paragraph == NULL) return;

	bool styled = false;

	bool bStartNewParagraph = !m_bKeepNextParagraph;
	
	if (oox_paragraph->m_oParagraphProperty) 
	{
		styled = true;
		odf::style_paragraph_properties	*paragraph_properties = NULL;

		if (m_bKeepNextParagraph)
		{
			odf::odf_style_state *state =  odt_context->styles_context()->last_state(odf::style_family::Paragraph);
			if (state)
				paragraph_properties = state->get_paragraph_properties();
		}
		else
		{
			odt_context->styles_context()->create_style(L"",odf::style_family::Paragraph, true, false, -1);					
			paragraph_properties = odt_context->styles_context()->last_state().get_paragraph_properties();
		}

		convert(oox_paragraph->m_oParagraphProperty, paragraph_properties); 
	}else m_bKeepNextParagraph = false; //Ќ”∆Ќќ ѕ–»¬я«ј“№ к ”–ќ¬Ќё на котором хранить m_bKeepNextParagraph !!! todooo
	//пока - если следующий не параграф - скидываем !!!
	
	if (bStartNewParagraph) odt_context->start_paragraph(styled);
	
	for ( int nIndex = 0; nIndex < oox_paragraph->m_arrItems.GetSize(); nIndex++ )
	{
		//те элементы которые тока дл€ Paragraph - здесь - остальные в общей куче		
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
	if (m_bKeepNextParagraph) odt_context->end_drop_cap();

	if (!m_bKeepNextParagraph)  odt_context->end_paragraph();
}
void DocxConverter::convert(OOX::Logic::CRun	*oox_run)//wordprocessing 22.1.2.87 math 17.3.2.25
{
	if (oox_run == NULL) return;
	
	//test for break - 2 first element Ё“ќ“ элемент Ќ”∆Ќќ вытащить отдельно !!!
	for(int i = 0; i < min (2,oox_run->m_arrItems.GetSize()); ++i)
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

		odt_context->styles_context()->create_style(L"",odf::style_family::Text, true, false, -1);					
		odf::style_text_properties	* text_properties = odt_context->styles_context()->last_state().get_text_properties();

		convert(oox_run->m_oRunProperty, text_properties);
	}
	
	odt_context->start_run(styled);
	
	for(int i = 0; i < oox_run->m_arrItems.GetSize(); ++i)
	{
		//те элементы которые тока дл€ Run - здесь - остальные в общей куче		
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
				if (pBr)odt_context->text_context()->add_break(pBr->m_oType.GetValue(), pBr->m_oClear.GetValue());
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
void DocxConverter::convert(OOX::Logic::CFldChar	*oox_fld)
{
	if (oox_fld == NULL) return;

		//nullable<SimpleTypes::COnOff<>       > m_oDirty;
		//nullable<SimpleTypes::COnOff<>       > m_oFldLock;
		//nullable<OOX::Logic::CFFData         > m_oFFData;
	if (oox_fld->m_oFldCharType.IsInit())
	{
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeBegin)	odt_context->start_field();
		if (oox_fld->m_oFldCharType->GetValue() == SimpleTypes::fldchartypeEnd)		odt_context->end_field();
	}

}
void DocxConverter::convert(OOX::Logic::CInstrText	*oox_instr)
{
	if (oox_instr == NULL) return;

	odt_context->set_field_instr(string2std_string(oox_instr->m_sText));

}
void DocxConverter::convert(OOX::Logic::CParagraphProperty	*oox_paragraph_pr, cpdoccore::odf::style_paragraph_properties * paragraph_properties)
{
	m_bKeepNextParagraph = false;
	
	if (!oox_paragraph_pr)		return;
	if (!paragraph_properties)	return;

	if (oox_paragraph_pr->m_oPStyle.IsInit() && oox_paragraph_pr->m_oPStyle->m_sVal.IsInit())
	{
		std::wstring style_name = string2std_string(*oox_paragraph_pr->m_oPStyle->m_sVal);
		odt_context->styles_context()->last_state().set_parent_style_name(style_name);
		/////////////////////////find parent properties 

		cpdoccore::odf::style_paragraph_properties  parent_paragraph_properties;
		odt_context->styles_context()->calc_paragraph_properties(style_name,odf::style_family::Paragraph, &parent_paragraph_properties.content());
		
		if (parent_paragraph_properties.content().outline_level_)
		{
			odt_context->text_context()->set_outline_level(*parent_paragraph_properties.content().outline_level_);
		}
	}

	if (oox_paragraph_pr->m_oSpacing.IsInit())
	{
		SimpleTypes::ELineSpacingRule rule = SimpleTypes::linespacingruleAtLeast;
		if (oox_paragraph_pr->m_oSpacing->m_oLineRule.IsInit())rule = oox_paragraph_pr->m_oSpacing->m_oLineRule->GetValue();

		if (oox_paragraph_pr->m_oSpacing->m_oLine.IsInit())
		{
			_CP_OPT(odf::length) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oLine.GetPointer()), length);
			if (length && rule == SimpleTypes::linespacingruleExact)
				paragraph_properties->content().fo_line_height_ = odf::line_width(*length);
			else if (length)
				paragraph_properties->content().style_line_height_at_least_= length;
		}
		if (oox_paragraph_pr->m_oSpacing->m_oAfter.IsInit())
		{
 			_CP_OPT(odf::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oSpacing->m_oAfter.GetPointer()), length);
			paragraph_properties->content().fo_margin_bottom_ = length;
		}
		if (oox_paragraph_pr->m_oSpacing->m_oBefore.IsInit())
		{
 			_CP_OPT(odf::length_or_percent) length;
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
 			_CP_OPT(odf::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oStart.GetPointer()), length);
			paragraph_properties->content().fo_margin_left_ = length;
		}
		if (oox_paragraph_pr->m_oInd->m_oEnd.IsInit())
		{
 			_CP_OPT(odf::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oEnd.GetPointer()), length);
			paragraph_properties->content().fo_margin_right_ = length;
		}
		if (oox_paragraph_pr->m_oInd->m_oFirstLine.IsInit())
		{
 			_CP_OPT(odf::length_or_percent) length;
			convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_paragraph_pr->m_oInd->m_oEnd.GetPointer()), length);
			paragraph_properties->content().fo_text_indent_ = length;
		}
		if (oox_paragraph_pr->m_oInd->m_oHanging.IsInit())
		{
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
		odf::style_text_properties * text_properties = odf_context()->text_context()->get_text_properties();
		if (text_properties) 
			convert(oox_paragraph_pr->m_oRPr.GetPointer(), text_properties);
	}
	if (oox_paragraph_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf::color) odf_color;
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
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::Lr); break;
		case SimpleTypes::textdirectionLrV  :
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::LrTb); break;
		case SimpleTypes::textdirectionRl   :
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::Rl); break;
		case SimpleTypes::textdirectionRlV  :
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::RlTb); break;
		case SimpleTypes::textdirectionTb   :
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::Tb); break;
		case SimpleTypes::textdirectionTbV  :
			paragraph_properties->content().style_writing_mode_= odf::writing_mode(odf::writing_mode::TbLr); break;
		}
	}
	//m_oSectPr

	if (oox_paragraph_pr->m_oOutlineLvl.IsInit() && oox_paragraph_pr->m_oOutlineLvl->m_oVal.IsInit())
	{
		int level = oox_paragraph_pr->m_oOutlineLvl->m_oVal->GetValue();
		paragraph_properties->content().outline_level_ =  level;
		odt_context->text_context()->set_outline_level ( level);
	}

	if (oox_paragraph_pr->m_oKeepNext.IsInit())
	{
		m_bKeepNextParagraph = true;
	}

	convert(oox_paragraph_pr->m_oFramePr.GetPointer(), paragraph_properties);		//буквица

	convert(oox_paragraph_pr->m_oSectPr.GetPointer());
}
void DocxConverter::convert(OOX::Logic::CSectionProperty *oox_section_pr, bool root)
{
	if (oox_section_pr == NULL) return;
	bool needGenerateSection  = true; // чтоб не сгенерить секцию однократно 
	
	odt_context->page_layout_context()->create_master_page(L"");
	
	odt_context->set_master_page_name(odt_context->page_layout_context()->last_master().get_name());
	
	if (oox_section_pr->m_oPgMar.IsInit())
	{
		_CP_OPT(odf::length) top, left, right, bottom;
		convert(oox_section_pr->m_oPgMar->m_oBottom.GetPointer(), bottom);
		convert(oox_section_pr->m_oPgMar->m_oLeft.GetPointer()	, left);
		convert(oox_section_pr->m_oPgMar->m_oRight.GetPointer()	, right);
		convert(oox_section_pr->m_oPgMar->m_oTop.GetPointer()	, top);
		
		odt_context->page_layout_context()->set_page_margin(top,left,bottom, right);
		
			//nullable<SimpleTypes::CTwipsMeasure        > m_oFooter;
			//nullable<SimpleTypes::CTwipsMeasure        > m_oGutter;
			//nullable<SimpleTypes::CTwipsMeasure        > m_oHeader;
	}

	if (oox_section_pr->m_oPgSz.IsInit())
	{
		if (oox_section_pr->m_oPgSz->m_oOrient.IsInit())
			odt_context->page_layout_context()->set_page_orientation(oox_section_pr->m_oPgSz->m_oOrient->GetValue());

		_CP_OPT(odf::length) width, height;

		convert(oox_section_pr->m_oPgSz->m_oW.GetPointer(), width);
		convert(oox_section_pr->m_oPgSz->m_oH.GetPointer(), height);

		odt_context->page_layout_context()->set_page_size(width, height);
		//nullable<SimpleTypes::CDecimalNumber<>   > m_oCode;
	}
			//nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oRtlGutter;
			//nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;

	// разметка + колонтитулы
			//CSimpleArray<ComplexTypes::Word::CHdrFtrRef                  > m_arrFooterReference;
			//CSimpleArray<ComplexTypes::Word::CHdrFtrRef                  > m_arrHeaderReference;	
			//nullable<ComplexTypes::Word::CPageMar                        > m_oPgMar;
			//nullable<ComplexTypes::Word::CPageNumber                     > m_oPgNumType;
			//nullable<ComplexTypes::Word::CPageSz                         > m_oPgSz;
			//nullable<OOX::Logic::CPageBorders                            > m_oPgBorders;
			//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oTitlePg;	

	//master page create
	// add colontitul
	// style name master page -> 0-й элемент текщей цепочки параграфов
//--------------------------------------------------------------------------------------------------------------------------------------------		
	// то что относитс€ собственно к секци€м 
	if (oox_section_pr->m_oType.IsInit() && oox_section_pr->m_oType->m_oVal.IsInit())
	{
		switch(oox_section_pr->m_oType->m_oVal->GetValue())
		{		
		case SimpleTypes::sectionmarkNextColumn :
		case SimpleTypes::sectionmarkContinious :
		case SimpleTypes::sectionmarkEvenPage   :
		case SimpleTypes::sectionmarkNextPage   :
		case SimpleTypes::sectionmarkOddPage    :
			odt_context->add_section();
			needGenerateSection = false; //??? 
			break;
		}
	}

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


	if (oox_section_pr->m_oCols.IsInit() && oox_section_pr->m_oCols->m_oNum.IsInit())//колонки
	{
		if (needGenerateSection) odt_context->add_section(); // дл€ колонок тож надо генерить секции /???

		int count = oox_section_pr->m_oCols->m_oNum->GetValue();

		double default_space_pt = -1;
		if (oox_section_pr->m_oCols->m_oSpace.IsInit())	default_space_pt = oox_section_pr->m_oCols->m_oSpace->ToPoints();
		
		bool separator = oox_section_pr->m_oCols->m_oSep.IsInit() && oox_section_pr->m_oCols->m_oSep->ToBool();
		
		odt_context->add_section_columns(count, default_space_pt, separator );

		std::vector<std::pair<double,double>> width_space;
		
		for (long i =0; i< oox_section_pr->m_oCols->m_arrColumns.GetSize(); i++)
		{
			double space = default_space_pt;
			if (oox_section_pr->m_oCols->m_arrColumns[i].m_oSpace.IsInit())
				space = oox_section_pr->m_oCols->m_arrColumns[i].m_oSpace->ToPoints();
		
			double w = -1; 
			if (oox_section_pr->m_oCols->m_arrColumns[i].m_oW.IsInit())
				w = oox_section_pr->m_oCols->m_arrColumns[i].m_oW->ToPoints();
			
			width_space.push_back(std::pair<double,double>(w,space));
		}
		for (long i= oox_section_pr->m_oCols->m_arrColumns.GetSize(); i< count; i ++)
		{
			width_space.push_back(std::pair<double,double>(-1, default_space_pt));
		}
		odt_context->add_section_column(width_space);

		if (root) odt_context->flush_section();
	}
}
void DocxConverter::convert(ComplexTypes::Word::CFramePr *oox_frame_pr, odf::style_paragraph_properties * paragraph_properties)
{
	if (oox_frame_pr == NULL) return;
	if (paragraph_properties == NULL) return;

	odt_context->start_drop_cap(paragraph_properties);

	if (oox_frame_pr->m_oDropCap.IsInit() && oox_frame_pr->m_oDropCap->GetValue() == SimpleTypes::dropcapMargin)
		odt_context->set_drop_cap_margin(true);
	if (oox_frame_pr->m_oLines.IsInit())
		odt_context->set_drop_cap_lines(oox_frame_pr->m_oLines->GetValue());
			//nullable<SimpleTypes::COnOff<>            > m_oAnchorLock;
			//nullable<SimpleTypes::CTwipsMeasure       > m_oH;
			//nullable<SimpleTypes::CHAnchor<>          > m_oHAnchor;
			//nullable<SimpleTypes::CHeightRule<>       > m_oHRule;
			//nullable<SimpleTypes::CTwipsMeasure       > m_oHSpace;
			//nullable<SimpleTypes::CVAnchor<>          > m_oVAnchor;
			//nullable<SimpleTypes::CTwipsMeasure       > m_oVSpace;
			//nullable<SimpleTypes::CTwipsMeasure       > m_oW;
			//nullable<SimpleTypes::CWrap<>             > m_oWrap;
			//nullable<SimpleTypes::CSignedTwipsMeasure > m_oX;
			//nullable<SimpleTypes::CXAlign<>           > m_oXAlign;
			//nullable<SimpleTypes::CSignedTwipsMeasure > m_oY;
			//nullable<SimpleTypes::CYAlign<>           > m_oYAlign;
}
void DocxConverter::convert(OOX::Logic::CTblBorders	*oox_border, odf::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring odf_border;
	if (oox_border->m_oBottom.IsInit())
	{
		convert(oox_border->m_oBottom.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = odf_border;
	}
	if (oox_border->m_oTop.IsInit())
	{
		convert(oox_border->m_oTop.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = odf_border;
	}
	if (oox_border->m_oStart.IsInit())
	{
		convert(oox_border->m_oStart.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = odf_border;
	}
	if (oox_border->m_oEnd.IsInit())
	{
		convert(oox_border->m_oEnd.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = odf_border;
	}
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
}
void DocxConverter::convert(OOX::Logic::CTcBorders 	*oox_border, odf::style_table_cell_properties	*table_cell_properties)
{
	if (oox_border == NULL) return;
	if (table_cell_properties == NULL) return;

	std::wstring odf_border;
	if (oox_border->m_oBottom.IsInit())
	{
		convert(oox_border->m_oBottom.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = odf_border;
	}
	if (oox_border->m_oTop.IsInit())
	{
		convert(oox_border->m_oTop.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = odf_border;
	}
	if (oox_border->m_oStart.IsInit())
	{
		convert(oox_border->m_oStart.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = odf_border;
	}
	if (oox_border->m_oEnd.IsInit())
	{
		convert(oox_border->m_oEnd.GetPointer(), odf_border);
		if (odf_border.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = odf_border;
	}
	//nullable<ComplexTypes::Word::CBorder > m_oInsideH;
	//nullable<ComplexTypes::Word::CBorder > m_oInsideV;
	//nullable<ComplexTypes::Word::CBorder > m_oTL2BR;
	//nullable<ComplexTypes::Word::CBorder > m_oTR2BL;

}

void DocxConverter::convert(OOX::Logic::CPBdr *oox_border, odf::style_paragraph_properties *paragraph_properties)
{
	if (oox_border == NULL) return;
	if (paragraph_properties == NULL) return;

	std::wstring odf_border;
	if (oox_border->m_oBottom.IsInit())
	{
		convert(oox_border->m_oBottom.GetPointer(), odf_border);
		if (odf_border.length() >0 ) paragraph_properties->content().common_border_attlist_.fo_border_bottom_ = odf_border;
	}
	if (oox_border->m_oTop.IsInit())
	{
		convert(oox_border->m_oTop.GetPointer(), odf_border);
		if (odf_border.length() >0 ) paragraph_properties->content().common_border_attlist_.fo_border_top_ = odf_border;
	}
	if (oox_border->m_oLeft.IsInit())
	{
		convert(oox_border->m_oLeft.GetPointer(), odf_border);
		if (odf_border.length() >0 ) paragraph_properties->content().common_border_attlist_.fo_border_left_ = odf_border;
	}
	if (oox_border->m_oRight.IsInit())
	{
		convert(oox_border->m_oRight.GetPointer(), odf_border);
		if (odf_border.length() >0 ) paragraph_properties->content().common_border_attlist_.fo_border_right_ = odf_border;
	}
	//if (oox_border->m_oTL2BR.IsInit())
	//{
	//	convert(oox_border->m_oTL2BR.GetPointer(), odf_border);
	//	if (odf_border.length() >0 ) paragraph_properties->content().style_diagonal_tl_br_ = odf_border;
	//}

}
void DocxConverter::convert(ComplexTypes::Word::CBorder *borderProp, std::wstring & odf_border_prop)
{
	if (!borderProp)return;

	std::wstring border_style;
	std::wstring border_color;
			//nullable<SimpleTypes::COnOff<>              > m_oFrame;
			//nullable<SimpleTypes::COnOff<>              > m_oShadow;
			//nullable<SimpleTypes::CPointMeasure<>       > m_oSpace;
			//nullable<SimpleTypes::CUcharHexNumber<>     > m_oThemeShade;

	if (borderProp->m_oSz.IsInit()) border_style = boost::lexical_cast<std::wstring>(borderProp->m_oSz->ToPoints()) + L"pt";
	else border_style = L"1pt";
	if (borderProp->m_oVal.IsInit())//266 styles  oO
	{
		switch(borderProp->m_oVal->GetValue())
		{
			case SimpleTypes::bordervalueDotDash:
				border_style += L" dashed";
			break;
			case SimpleTypes::bordervalueDashed:
				border_style += L" dashed";
			break;
			case SimpleTypes::bordervalueDotted:
				border_style += L" dotted";
			break;
			case SimpleTypes::bordervalueDouble:
				border_style += L" double";
			break;
			case SimpleTypes::bordervalueSingle:
				border_style += L" solid";
			break;
			case SimpleTypes::bordervalueNone:
				border_style == L" none";
				return;
			break;
		}
	}else border_style += L" solid";
///////////////////////////////////////////////////////////////////
	_CP_OPT(odf::color) color;

	if (borderProp->m_oColor.IsInit())
	{
		if (borderProp->m_oColor->GetValue() != SimpleTypes::hexcolorAuto)
			convert(borderProp->m_oColor.GetPointer(), borderProp->m_oThemeColor.GetPointer(), borderProp->m_oThemeTint.GetPointer(),borderProp->m_oThemeShade.GetPointer(), color);
	}
	if (color) border_color = color->get_hex_value();
	else border_color = L"000000";
	
	odf_border_prop = border_style + L" #" + border_color;
}
void DocxConverter::convert(ComplexTypes::Word::CJc * oox_jc,  _CP_OPT(odf::text_align) & align)
{
	if (oox_jc == NULL) return;
	if (oox_jc->m_oVal.IsInit() == false) return;

	switch(oox_jc->m_oVal->GetValue())
	{
		case SimpleTypes::jcBoth            : align = odf::text_align(odf::text_align::Justify);break;
		case SimpleTypes::jcCenter          : align = odf::text_align(odf::text_align::Center);	break;
		case SimpleTypes::jcThaiDistribute  :	
		case SimpleTypes::jcDistribute      : align = odf::text_align(odf::text_align::Justify);break;
		case SimpleTypes::jcEnd             : align = odf::text_align(odf::text_align::End);	break;
		case SimpleTypes::jcHighKashida     :	break;
		case SimpleTypes::jcLowKashida      :	break;
		case SimpleTypes::jcMediumKashida   :	break;
		case SimpleTypes::jcNumTab          :	break;
		case SimpleTypes::jcStart           : align = odf::text_align(odf::text_align::Start);	break;
		case SimpleTypes::jcLeft            : align = odf::text_align(odf::text_align::Left);	break;
		case SimpleTypes::jcRight           : align = odf::text_align(odf::text_align::Right);	break;
	}
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure *oox_size, _CP_OPT(odf::length) & odf_size)
{
	if (oox_size == NULL)return;

	odf_size = odf::length(oox_size->ToPoints(), odf::length::pt);
}
void DocxConverter::convert(SimpleTypes::CUniversalMeasure* oox_size, _CP_OPT(odf::length_or_percent) & odf_size)
{
	if (oox_size == NULL)return;

		//odf_size = odf::percent(oox_size->ToPoints(());
	odf_size = odf::length(oox_size->ToPoints(), odf::length::pt);

}

void DocxConverter::convert(OOX::Logic::CRunProperty *oox_run_pr, odf::style_text_properties * text_properties)
{
	if (oox_run_pr == NULL) return;
	if (text_properties == NULL)return;
	
	if (oox_run_pr->m_oRStyle.IsInit() && oox_run_pr->m_oRStyle->m_sVal.IsInit())
	{
		odt_context->styles_context()->last_state().set_parent_style_name(string2std_string(*oox_run_pr->m_oRStyle->m_sVal));
	}
	if (oox_run_pr->m_oBold.IsInit())
	{
		if (oox_run_pr->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}
	convert(oox_run_pr->m_oColor.GetPointer(),text_properties->content().fo_color_);

	text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::None);
	if (oox_run_pr->m_oU.IsInit())
	{
		text_properties->content().style_text_underline_style_ = odf::line_style(odf::line_style::Solid);
		text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Single);
		if (oox_run_pr->m_oU->m_oVal.IsInit())
		{
			SimpleTypes::EUnderline 	type = oox_run_pr->m_oU->m_oVal->GetValue();
			text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Single);
			switch(type)
			{
			case SimpleTypes::underlineDouble		:
					text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::Double);break;
			case SimpleTypes::underlineNone			:
					text_properties->content().style_text_underline_type_= odf::line_type(odf::line_type::None);break;
			case SimpleTypes::underlineDash            :
			case SimpleTypes::underlineDashedHeavy     :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::Dash);break;
			case SimpleTypes::underlineDashLong        :
			case SimpleTypes::underlineDashLongHeavy   :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::LongDash);break;
			case SimpleTypes::underlineDotDash         :
			case SimpleTypes::underlineDashDotHeavy    :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::DotDash);break;
			case SimpleTypes::underlineDotted          :
			case SimpleTypes::underlineDottedHeavy     :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::Dotted);break;
			case SimpleTypes::underlineDotDotDash      :
			case SimpleTypes::underlineDashDotDotHeavy :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::DotDotDash);break;
			case SimpleTypes::underlineThick           :
			case SimpleTypes::underlineWave            :
			case SimpleTypes::underlineWavyDouble      :
			case SimpleTypes::underlineWavyHeavy       :
					text_properties->content().style_text_underline_style_= odf::line_style(odf::line_style::Wave);break;
			}
		}
		text_properties->content().style_text_underline_color_ = odf::underline_color(odf::underline_color::FontColor);
	}
	if (oox_run_pr->m_oItalic.IsInit())
	{
		if (oox_run_pr->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Normal);
	}
	if (oox_run_pr->m_oSz.IsInit() && oox_run_pr->m_oSz->m_oVal.IsInit())
	{
		OoxConverter::convert(oox_run_pr->m_oSz->m_oVal->ToPoints(), text_properties->content().fo_font_size_);
	}
	if (oox_run_pr->m_oCaps.IsInit() && oox_run_pr->m_oCaps->m_oVal.ToBool())
		text_properties->content().fo_text_transform_ = odf::text_transform(odf::text_transform::Uppercase);
	
	if (oox_run_pr->m_oSmallCaps.IsInit() && oox_run_pr->m_oSmallCaps->m_oVal.ToBool())
		text_properties->content().fo_font_variant_ = odf::font_variant(odf::font_variant::SmallCaps);

	if (oox_run_pr->m_oRFonts.IsInit())
	{
		if (oox_run_pr->m_oRFonts->m_sAscii.IsInit())
			text_properties->content().fo_font_family_ = string2std_string(oox_run_pr->m_oRFonts->m_sAscii.get());
		if (oox_run_pr->m_oRFonts->m_sCs.IsInit())
			text_properties->content().style_font_family_complex_ = string2std_string(oox_run_pr->m_oRFonts->m_sCs.get());
		if (oox_run_pr->m_oRFonts->m_sEastAsia.IsInit())
			text_properties->content().style_font_family_asian_= string2std_string(oox_run_pr->m_oRFonts->m_sEastAsia.get());
			//nullable<CString              > m_sAscii;
			//nullable<SimpleTypes::CTheme<>> m_oAsciiTheme;
			//nullable<SimpleTypes::CTheme<>> m_oCsTheme;
			//nullable<CString              > m_sEastAsia;
			//nullable<SimpleTypes::CTheme<>> m_oEastAsiaTheme;
			//nullable<CString              > m_sHAnsi;
			//nullable<SimpleTypes::CTheme<>> m_oHAnsiTheme;
			//nullable<SimpleTypes::CHint<> > m_oHint;
	}
	if (oox_run_pr->m_oVertAlign.IsInit() && oox_run_pr->m_oVertAlign->m_oVal.IsInit())
	{ 
		switch(oox_run_pr->m_oVertAlign->m_oVal->GetValue())
		{
		case SimpleTypes::verticalalignrunSuperscript:
			//text_properties->content().style_text_position_ = odf::text_position(odf::text_position::Super); break;
			text_properties->content().style_text_position_ = odf::text_position(+33.); break;
		case SimpleTypes::verticalalignrunSubscript:
			//text_properties->content().style_text_position_ = odf::text_position(odf::text_position::Sub); break;
			text_properties->content().style_text_position_ = odf::text_position(-33.); break;//по умолчанию 58% - хуже выгл€дит
		}
	}
	if (oox_run_pr->m_oW.IsInit() && oox_run_pr->m_oW->m_oVal.IsInit())
	{
			text_properties->content().style_text_scale_ = odf::percent(oox_run_pr->m_oW->m_oVal->GetValue());
	}
	if (oox_run_pr->m_oStrike.IsInit())
		text_properties->content().style_text_line_through_type_ = odf::line_type(odf::line_type::Single);

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
			text_properties->content().fo_background_color_ = odf::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString().Right(6)));
			delete oRgbColor;
		}
	}
	if (oox_run_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf::color) odf_color;
		convert(oox_run_pr->m_oShd->m_oFill.GetPointer(), oox_run_pr->m_oShd->m_oThemeFill.GetPointer(),
			oox_run_pr->m_oShd->m_oThemeFillTint.GetPointer(), oox_run_pr->m_oShd->m_oThemeShade.GetPointer(), odf_color);
		if (odf_color)
			text_properties->content().fo_background_color_ = *odf_color;
	}
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

	odt_context->start_drawings();
		convert(oox_drawing->m_oAnchor.GetPointer());
		convert(oox_drawing->m_oInline.GetPointer());
	odt_context->end_drawings();
}
void DocxConverter::convert(OOX::Drawing::CAnchor *oox_anchor)
{
	if (oox_anchor == NULL)return;

	double width =0, height=0;
	double x =0, y=0;
	if (oox_anchor->m_oExtent.IsInit()) //size
	{
		width = oox_anchor->m_oExtent->m_oCx.ToPoints();
		height = oox_anchor->m_oExtent->m_oCx.ToPoints();
	}
	if (oox_anchor->m_oDistL.IsInit())odt_context->drawing_context()->set_margin_left(oox_anchor->m_oDistL->ToPoints());
	if (oox_anchor->m_oDistT.IsInit())odt_context->drawing_context()->set_margin_top(oox_anchor->m_oDistT->ToPoints());
	if (oox_anchor->m_oDistR.IsInit())odt_context->drawing_context()->set_margin_right(oox_anchor->m_oDistR->ToPoints());
	if (oox_anchor->m_oDistB.IsInit())odt_context->drawing_context()->set_margin_bottom(oox_anchor->m_oDistB->ToPoints());

	odt_context->drawing_context()->set_drawings_rect(-1, -1, width, height);

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
	//if (oox_anchor->m_oWrapNone.IsInit())
	//{
	//}
	if (oox_anchor->m_oWrapSquare.IsInit())
	{
		if (oox_anchor->m_oWrapSquare->m_oWrapText.IsInit() && oox_anchor->m_oWrapSquare->m_oWrapText->GetValue() == SimpleTypes::wraptextLargest)
			odt_context->drawing_context()->set_wrap_style(odf::style_wrap::Dynamic);
		else	
			odt_context->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
	}	
	if (oox_anchor->m_oWrapThrough.IsInit())//style:wrap="run-through" draw:wrap-influence-on-position style:wrap-contour
	{
		odt_context->drawing_context()->set_wrap_style(odf::style_wrap::RunThrough);
	}
	if (oox_anchor->m_oWrapTight.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
	}
	if (oox_anchor->m_oWrapTopAndBottom.IsInit())
	{
		odt_context->drawing_context()->set_wrap_style(odf::style_wrap::Parallel);
	}
	convert(oox_anchor->m_oGraphic.GetPointer());
}
void DocxConverter::convert(OOX::Drawing::CInline *oox_inline)
{
	if (oox_inline == NULL)return;

	double width =0, height=0;
	if (oox_inline->m_oExtent.IsInit()) //size
	{
		width = oox_inline->m_oExtent->m_oCx.ToPoints();
		height = oox_inline->m_oExtent->m_oCy.ToPoints();
	}

	odt_context->drawing_context()->set_drawings_rect(0, 0, width, height);

	odt_context->drawing_context()->set_anchor(odf::anchor_type::AsChar); 
	
	if (oox_inline->m_oDistL.IsInit())odt_context->drawing_context()->set_margin_left(oox_inline->m_oDistL->ToPoints());
	if (oox_inline->m_oDistT.IsInit())odt_context->drawing_context()->set_margin_top(oox_inline->m_oDistT->ToPoints());
	if (oox_inline->m_oDistR.IsInit())odt_context->drawing_context()->set_margin_right(oox_inline->m_oDistR->ToPoints());
	if (oox_inline->m_oDistB.IsInit())odt_context->drawing_context()->set_margin_bottom(oox_inline->m_oDistB->ToPoints());

	convert(oox_inline->m_oGraphic.GetPointer());
}
void DocxConverter::convert(OOX::Drawing::CGraphic *oox_graphic)
{
	if (oox_graphic == NULL)return;

	convert(oox_graphic->m_oPicture.GetPointer());
	convert(oox_graphic->m_oChart.GetPointer());
	convert(oox_graphic->m_oShape.GetPointer());
	convert(oox_graphic->m_oGroupShape.GetPointer());
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
		_gdi_graphics_::GetResolution(pathImage, Width, Height);
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
			odt_context->drawing_context()->set_image_client_rect(oox_picture->m_oBlipFill.m_oSrcRect->m_oL.GetValue() * Width/100. ,
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oT.GetValue() * Height/100.,
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oR.GetValue() * Width/100. , 
																 oox_picture->m_oBlipFill.m_oSrcRect->m_oB.GetValue() * Height/100.);
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

void DocxConverter::convert(OOX::Drawing::CChart * oox_chart)
{
	if (oox_chart == NULL)return;

	if (oox_chart->m_oRId.IsInit())
	{
		smart_ptr<OOX::File> oFile = docx_document->GetDocument()->Find(oox_chart->m_oRId->GetValue());
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
					
					oox_current_chart = pChart;
					odf_context()->start_chart();
						odf_context()->chart_context()->set_chart_size(width, height);
						OoxConverter::convert(&pChart->m_oChartSpace);
					odf_context()->end_chart();
					oox_current_chart = NULL; // object???

				odt_context->drawing_context()->end_object();	
				odt_context->drawing_context()->end_drawing();
			}
		}
	}

}

void DocxConverter::convert(OOX::Logic::CGroupShape	 *oox_group_shape)
{
	if (oox_group_shape == NULL)return;
	if (oox_group_shape->m_arrItems.GetSize() < 1) return;

	std::wstring name;
	int id = -1;
	
	//if (oox_group_shape->m_oCNvPr.IsInit())
	{
		//if (oox_group_shape->m_oCNvPr->m_sName.IsInit())
		//		name = string2std_string(*oox_group_shape->m_oCNvPr->m_sName);
		//if (oox_group_shape->m_oCNvPr->m_oId.IsInit())
		//		id = oox_group_shape->m_oCNvPr->m_oId->GetValue();
	}

	odt_context->drawing_context()->start_group(name,id);

	OoxConverter::convert(oox_group_shape->m_oGroupSpPr.GetPointer());

	for (long i=0; i < oox_group_shape->m_arrItems.GetSize(); i++)
	{
		convert(oox_group_shape->m_arrItems[i]);
	}

	odt_context->drawing_context()->end_group();
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
		if (oox_shape->m_oCNvSpPr.IsInit())
		{
			if (oox_shape->m_oCNvSpPr->m_otxBox.GetValue() == 1)
				type = 2000; //textBox
		}
		if (type < 0)return;
	/////////////////////////////////////////////////////////////////////////////////
		if (type == 2000)	odt_context->drawing_context()->start_text_box(); 
		else				odt_context->drawing_context()->start_shape(type);
		
		OoxConverter::convert(oox_shape->m_oSpPr.GetPointer(), oox_shape->m_oShapeStyle.GetPointer());
	//им€, описалово, номер ...		
		OoxConverter::convert(oox_shape->m_oCNvPr.GetPointer());	
	//заблокированности 
		OoxConverter::convert(oox_shape->m_oCNvSpPr.GetPointer());	
		
		if (oox_shape->m_oCNvConnSpPr.IsInit())
		{
			OoxConverter::convert(oox_shape->m_oCNvConnSpPr.GetPointer());
			//частенько приплывает из стил€ заполенение объекта .. а он то одномерный :)
			odf_context()->drawing_context()->start_area_properies();
				odf_context()->drawing_context()->set_no_fill();
			odf_context()->drawing_context()->end_area_properies();
		}
		
		if (oox_shape->m_oTxBody.IsInit() && oox_shape->m_oTxBody->m_oTxtbxContent.IsInit())
		{
			odt_context->start_text_context();
			for (long i=0 ; i < oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems.GetSize();i++)
			{
				convert(oox_shape->m_oTxBody->m_oTxtbxContent->m_arrItems[i]);
			}
			odt_context->drawing_context()->set_text( odt_context->text_context());
			odt_context->end_text_context();	
		}
		OoxConverter::convert(oox_shape->m_oTxBodyProperties.GetPointer());

	if (type == 2000)	odt_context->drawing_context()->end_text_box();
	else				odt_context->drawing_context()->end_shape();
	
	odt_context->drawing_context()->end_drawing();
}

void DocxConverter::convert(SimpleTypes::CHexColor<>		*color, 
							SimpleTypes::CThemeColor<>		*theme_color, 
							SimpleTypes::CUcharHexNumber<>	*theme_tint,
							SimpleTypes::CUcharHexNumber<>	*theme_shade, _CP_OPT(odf::color) & odf_color)
{
	odf_color = boost::none;

	unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
	bool result = false;	

	if(color)//easy, faster,realy  !!
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
			odf_color = odf::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString().Right(6)));
			delete oRgbColor;
		}
	}
}
void DocxConverter::convert(ComplexTypes::Word::CColor *color, _CP_OPT(odf::color) & odf_color)
{
	if (!color)return;
	convert(color->m_oVal.GetPointer(), color->m_oThemeColor.GetPointer(),color->m_oThemeTint.GetPointer(),color->m_oThemeShade.GetPointer(), odf_color);
}
void DocxConverter::convert_styles()
{
	if (!odt_context) return;

	OOX::CStyles * docx_styles = docx_document->GetStyles();
	
	if (!docx_styles)return;

	//nullable<OOX::CLatentStyles > m_oLatentStyles;

	convert(docx_styles->m_oDocDefaults.GetPointer());

	for (long i=0; i< docx_styles->m_arrStyle.GetSize(); i++)
	{
		convert(&docx_styles->m_arrStyle[i]);
	}

}

void DocxConverter::convert(OOX::Logic::CHyperlink *oox_hyperlink)
{
	if (oox_hyperlink == NULL)return;
	if (oox_hyperlink->m_oId.IsInit() == false) return;

	std::wstring ref = find_link_by_id(oox_hyperlink->m_oId->GetValue(),2);

	odt_context->start_hyperlink(ref);
	
	for (long i=0; i< oox_hyperlink->m_arrItems.GetSize(); i++)
	{
		convert(oox_hyperlink->m_arrItems[i]);
	}
	odt_context->end_hyperlink();
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
		odt_context->styles_context()->create_default_style(odf::style_family::Paragraph);					
		odf::style_paragraph_properties	* paragraph_properties	= odt_context->styles_context()->last_state().get_paragraph_properties();

		convert(def_style->m_oParPr.GetPointer(), paragraph_properties/*, text_properties*/); 
	}
	if (def_style->m_oRunPr.IsInit())
	{
		odt_context->styles_context()->create_default_style(odf::style_family::Text);					
		odf::style_text_properties	* text_properties = odt_context->styles_context()->last_state().get_text_properties();

		convert(def_style->m_oRunPr.GetPointer(), text_properties);
	}

}


void DocxConverter::convert_table_style(OOX::CStyle *oox_style)
{
	if (oox_style == NULL)return;

	std::wstring oox_name = oox_style->m_sStyleId.IsInit() ? string2std_string(*oox_style->m_sStyleId) : L"";

	odt_context->styles_context()->table_styles().start_style(oox_name);
//общие

	if (oox_style->m_oTblPr.IsInit())
	{
		//odf::style_table_properties	* table_properties = odt_context->styles_context()->table_styles().get_table_properties();
		//convert(oox_style->m_oTblPr.GetPointer(), table_properties);
		
		//нужно проверить стоит ли отнаследоватьс€ от base_on 
		
		if (oox_style->m_oTblPr->m_oTblBorders.IsInit())
		{
			odf::style_table_cell_properties * table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
			convert(oox_style->m_oTblPr->m_oTblBorders.GetPointer(), table_cell_properties);
		}
	}
	if (oox_style->m_oRunPr.IsInit())
	{
		odf::style_text_properties	* text_properties = odt_context->styles_context()->table_styles().get_text_properties();
		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf::style_paragraph_properties	* paragraph_properties = odt_context->styles_context()->table_styles().get_paragraph_properties();
		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);
	}

	if (oox_style->m_oTcPr.IsInit())
	{
		odf::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->table_styles().get_table_cell_properties();
		convert(oox_style->m_oTcPr.GetPointer(), table_cell_properties);
	}	
	//if (oox_style->m_oTrPr.IsInit())
	//{
	//	odf::style_table_row_properties	* table_row_properties = odt_context->styles_context()->table_styles().get_table_row_properties();
	//	convert(oox_style->m_oTrPr.GetPointer(), table_row_properties);
	//}
//отдельные
	for (long i = 0 ; i <oox_style->m_arrTblStylePr.GetSize() ; i++)
	{
		if (oox_style->m_arrTblStylePr[i].m_oType.IsInit() == false) continue;
		switch (oox_style->m_arrTblStylePr[i].m_oType->GetValue())
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
		//сначела отнаследоватьс€ от общих настроек???
		convert(oox_style->m_arrTblStylePr[i].m_oTcPr.GetPointer(),	odt_context->styles_context()->table_styles().get_table_cell_properties());
		convert(oox_style->m_arrTblStylePr[i].m_oRunPr.GetPointer(),odt_context->styles_context()->table_styles().get_text_properties());
		convert(oox_style->m_arrTblStylePr[i].m_oParPr.GetPointer(),odt_context->styles_context()->table_styles().get_paragraph_properties());

			//nullable<OOX::Logic::CTableProperty      >      m_oTblPr;
			//nullable<OOX::Logic::CTableRowProperties >      m_oTrPr;
	}

	odt_context->styles_context()->table_styles().end_style();

}
void DocxConverter::convert(OOX::CStyle	*oox_style)
{
	if (oox_style == NULL)return;

	if (oox_style->m_oType.IsInit() == false)return;
	odf::style_family family = odf::style_family::None;

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
		case SimpleTypes::styletypeCharacter : family = odf::style_family::Text;		break;
		case SimpleTypes::styletypeParagraph : family = odf::style_family::Paragraph;	break;
		default:  
			return;
	}

	std::wstring oox_name = oox_style->m_sStyleId.IsInit() ? string2std_string(*oox_style->m_sStyleId) : L"";

	odt_context->styles_context()->create_style(oox_name,family, false, true, -1); 

	if (oox_style->m_oName.IsInit() && oox_style->m_oName->m_sVal.IsInit()) 
		odt_context->styles_context()->last_state().set_display_name(string2std_string(*oox_style->m_oName->m_sVal));

	if (oox_style->m_oRunPr.IsInit())
	{
		odf::style_text_properties	* text_properties = odt_context->styles_context()->last_state().get_text_properties();
		convert(oox_style->m_oRunPr.GetPointer(), text_properties);
	}
	if (oox_style->m_oParPr.IsInit())
	{
		odf::style_paragraph_properties	* paragraph_properties = odt_context->styles_context()->last_state().get_paragraph_properties();
		convert(oox_style->m_oParPr.GetPointer(), paragraph_properties);
	}
	if (oox_style->m_oBasedOn.IsInit() && oox_style->m_oBasedOn->m_sVal.IsInit())
		odt_context->styles_context()->last_state().set_parent_style_name(string2std_string(*oox_style->m_oBasedOn->m_sVal));

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

	if (added == false)
	{
		//значит старт тута а не по RangeStart
		convert_comment(oox_comm_id);
	}

}
void DocxConverter::convert_comment(int oox_comm_id)
{
	OOX::CComments * docx_comments = docx_document->GetComments();
	if (!docx_comments)return;

	for (int comm =0 ; comm < docx_comments->m_arrComments.GetSize(); comm++)
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

				for (long i=0; i <oox_comment->m_arrItems.GetSize(); i++)
				{
					convert(oox_comment->m_arrItems[i]);
				}
			}
			odt_context->end_comment_content();
		}
	}
}
void DocxConverter::convert(OOX::Logic::CTbl *oox_table)
{
	if (oox_table == NULL) return;

	if (m_bKeepNextParagraph)
	{
		odt_context->end_paragraph();
		m_bKeepNextParagraph = false;
	}


	convert(oox_table->m_oTableProperties);	
	odt_context->start_table(true); 

//настройка предустановленного стил€
	if (oox_table->m_oTableProperties && (oox_table->m_oTableProperties->m_oTblStyle.IsInit() && oox_table->m_oTableProperties->m_oTblStyle->m_sVal.IsInit()))
	{
		std::wstring base_style_name = string2std_string(*oox_table->m_oTableProperties->m_oTblStyle->m_sVal);

		bool res = odt_context->styles_context()->table_styles().start_table(base_style_name);
		if (res)
		{
			odt_context->table_context()->set_table_styled(res);
			if (oox_table->m_oTableProperties->m_oTblLook.IsInit() && oox_table->m_oTableProperties->m_oTblLook->m_oVal.IsInit())
				odt_context->styles_context()->table_styles().set_flags(oox_table->m_oTableProperties->m_oTblLook->m_oVal->GetValue());
		}
	}

	int count_rows = oox_table->m_nCountRow;
	int count_columns = 0;
	if (oox_table->m_oTblGrid.IsInit())count_columns = oox_table->m_oTblGrid->m_arrGridCol.GetSize();
	
	odt_context->styles_context()->table_styles().set_current_dimension(count_columns, count_rows);

//------ колонки
	convert(oox_table->m_oTblGrid.GetPointer());

//------ строки
	for (int i =0 ; i < oox_table->m_arrItems.GetSize(); i++)
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

	odt_context->end_table();

	if (odt_context->table_context()->is_styled())
		odt_context->styles_context()->table_styles().end_table();
}
void DocxConverter::convert(OOX::Logic::CTblGrid	*oox_table_grid)
{
	if (oox_table_grid == NULL) return;

	odt_context->start_table_columns();
	//nullable<OOX::Logic::CTblGridChange          > m_oTblGridChange;
	for (int i =0 ; i < oox_table_grid->m_arrGridCol.GetSize(); i++)
	{
		double width = -1;

		if (oox_table_grid->m_arrGridCol[i].m_oW.IsInit())
		{
			width = oox_table_grid->m_arrGridCol[i].m_oW->ToPoints();
		}
		
		odt_context->add_table_column(width);
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

	for (int i =0 ; i < oox_table_row->m_arrItems.GetSize(); i++)
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

	for (int i =0 ; i < oox_table_cell->m_arrItems.GetSize(); i++)
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
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr, odf::style_table_properties	* table_properties )
{
	if (oox_table_pr == NULL) return false;
	if (table_properties == NULL) return false;

	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oBidiVisual;
	//nullable<ComplexTypes::Word::CJcTable                        > m_oJc;
	//nullable<ComplexTypes::Word::CShading                        > m_oShade;
	//nullable<OOX::Logic::CTblBorders                             > m_oTblBorders;
	//nullable<ComplexTypes::Word::CString_                        > m_oTblCaption;
	//nullable<OOX::Logic::CTblCellMar                             > m_oTblCellMar;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblCellSpacing;
	//nullable<ComplexTypes::Word::CString_                        > m_oTblDescription;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblInd;
	//nullable<ComplexTypes::Word::CTblLayoutType                  > m_oTblLayout;
	//nullable<ComplexTypes::Word::CTblLook                        > m_oTblLook;
	//nullable<ComplexTypes::Word::CTblOverlap                     > m_oTblOverlap;
	//nullable<ComplexTypes::Word::CTblPPr                         > m_oTblpPr;
	//nullable<OOX::Logic::CTblPrChange                            > m_oTblPrChange;
	//nullable<ComplexTypes::Word::CString_                        > m_oTblStyle;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleColBandSize;
	//nullable<ComplexTypes::Word::CDecimalNumber                  > m_oTblStyleRowBandSize;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTblW;

	return true;
}
bool DocxConverter::convert(OOX::Logic::CTableProperty *oox_table_pr)
{
	odt_context->styles_context()->create_style(L"",odf::style_family::Table, true, false, -1); //ради нормального задани€ дефолтовых свойств на cells
	if (oox_table_pr == NULL) return false;
	
	odf::style_table_properties	* table_properties = odt_context->styles_context()->last_state().get_table_properties();

	convert(oox_table_pr, table_properties);
	
	if (oox_table_pr->m_oTblBorders.IsInit())
	{//напр€мую задать cell_prop на саму таблицу низ€ - тока как default-cell-style-name на columns & row

		odf::style_table_cell_properties	* table_cell_properties = odt_context->styles_context()->last_state().get_table_cell_properties();
		convert(oox_table_pr->m_oTblBorders.GetPointer(), table_cell_properties);
	}
///////////////////////////////////////////////////////////////////////////

	//дл€ красивой отрисовки в редакторах - разрешим объеденить стили пересекающихс€ обрамлений 
	table_properties->table_format_properties_.table_border_model_ = odf::border_model(odf::border_model::Collapsing);


	return true;
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr, odf::style_table_row_properties	* table_row_properties)
{
	if (oox_table_row_pr == NULL) return;
	if (table_row_properties == NULL) return;

	if (oox_table_row_pr->m_oTblHeight.IsInit())
	{
		_CP_OPT(odf::length) length;
		convert(static_cast<SimpleTypes::CUniversalMeasure *>(oox_table_row_pr->m_oTblHeight->m_oVal.GetPointer()), length);

		if (oox_table_row_pr->m_oTblHeight->m_oHRule.IsInit())
		{
			switch(oox_table_row_pr->m_oTblHeight->m_oHRule->GetValue())
			{
				case SimpleTypes::heightruleAtLeast:
					table_row_properties->style_table_row_properties_attlist_.style_min_row_height_ = length; break;
				case SimpleTypes::heightruleExact:
					table_row_properties->style_table_row_properties_attlist_.style_row_height_ = length; break;
				case SimpleTypes::heightruleAuto:
					table_row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true;		break;
			}
		}
	}
}
void DocxConverter::convert(OOX::Logic::CTableRowProperties *oox_table_row_pr)
{
	if (oox_table_row_pr == NULL) return;

	odf::style_table_row_properties	* table_row_properties = odt_context->styles_context()->last_state().get_table_row_properties();

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
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr,odf::style_table_cell_properties	* table_cell_properties)
{
	if (oox_table_cell_pr == NULL) return false;
	if (table_cell_properties == NULL) return false;

	if (oox_table_cell_pr->m_oShd.IsInit())
	{
		_CP_OPT(odf::color) odf_color;
		convert(oox_table_cell_pr->m_oShd->m_oFill.GetPointer(), oox_table_cell_pr->m_oShd->m_oThemeFill.GetPointer(),
			oox_table_cell_pr->m_oShd->m_oThemeFillTint.GetPointer(), oox_table_cell_pr->m_oShd->m_oThemeShade.GetPointer(), odf_color);
		if (odf_color)
			table_cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_ = *odf_color;
	}

	if (oox_table_cell_pr->m_oTextDirection.IsInit())
	{
	}
	convert(oox_table_cell_pr->m_oTcBorders.GetPointer() , table_cell_properties);

	
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oCellDel;
	//nullable<ComplexTypes::Word::CTrackChange                    > m_oCellIns;
	//nullable<ComplexTypes::Word::CCellMergeTrackChange           > m_oCellMerge;
	//nullable<ComplexTypes::Word::CCnf                            > m_oCnfStyle;
	//nullable<OOX::Logic::CHeaders                                > m_oHeaders;
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oHideMark;
	//nullable<ComplexTypes::Word::CHMerge                         > m_oHMerge;
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oNoWrap;
	//nullable<ComplexTypes::Word::COnOff2<SimpleTypes::onoffTrue> > m_oTcFitText;
	//nullable<OOX::Logic::CTcMar                                  > m_oTcMar;
	//nullable<OOX::Logic::CTcPrChange                             > m_oTcPrChange;
	//nullable<ComplexTypes::Word::CTblWidth                       > m_oTcW;
	//nullable<ComplexTypes::Word::CTextDirection                  > m_oTextDirection;
	//nullable<ComplexTypes::Word::CVerticalJc                     > m_oVAlign;
	return true;
}
bool DocxConverter::convert(OOX::Logic::CTableCellProperties *oox_table_cell_pr, int col)
{
	bool is_base_styled = odt_context->table_context()->is_styled();
	
	if (col < 0) col=odt_context->table_context()->current_column()+1;
	int row=odt_context->table_context()->current_row();
		
	odf::style_table_cell_properties * parent_cell_properties = NULL;

	odf::odf_style_state * state  = odt_context->styles_context()->last_state(odf::style_family::Table); // стиль придетс€ генерить всегда
	//проверить ситуацию когда дефолтовые настройки в целом на все €чейки есть, а локальные свойства ваще не заданы
	if (state)parent_cell_properties = state->get_table_cell_properties();
	
	if (oox_table_cell_pr == NULL && is_base_styled == false && parent_cell_properties == NULL) return false;
	
	odt_context->styles_context()->create_style(L"",odf::style_family::TableCell, true, false, -1); 	
	odf::style_table_cell_properties	* cell_properties = odt_context->styles_context()->last_state().get_table_cell_properties();

	if (cell_properties == NULL) return false;

	if (is_base_styled)
	{
		odf::style_text_properties		* text_properties		= odt_context->styles_context()->last_state().get_text_properties();
		odf::style_paragraph_properties	* paragraph_properties	= odt_context->styles_context()->last_state().get_paragraph_properties();
		
		if (col < 0) col=odt_context->table_context()->current_column()+1;
		int row=odt_context->table_context()->current_row();
		
		odt_context->styles_context()->table_styles().get_table_cell_properties (col, row, cell_properties);
		odt_context->styles_context()->table_styles().get_text_properties (col, row, text_properties);
		odt_context->styles_context()->table_styles().get_paragraph_properties (col, row, paragraph_properties);
	}
	cell_properties->apply_from(parent_cell_properties);
	bool res = convert(oox_table_cell_pr, cell_properties);

	return true;
}

} 

