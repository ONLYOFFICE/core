#pragma once

#include "stdAfx.h"

#include "XlsxConverter.h"

#include <boost/foreach.hpp>

#include "ods_conversion_context.h"


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

	for (long i=0;i< xlsx_styles->m_oCellXfs->m_oCount->GetValue(); i++)
	{
		ods_context->styles_context().start_style(L"" ,odf::style_family::TableCell,true); //имя для стиля генерится - поиск и доступ (соответствие) - по номеры oox

		//запишем содержимое стиля
	//	//find_fill_by_id			(xlsx_styles->m_oCellXfs->m_arrItems[i]->m_oFillId); // это ссылка на graphic_properies
	//	//find_num_format_by_id	(xlsx_styles->m_oCellXfs->m_arrItems[i]->m_oNumFmtId); // это ссылка формат ячейки
	//	//find_text_by_id			(xlsx_styles->m_oCellXfs->m_arrItems[i]->m_oFontId); // ссылка на text_properties
	//	
		ods_context->styles_context().end_style();
	}
	
	for (long i=0;i< xlsx_styles->m_oCellStyles->m_oCount->GetValue(); i++)//styles.xml
	{
		std::wstring style_name = string2std_string(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oName.get2());

		ods_context->styles_context().start_style(style_name ,odf::style_family::TableCell);
		int id_xfd = xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oXfId->GetValue();

		xlsx_styles->m_oCellXfs->m_arrItems[id_xfd];//содержимое стиля
		//odf::office_element_ptr & elm = ods_context->styles_context().find_by_id(, odf::style_family::TableCell);
		//ods_context->styles_.back()->add_child(elm);
		

		//ods_context->styles_context().start_style(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oName,odf::style_family::TableCell);
		//generate style:style

		//ods_context->styles_context().current_style().set_cell_style_id();
		////ниже - найти стиль по номеру, вытащить text_properies, ... - автозаполнение//style:table-cell-properties, style:paragraph-properties , ...

		//ods_context->styles_context().current_style().set_hidden(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oHidden->ToBool())
		//ods_context->styles_context().current_style().set_level(xlsx_styles->m_oCellStyles->m_arrItems[i]->m_oILevel->GetValue())
		//
		ods_context->styles_context().end_style();
	}

}



} // namespace Docx2Odt