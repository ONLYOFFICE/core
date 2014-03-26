#pragma once

#include "stdAfx.h"

#include "XlsxConverter.h"

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"

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
	//текущие дефолтные свойства 
	if (oox_sheet->m_oSheetFormatPr.IsInit())
		convert(oox_sheet->m_oSheetFormatPr.GetPointer());
	if (oox_sheet->m_oSheetPr.IsInit())
		convert(oox_sheet->m_oSheetPr.GetPointer());

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
}

void XlsxConverter::convert(OOX::Spreadsheet::CRow *oox_row)
{
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
	}

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
				//nullable<SimpleTypes::Spreadsheet::CCellTypeType<>>	m_oType;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

	std::wstring ref = oox_cell->m_oRef.IsInit() ? string2std_string(oox_cell->m_oRef.get()) : L"";

	int ifx_style = oox_cell->m_oStyle.IsInit() ? oox_cell->m_oStyle->GetValue() : -1;

	ods_context->start_cell(ref,ifx_style);

	int value_type = 0;//general
	if (oox_cell->m_oType.IsInit())
	{
		value_type = oox_cell->m_oType->GetValue();
	}
	ods_context->current_table().set_cell_type(value_type);

	switch (value_type)
	{
	case SimpleTypes::Spreadsheet::celltypeSharedString:
		break;
	default :
		if (oox_cell->m_oValue.IsInit())//тут может быть и значение формулы
		{
			ods_context->current_table().set_cell_value (string2std_string(oox_cell->m_oValue->m_sText));
		}		
		break;
	}
	if (oox_cell->m_oFormula.IsInit())
	{
	}
	if (oox_cell->m_oRichText.IsInit())
	{
		convert(oox_cell->m_oRichText.GetPointer());
	}
	ods_context->end_cell();
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
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CSi  *oox_shared_string)
{
	if (oox_shared_string == NULL)return;

	//ods_context->start_paragraph();
	for(int i = 0; i < oox_shared_string->m_arrItems.GetSize(); ++i)
	{
		convert(oox_shared_string->m_arrItems[i]);
	}
	//ods_context->end_paragraph();
}
void XlsxConverter::convert(OOX::Spreadsheet::CRun *oox_text_run)
{
	if (oox_text_run == NULL)return;
	//ods_context->text_context().start_span();

	//ods_context->end_span();

}
void XlsxConverter::convert(OOX::Spreadsheet::CText *oox_text)
{
	if (oox_text == NULL)return;
	//ods_context->start_text();

	//ods_context->end_text();
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
		//нужно добавить дефолтный стиль дл€ €чеек ƒјЌЌќ√ќ листа
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
		double width = 8.1; // из приложни€ MS Office 2010
		//в xlsx необ€зательно задавать ширину (колонок) - дефолтное по приложению. в oo - об€зательно
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

	if (!xlsx_styles->m_oCellStyleXfs.IsInit())return;

	for (long i=0;i< xlsx_styles->m_oCellStyleXfs->m_oCount->GetValue(); i++)//in styles record
	{
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, false, true);
	}
///////////////////////////////////	
	for (long i=0;i< xlsx_styles->m_oCellXfs->m_oCount->GetValue(); i++)
	{
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i],i, true,false);

	}
	
	for (long i=0;i< xlsx_styles->m_oCellStyles->m_oCount->GetValue(); i++)//styles.xml
	{
		//тоже самое что и 1 но с именем .. типо заголовок, обычный, ...
		//и доп свойствами
		convert(xlsx_styles->m_oCellStyles->m_arrItems[i],i);
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CFont * font, odf::office_element_ptr  & odf_style_)
{
	if (font == NULL)return;
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;
	
	odf::style_text_properties * text_properties = style->style_content_.get_style_text_properties();//если элемента нет - создастьс€

 	if (text_properties == NULL)return;
	if (font->m_oBold.IsInit())
	{
		if (font->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WBold);
		else
			text_properties->content().fo_font_weight_ = odf::font_weight(odf::font_weight::WNormal);
	}
	if (font->m_oColor.IsInit())
	{
		convert(font->m_oColor.GetPointer(),text_properties->content().fo_color_);
	}
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
	//нужно сделать преобразовани€ типов oox_size
	//???
	odf_size = odf::length(oox_size, odf::length::pt);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFill * fill, odf::office_element_ptr  & odf_style_)
{
	if (fill == NULL)return;
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;

	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();//если элемента нет - создастьс€

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
void XlsxConverter::convert(OOX::Spreadsheet::CNumFmt *numFmt, odf::office_element_ptr  & odf_style_)
{
	if (!numFmt)return;


}
void XlsxConverter::convert(OOX::Spreadsheet::CBorder *border, odf::office_element_ptr  & odf_style_)
{
	if (!border)return;

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
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(parent_id, odf::style_family::TableCell);
	
	//m_oBuiltinId, m_oCustomBuiltin, m_oHidden, m_oILevel;	???????
}
void XlsxConverter::convert(OOX::Spreadsheet::CXfs * xfc_style, int oox_id, bool automatic, bool root)
{
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();

	int id_parent	= xfc_style->m_oXfId.IsInit()		? xfc_style->m_oXfId->GetValue()	: -1; //parent 
	int fill_id		= xfc_style->m_oFillId.IsInit()	? xfc_style->m_oFillId->GetValue()		: -1;
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): 0; //general
	int font_id		= xfc_style->m_oFontId.IsInit()	? xfc_style->m_oFontId->GetValue()		: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context().create_style(L"",odf::style_family::TableCell, automatic, root, oox_id); 

	odf::office_element_ptr & elm_style = ods_context->styles_context().last_state().get_office_element();
	
	if (xlsx_styles->m_oFonts.IsInit() && font_id >=0 && (id_parent < 0 || xfc_style->m_oApplyFont.IsInit()))		
				convert(xlsx_styles->m_oFonts->m_arrItems[font_id], elm_style); 
	if (xlsx_styles->m_oFills.IsInit() && fill_id >=0 && (id_parent < 0 || xfc_style->m_oApplyFill.IsInit()))
				convert(xlsx_styles->m_oFills->m_arrItems[fill_id], elm_style); 
	if (xlsx_styles->m_oNumFmts.IsInit() && numFmt_id>=0 && (id_parent < 0 || xfc_style->m_oApplyNumberFormat.IsInit()))	
	{
		//if (numFmt_id < xlsx_styles->m_oNumFmts.Count())
		//{
		//	convert(xlsx_styles->m_oNumFmts->m_arrItems[numFmt_id], elm_style); 
		//}
		//это не индекс .. а тип данных  .. а тута описание формата ..
	}
	if (xlsx_styles->m_oBorders.IsInit() && border_id >=0 && (id_parent < 0 || xfc_style->m_oApplyBorder.IsInit()))	
				convert(xlsx_styles->m_oBorders->m_arrItems[border_id], elm_style); 

	ods_context->styles_context().last_state().set_number_format(numFmt_id);
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	odf::style* style = dynamic_cast<odf::style*>(elm_style.get());
	if (!style)return;

	if (id_parent >=0)
	{
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(id_parent, odf::style_family::TableCell);
			
	}
	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();
	
}
} // namespace Docx2Odt