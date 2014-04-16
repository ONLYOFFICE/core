#pragma once

#include "stdAfx.h"

#include "XlsxConverter.h"
#include "shape_types_mapping.h"

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"

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

XlsxConverter::XlsxConverter(const std::wstring & path) 
{
	const OOX::CPath oox_path(CString(path.c_str()));			

	xlsx_document = new OOX::Spreadsheet::CXlsx(oox_path);	
	output_document = new odf::package::odf_document(L"spreadsheet");
}

void XlsxConverter::write(const std::wstring & path)
{
	if (!output_document)return;
	output_document->write(path);
}
odf::odf_conversion_context* XlsxConverter::odf_context()
{
	return ods_context;
}

void XlsxConverter::convert()
{
	if (!xlsx_document)return;
	if (!output_document)return;

	ods_context = new odf::ods_conversion_context(output_document);

	if (!ods_context)return;
	
	convert_styles();
		
	ods_context->start_document();

	convert_sheets();

	ods_context->end_document();

}
void XlsxConverter::convert_sheets()
{
	if (!ods_context) return;
	
	const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->GetWorkbook();
	if (!Workbook) return;

	CAtlMap<CString, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = xlsx_document->GetWorksheets();
	if(Workbook->m_oSheets.IsInit())
	{			
		CSimpleArray<OOX::Spreadsheet::CSheet*>& aWs = Workbook->m_oSheets->m_arrItems;//отсюда только имена 
		
		for(int i = 0, length = aWs.GetSize(); i < length; ++i)
		{
			OOX::Spreadsheet::CSheet* pSheet = aWs[i];
				
			if(pSheet->m_oRid.IsInit())
			{
				CString sSheetRId = pSheet->m_oRid.get2().ToString();
				CAtlMap<CString, OOX::Spreadsheet::CWorksheet*>::CPair* pPair = arrWorksheets.Lookup(sSheetRId);
				if (NULL != pPair)
				{
					OOX::Spreadsheet::CWorksheet *pWorksheet = pPair->m_value;
					if (NULL != pWorksheet && pWorksheet->m_oSheetData.IsInit())
					{
						std::wstring name = string2std_string(pSheet->m_oName.get2());
						ods_context->start_sheet(name);
						if (pSheet->m_oState.IsInit() && (	pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleHidden || 
															pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleVeryHidden))
							ods_context->current_table().set_table_hidden(true);
						convert(pWorksheet);
						ods_context->end_sheet();						
					}
				}

			}
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (oox_sheet->m_oDimension.IsInit())
	{
		ods_context->set_sheet_dimension(string2std_string(oox_sheet->m_oDimension->m_oRef.get()));
	}
	//текущие дефолтные свойства 
	if (oox_sheet->m_oSheetFormatPr.IsInit())
		convert(oox_sheet->m_oSheetFormatPr.GetPointer());
	if (oox_sheet->m_oSheetPr.IsInit())
		convert(oox_sheet->m_oSheetPr.GetPointer());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Предобработка
	//гиперлинки 
	for (long hyp = 0; oox_sheet->m_oHyperlinks.IsInit() && hyp < oox_sheet->m_oHyperlinks->m_arrItems.GetSize(); hyp++)
	{
		convert(oox_sheet->m_oHyperlinks->m_arrItems[hyp],oox_sheet);
	}	
	//комментарии
	//автофильтры
	//todooo для оптимизации - перенести мержи в начало
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//колонки
	ods_context->start_columns();
		for (long col = 0 ; oox_sheet->m_oCols.IsInit() && col < oox_sheet->m_oCols->m_arrItems.GetSize(); col++)
		{
			convert(oox_sheet->m_oCols->m_arrItems[col]);
		}
	ods_context->end_columns();

	//строки
	ods_context->start_rows();
		for (long row = 0 ; oox_sheet->m_oSheetData.IsInit() && row < oox_sheet->m_oSheetData->m_arrItems.GetSize(); row++)
		{
			convert(oox_sheet->m_oSheetData->m_arrItems[row]);
		}
	ods_context->end_rows();

	//мержи
	for (long mrg = 0 ; oox_sheet->m_oMergeCells.IsInit() && mrg < oox_sheet->m_oMergeCells->m_arrItems.GetSize(); mrg++)
	{
		if (oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.IsInit())
			ods_context->add_merge_cells(string2std_string(oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.get()));
	}
	//рисование - делаем ВСЕГДА привязку с таблице(странице) ... так проще 
	if (oox_sheet->m_oDrawing.IsInit() && oox_sheet->m_oDrawing->m_oId.IsInit())
	{
		smart_ptr<OOX::File> oFile = oox_sheet->Find(oox_sheet->m_oDrawing->m_oId->GetValue());
		if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Drawings == oFile->type())
		{
			OOX::Spreadsheet::CDrawing* pDrawing = (OOX::Spreadsheet::CDrawing*)oFile.operator->();
			
			convert(pDrawing);
		}
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CHyperlink *oox_hyperlink,OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (oox_hyperlink == NULL)return;
	if (oox_sheet == NULL)return;

	std::wstring ref = oox_hyperlink->m_oRef.IsInit() ? string2std_string(oox_hyperlink->m_oRef.get()) : L"";
	std::wstring link;
	if (oox_hyperlink->m_oRid.IsInit() && oox_sheet->GetCurRls())
	{
		OOX::Rels::CRelationShip* oRels = NULL;
		oox_sheet->GetCurRls()->GetRel( OOX::RId(oox_hyperlink->m_oRid->GetValue()), &oRels);
		if(NULL != oRels && _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink") == oRels->Type() )
		{
			if(oRels->IsExternal())
				link= oRels->Target().GetPath();
		}
	}
	std::wstring display = oox_hyperlink->m_oDisplay.IsInit() ? string2std_string(oox_hyperlink->m_oDisplay.get()) : L"";
	ods_context->add_hyperlink(ref, link, display);

}

void XlsxConverter::convert(OOX::Spreadsheet::CRow *oox_row)
{
	if (oox_row == NULL)return;

	int row_number = oox_row->m_oR.IsInit() ? oox_row->m_oR->GetValue() : -1;

	bool _default = true;
	
	if (oox_row->m_oHt.IsInit() || oox_row->m_oCustomHeight.IsInit() || oox_row->m_oHidden.IsInit() || oox_row->m_oCollapsed.IsInit()) _default = false;
	
	int level = 0;
	if (oox_row->m_oOutlineLevel.IsInit())
	{
		level = oox_row->m_oOutlineLevel->GetValue();
	}

	ods_context->start_row(row_number,1,level,_default);
	
	if (oox_row->m_oHidden.IsInit())		ods_context->current_table().set_row_hidden(true);
	if (oox_row->m_oCollapsed.IsInit())		ods_context->current_table().set_row_hidden(true);
	
	std::wstring style_cell_name;
	if (oox_row->m_oS.IsInit() && ( oox_row->m_oCustomFormat.IsInit() && oox_row->m_oCustomFormat->GetValue()==1))
	{
		int xfd_id = oox_row->m_oS->GetValue();

		style_cell_name  = ods_context->styles_context().find_odf_style_name(xfd_id,odf::style_family::TableCell);
	}else
	{
		//style_cell_name  = ods_context->styles_context().find_odf_style_name_default(odf::style_family::TableCell);
	}
	ods_context->current_table().set_row_default_cell_style(style_cell_name );
	
	if (oox_row->m_oHt.IsInit() == true)
	{
		double height = oox_row->m_oHt->GetValue();
		ods_context->current_table().set_row_height(height);
		//нужно преобразование размерности !!!
	}
	if (oox_row->m_oCustomHeight.IsInit() && oox_row->m_oCustomHeight->GetValue() == 1)
	{ 
		ods_context->current_table().set_row_optimal_height(false);
	}else
		ods_context->current_table().set_row_optimal_height(true);

	for (long cell = 0 ; cell < oox_row->m_arrItems.GetSize();cell++)
	{
		convert(oox_row->m_arrItems[cell]);
	}
	ods_context->end_row();
}
void XlsxConverter::convert(OOX::Spreadsheet::CCell *oox_cell)
{
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCellMetadata;
				//nullable<SimpleTypes::COnOff<>>						m_oShowPhonetic;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

	std::wstring ref = oox_cell->m_oRef.IsInit() ? string2std_string(oox_cell->m_oRef.get()) : L"";

	int ifx_style = oox_cell->m_oStyle.IsInit() ? oox_cell->m_oStyle->GetValue() : -1;

	ods_context->start_cell(ref,ifx_style);

	int value_type = -1;//not defined
	if (oox_cell->m_oType.IsInit())
	{
		value_type = oox_cell->m_oType->GetValue();
	}

	if (oox_cell->m_oFormula.IsInit())
	{
		convert(oox_cell->m_oFormula.GetPointer());
	}	
	
	if (oox_cell->m_oValue.IsInit())
	{	
		if (value_type == SimpleTypes::Spreadsheet::celltypeSharedString)
		{
			convert_sharing_string(_wtoi(oox_cell->m_oValue->m_sText));
			ods_context->current_table().set_cell_type(5);
		}
		else
		{
			ods_context->current_table().set_cell_value (string2std_string(oox_cell->m_oValue->m_sText));
			ods_context->current_table().set_cell_type	(value_type);
		}
	}

	//m_oRichText - вспомогательное поле для СОЗДАНИЯ xlsx
	ods_context->end_cell();
}
void XlsxConverter::convert_sharing_string(int number)
{
	if (!ods_context) return;
	
	const OOX::Spreadsheet::CSharedStrings *SharedStrings= xlsx_document->GetSharedStrings();
	if (!SharedStrings) return;

	if (number <0 || (SharedStrings->m_oCount.IsInit() && number > SharedStrings->m_oCount->GetValue()))return;//???? m_oUniqueCount;

	OOX::Spreadsheet::CSi* pSi = static_cast<OOX::Spreadsheet::CSi*>(SharedStrings->m_arrItems[number]);

	if (pSi == NULL)return;

	ods_context->start_cell_text();

	for(int i = 0; i < pSi->m_arrItems.GetSize(); ++i)
	{
		convert(pSi->m_arrItems[i]);
	}

	ods_context->end_cell_text();
	ods_context->current_table().set_cell_text( ods_context->current_text_context());
}
void XlsxConverter::convert(OOX::Spreadsheet::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::Spreadsheet::et_r:
		{
			OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(oox_unknown);
			convert(pRun);
		}break;
		case OOX::Spreadsheet::et_t:
		{
			OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(oox_unknown);
			convert(pText);
		}break;
		default:
		{
			std::wstringstream ss;
			ss << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
			_CP_LOG(error) << ss.str();
		}
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CRun *oox_text_run)
{
	if (oox_text_run == NULL)return;
	
	odf::office_element_ptr odf_style;

	convert(oox_text_run->m_oRPr.GetPointer());
	ods_context->current_text_context()->start_span();	//пока принимает последний сгенереннй на контексте .. вопрос  - может по имени связать??
	
	for(int i = 0; i < oox_text_run->m_arrItems.GetSize(); ++i)
	{
		convert(oox_text_run->m_arrItems[i]);
	}
	ods_context->current_text_context()->end_span();
}
void XlsxConverter::convert(OOX::Spreadsheet::CRPr *oox_text_pr)
{
	if (oox_text_pr == NULL)return;

	bool automatic = true;
	bool root = false;

	ods_context->styles_context().create_style(L"",odf::style_family::Text, automatic, root, -1);	
	
	odf::style_text_properties	* text_properties = ods_context->styles_context().last_state().get_text_properties();
	if (text_properties == NULL)return;

	if (oox_text_pr->m_oBold.IsInit())
	{
		if (oox_text_pr->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}
	convert(oox_text_pr->m_oColor.GetPointer(),text_properties->content().fo_color_);

	if (oox_text_pr->m_oUnderline.IsInit())
	{
		//convert_element ????
	}
	if (oox_text_pr->m_oItalic.IsInit())
	{
		if (oox_text_pr->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Normal);
	}
	if (oox_text_pr->m_oSz.IsInit())
	{
		convert(oox_text_pr->m_oSz->m_oVal->GetValue(), text_properties->content().fo_font_size_);
	}
	if (oox_text_pr->m_oFamily.IsInit())
	{
	}

	if (oox_text_pr->m_oRFont.IsInit())
	{
		text_properties->content().style_font_name_ = string2std_string(oox_text_pr->m_oRFont->m_sVal.get());
	}

	//convert(oox_text_pr->m_oVertAlign.GetPointer(),...
			//nullable<CCharset>												m_oCharset;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oCondense;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oExtend;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oOutline;
			//nullable<CFontScheme>											m_oScheme;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oShadow;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oStrike;


}
void XlsxConverter::convert(OOX::Spreadsheet::CText *oox_text)
{
	if (oox_text == NULL)return;

	ods_context->add_text_content( string2std_string(oox_text->m_sText) );
}
void XlsxConverter::convert(OOX::Spreadsheet::CFormula *oox_formula)
{
	if (oox_formula == NULL)return;

	ods_context->current_table().set_cell_formula(string2std_string(oox_formula->m_sText));
}
void XlsxConverter::convert(OOX::Spreadsheet::CCol *oox_column)
{
	if (oox_column == NULL)return;

	int start_column = oox_column->m_oMin.IsInit() ? oox_column->m_oMin->GetValue() : 0 ;
	int repeated =	(oox_column->m_oMax.IsInit() ? oox_column->m_oMax->GetValue() : 0) - 
					(oox_column->m_oMin.IsInit() ? oox_column->m_oMin->GetValue() : 0) + 1;

	int level = 0;
	if (oox_column->m_oOutlineLevel.IsInit())
	{
		level = oox_column->m_oOutlineLevel->GetValue();
	}

	ods_context->add_column(start_column, repeated, level);

	double width = oox_column->m_oWidth.IsInit() ? oox_column->m_oWidth->GetValue() : -1;

	if (width < 0 || (oox_column->m_oBestFit.IsInit()  && oox_column->m_oBestFit->GetValue() == 1))
	{
		ods_context->current_table().set_column_optimal_width(true);
	}
	else if (oox_column->m_oCustomWidth.IsInit() == false || 
			(oox_column->m_oCustomWidth.IsInit() == true && oox_column->m_oCustomWidth->GetValue() == 1))
	{
		ods_context->current_table().set_column_width(width);
		ods_context->current_table().set_column_optimal_width(false);
		// пока не преобразовываем правильно размерность !!!
		//???
	}

	std::wstring style_cell_name;
	if (oox_column->m_oStyle.IsInit())
	{	
		int xfd_id = oox_column->m_oStyle->GetValue();

		style_cell_name  = ods_context->styles_context().find_odf_style_name(xfd_id,odf::style_family::TableCell);
	}
	else
	{
		//нужно добавить дефолтный стиль для ячеек ДАННОГО листа
		//???
		style_cell_name= ods_context->styles_context().find_odf_style_name_default(odf::style_family::TableCell);
	}
	if (style_cell_name.length() > 0)
		ods_context->current_table().set_column_default_cell_style(style_cell_name );
}
void XlsxConverter::convert(OOX::Spreadsheet::CSheetPr *oox_sheet_pr)
{
	if (!oox_sheet_pr)return;
			//nullable<CString>					m_oCodeName;
			//nullable<SimpleTypes::COnOff<>>		m_oEnableFormatConditionsCalculation;
			//nullable<SimpleTypes::COnOff<>>		m_oFilterMode;
			//nullable<SimpleTypes::COnOff<>>		m_oPublished;
			//nullable<SimpleTypes::COnOff<>>		m_oSyncHorizontal;
			//nullable<CString>					m_oSyncRef;
			//nullable<SimpleTypes::COnOff<>>		m_oSyncVertical;
			//nullable<SimpleTypes::COnOff<>>		m_oTransitionEntry;
			//nullable<SimpleTypes::COnOff<>>		m_oTransitionEvaluation;
	if (oox_sheet_pr->m_oTabColor.IsInit())
	{
		_CP_OPT(odf::color) odf_color;
		convert(oox_sheet_pr->m_oTabColor.GetPointer(), odf_color);
		ods_context->current_table().set_table_tab_color(odf_color);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CSheetFormatPr *oox_sheet_format_pr)
{
	if (!oox_sheet_format_pr)return;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oBaseColWidth;
				//nullable<SimpleTypes::COnOff<>>					m_oCustomHeight;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelCol;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevelRow;
				//nullable<SimpleTypes::COnOff<>>					m_oThickBottom;
				//nullable<SimpleTypes::COnOff<>>					m_oThickTop;
				//nullable<SimpleTypes::COnOff<>>					m_oZeroHeight;
//пока не красиво :(
//Cell default////////////////////////////
	odf::odf_style_state  *default_cell_style=NULL;
	ods_context->styles_context().find_odf_style_state(0,odf::style_family::TableCell, default_cell_style, true);
	if (default_cell_style)
		ods_context->styles_context().add_default(*default_cell_style);
///Column///////////////////////////////////////////////////////////////////////////////////////
	ods_context->styles_context().create_style(L"",odf::style_family::TableColumn, true, false, -1);		
	{	
		double width = 8.1; // из приложния MS Office 2010
		//в xlsx необязательно задавать ширину (колонок) - дефолтное по приложению. в oo - обязательно
		odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
		if (style)
		{
			odf::style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
			if (column_properties)
			{		
				column_properties->style_table_column_properties_attlist_.common_break_attlist_.fo_break_before_ = odf::fo_break(odf::fo_break::Auto);
				if (oox_sheet_format_pr->m_oDefaultColWidth.IsInit())
				{			
					width =  oox_sheet_format_pr->m_oDefaultColWidth->GetValue();
				}
				ods_context->current_table().defaut_column_width_ = 6*width;//pt
				column_properties->style_table_column_properties_attlist_.style_column_width_ = odf::length(width/4.35,odf::length::cm);
			}
		}
		ods_context->styles_context().add_default(ods_context->styles_context().last_state());
	}
//Row default //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		ods_context->styles_context().create_style(L"",odf::style_family::TableRow, true, false, -1);	
		odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
		if (style)
		{		
			odf::style_table_row_properties * row_properties = style->style_content_.get_style_table_row_properties();
			if (row_properties)
			{
				if (oox_sheet_format_pr->m_oDefaultRowHeight.IsInit())
				{
					double height = oox_sheet_format_pr->m_oDefaultRowHeight->GetValue();

					ods_context->current_table().defaut_row_height_ = height;//pt
					row_properties->style_table_row_properties_attlist_.style_row_height_ = odf::length(height/32.,odf::length::cm);
				}
				row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; //???? не знаю cтоит ли 
				row_properties->style_table_row_properties_attlist_.common_break_attlist_.fo_break_before_ = odf::fo_break(odf::fo_break::Auto);
			}
		}
		ods_context->styles_context().add_default(ods_context->styles_context().last_state());
	}
}
void XlsxConverter::convert_styles()
{
	if (!ods_context) return;

//	add default styles
	ods_context->styles_context().create_default(odf::style_family::TableCell);

///////////////////////////////////////////////////////////////////////////////////////////////////
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
	if (!xlsx_styles)return;

	for (long i=0; xlsx_styles->m_oNumFmts.IsInit() && i< xlsx_styles->m_oNumFmts->m_oCount->GetValue(); i++)//in styles record
	{
		convert(xlsx_styles->m_oNumFmts->m_arrItems[i]);
	}

	for (long i=0; xlsx_styles->m_oCellStyleXfs.IsInit() && i< xlsx_styles->m_oCellStyleXfs->m_oCount->GetValue(); i++)//in styles record
	{
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, false, true);//их надо сделать "невидимыми" для программ
	}
///////////////////////////////////	
	for (long i=0; xlsx_styles->m_oCellXfs.IsInit() && i< xlsx_styles->m_oCellXfs->m_oCount->GetValue(); i++)
	{
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i],i, true,false);

	}
	
	for (long i=0; xlsx_styles->m_oCellStyles.IsInit() && i< xlsx_styles->m_oCellStyles->m_oCount->GetValue(); i++)//styles.xml
	{
		//тоже самое что и 1 но с именем .. типо заголовок, обычный, ...
		//и доп свойствами
		convert(xlsx_styles->m_oCellStyles->m_arrItems[i],i); //видимые стили для программ
	}
}


void XlsxConverter::convert(OOX::Spreadsheet::CFont * font, odf::style_text_properties * text_properties)
{
	if (font == NULL)return;
 	if (text_properties == NULL)return;

	if (font->m_oBold.IsInit())
	{
		if (font->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}
	convert(font->m_oColor.GetPointer(),text_properties->content().fo_color_);

	if (font->m_oUnderline.IsInit())
	{
		//convert_element ????
	}
	if (font->m_oItalic.IsInit())
	{
		if (font->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Italic);
		else
			text_properties->content().fo_font_style_ = odf::font_style(odf::font_style::Normal);
	}
	if (font->m_oSz.IsInit())
	{
		convert(font->m_oSz->m_oVal->GetValue(), text_properties->content().fo_font_size_);
	}
	if (font->m_oFamily.IsInit())
	{
	}

	if (font->m_oRFont.IsInit())
	{
		text_properties->content().style_font_name_ = string2std_string(font->m_oRFont->m_sVal.get());
	}
	/////
	//...
	/////
}
void XlsxConverter::convert(double oox_font_size,  _CP_OPT(odf::font_size) & odf_font_size)
{
	 _CP_OPT(odf::length) odf_length;

	 convert(oox_font_size, odf_length);
	 
	 if (odf_length)
		 odf_font_size = odf::font_size(odf_length.get());
}
void XlsxConverter::convert(double oox_size,  _CP_OPT(odf::length) & odf_size)
{
	//нужно сделать преобразования типов oox_size
	//???
	odf_size = odf::length(oox_size, odf::length::pt);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFill * fill, odf::style_table_cell_properties * cell_properties)
{
	if (fill == NULL)return;
 	if (cell_properties == NULL)return;

	if (fill->m_oGradientFill.IsInit())
	{
	}
	if (fill->m_oPatternFill.IsInit())
	{//solid, gradient
		if (fill->m_oPatternFill->m_oPatternType.IsInit())
		{}
		
		if (fill->m_oPatternFill->m_oFgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oFgColor.GetPointer(), 
				cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_);
		}
		else if (fill->m_oPatternFill->m_oBgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oBgColor.GetPointer(), 
				cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_);
		}	
					
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::background_color) & odf_bckgrd_color)
{
	if (!color)return;

	_CP_OPT(odf::color) odf_color;
	convert(color, odf_color);

	if (odf_color)odf_bckgrd_color = odf::background_color(odf_color.get());
}
void XlsxConverter::convert(OOX::Spreadsheet::CNumFmt *numFmt)
{
	if (!numFmt)return;

	if (numFmt->m_oFormatCode.IsInit() && numFmt->m_oNumFmtId.IsInit())
	{
		ods_context->numbers_styles_context().add_or_find(numFmt->m_oNumFmtId->GetValue(), string2std_string(numFmt->m_oFormatCode.get()));
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CBorder *border, odf::style_table_cell_properties * cell_properties)
{
	if (!border)return;

	if (border->m_oBottom.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oBottom.GetPointer(), odf_border);
		cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_ = odf_border;
	}
	if (border->m_oTop.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oTop.GetPointer(), odf_border);
		cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_ = odf_border;
	}
	if (border->m_oStart.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oStart.GetPointer(), odf_border);
		if (odf_border.length()>0)
			cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_ = odf_border;
	}
	if (border->m_oEnd.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oEnd.GetPointer(), odf_border);
		if (odf_border.length()>0)
			cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = odf_border;
	}
	if (border->m_oEnd.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oEnd.GetPointer(), odf_border);
		if (odf_border.length()>0)
			cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_ = odf_border;
	}
	if (border->m_oDiagonal.IsInit())
	{
		std::wstring odf_border;
		convert(border->m_oDiagonal.GetPointer(), odf_border);
		if (border->m_oDiagonalDown.IsInit() && odf_border.length()>0) //and true???
			cell_properties->style_table_cell_properties_attlist_.style_diagonal_tl_br_= odf_border;
		if (border->m_oDiagonalUp.IsInit() && odf_border.length()>0) //and true???
			cell_properties->style_table_cell_properties_attlist_.style_diagonal_bl_tr_= odf_border;
	}
	//nullable<CBorderProp>						m_oHorizontal;
	//nullable<CBorderProp>						m_oVertical;
	//nullable<SimpleTypes::COnOff<>>			m_oOutline;
}
void XlsxConverter::convert(OOX::Spreadsheet::CBorderProp *borderProp, std::wstring & odf_border_prop)
{
	if (!borderProp)return;

	std::wstring border_style;
	std::wstring border_color;

	if (borderProp->m_oStyle.IsInit())
	{
		switch(borderProp->m_oStyle->GetValue())
		{
			case SimpleTypes::Spreadsheet::borderstyleDashDot:
				border_style = L"1pt dot-dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleDashDotDot:
				border_style = L"1pt dot-dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleDashed:
				border_style = L"1pt dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleDotted:
				border_style = L"1pt dotted";
			break;
			case SimpleTypes::Spreadsheet::borderstyleDouble:
				border_style = L"1pt double";
			break;
			case SimpleTypes::Spreadsheet::borderstyleHair:
				border_style = L"1pt solid";
			break;
			case SimpleTypes::Spreadsheet::borderstyleMedium:
				border_style = L"2.49pt solid";
			break;
			case SimpleTypes::Spreadsheet::borderstyleMediumDashDot:
				border_style = L"2.49pt dot-dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleMediumDashDotDot:
				border_style = L"2.49pt dot-dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleMediumDashed:
				border_style = L"2.49pt dashed";
			break;
			case SimpleTypes::Spreadsheet::borderstyleNone:
				border_style = L"none";
				return;
			break;
			case SimpleTypes::Spreadsheet::borderstyleSlantDashDot:
				border_style = L"1pt solid";
			break;
			case SimpleTypes::Spreadsheet::borderstyleThick:
				border_style = L"1pt solid";
			break;
			case SimpleTypes::Spreadsheet::borderstyleThin:
				 border_style = L"1pt solid";
			break;
		}
	}
	else return;

	if (borderProp->m_oColor.IsInit())
	{
		_CP_OPT(odf::color) odf_color;
		convert(borderProp->m_oColor.GetPointer(), odf_color);
		if (odf_color)
			border_color = L" #" + odf_color->get_hex_value();
	}
	if (border_color.length()<1)border_color = L" #000000";
	
	odf_border_prop = border_style + border_color;
}
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color)//стоит ли сюда тащить odf type???
{
	if (!color)return;

	unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
	bool result = false;
	
	if(color->m_oRgb.IsInit())//easy, faster,realy  !!
	{
		ucR = color->m_oRgb->Get_R(); 
		ucB = color->m_oRgb->Get_B(); 
		ucG = color->m_oRgb->Get_G(); 
		ucA = color->m_oRgb->Get_A(); 
		result = true;
	}
	if(color->m_oThemeColor.IsInit())
	{
		OOX::CTheme * xlsx_theme= xlsx_document->GetTheme();
		int theme_ind = color->m_oThemeColor->GetValue();
		switch(theme_ind)//а вот нет CColorMapping на чтение !!!
		{
			case SimpleTypes::Spreadsheet::themecolorLight1:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oLt1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorLight2:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oLt2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorDark1:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oDk1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorDark2:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oDk2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent1:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent1.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent2:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent2.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent3:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent3.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent4:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent4.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent5:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent5.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorAccent6:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oAccent6.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorFollowedHyperlink:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oFolHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
			case SimpleTypes::Spreadsheet::themecolorHyperlink:
				result = xlsx_theme->m_oThemeElements.m_oClrScheme.m_oHlink.tryGetRgb(ucR, ucG, ucB, ucA); break;
		}
	}
	if(color->m_oIndexed.IsInit())
	{
		OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
		int ind = color->m_oIndexed->GetValue();

		if(xlsx_styles->m_oColors.IsInit() && xlsx_styles->m_oColors->m_oIndexedColors.IsInit())
		{
			if ((xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]) && 
				(xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb.IsInit()))
			{			
				ucR = xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb->Get_R(); 
				ucB = xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb->Get_B(); 
				ucG = xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb->Get_G(); 
				ucA = xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb->Get_A(); 
				result = true;
			}
		}
		else
		{
			result = OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(ind, ucR, ucG, ucB, ucA);
		}
	}
	if (result == true)
	{
		if ((color->m_oTint.IsInit()) && (color->m_oTint->GetValue() > 0))
		{
			ucR = color->m_oTint->GetValue() * 0xff + (1. - color->m_oTint->GetValue()) * ucR; 
			ucG = color->m_oTint->GetValue() * 0xff + (1. - color->m_oTint->GetValue()) * ucG;
			ucB = color->m_oTint->GetValue() * 0xff + (1. - color->m_oTint->GetValue()) * ucB; 
		}
		if ((color->m_oTint.IsInit()) && (color->m_oTint->GetValue() <  0))
		{
			ucR = -color->m_oTint->GetValue() * 0x00 + (1. + color->m_oTint->GetValue()) * ucR; 
			ucG = -color->m_oTint->GetValue() * 0x00 + (1. + color->m_oTint->GetValue()) * ucG; 
			ucB = -color->m_oTint->GetValue() * 0x00 + (1. + color->m_oTint->GetValue()) * ucB; 
		}
		SimpleTypes::Spreadsheet::CHexColor *oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(ucR,ucG,ucB,ucA);
		if (oRgbColor)
		{		
			odf_color = odf::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString().Right(6)));
			delete oRgbColor;
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CCellStyle * cell_style, int oox_id)
{
	if (cell_style == NULL)return;
	
	bool automatic = false;
	bool root = true;

	std::wstring style_name = L"";
	if (cell_style->m_oName.IsInit())style_name = string2std_string(cell_style->m_oName.get());

	ods_context->styles_context().create_style(style_name,odf::style_family::TableCell, automatic, root, oox_id);

	odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
	if (!style)return;

	int parent_id = cell_style->m_oXfId.IsInit() ? cell_style->m_oXfId->GetValue() : -1;

	if (parent_id >=0) 
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(parent_id, odf::style_family::TableCell,true);
	
	//m_oBuiltinId, m_oCustomBuiltin, m_oHidden, m_oILevel;	???????
}
void XlsxConverter::convert(OOX::Spreadsheet::CXfs * xfc_style, int oox_id, bool automatic, bool root)
{
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();

	int id_parent	= xfc_style->m_oXfId.IsInit()		? xfc_style->m_oXfId->GetValue()	: -1; 
	int fill_id		= xfc_style->m_oFillId.IsInit()		? xfc_style->m_oFillId->GetValue()	: -1;
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): -1; 
	int font_id		= xfc_style->m_oFontId.IsInit()		? xfc_style->m_oFontId->GetValue()	: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context().create_style(L"",odf::style_family::TableCell, automatic, root, oox_id); 
	
	odf::style_text_properties			* text_properties		= ods_context->styles_context().last_state().get_text_properties();
	odf::style_table_cell_properties	* table_cell_properties = ods_context->styles_context().last_state().get_table_cell_properties();
	
	if (xlsx_styles->m_oFonts.IsInit() && font_id >=0 && (id_parent < 0 || xfc_style->m_oApplyFont.IsInit()))	
	{
		convert(xlsx_styles->m_oFonts->m_arrItems[font_id], text_properties); 
	}
	if (xlsx_styles->m_oFills.IsInit() && fill_id >=0 && (id_parent < 0 || xfc_style->m_oApplyFill.IsInit()))
	{
		convert(xlsx_styles->m_oFills->m_arrItems[fill_id], table_cell_properties); 
	}
	if (numFmt_id>=0/* && (id_parent < 0 || xfc_style->m_oApplyNumberFormat.IsInit())*/)	
	{
		ods_context->styles_context().last_state().set_data_style_name(ods_context->numbers_styles_context().add_or_find(numFmt_id).style_name);
		ods_context->styles_context().last_state().set_number_format(numFmt_id);
	}
	if (xlsx_styles->m_oBorders.IsInit() && border_id >=0 && (id_parent < 0 || xfc_style->m_oApplyBorder.IsInit()))	
	{
		convert(xlsx_styles->m_oBorders->m_arrItems[border_id], table_cell_properties); 
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (id_parent >=0)
	{
		odf::odf_style_state  *parent_style_state=NULL;
		ods_context->styles_context().find_odf_style_state(id_parent, odf::style_family::TableCell,parent_style_state, true );

		if (parent_style_state)
		{
			ods_context->styles_context().last_state().set_parent_style_name(parent_style_state->get_name());

			if (numFmt_id < 0)
			{
				ods_context->styles_context().last_state().set_number_format(parent_style_state->get_number_format());
			}
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XlsxConverter::convert(OOX::Spreadsheet::CCellAnchor *oox_anchor)
{
////////////////// 
	if (oox_anchor->m_oFrom.IsInit() || oox_anchor->m_oTo.IsInit())
	{
		oox_table_position from={}, to={};
		
		convert(oox_anchor->m_oFrom.GetPointer(), &from);	
		convert(oox_anchor->m_oTo.GetPointer(),&to);

		double x1=0, y1=0, x2=0, y2=0;
		ods_context->current_table().convert_position(from, x1, y1);
		ods_context->current_table().convert_position(to, x2, y2);
		
		ods_context->drawing_context().set_rect(x1, y1, x2-x1, y2-y1);
	}
		
//////////////
	if (oox_anchor->m_oPos.IsInit())
	{
	}

	if (oox_anchor->m_oExt.IsInit())
	{
	}
//собственно
	if (oox_anchor->m_oPicture.IsInit())//picture
	{
		ods_context->drawing_context().start_frame();
			convert(oox_anchor->m_oPicture.GetPointer());
		ods_context->drawing_context().end_frame();
	}	
	else if (oox_anchor->m_oShape.IsInit())
	{
		convert(oox_anchor->m_oShape.GetPointer());
	}	
	else if (oox_anchor->m_oGraphicFrame.IsInit())//chart
	{
		ods_context->drawing_context().start_frame();
		//m_oChartGraphic
		ods_context->drawing_context().end_frame();
	}	

}

void XlsxConverter::convert(OOX::Spreadsheet::CDrawing *oox_drawing)
{
	if (!oox_drawing)return;
	xlsx_current_drawing = oox_drawing;

	for (long dr = 0 ; dr < oox_drawing->m_arrItems.GetSize(); dr++)
	{
		ods_context->start_drawing();
			convert(oox_drawing->m_arrItems[dr]);
		ods_context->end_drawing();
	}
}


void XlsxConverter::convert(OOX::Spreadsheet::CFromTo* oox_from_to, oox_table_position * pos)
{
	if (!oox_from_to || !pos)return;

	if (oox_from_to->m_oCol.IsInit()) pos->col = oox_from_to->m_oCol->GetValue();
	if (oox_from_to->m_oRow.IsInit()) pos->row = oox_from_to->m_oRow->GetValue();
	if (oox_from_to->m_oRowOff.IsInit()) pos->row_off = oox_from_to->m_oRowOff->GetValue();//pt
	if (oox_from_to->m_oColOff.IsInit()) pos->col_off = oox_from_to->m_oColOff->GetValue();//pt
}

void XlsxConverter::convert(OOX::Spreadsheet::CShape* oox_shape)
{
	if (!oox_shape)return;
	if (!oox_shape->m_oSpPr.IsInit()) return;

	std::wstring sub_type;
	int type = -1;
	if (oox_shape->m_oSpPr->m_eGeomType == OOX::Drawing::geomtypeCustom)
	{
		type = 6;
	}
	else if (oox_shape->m_oSpPr->m_eGeomType == OOX::Drawing::geomtypePreset)
	{
		if (oox_shape->m_oSpPr->m_oPrstGeom.IsInit())
		{
			OOX::Drawing::CPresetGeometry2D * geometry = oox_shape->m_oSpPr->m_oPrstGeom.GetPointer();
			int oox_shape = (int)(geometry->m_oPrst.GetValue());
			sub_type = Shape_Types_Mapping[oox_shape].first;
			type = Shape_Types_Mapping[oox_shape].second;
		}
	}
	else
		return;

	ods_context->drawing_context().start_shape(type,sub_type );
	{
		convert_SpPr(oox_shape->m_oSpPr.GetPointer());//все, окромя типа геометрии

		if (oox_shape->m_oNvSpPr.IsInit())
		{
			if (oox_shape->m_oNvSpPr->m_oCNvPr.IsInit())
			{
				convert_CNvPr(oox_shape->m_oNvSpPr->m_oCNvPr.GetPointer());		
			}

			if (oox_shape->m_oNvSpPr->m_oCNvSpPr.IsInit())
			{
			}
		}


		if (oox_shape->m_oShapeStyle.IsInit())
		{
		}
	}
	ods_context->drawing_context().end_shape();
}


void XlsxConverter::convert(OOX::Spreadsheet::CPic* oox_picture)
{
	if (!oox_picture)return;
	if (!oox_picture->m_oBlipFill.IsInit()) return; // невeрная структура оох

	CString pathImage;

	if (oox_picture->m_oBlipFill->m_oBlip.IsInit())
	{
		CString sID = oox_picture->m_oBlipFill->m_oBlip->m_oEmbed.GetValue();
		
		smart_ptr<OOX::File> oFile = xlsx_current_drawing->Find(sID);
		if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Image == oFile->type())
		{
			OOX::Spreadsheet::Image* pImage = (OOX::Spreadsheet::Image*)oFile.operator->();

			pathImage = pImage->filename().GetPath();
		}		
	}
	ods_context->start_image(string2std_string(pathImage));
	{
		if (oox_picture->m_oBlipFill->m_oTile.IsInit()) 
			ods_context->drawing_context().set_tile(true);

		if (oox_picture->m_oBlipFill->m_oSrcRect.IsInit())
		{
			//set client rect //проценты m_oB; m_oL; m_oR; m_oT;
			
		}		
		if (oox_picture->m_oNvPicPr.IsInit())
		{
			if (oox_picture->m_oNvPicPr->m_oCNvPr.IsInit())
			{
				convert_CNvPr(oox_picture->m_oNvPicPr->m_oCNvPr.GetPointer());		
			}

			if (oox_picture->m_oNvPicPr->m_oCNvPicPr.IsInit())
			{
				if (oox_picture->m_oNvPicPr->m_oCNvPicPr->m_oPicLocks.IsInit())
				{
					//if (oox_picture->m_oNvPicPr->m_oCNvPicPr->m_oPicLocks->m_oNoChangeAspect)
					//{
					//}
					//if (oox_picture->m_oNvPicPr->m_oCNvPicPr->m_oPicLocks->m_oNoCrop))
					//{
					//}
					//if (oox_picture->m_oNvPicPr->m_oCNvPicPr->m_oPicLocks->m_oNoResize)
					//{
					//}
				}	
				//m_oExtLst
			}
		}
		if (oox_picture->m_oSpPr.IsInit())
		{
			convert_SpPr(oox_picture->m_oSpPr.GetPointer());
		}

		if (oox_picture->m_oShapeStyle.IsInit())
		{
		}
	}
	ods_context->end_image();
}

} // namespace Docx2Odt