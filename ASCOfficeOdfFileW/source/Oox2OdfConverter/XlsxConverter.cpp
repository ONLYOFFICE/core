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
}

void XlsxConverter::convert_styles()
{
	if (!ods_context) return;
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
	if (!xlsx_styles)return;

	if (!xlsx_styles->m_oCellStyleXfs.IsInit())return;

	for (long i=0;i< xlsx_styles->m_oCellStyleXfs->m_oCount->GetValue(); i++)
	{
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i);
	}
	
	for (long i=0;i< xlsx_styles->m_oCellXfs->m_oCount->GetValue(); i++)
	{
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i],i);

	}
	
	for (long i=0;i< xlsx_styles->m_oCellStyles->m_oCount->GetValue(); i++)//styles.xml
	{
		//std::wstring style_name = string2std_string(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oName.get2());

		//ods_context->styles_context().start_style(style_name ,odf::style_family::TableCell);
		//int id_xfd = xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oXfId->GetValue();

		//xlsx_styles->m_oCellXfs->m_arrItems[id_xfd];//содержимое стил€
		//odf::office_element_ptr & elm = ods_context->styles_context().find_by_id(, odf::style_family::TableCell);
		//ods_context->styles_.back()->add_child(elm);
		

		//ods_context->styles_context().start_style(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oName,odf::style_family::TableCell);
		//generate style:style

		//ods_context->styles_context().current_style().set_cell_style_id();
		////ниже - найти стиль по номеру, вытащить text_properies, ... - автозаполнение//style:table-cell-properties, style:paragraph-properties , ...

		//ods_context->styles_context().current_style().set_hidden(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oHidden->ToBool())
		//ods_context->styles_context().current_style().set_level(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oILevel->GetValue())
		//
	}

}

void XlsxConverter::convert(OOX::Spreadsheet::CFont * font, odf::office_element_ptr  & odf_style_)
{
	odf::style* style = dynamic_cast<odf::style*>(odf_style_.get());
	if (!style)return;
	
	odf::style_text_properties * text_properties = style->style_content_.get_style_text_properties();//если элемента нет - создастьс€

 	if (text_properties == NULL)return;


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

odf::office_element_ptr XlsxConverter::convert(OOX::Spreadsheet::CXfs * cell_style, int oox_id)
{
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();

	int id_parent	= cell_style->m_oXfId.IsInit()		? cell_style->m_oXfId->GetValue()		: -1; //parent 
	int fill_id		= cell_style->m_oFillId.IsInit()	? cell_style->m_oFillId->GetValue()		: -1;
	int numFmt_id	= cell_style->m_oNumFmtId.IsInit()	? cell_style->m_oNumFmtId->GetValue()	: -1;
	int font_id		= cell_style->m_oFontId.IsInit()	? cell_style->m_oFontId->GetValue()		: -1;
	int border_id	= cell_style->m_oBorderId.IsInit()	? cell_style->m_oBorderId->GetValue()	: -1;
		
	std::wstring cell_style_name_new = L"ce" + boost::lexical_cast<std::wstring>(oox_id+1);//ваще то потом нужно искать свободнй номер
	odf::office_element_ptr elm_style = ods_context->styles_context().add_or_find(cell_style_name_new ,odf::style_family::TableCell, true, oox_id); 
				//им€ дл€ стил€ cгенеритс€ если его нет - поиск и доступ (соответствие) - по номеры oox

	if (xlsx_styles->m_oFonts.IsInit())		convert(xlsx_styles->m_oFonts->m_arrItems[font_id], elm_style); //провер€ть также applyFont
	if (xlsx_styles->m_oFills.IsInit())		convert(xlsx_styles->m_oFills->m_arrItems[fill_id], elm_style); //провер€ть также applyFill
	if (xlsx_styles->m_oNumFmts.IsInit())	convert(xlsx_styles->m_oNumFmts->m_arrItems[numFmt_id], elm_style); 
	if (xlsx_styles->m_oBorders.IsInit())	convert(xlsx_styles->m_oBorders->m_arrItems[border_id], elm_style); 

	odf::style* style = dynamic_cast<odf::style*>(elm_style.get());
	if (!style)return elm_style;

	odf::style_table_cell_properties * cell_properties = style->style_content_.get_style_table_cell_properties();
	
		
	return elm_style;

}
} // namespace Docx2Odt