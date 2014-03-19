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
	//колонки
	if(oox_sheet->m_oCols.IsInit())
	{
		for (long col = 0 ; col < oox_sheet->m_oCols->m_arrItems.GetSize();col++)
		{
			convert(oox_sheet->m_oCols->m_arrItems[col]);
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CCol *oox_column)
{
				//nullable<SimpleTypes::COnOff<>>					m_oBestFit;
				//nullable<SimpleTypes::COnOff<>>					m_oCollapsed;
				//nullable<SimpleTypes::COnOff<>>					m_oCustomWidth;
				//nullable<SimpleTypes::COnOff<>>					m_oHidden;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMax;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMin;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oOutlineLevel;
				//nullable<SimpleTypes::COnOff<>>					m_oPhonetic;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oStyle;
				//nullable<SimpleTypes::CDouble>					m_oWidth;
	int repeated = (oox_column->m_oMax.IsInit() ? oox_column->m_oMax->GetValue() : 0) - 
					(oox_column->m_oMin.IsInit() ? oox_column->m_oMin->GetValue() : 0);

	int oox_style_id = -1;

	ods_context->styles_context().create_style(L"",odf::style_family::TableColumn, true, false, oox_style_id);

	odf::style* style = dynamic_cast<odf::style*>(ods_context->styles_context().last_state().get_office_element().get());
	if (!style)return;
	
	odf::style_table_column_properties * column_properties = style->style_content_.get_style_table_column_properties();
 	if (column_properties == NULL)return; //error ????

	double width = oox_column->m_oWidth.IsInit() ? oox_column->m_oWidth->GetValue() : 0;

	if (oox_column->m_oStyle.IsInit())
	{	
		//m_oCustomWidth 
	}	
	
	if (width < 1 || (oox_column->m_oBestFit.IsInit()  && oox_column->m_oBestFit->GetValue()==true))
		column_properties->style_table_column_properties_attlist_.style_use_optimal_column_width_ = true;
	else
	{
		column_properties->style_table_column_properties_attlist_.style_column_width_ = odf::length(width,odf::length::cm); 
		// пока не преобразовываем правельно размерность !!!
	}
	ods_context->add_column(repeated, style->style_name_);
}

void XlsxConverter::convert_styles()
{
	if (!ods_context) return;
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
	if (!xlsx_styles)return;

	if (!xlsx_styles->m_oCellStyleXfs.IsInit())return;

	for (long i=0;i< xlsx_styles->m_oCellStyleXfs->m_oCount->GetValue(); i++)//in styles record
	{
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, false, true);
	}
	
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
	
	odf::style_text_properties * text_properties = style->style_content_.get_style_text_properties();//если элемента нет - создасться

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
	odf_size = odf::length(oox_size, odf::length::pt);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFill * fill, odf::office_element_ptr  & odf_style_)
{
	if (fill == NULL)return;
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;

	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();//если элемента нет - создасться

 	if (cell_properties == NULL)return;

	if (fill->m_oGradientFill.IsInit())
	{
	}
	if (fill->m_oPatternFill.IsInit())
	{//solid, gradient
		if (fill->m_oPatternFill->m_oPatternType.IsInit())
		{}
		if (fill->m_oPatternFill->m_oBgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oBgColor.GetPointer(), 
				cell_properties->style_table_cell_properties_attlist_.common_background_color_attlist_.fo_background_color_);
		}
		if (fill->m_oPatternFill->m_oFgColor.IsInit())
		{}

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

void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf::color) & odf_color)
{
	if (!color)return;

	if(color->m_oRgb.IsInit())
	{
		odf_color = odf::color(std::wstring(L"#") + string2std_string(color->m_oRgb->ToString()));
	}
	if(color->m_oThemeColor.IsInit())
	{
		OOX::CTheme * xlsx_theme= xlsx_document->GetTheme();

		int theme_ind = color->m_oThemeColor->GetValue();

	}
	if(color->m_oIndexed.IsInit())
	{
		OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
		
		int ind = color->m_oIndexed->GetValue();

		SimpleTypes::Spreadsheet::CHexColor *oRgbColor=NULL;
		if(xlsx_styles->m_oColors.IsInit() && xlsx_styles->m_oColors->m_oIndexedColors.IsInit())
		{
			if (xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind])
				oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems[ind]->m_oRgb.get());
		}
		else
		{
			unsigned char ucA=0, ucR=0, ucG=0, ucB=0;
			if(OOX::Spreadsheet::CIndexedColors::GetDefaultRGBAByIndex(ind, ucR, ucG, ucB, ucA))
				oRgbColor = new SimpleTypes::Spreadsheet::CHexColor(ucR,ucG,ucB,ucA);
		}
		if (oRgbColor)
		{
			odf_color = odf::color(std::wstring(L"#") + string2std_string(oRgbColor->ToString()));
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
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): -1;
	int font_id		= xfc_style->m_oFontId.IsInit()	? xfc_style->m_oFontId->GetValue()		: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context().create_style(L"",odf::style_family::TableCell, automatic, root, oox_id); 

	odf::office_element_ptr & elm_style = ods_context->styles_context().last_state().get_office_element();
	
	if (xlsx_styles->m_oFonts.IsInit())		convert(xlsx_styles->m_oFonts->m_arrItems[font_id], elm_style); //проверять также applyFont
	if (xlsx_styles->m_oFills.IsInit())		convert(xlsx_styles->m_oFills->m_arrItems[fill_id], elm_style); //проверять также applyFill
	if (xlsx_styles->m_oNumFmts.IsInit())	convert(xlsx_styles->m_oNumFmts->m_arrItems[numFmt_id], elm_style); 
	if (xlsx_styles->m_oBorders.IsInit())	convert(xlsx_styles->m_oBorders->m_arrItems[border_id], elm_style); 

	odf::style* style = dynamic_cast<odf::style*>(elm_style.get());
	if (!style)return;

	if (id_parent >=0)
	{
		style->style_parent_style_name_ = ods_context->styles_context().find_odf_style_name(id_parent, odf::style_family::TableCell);
			
	}
	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();
	
		

}
} // namespace Docx2Odt