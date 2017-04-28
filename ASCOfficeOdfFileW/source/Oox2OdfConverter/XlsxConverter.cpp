/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "XlsxConverter.h"
#include "../utils.h"

#include <boost/foreach.hpp>

#include "../OdfFormat/ods_conversion_context.h"

#include "../OdfFormat/odf_text_context.h"
#include "../OdfFormat/odf_drawing_context.h"

#include "../OdfFormat/styles.h"

#include "../OdfFormat/style_table_properties.h"
#include "../OdfFormat/style_text_properties.h"
#include "../OdfFormat/style_paragraph_properties.h"
#include "../OdfFormat/style_graphic_properties.h"

#include "../../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"

using namespace cpdoccore;

namespace Oox2Odf
{
 
XlsxConverter::XlsxConverter(const std::wstring & path, const ProgressCallback* CallBack) 
{
    const OOX::CPath oox_path(std::wstring(path.c_str()));

    xlsx_document   = new OOX::Spreadsheet::CXlsx(oox_path);

	output_document = new odf_writer::package::odf_document(L"spreadsheet");
    ods_context     = new odf_writer::ods_conversion_context(output_document);

    pCallBack = CallBack;

    xlsx_current_container = NULL;

	if (UpdateProgress(400000))return;
}
XlsxConverter::~XlsxConverter() 
{
	if (ods_context)		delete ods_context;		ods_context		= NULL;
	if (xlsx_document)		delete xlsx_document;	xlsx_document	= NULL;
	if (output_document)	delete output_document;	output_document = NULL;
}
void XlsxConverter::write(const std::wstring & path)
{
	if (!output_document)return;
	output_document->write(path);

	delete output_document; output_document = NULL;

	if (UpdateProgress(1000000))return;
}
odf_writer::odf_conversion_context* XlsxConverter::odf_context()
{
	return ods_context;
}
PPTX::Theme* XlsxConverter::oox_theme()
{
	if (xlsx_document)
		return xlsx_document->GetTheme();
	else
		return NULL;
}
OOX::IFileContainer* XlsxConverter::current_document()
{
	if (xlsx_current_container)
		return xlsx_current_container;
	else if (oox_current_child_document)
		return oox_current_child_document;
	else
		return NULL;
}
smart_ptr<OOX::File> XlsxConverter::find_file_by_id(std::wstring sId)
{
	smart_ptr<OOX::File> oFile;
	
	if (xlsx_current_container)
		oFile = xlsx_current_container->Find(sId);
	else if (oox_current_child_document)
		oFile = oox_current_child_document->Find(sId);
		
	return oFile;
}
std::wstring XlsxConverter::find_link_by_id (std::wstring sId, int type)
{
    std::wstring ref;

	if (type == 1)
	{
		if (ref.empty() && xlsx_current_container)
		{
			smart_ptr<OOX::File> oFile = xlsx_current_container->Find(sId);
			if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
		}
        if (ref.empty() && oox_current_child_document)
		{
			smart_ptr<OOX::File> oFile = oox_current_child_document->Find(sId);
			if (oFile.IsInit() && OOX::FileTypes::Image == oFile->type())
			{
				OOX::Image* pImage = (OOX::Image*)oFile.operator->();

				ref = pImage->filename().GetPath();
			}
		}
	}
	return ref;
}


void XlsxConverter::convertDocument()
{
    if (!xlsx_document)     return;
    if (!output_document)   return;
    if (!ods_context)       return;

	ods_context->start_document();

	convert_styles();
	if (UpdateProgress(500000))return;

	convert_sheets();

	if (UpdateProgress(800000))return;
	delete xlsx_document; xlsx_document = NULL;

	ods_context->end_document();

	if (UpdateProgress(850000))return;
}

void XlsxConverter::convert_sheets()
{
	if (!ods_context) return;
	
	const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->GetWorkbook();
	if (!Workbook) return;

	std::map<std::wstring, OOX::Spreadsheet::CWorksheet*> &arrWorksheets = xlsx_document->GetWorksheets();
	
	if(Workbook->m_oBookViews.IsInit())
	{	
		for (unsigned int i = 0; i < Workbook->m_oBookViews->m_arrItems.size(); i++)
		{
			convert(Workbook->m_oBookViews->m_arrItems[i]);
		}
	}
	if(Workbook->m_oSheets.IsInit())
	{				
		for(size_t i = 0, length = Workbook->m_oSheets->m_arrItems.size(); i < length; ++i)
		{
			OOX::Spreadsheet::CSheet* pSheet = Workbook->m_oSheets->m_arrItems[i];
				
			if(pSheet->m_oRid.IsInit())
			{
                std::wstring sSheetRId = pSheet->m_oRid.get2().ToString();
				std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>::iterator pItWorksheet = arrWorksheets.find(sSheetRId);
				
				if (pItWorksheet->second)
				{
					ods_context->start_sheet();
						ods_context->current_table().set_table_name(pSheet->m_oName.get2());
						if (pSheet->m_oState.IsInit() && (	pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleHidden || 
															pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleVeryHidden))
							ods_context->current_table().set_table_hidden(true);
						
						convert(pItWorksheet->second);
					ods_context->end_sheet();	
				}
			}
		}
	}
	if (Workbook->m_oDefinedNames.IsInit())
	{
		for (unsigned int i = 0; i < Workbook->m_oDefinedNames->m_arrItems.size(); i++)
		{
			convert(Workbook->m_oDefinedNames->m_arrItems[i]);
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CDefinedName *oox_defined)
{
	if (oox_defined == NULL) return;

	int sheet_id = -1;
	if (oox_defined->m_oLocalSheetId.IsInit())
		sheet_id = oox_defined->m_oLocalSheetId->GetValue();

	if (oox_defined->m_oName.IsInit() && oox_defined->m_oRef.IsInit())
	{
		std::wstring name = oox_defined->m_oName.get2();

		bool printable = false;
		if (name  == L"_xlnm.Print_Area")printable = true;

		if (false)//если простой - range, составной - выражение
			ods_context->add_defined_range (name, oox_defined->m_oRef.get2(), sheet_id, printable);
		else
			ods_context->add_defined_expression (name, oox_defined->m_oRef.get2(), sheet_id, printable);
	}

}
void XlsxConverter::convert(OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (!oox_sheet) return;

	if (oox_sheet->m_oDimension.IsInit())
	{
		ods_context->set_sheet_dimension(oox_sheet->m_oDimension->m_oRef.get());
	}
	convert(oox_sheet->m_oSheetFormatPr.GetPointer());
	convert(oox_sheet->m_oSheetPr.GetPointer());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Предобработка
	//гиперлинки 
	for (unsigned int hyp = 0; oox_sheet->m_oHyperlinks.IsInit() && hyp < oox_sheet->m_oHyperlinks->m_arrItems.size(); hyp++)
	{
		convert(oox_sheet->m_oHyperlinks->m_arrItems[hyp],oox_sheet);
	}	
	//комментарии
	std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::iterator pos = oox_sheet->m_mapComments.begin();
	while ( oox_sheet->m_mapComments.end() != pos )
	{
		convert(pos->second);
		pos++;
	}
	//todooo для оптимизации - перенести мержи в начало
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//колонки
	ods_context->start_columns();
		for (unsigned int col = 0 ; oox_sheet->m_oCols.IsInit() && col < oox_sheet->m_oCols->m_arrItems.size(); col++)
		{
			convert(oox_sheet->m_oCols->m_arrItems[col]);
		}
	ods_context->end_columns();

	//строки
	if (oox_sheet->m_oSheetData.IsInit() )
	{
		ods_context->start_rows();
			for (unsigned int row = 0 ; row < oox_sheet->m_oSheetData->m_arrItems.size(); row++)
			{
				convert(oox_sheet->m_oSheetData->m_arrItems[row]);
				
				if ( oox_sheet->m_oSheetData->m_arrItems[row] )
					delete oox_sheet->m_oSheetData->m_arrItems[row];
				oox_sheet->m_oSheetData->m_arrItems[row] = NULL;
			}
		ods_context->end_rows();
		oox_sheet->m_oSheetData.reset();
	}

	//мержи
	for (unsigned int mrg = 0 ; oox_sheet->m_oMergeCells.IsInit() && mrg < oox_sheet->m_oMergeCells->m_arrItems.size(); mrg++)
	{
		if (oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.IsInit())
			ods_context->add_merge_cells(oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.get());
	}
	if (oox_sheet->m_oDrawing.IsInit() && oox_sheet->m_oDrawing->m_oId.IsInit())
	{
		smart_ptr<OOX::File> oFile = oox_sheet->Find(oox_sheet->m_oDrawing->m_oId->GetValue());
		if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Drawings == oFile->type())
		{
			OOX::Spreadsheet::CDrawing* pDrawing = (OOX::Spreadsheet::CDrawing*)oFile.operator->();
			
			convert(pDrawing);
		}
	}
	////сортировки
	//convert(oox_sheet->m_oSortState.GetPointer());
	
	//автофильтры
	convert(oox_sheet->m_oAutofilter.GetPointer());

	//условное форматирование
	if (oox_sheet->m_arrConditionalFormatting.size() >0)
	{
		ods_context->start_conditional_formats();
		for (unsigned int fmt =0; fmt < oox_sheet->m_arrConditionalFormatting.size(); fmt++)
		{
			convert(oox_sheet->m_arrConditionalFormatting[fmt]);
		}
		ods_context->end_conditional_formats();
	}
	//выносные части таблицы
	if (oox_sheet->m_oTableParts.IsInit())
	{
		for (unsigned int i=0 ; i < oox_sheet->m_oTableParts->m_arrItems.size(); i++)
		{
			OOX::Spreadsheet::CTablePart *oox_table_part = oox_sheet->m_oTableParts->m_arrItems[i];
			if (!oox_table_part)continue;
			if (!oox_table_part->m_oRId.IsInit())continue;

			smart_ptr<OOX::File> oFile = oox_sheet->Find(oox_table_part->m_oRId->GetValue());
			if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Table == oFile->type())
			{
				OOX::Spreadsheet::CTableFile* pTableFile = (OOX::Spreadsheet::CTableFile*)oFile.operator->();
						
				if ((pTableFile) && (pTableFile->m_oTable.IsInit()))
				{				
					oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(pTableFile);					
						convert(pTableFile->m_oTable.GetPointer());				
					oox_current_child_document = NULL;
				}
			}
		}
	}
/////////////////////////////////////////////////////////////////////////
	convert(oox_sheet->m_oSheetViews.GetPointer());
	convert(oox_sheet->m_oPageSetup.GetPointer());
	convert(oox_sheet->m_oPageMargins.GetPointer());
}

void XlsxConverter::convert(OOX::Spreadsheet::CTable *oox_table_part)
{
	if (!oox_table_part) return;
//nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oHeaderRowCount;
//nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oTotalsRowCount;

//nullable<CSortState > m_oSortState;
//nullable<CTableColumns > m_oTableColumns;

//----------------------------------------------------------------------------------------------------------------
	std::wstring name;
	std::wstring ref;

	if (oox_table_part->m_oDisplayName.IsInit() && oox_table_part->m_oRef.IsInit())
	{
		name	= oox_table_part->m_oDisplayName.get();
		ref		= oox_table_part->m_oRef->GetValue();
	}

	ods_context->start_table_part(name, ref);

	if (oox_table_part->m_oAutoFilter.IsInit())
		ods_context->set_table_part_autofilter(true);

	ods_context->end_table_part();

}

void XlsxConverter::convert(OOX::Spreadsheet::CCommentItem * oox_comment)
{
	if (!oox_comment) return;

	int col = oox_comment->m_nCol.IsInit() ? oox_comment->m_nCol.get()+1 : -1;
	int row = oox_comment->m_nRow.IsInit() ? oox_comment->m_nRow.get()+1 : -1;

	std::wstring author = oox_comment->m_sAuthor.IsInit() ? oox_comment->m_sAuthor.get() : L"";

	ods_context->start_comment(col, row, author);	
	if (oox_comment->m_dLeftMM.IsInit() &&  oox_comment->m_dTopMM.IsInit() && oox_comment->m_dWidthMM.IsInit() && oox_comment->m_dHeightMM.IsInit())
	{
		ods_context->set_comment_rect(oox_comment->m_dLeftMM.get(), oox_comment->m_dTopMM.get(), oox_comment->m_dWidthMM.get(), oox_comment->m_dHeightMM.get());
	}

	if (oox_comment->m_oText.IsInit())
	{
		for(unsigned int i = 0; i < oox_comment->m_oText->m_arrItems.size(); ++i)
		{
			convert(oox_comment->m_oText->m_arrItems[i]);
		}
	}
	ods_context->end_comment();
}

void XlsxConverter::convert(OOX::Spreadsheet::CHyperlink *oox_hyperlink,OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (oox_hyperlink == NULL)return;
	if (oox_sheet == NULL)return;

	std::wstring ref = oox_hyperlink->m_oRef.IsInit() ? oox_hyperlink->m_oRef.get() : L"";
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
	std::wstring display = oox_hyperlink->m_oDisplay.IsInit() ? oox_hyperlink->m_oDisplay.get() : L"";
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

		style_cell_name  = ods_context->styles_context()->find_odf_style_name(xfd_id,odf_types::style_family::TableCell,false,true);
	}else
	{
		//style_cell_name  = ods_context->styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
	}
	ods_context->current_table().set_row_default_cell_style(style_cell_name );
	
	if (oox_row->m_oHt.IsInit())
	{
		double dyDescent = 0.25;
		double height = oox_row->m_oHt->GetValue();
		if (oox_row->m_oDyDescent.IsInit())
		{
			dyDescent = oox_row->m_oDyDescent->GetValue();
		}
		ods_context->current_table().set_row_height(height);
	}
	if ((oox_row->m_oCustomHeight.IsInit() && oox_row->m_oCustomHeight->GetValue() == 1) || 
		(oox_row->m_oCustomFormat.IsInit() && oox_row->m_oCustomFormat->GetValue() == 1 && oox_row->m_oHt.IsInit()) )
	{ 
		ods_context->current_table().set_row_optimal_height(false);
	}else
		ods_context->current_table().set_row_optimal_height(true);

	for (unsigned int cell = 0 ; cell < oox_row->m_arrItems.size();cell++)
	{
		convert(oox_row->m_arrItems[cell]);
	}
	ods_context->end_row();
}
void XlsxConverter::convert(OOX::Spreadsheet::CCell *oox_cell)
{
	if (oox_cell == NULL)return;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oCellMetadata;
				//nullable<SimpleTypes::COnOff<>>						m_oShowPhonetic;
				//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oValueMetadata;

	std::wstring ref = oox_cell->isInitRef() ? oox_cell->getRef() : L"";

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
			convert_sharing_string(_wtoi(oox_cell->m_oValue->m_sText.c_str()));
		}

		else
		{
			if (value_type >=0)		
				ods_context->current_table().set_cell_type	(value_type);
			
			ods_context->current_table().set_cell_value (oox_cell->m_oValue->m_sText);
		}
	}

	if (oox_cell->m_oRichText.IsInit())
	{
		convert(oox_cell->m_oRichText.GetPointer());
	}

	ods_context->end_cell();
}

void XlsxConverter::convert_sharing_string(int number)
{
	if (!ods_context) return;
	
	const OOX::Spreadsheet::CSharedStrings *SharedStrings= xlsx_document->GetSharedStrings();
	if (!SharedStrings) return;

	if (number <0 || (SharedStrings->m_oCount.IsInit() && number > SharedStrings->m_oCount->GetValue()))return;//???? m_oUniqueCount;

	convert(dynamic_cast<OOX::Spreadsheet::CSi*>(SharedStrings->m_arrItems[number]));
}

void XlsxConverter::convert(OOX::Spreadsheet::CSi* oox_rtf_text)
{
	if (oox_rtf_text == NULL) return;
	
	ods_context->start_cell_text();

	for(size_t i = 0; i < oox_rtf_text->m_arrItems.size(); ++i)
	{
		convert(oox_rtf_text->m_arrItems[i]);
	}

	ods_context->end_cell_text();
	ods_context->current_table().set_cell_text( ods_context->text_context());
}
void XlsxConverter::convert(OOX::Spreadsheet::WritingElement  *oox_unknown)
{
	if (oox_unknown == NULL)return;

	switch(oox_unknown->getType())
	{
		case OOX::et_x_r:
		{
			OOX::Spreadsheet::CRun* pRun = dynamic_cast<OOX::Spreadsheet::CRun*>(oox_unknown);
			convert(pRun);
		}break;
		case OOX::et_x_t:
		{
			OOX::Spreadsheet::CText* pText = dynamic_cast<OOX::Spreadsheet::CText*>(oox_unknown);
			convert(pText);
		}break;
		case OOX::et_x_IconSet:
		{
			OOX::Spreadsheet::CIconSet *pIc = dynamic_cast<OOX::Spreadsheet::CIconSet*>(oox_unknown);
			convert(pIc);
		}break;
		case OOX::et_x_DataBar:
		{
			OOX::Spreadsheet::CDataBar *pB = dynamic_cast<OOX::Spreadsheet::CDataBar*>(oox_unknown);
			convert(pB);
		}break;
		case OOX::et_x_FormulaCF:
		{
			OOX::Spreadsheet::CFormulaCF *pF = dynamic_cast<OOX::Spreadsheet::CFormulaCF*>(oox_unknown);
			convert(pF);
		}break;		
		default:
		{
			_CP_LOG << L"[warning] :  no convert element(" << oox_unknown->getType() << L")\n";
		}
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CRun *oox_text_run)
{
	if (oox_text_run == NULL)return;
	
	convert(oox_text_run->m_oRPr.GetPointer());
	ods_context->text_context()->start_span(oox_text_run->m_oRPr.GetPointer() ? true : false);
	
	for(unsigned int i = 0; i < oox_text_run->m_arrItems.size(); ++i)
	{
		convert(oox_text_run->m_arrItems[i]);
	}
	ods_context->text_context()->end_span();
}
void XlsxConverter::convert(OOX::Spreadsheet::CRPr *oox_run_pr)
{
	if (oox_run_pr == NULL)return;

	bool automatic = true;
	bool root = false;

	ods_context->styles_context()->create_style(L"",odf_types::style_family::Text, automatic, root, -1);	
	
	odf_writer::style_text_properties	* text_properties = ods_context->styles_context()->last_state()->get_text_properties();
	if (text_properties == NULL)return;

	if (oox_run_pr->m_oBold.IsInit())
	{
		if (oox_run_pr->m_oBold->m_oVal.ToBool() ==true) 
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
		else
			text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);
	}
	convert(oox_run_pr->m_oColor.GetPointer(),text_properties->content_.fo_color_);

    text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::None);
	if (oox_run_pr->m_oUnderline.IsInit())
	{
		text_properties->content_.style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Solid);
		text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Single);
		
		if (oox_run_pr->m_oUnderline->m_oUnderline.IsInit())
		{
			SimpleTypes::Spreadsheet::EUnderline 	type = oox_run_pr->m_oUnderline->m_oUnderline->GetValue();
			switch(type)
			{
			case SimpleTypes::Spreadsheet::underlineDouble				:
			case SimpleTypes::Spreadsheet::underlineDoubleAccounting	:
					text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);break;
			case SimpleTypes::Spreadsheet::underlineNone				:
					text_properties->content_.style_text_underline_style_	= boost::none;
                    text_properties->content_.style_text_underline_type_	= odf_types::line_type(odf_types::line_type::None);break;
			case SimpleTypes::Spreadsheet::underlineSingle				:
			case SimpleTypes::Spreadsheet::underlineSingleAccounting	:
					text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Single);break;	
			}
		}
		text_properties->content_.style_text_underline_color_ = odf_types::underline_color(odf_types::underline_color::FontColor);
	}
	if (oox_run_pr->m_oItalic.IsInit())
	{
		if (oox_run_pr->m_oItalic->m_oVal.ToBool() ==true)
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
		else
			text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	}
	if (oox_run_pr->m_oSz.IsInit())
	{
		OoxConverter::convert(oox_run_pr->m_oSz->m_oVal->GetValue(), text_properties->content_.fo_font_size_);
	}
	if (oox_run_pr->m_oFamily.IsInit())
	{
	}

	if (oox_run_pr->m_oRFont.IsInit())
	{
		//text_properties->content_.style_font_name_ = oox_run_pr->m_oRFont->m_sVal.get();
		text_properties->content_.fo_font_family_ = oox_run_pr->m_oRFont->m_sVal.get();

	}
	if (oox_run_pr->m_oVertAlign.IsInit() && oox_run_pr->m_oVertAlign->m_oVerticalAlign.IsInit())
	{ 
		switch(oox_run_pr->m_oVertAlign->m_oVerticalAlign->GetValue())
		{
		case SimpleTypes::verticalalignrunSuperscript:
			//text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Super); break;
			text_properties->content_.style_text_position_ = odf_types::text_position(+33.); break;
		case SimpleTypes::verticalalignrunSubscript:
			//text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Sub); break;
			text_properties->content_.style_text_position_ = odf_types::text_position(-33.); break;//по умолчанию 58% - хуже выглядит
		}
	}
	if (oox_run_pr->m_oScheme.IsInit() && oox_run_pr->m_oScheme->m_oFontScheme.IsInit())
	{
		SimpleTypes::Spreadsheet::EFontScheme type = oox_run_pr->m_oScheme->m_oFontScheme->GetValue();
		if (type == SimpleTypes::Spreadsheet::fontschemeMajor)
		{
			text_properties->content_.style_text_scale_ = odf_types::percent(+30.);
		}
		if (type == SimpleTypes::Spreadsheet::fontschemeMinor &&  !text_properties->content_.style_text_position_)
		{
			//text_properties->content_.style_text_position_ = odf_types::text_position(-33.);
			text_properties->content_.style_text_scale_ = odf_types::percent(-30.);
		}
	}
	if ((oox_run_pr->m_oStrike.IsInit()) && (oox_run_pr->m_oStrike->m_oVal.ToBool()))
	{
		text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Single);
	}

			//nullable<CCharset>													m_oCharset;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oCondense;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oExtend;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oOutline;
			//nullable<ComplexTypes::Spreadsheet::COnOff2<SimpleTypes::onoffTrue> >	m_oShadow;

}
void XlsxConverter::convert(OOX::Spreadsheet::CText *oox_text)
{
	if (oox_text == NULL)return;

	ods_context->add_text_content( oox_text->m_sText );
}
void XlsxConverter::convert(OOX::Spreadsheet::CFormula *oox_formula)
{
	if (oox_formula == NULL)return;

	std::wstring formula = oox_formula->m_sText;
	std::wstring ref;

	int ind = -1;

	if (oox_formula->m_oSi.IsInit())	ind = oox_formula->m_oSi->GetValue();
	if (oox_formula->m_oRef.IsInit())	ref = oox_formula->m_oRef.get2();

	if (oox_formula->m_oT.IsInit())
	{
		if (oox_formula->m_oT->GetValue() == SimpleTypes::Spreadsheet::cellformulatypeShared)
		{
			ods_context->current_table().add_or_find_cell_shared_formula(formula,ref, ind);
		}
		else if (oox_formula->m_oT->GetValue() == SimpleTypes::Spreadsheet::cellformulatypeArray)
		{
			ods_context->current_table().set_cell_array_formula(formula,ref);
		}
		else if (oox_formula->m_oT->GetValue() == SimpleTypes::Spreadsheet::cellformulatypeDataTable)
		{
		}
		else 
			ods_context->current_table().set_cell_formula(formula);
	}else 
		ods_context->current_table().set_cell_formula(formula);

				//nullable<SimpleTypes::COnOff<>>							m_oAca;
				//nullable<SimpleTypes::COnOff<>>							m_oBx;
				//nullable<SimpleTypes::COnOff<>>							m_oCa;
				//nullable<SimpleTypes::COnOff<>>							m_oDel1;
				//nullable<SimpleTypes::COnOff<>>							m_oDel2;
				//nullable<SimpleTypes::COnOff<>>							m_oDt2D;
				//nullable<SimpleTypes::COnOff<>>							m_oDtr;

                //nullable<std::wstring>										m_oR1;
                //nullable<std::wstring>										m_oR2;
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
		ods_context->current_table().set_column_optimal_width(false);
	}
	
	if (oox_column->m_oHidden.IsInit() && oox_column->m_oHidden->ToBool()) width = 0;

	if (width <0.01)
	{
		width = 0;
		ods_context->current_table().set_column_hidden(true);
	}
	
	width = ods_context->convert_symbol_width(width);	
	ods_context->current_table().set_column_width(width);

	std::wstring style_cell_name;
	if (oox_column->m_oStyle.IsInit())
	{	
		int xfd_id = oox_column->m_oStyle->GetValue();

		style_cell_name  = ods_context->styles_context()->find_odf_style_name(xfd_id, odf_types::style_family::TableCell, false, true);
	}
	else
	{
		//нужно добавить дефолтный стиль для ячеек ДАННОГО листа
		//???
		style_cell_name= ods_context->styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
	}
	if (style_cell_name.length() > 0)
		ods_context->current_table().set_column_default_cell_style(style_cell_name );
}
void XlsxConverter::convert(OOX::Spreadsheet::CSheetPr *oox_sheet_pr)
{
	if (!oox_sheet_pr)return;
            //nullable<std::wstring>					m_oCodeName;
			//nullable<SimpleTypes::COnOff<>>		m_oEnableFormatConditionsCalculation;
			//nullable<SimpleTypes::COnOff<>>		m_oFilterMode;
			//nullable<SimpleTypes::COnOff<>>		m_oPublished;
			//nullable<SimpleTypes::COnOff<>>		m_oSyncHorizontal;
            //nullable<std::wstring>					m_oSyncRef;
			//nullable<SimpleTypes::COnOff<>>		m_oSyncVertical;
			//nullable<SimpleTypes::COnOff<>>		m_oTransitionEntry;
			//nullable<SimpleTypes::COnOff<>>		m_oTransitionEvaluation;
	if (oox_sheet_pr->m_oTabColor.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(oox_sheet_pr->m_oTabColor.GetPointer(), odf_color);
		ods_context->current_table().set_table_tab_color(odf_color);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CWorkbookView *oox_book_views)
{
	if (!oox_book_views)return;
	const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->GetWorkbook();
	if (!Workbook) return;

	ods_context->settings_context()->start_view();

	if (oox_book_views->m_oActiveTab.IsInit())
	{
		int	table_id	= oox_book_views->m_oActiveTab->GetValue();

		if (table_id >=0 && table_id < Workbook->m_oSheets->m_arrItems.size())
		{
			OOX::Spreadsheet::CSheet* pSheet = Workbook->m_oSheets->m_arrItems[table_id];

			if (pSheet)
			{
				ods_context->settings_context()->add_property(L"ActiveTable", L"string", pSheet->m_oName.get2());
			}
		}
	}
	if (oox_book_views->m_oShowSheetTabs.IsInit())
	{
		ods_context->settings_context()->add_property(L"HasSheetTabs", L"boolean", oox_book_views->m_oShowSheetTabs->ToBool() ? L"true" : L"false");
	}
	if ((oox_book_views->m_oShowHorizontalScroll.IsInit()) && (oox_book_views->m_oShowHorizontalScroll->ToBool()==false))
	{
		ods_context->settings_context()->add_property(L"HorizontalScrollbarWidth", L"int", L"0");
	}
	if ((oox_book_views->m_oShowVerticalScroll.IsInit()) && (oox_book_views->m_oShowVerticalScroll->ToBool()==false))
	{
		ods_context->settings_context()->add_property(L"VerticalScrollbarWidth", L"int",  L"0");
	}
	ods_context->settings_context()->add_property(L"ZoomType", L"short", L"0");
	ods_context->settings_context()->add_property(L"ZoomValue", L"int", L"100");
	
	ods_context->settings_context()->end_view();

	//nullable<SimpleTypes::COnOff<>>						m_oAutoFilterDateGrouping;
	//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oFirstSheet;
	//nullable<SimpleTypes::COnOff<>>						m_oMinimized;
	//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oTabRatio;
	//nullable<SimpleTypes::Spreadsheet::CVisibleType<>>	m_oVisibility;
	//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowHeight;
	//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oWindowWidth;
	//nullable<SimpleTypes::CDecimalNumber<>>				m_oXWindow;
	//nullable<SimpleTypes::CDecimalNumber<>>				m_oYWindow;
}

void XlsxConverter::convert(OOX::Spreadsheet::CSheetViews *oox_sheet_views)
{
	if (!oox_sheet_views)return;

	for (unsigned long i =0; i < oox_sheet_views->m_arrItems.size(); i++)
	{
		OOX::Spreadsheet::CSheetView *sheet_view = oox_sheet_views->m_arrItems[i];
		if (!sheet_view) continue;

		int view_id = sheet_view->m_oWorkbookViewId->GetValue();
		if (view_id < 0) continue;

		ods_context->start_table_view( view_id );
		
		if (sheet_view->m_oRightToLeft.IsInit() && sheet_view->m_oRightToLeft->GetValue()==1)
			ods_context->current_table().set_table_rtl(true);

		if (sheet_view->m_oShowGridLines.IsInit() && sheet_view->m_oShowGridLines->GetValue()==0)
		{
			ods_context->settings_context()->add_property(L"ShowGrid", L"boolean", L"false");
		}
		else
		{
			ods_context->settings_context()->add_property(L"ShowGrid", L"boolean", L"true");
		}

		if (sheet_view->m_oView.IsInit())
		{
			//ods_context->set_settings_table_viewtype(sheet_view->m_oView->GetValue());
		}

		ods_context->settings_context()->add_property(L"ZoomType", L"short", L"0");
		if (sheet_view->m_oZoomScale.IsInit())
		{
            ods_context->settings_context()->add_property(L"ZoomValue", L"int", sheet_view->m_oZoomScale->ToString());
		}else
		{
			ods_context->settings_context()->add_property(L"ZoomValue", L"int", L"100");
		}

		if (sheet_view->m_oColorId.IsInit() && !sheet_view->m_oDefaultGridColor.IsInit())
		{
            ods_context->settings_context()->add_property(L"GridColor", L"int", sheet_view->m_oColorId->ToString());
		}
		bool bPaneX			= false;
		bool bPaneY			= false;
		int ActiveCellX = -1, ActiveCellY = -1;

		for (size_t j = 0; j < sheet_view->m_arrItems.size(); j++)
		{
			OOX::Spreadsheet::CSelection *selection = sheet_view->m_arrItems[j];

			if (selection->m_oActiveCell.IsInit())
			{
				std::wstring ref(selection->m_oActiveCell.get());
				odf_writer::utils::parsing_ref (ref, ActiveCellX, ActiveCellY);

				if (ActiveCellX >= 0 && ActiveCellY >= 0)
				{
					ods_context->settings_context()->add_property(L"CursorPositionX", L"int", boost::lexical_cast<std::wstring>(ActiveCellX));
					ods_context->settings_context()->add_property(L"CursorPositionY", L"int", boost::lexical_cast<std::wstring>(ActiveCellY));
				}
			}
			if (selection->m_oSqref.IsInit())
			{
				//D6:D9 I9:I12 M16:M21 C20:I24
				//в OpenOffice этого нету
			}
			break; // в OpenOffice нет множественного селекта 
		}
		if (sheet_view->m_oPane.IsInit())
		{
			
			if (sheet_view->m_oPane->m_oXSplit.IsInit())
			{
				std::wstring sVal = boost::lexical_cast<std::wstring>((int)sheet_view->m_oPane->m_oXSplit->GetValue());
				
				ods_context->settings_context()->add_property(L"HorizontalSplitMode", L"short", L"2");
				ods_context->settings_context()->add_property(L"HorizontalSplitPosition", L"int",  sVal);
				ods_context->settings_context()->add_property(L"PositionLeft", L"int",  L"0");
				ods_context->settings_context()->add_property(L"PositionRight", L"int",  sVal);
				bPaneX = true;
			}
			if (sheet_view->m_oPane->m_oYSplit.IsInit())
			{
				std::wstring sVal = boost::lexical_cast<std::wstring>((int)sheet_view->m_oPane->m_oYSplit->GetValue());
				ods_context->settings_context()->add_property(L"VerticalSplitMode", L"short", L"2");
				ods_context->settings_context()->add_property(L"VerticalSplitPosition", L"int", sVal);
				ods_context->settings_context()->add_property(L"PositionTop", L"int",  L"0");
				ods_context->settings_context()->add_property(L"PositionBottom", L"int",  sVal);
				bPaneY = true;
			}
		}
		//if (!bPaneX && ActiveCellX >= 0)
		//{
		//	ods_context->settings_context()->add_property(L"HorizontalSplitMode", L"short", L"0");
		//	ods_context->settings_context()->add_property(L"HorizontalSplitPosition", L"int", L"0");
		//	ods_context->settings_context()->add_property(L"PositionLeft", L"int",  L"0");
		//	ods_context->settings_context()->add_property(L"PositionRight", L"int", boost::lexical_cast<std::wstring>(ActiveCellX));
		//}
		//if (!bPaneY && ActiveCellY >= 0)
		//{
		//	ods_context->settings_context()->add_property(L"VerticalSplitMode", L"short", L"0");
		//	ods_context->settings_context()->add_property(L"VerticalSplitPosition", L"int", L"0");
		//	ods_context->settings_context()->add_property(L"PositionTop", L"int",  L"0");
		//	ods_context->settings_context()->add_property(L"PositionBottom", L"int", boost::lexical_cast<std::wstring>(ActiveCellY));
		//}
		if (sheet_view->m_oShowRowColHeaders.IsInit())
		{
			ods_context->settings_context()->add_property(L"HasColumnRowHeaders", L"boolean", sheet_view->m_oShowRowColHeaders->ToBool() ? L"true" : L"false");
		}
		//if (sheet_view->m_oTabSelected.IsInit())
		//{
		//	ods_context->settings_context()->add_property(L"HasSheetTabs", "boolean", sheet_view->m_oTabSelected->ToBool() ? L"true", L"false");
		//}
		//if (sheet_view->m_oShowFormulas.IsInit())
		//{
		//	ods_context->settings_context()->add_property(L"ShowFormulas", "boolean", sheet_view->m_oShowFormulas->ToBool() ? L"true", L"false");
		//}
		if (sheet_view->m_oShowOutlineSymbols.IsInit())
		{
			ods_context->settings_context()->add_property(L"IsOutlineSymbolsSet", L"boolean", sheet_view->m_oShowOutlineSymbols->ToBool() ? L"true" : L"false");
		}
		if (sheet_view->m_oShowZeros.IsInit())
		{
			ods_context->settings_context()->add_property(L"ShowZeroValues", L"boolean", sheet_view->m_oShowZeros->ToBool() ? L"true" : L"false");
		}
		if (sheet_view->m_oZoomScalePageLayoutView.IsInit())
		{
            ods_context->settings_context()->add_property(L"PageViewZoomValue", L"int", sheet_view->m_oZoomScalePageLayoutView->ToString());
		}
		//nullable<SimpleTypes::COnOff<>>						m_oDefaultGridColor;
		//nullable<SimpleTypes::COnOff<>>						m_oShowRuler;
		//nullable<SimpleTypes::COnOff<>>						m_oShowWhiteSpace;
        //nullable<std::wstring>										m_oTopLeftCell;
		//nullable<SimpleTypes::Spreadsheet::CSheetViewType<>>	m_oView;
		//nullable<SimpleTypes::COnOff<>>						m_oWindowProtection;
		//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleNormal;
		//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleSheetLayoutView;		
		ods_context->end_table_view();
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CPageSetup *oox_page)
{
	if (!oox_page) return;

	int type = 1;	
	ods_context->page_layout_context()->set_page_orientation(type);
}

void XlsxConverter::convert(OOX::Spreadsheet::CPageMargins			*oox_page)
{
	if (!oox_page) return;
	_CP_OPT(double) top, left,right,header,footer,bottom;

	ods_context->page_layout_context()->set_page_margin(top,left,bottom, right,header,footer);

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
//Cell default////////////////////////////
	////пока не красиво :(  и главное НЕПРАВИЛЬНО c чегой то раз первый - то и делтный ???? - НЕФИГА - хз чё делать :(
	//odf_writer::odf_style_state_ptr default_cell_style;
	//ods_context->styles_context()->find_odf_style_state(0,odf_types::style_family::TableCell, default_cell_style, true);
	//if (default_cell_style)	ods_context->styles_context()->add_default(default_cell_style);

	odf_writer::odf_style_state_ptr default_cell_style;
	
	ods_context->styles_context()->find_odf_style_state (0, odf_types::style_family::TableCell, default_cell_style, true, false); 
	if (default_cell_style)	
		ods_context->styles_context()->add_default( default_cell_style );

///Column///////////////////////////////////////////////////////////////////////////////////////
	ods_context->styles_context()->create_style(L"", odf_types::style_family::TableColumn, true, false, -1);		
	{	
		double width = 8.1; // из приложния MS Office 2010
		//в xlsx необязательно задавать ширину (колонок) - дефолтное по приложению. в oo - обязательно
		odf_writer::style* style = dynamic_cast<odf_writer::style*>(ods_context->styles_context()->last_state()->get_office_element().get());
		if (style)
		{
			odf_writer::style_table_column_properties * column_properties = style->content_.get_style_table_column_properties();
			if (column_properties)
			{		
				column_properties->style_table_column_properties_attlist_.common_break_attlist_.fo_break_before_ = odf_types::fo_break(odf_types::fo_break::Auto);
				if (oox_sheet_format_pr->m_oDefaultColWidth.IsInit())
				{			
					width =  oox_sheet_format_pr->m_oDefaultColWidth->GetValue();
				}
				width = ods_context->convert_symbol_width(width);
				ods_context->current_table().defaut_column_width_ = width;//pt
				column_properties->style_table_column_properties_attlist_.style_column_width_ = odf_types::length(odf_types::length(width,odf_types::length::pt).get_value_unit(odf_types::length::cm),odf_types::length::cm);
			}
		}
		ods_context->styles_context()->add_default( ods_context->styles_context()->last_state() );
	}
//Row default //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	{
		ods_context->styles_context()->create_style(L"",odf_types::style_family::TableRow, true, false, -1);	
		odf_writer::style* style = dynamic_cast<odf_writer::style*>(ods_context->styles_context()->last_state()->get_office_element().get());
		if (style)
		{		
			odf_writer::style_table_row_properties * row_properties = style->content_.get_style_table_row_properties();
			if (row_properties)
			{
				if (oox_sheet_format_pr->m_oDefaultRowHeight.IsInit())
				{
					double height = oox_sheet_format_pr->m_oDefaultRowHeight->GetValue();

					ods_context->current_table().defaut_row_height_ = height;//pt
					row_properties->style_table_row_properties_attlist_.style_row_height_ = odf_types::length(odf_types::length(height,odf_types::length::pt).get_value_unit(odf_types::length::cm),odf_types::length::cm);
				}
				row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; //???? не знаю cтоит ли 
				row_properties->style_table_row_properties_attlist_.common_break_attlist_.fo_break_before_ = odf_types::fo_break(odf_types::fo_break::Auto);
			}
		}
		ods_context->styles_context()->add_default( ods_context->styles_context()->last_state() );
	}
}
void XlsxConverter::convert_styles()
{
	if (!ods_context) return;

///////////// default styles
	ods_context->styles_context()->create_default_style(odf_types::style_family::TableCell);

///////////////////////////////////////////////////////////////////////////////////////////////////
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
	if (!xlsx_styles)return;
//todooo ?? стоит ли обращать на параметр Count ??
////////////форматы данных
	for (unsigned int i = 0; xlsx_styles->m_oNumFmts.IsInit() && i < xlsx_styles->m_oNumFmts->m_arrItems.size(); i++)
	{
		convert(xlsx_styles->m_oNumFmts->m_arrItems[i]);
	}
/////////////стили ячеек
	for (unsigned int i = 0; xlsx_styles->m_oCellStyleXfs.IsInit() && i < xlsx_styles->m_oCellStyleXfs->m_arrItems.size(); i++)
	{
		//automatical, root - noname - они тока для named
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, true, true);
	}
	for (unsigned int i = 0; xlsx_styles->m_oCellStyles.IsInit() && i < xlsx_styles->m_oCellStyles->m_arrItems.size(); i++)//styles.xml
	{
		//non automatical, root - named 
		convert(xlsx_styles->m_oCellStyles->m_arrItems[i]); 
	}	
	
	//кастомные стили ячеек
	for (unsigned int i = 0; xlsx_styles->m_oCellXfs.IsInit() && i < xlsx_styles->m_oCellXfs->m_arrItems.size(); i++)
	{
		//automatical, non root
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i], i, true, false);
	}	

////////////стили условного форматирования 
	for (unsigned int i=0; xlsx_styles->m_oDxfs.IsInit() && i < xlsx_styles->m_oDxfs->m_arrItems.size(); i++)
	{
		convert(xlsx_styles->m_oDxfs->m_arrItems[i], i); 
	}
}



void XlsxConverter::convert(OOX::Spreadsheet::CFont * font, odf_writer::style_text_properties * text_properties)
{
	if (font == NULL)return;
 	if (text_properties == NULL)return;

	std::wstring	font_name;
	double			font_size = 0;
	bool			font_bold = false;
	bool			font_italic = false;

	if (font->m_oBold.IsInit() && (font->m_oBold->m_oVal.ToBool() ==true) )font_bold = true;

	if (font_bold)text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold);
	else 		  text_properties->content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WNormal);	
	
	convert(font->m_oColor.GetPointer(),text_properties->content_.fo_color_);

	if (font->m_oUnderline.IsInit() )
	{
		text_properties->content_.style_text_underline_style_ = odf_types::line_style(odf_types::line_style::Solid);
		text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Single);
		
		if (font->m_oUnderline->m_oUnderline.IsInit())
		{
			SimpleTypes::Spreadsheet::EUnderline 	type = font->m_oUnderline->m_oUnderline->GetValue();
			switch(type)
			{
			case SimpleTypes::Spreadsheet::underlineDouble				:
			case SimpleTypes::Spreadsheet::underlineDoubleAccounting	:
					text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Double);break;
			case SimpleTypes::Spreadsheet::underlineNone				:
					text_properties->content_.style_text_underline_style_ = boost::none;
                    text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::None);break;
			case SimpleTypes::Spreadsheet::underlineSingle				:
			case SimpleTypes::Spreadsheet::underlineSingleAccounting	:
					text_properties->content_.style_text_underline_type_= odf_types::line_type(odf_types::line_type::Single);break;		
			}
		}
	}
	if (font->m_oItalic.IsInit() && (font->m_oItalic->m_oVal.ToBool() ==true))font_italic = true;

	if (font_italic) text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
	else			 text_properties->content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Normal);
	
	if (font->m_oSz.IsInit())
	{
		font_size = font->m_oSz->m_oVal->GetValue();
		OoxConverter::convert(font_size, text_properties->content_.fo_font_size_);

		text_properties->content_.style_font_size_asian_= text_properties->content_.fo_font_size_;
		text_properties->content_.style_font_size_complex_ = text_properties->content_.fo_font_size_;
	}
	if (font->m_oFamily.IsInit())
	{
	}

	if (font->m_oRFont.IsInit())
	{
		font_name = font->m_oRFont->m_sVal.get();
		text_properties->content_.fo_font_family_ = font_name;
		text_properties->content_.style_font_family_asian_ = font_name;
		text_properties->content_.style_font_family_complex_ = font_name;
	}
	if (font->m_oVertAlign.IsInit() && font->m_oVertAlign->m_oVerticalAlign.IsInit())
	{ 
		switch(font->m_oVertAlign->m_oVerticalAlign->GetValue())
		{
		case SimpleTypes::verticalalignrunSuperscript:
			//text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Super); break;
			text_properties->content_.style_text_position_ = odf_types::text_position(+33.); break;
		case SimpleTypes::verticalalignrunSubscript:
			//text_properties->content_.style_text_position_ = odf_types::text_position(odf_types::text_position::Sub); break;
			text_properties->content_.style_text_position_ = odf_types::text_position(-33.); break;//по умолчанию 58% - хуже выглядит
		}
	}
	if (font->m_oScheme.IsInit() && font->m_oScheme->m_oFontScheme.IsInit())
	{
		SimpleTypes::Spreadsheet::EFontScheme type = font->m_oScheme->m_oFontScheme->GetValue();
		if (type == SimpleTypes::Spreadsheet::fontschemeMajor)
		{
			text_properties->content_.style_text_scale_ = odf_types::percent(+30.);
		}
		if (type == SimpleTypes::Spreadsheet::fontschemeMinor && !text_properties->content_.style_text_position_)
		{
			text_properties->content_.style_text_scale_ = odf_types::percent(-30.);
		}
	}
	if ((font->m_oStrike.IsInit()) && (font->m_oStrike->m_oVal.ToBool()))
		text_properties->content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Single);

	ods_context->calculate_font_metrix(font_name,font_size,font_italic,font_bold);
}

void XlsxConverter::convert(double oox_size,  _CP_OPT(odf_types::length) & odf_size)
{
	//нужно сделать преобразования типов oox_size
	//???
	odf_size = odf_types::length(oox_size, odf_types::length::pt);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFill * fill, odf_writer::style_table_cell_properties * cell_properties)
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
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf_types::background_color) & odf_bckgrd_color)
{
	if (!color)return;

	_CP_OPT(odf_types::color) odf_color;
	convert(color, odf_color);

	if (odf_color)odf_bckgrd_color = odf_types::background_color(odf_color.get());
}
void XlsxConverter::convert(OOX::Spreadsheet::CNumFmt *numFmt)
{
	if (!numFmt)return;

	if (numFmt->m_oFormatCode.IsInit() && numFmt->m_oNumFmtId.IsInit())
	{
		ods_context->numbers_styles_context()->add_or_find(numFmt->m_oNumFmtId->GetValue(), numFmt->m_oFormatCode.get());
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CAligment *aligment, odf_writer::style_paragraph_properties	* paragraph_properties
																 , odf_writer::style_table_cell_properties * cell_properties)
{
	if (!aligment)return;

	bool rtl = false;
	if (aligment->m_oReadingOrder.IsInit() && (aligment->m_oReadingOrder->GetValue() == 1))
	{
		paragraph_properties->content_.style_writing_mode_= odf_types::writing_mode(odf_types::writing_mode::RlTb);
		rtl = true;
	}
	if (aligment->m_oVertical.IsInit())
	{
		switch(aligment->m_oVertical->GetValue())
		{
		case SimpleTypes::Spreadsheet::verticalalignmentBottom: paragraph_properties->content_.style_vertical_align_ = 
												odf_types::vertical_align(odf_types::vertical_align::Bottom); break;
		case SimpleTypes::Spreadsheet::verticalalignmentCenter: paragraph_properties->content_.style_vertical_align_ = 
												odf_types::vertical_align(odf_types::vertical_align::Middle); break;
		case SimpleTypes::Spreadsheet::verticalalignmentDistributed: paragraph_properties->content_.style_vertical_align_ = 
												odf_types::vertical_align(odf_types::vertical_align::Auto); break;
		case SimpleTypes::Spreadsheet::verticalalignmentJustify: paragraph_properties->content_.style_vertical_align_ = 
												odf_types::vertical_align(odf_types::vertical_align::Justify); break;
		case SimpleTypes::Spreadsheet::verticalalignmentTop: paragraph_properties->content_.style_vertical_align_ = 
												odf_types::vertical_align(odf_types::vertical_align::Top); break;

		}
		if (paragraph_properties->content_.style_vertical_align_)
			cell_properties->style_table_cell_properties_attlist_.style_vertical_align_ = paragraph_properties->content_.style_vertical_align_;
	}
	if (aligment->m_oTextRotation.IsInit())
	{
		int rot = aligment->m_oTextRotation->GetValue();
		if (rot <=180 && rot >= 0 ) 
		{
			cell_properties->style_table_cell_properties_attlist_.common_rotation_angle_attlist_.style_rotation_angle_ = rot;
			cell_properties->style_table_cell_properties_attlist_.style_rotation_align_= odf_types::rotation_align(odf_types::rotation_align::Bottom);
		}
		else if (rot == 0xff)//вертикальный текст
			cell_properties->style_table_cell_properties_attlist_.style_direction_ = odf_types::direction(odf_types::direction::Ttb);

	}
	if(aligment->m_oHorizontal.IsInit())
	{
		switch(aligment->m_oHorizontal->GetValue())
		{
		case SimpleTypes::Spreadsheet::horizontalalignmentCenter:	
			paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Center); break;
		case SimpleTypes::Spreadsheet::horizontalalignmentFill:	
			paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Start); break;
		case SimpleTypes::Spreadsheet::horizontalalignmentJustify:	
			paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify); break;
		case SimpleTypes::Spreadsheet::horizontalalignmentRight:	
			paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::End); break;
		
		case SimpleTypes::Spreadsheet::horizontalalignmentLeft:	
		default:
			paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Start); break;		
		}
		
		cell_properties->style_table_cell_properties_attlist_.style_text_align_source_ = odf_types::text_align_source(odf_types::text_align_source::Fix);
	}

	if (aligment->m_oWrapText.IsInit())
	{
		if (aligment->m_oWrapText->GetValue()) 
			cell_properties->style_table_cell_properties_attlist_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::Wrap);
	}
	if (aligment->m_oShrinkToFit.IsInit())
	{
		cell_properties->style_table_cell_properties_attlist_.style_shrink_to_fit_ = aligment->m_oShrinkToFit->ToBool();
	}
	if (aligment->m_oIndent.IsInit())
	{
	}
		//nullable<SimpleTypes::COnOff<>>									m_oJustifyLastLine;
		//nullable<SimpleTypes::CDecimalNumber<>>							m_oRelativeIndent;

}
void XlsxConverter::convert(OOX::Spreadsheet::CBorder *oox_border, odf_writer::style_table_cell_properties * table_cell_properties)
{
	if (!oox_border)return;

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
		if (bottom.length() >0 )table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (top.length() >0 )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_top_		= top;
		if (left.length() >0 )	table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_left_		= left;
		if (right.length() >0 ) table_cell_properties->style_table_cell_properties_attlist_.common_border_attlist_.fo_border_right_		= right;
	}

	convert(oox_border->m_oDiagonal.GetPointer(), other);
	
	if (oox_border->m_oDiagonalDown.IsInit() && other.length()>0) //and true???
		table_cell_properties->style_table_cell_properties_attlist_.style_diagonal_tl_br_= other;
	if (oox_border->m_oDiagonalUp.IsInit() && other.length()>0) //and true???
		table_cell_properties->style_table_cell_properties_attlist_.style_diagonal_bl_tr_= other;
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
		_CP_OPT(odf_types::color) odf_color;
		convert(borderProp->m_oColor.GetPointer(), odf_color);
		if (odf_color)
			border_color = L" #" + odf_color->get_hex_value();
	}
	if (border_color.length()<1)border_color = L" #000000";
	
	odf_border_prop = border_style + border_color;
}
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf_types::color) & odf_color)//стоит ли сюда тащить odf_writer type???
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
		std::wstring sColor = color->m_oThemeColor->ToString();

		DWORD argb = 0;
		result = OoxConverter::convert(sColor, argb) ; 
		
		ucR = (argb & 0x0000FF); 
		ucB = (argb & 0x00FF00)	>> 8; 
		ucG = (argb & 0xFF0000)	>> 16; 
		ucA = argb >> 24; 
	}
	if(color->m_oIndexed.IsInit())
	{
		OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();
	
		int ind = color->m_oIndexed->GetValue();

		if(xlsx_styles->m_oColors.IsInit() && xlsx_styles->m_oColors->m_oIndexedColors.IsInit() && 
			ind < xlsx_styles->m_oColors->m_oIndexedColors->m_arrItems.size())
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
			std::wstring sRgbColor = oRgbColor->ToString();
			odf_color = odf_types::color(std::wstring(L"#") + sRgbColor.substr(sRgbColor.length() - 6, 6));
			delete oRgbColor;
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CCellStyle * cell_style)
{
	if (cell_style == NULL)return;
	
	bool automatic = false;
	bool root = true;
	int parent_id = cell_style->m_oXfId.IsInit() ? cell_style->m_oXfId->GetValue() : -1;
	int oox_id = parent_id; //

	std::wstring style_name = L"";
	if (cell_style->m_oName.IsInit())style_name = cell_style->m_oName.get();

	ods_context->styles_context()->create_style(style_name,odf_types::style_family::TableCell, automatic, root, oox_id);

	odf_writer::odf_style_state_ptr style_state = ods_context->styles_context()->last_state(odf_types::style_family::TableCell);

	if(!style_state)return; 
	if (style_name.length() > 0)style_state->set_display_name(style_name);

	//if (parent_id >=0) 
	//	ods_context->styles_context()->last_state()->set_parent_style_name(ods_context->styles_context()->find_odf_style_name(parent_id, odf_types::style_family::TableCell,true, true));
	
	//фишечка либры и оо - они НЕ ЧИТАЮТ автоматические стили для стилей
	//придется копировать свойства все .. они автоматические стили удалить (не сохранять в файл)

	odf_writer::odf_style_state_ptr parent_style_state;
	if (ods_context->styles_context()->find_odf_style_state(parent_id, odf_types::style_family::TableCell, parent_style_state, true, true) && parent_style_state)
	{
		//parent_style_state->set_dont_write(true);

		odf_writer::style_text_properties		*text_props	= style_state->get_text_properties();
		odf_writer::style_paragraph_properties	*para_props	= style_state->get_paragraph_properties() ;
		odf_writer::style_table_cell_properties	*cell_props	= style_state->get_table_cell_properties();

		odf_writer::style_text_properties		*parent_text_props	= parent_style_state->get_text_properties();
		odf_writer::style_paragraph_properties	*parent_para_props	= parent_style_state->get_paragraph_properties() ;
		odf_writer::style_table_cell_properties	*parent_cell_props	= parent_style_state->get_table_cell_properties();

		if (text_props) text_props->apply_from(parent_text_props);
		if (para_props) para_props->apply_from(parent_para_props);
		if (cell_props) cell_props->apply_from(parent_cell_props);
	}

}
void XlsxConverter::convert(OOX::Spreadsheet::CDxf *dxFmt, int oox_dx_id)
{
	if (!dxFmt)return;
	
	ods_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, false, true, oox_dx_id); 
	ods_context->styles_context()->last_state()->set_conditional(true);

	odf_writer::style_text_properties		* text_properties		= ods_context->styles_context()->last_state()->get_text_properties();
	odf_writer::style_table_cell_properties	* table_cell_properties = ods_context->styles_context()->last_state()->get_table_cell_properties();

	convert(dxFmt->m_oBorder.GetPointer()	, table_cell_properties); 
	convert(dxFmt->m_oFill.GetPointer()		, table_cell_properties); 
	convert(dxFmt->m_oFont.GetPointer()		, text_properties); 
	
	if (dxFmt->m_oNumFmt.IsInit())	
	{
		int numFmt_id =  dxFmt->m_oNumFmt->m_oNumFmtId.IsInit() ? dxFmt->m_oNumFmt->m_oNumFmtId->GetValue() : -1;
		if (numFmt_id >=0)
		{
			ods_context->styles_context()->last_state()->set_data_style_name(ods_context->numbers_styles_context()->add_or_find(numFmt_id).style_name);
			ods_context->styles_context()->last_state()->set_number_format(numFmt_id);
		}
	}
	if (dxFmt->m_oAlignment.IsInit())
	{
		odf_writer::style_paragraph_properties	* paragraph_properties = ods_context->styles_context()->last_state()->get_paragraph_properties();
		paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::Page);		
		
		convert(dxFmt->m_oAlignment.GetPointer(), paragraph_properties, table_cell_properties); 
	}
	//convert(dxFmt->m_oProtection.GetPointer(), table_cell_properties); 
}
void XlsxConverter::convert(OOX::Spreadsheet::CXfs * xfc_style, int oox_id, bool automatic, bool root)
{
	OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->GetStyles();

	int id_parent	= xfc_style->m_oXfId.IsInit()		? xfc_style->m_oXfId->GetValue()	: -1; 
	int fill_id		= xfc_style->m_oFillId.IsInit()		? xfc_style->m_oFillId->GetValue()	: -1;
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): -1; 
	int font_id		= xfc_style->m_oFontId.IsInit()		? xfc_style->m_oFontId->GetValue()	: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, automatic, root, oox_id); 
	
	odf_writer::style_text_properties		* text_properties		= ods_context->styles_context()->last_state()->get_text_properties();
	odf_writer::style_table_cell_properties	* table_cell_properties = ods_context->styles_context()->last_state()->get_table_cell_properties();

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
		ods_context->styles_context()->last_state()->set_data_style_name(ods_context->numbers_styles_context()->add_or_find(numFmt_id).style_name);
		ods_context->styles_context()->last_state()->set_number_format(numFmt_id);
	}
	if (xlsx_styles->m_oBorders.IsInit() && border_id >=0 && (id_parent < 0 || xfc_style->m_oApplyBorder.IsInit()))	
	{
		convert(xlsx_styles->m_oBorders->m_arrItems[border_id], table_cell_properties); 
	}
	
	odf_writer::style_paragraph_properties	* paragraph_properties = ods_context->styles_context()->last_state()->get_paragraph_properties();
	paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::Page);

	if (xfc_style->m_oAligment.IsInit() && xfc_style->m_oApplyAlignment.IsInit())
	{
		convert(xfc_style->m_oAligment.GetPointer(), paragraph_properties, table_cell_properties);
	}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (id_parent >=0)
	{
		odf_writer::odf_style_state_ptr parent_style_state;
	
		if (ods_context->styles_context()->find_odf_style_state(id_parent, odf_types::style_family::TableCell,parent_style_state, true, false) 
			 && parent_style_state) // сначала в поименованных
		{
			ods_context->styles_context()->last_state()->set_parent_style_name(parent_style_state->get_name());

			if (numFmt_id < 0)
			{
				ods_context->styles_context()->last_state()->set_number_format(parent_style_state->get_number_format());
			}
		}
		else
		{
			if (ods_context->styles_context()->find_odf_style_state(id_parent, odf_types::style_family::TableCell,parent_style_state, true, true) 
				 && parent_style_state)
			{
				ods_context->styles_context()->last_state()->set_parent_style_name(parent_style_state->get_name());

				if (numFmt_id < 0)
				{
					ods_context->styles_context()->last_state()->set_number_format(parent_style_state->get_number_format());
				}
			}
		}
	}
	
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XlsxConverter::convert(OOX::Spreadsheet::CCellAnchor *oox_anchor)
{
	if (!oox_anchor) return;

	if (oox_anchor->m_bShapeOle) return; 
////////////////// 
	if (oox_anchor->m_oFrom.IsInit() || oox_anchor->m_oTo.IsInit())
	{
		oox_table_position from={}, to={};
		
		convert(oox_anchor->m_oFrom.GetPointer(),	&from);	
		convert(oox_anchor->m_oTo.GetPointer(),		&to);

		double x1=0, y1=0, x2=0, y2=0;
		ods_context->current_table().convert_position(from, x1, y1);
		ods_context->current_table().convert_position(to,	x2, y2);
		
		ods_context->drawing_context()->set_drawings_rect(x1, y1, x2 - x1, y2 - y1);
	}
		
//////////////
	if (oox_anchor->m_oPos.IsInit())
	{
	}

	if (oox_anchor->m_oExt.IsInit())
	{
	}
	odf_context()->drawing_context()->start_drawing();
		OoxConverter::convert(oox_anchor->m_oElement.GetPointer());
	odf_context()->drawing_context()->end_drawing();
}

void XlsxConverter::convert(OOX::Spreadsheet::CDrawing *oox_drawing)
{
	if (!oox_drawing)return;
	
	xlsx_current_container = dynamic_cast<OOX::IFileContainer*>(oox_drawing);

	for (unsigned int dr = 0 ; dr < oox_drawing->m_arrItems.size(); dr++)
	{
		ods_context->start_drawings();
			convert(oox_drawing->m_arrItems[dr]);
		ods_context->end_drawings();
	}

	xlsx_current_container = NULL;
}


void XlsxConverter::convert(OOX::Spreadsheet::CFromTo* oox_from_to, oox_table_position * pos)
{
	if (!oox_from_to || !pos)return;

	if (oox_from_to->m_oCol.IsInit()) pos->col = oox_from_to->m_oCol->GetValue();
	if (oox_from_to->m_oRow.IsInit()) pos->row = oox_from_to->m_oRow->GetValue();
	if (oox_from_to->m_oRowOff.IsInit()) pos->row_off = oox_from_to->m_oRowOff->GetValue();//pt
	if (oox_from_to->m_oColOff.IsInit()) pos->col_off = oox_from_to->m_oColOff->GetValue();//pt
}

void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormatting *oox_cond_fmt)
{
	if (!oox_cond_fmt)return;

	if (oox_cond_fmt->m_oSqRef.IsInit())	
	{
		ods_context->current_table().start_conditional_format(oox_cond_fmt->m_oSqRef.get());

		for (unsigned int i=0; i< oox_cond_fmt->m_arrItems.size(); i++)
			convert(oox_cond_fmt->m_arrItems[i]);//rule
		
		ods_context->current_table().end_conditional_format();
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormattingRule *oox_cond_rule)
{
	if (!oox_cond_rule)return;

	if (oox_cond_rule->m_oType.IsInit())
	{
		ods_context->current_table().start_conditional_rule(oox_cond_rule->m_oType->GetValue());
		{
			if (oox_cond_rule->m_oDxfId.IsInit()) 
			{
				std::wstring odf_style_name = odf_context()->styles_context()->find_conditional_style_name(oox_cond_rule->m_oDxfId->GetValue(), odf_types::style_family::TableCell);

				ods_context->current_table().set_conditional_style_name(odf_style_name);
			}
			if (oox_cond_rule->m_oOperator.IsInit()) 
				ods_context->current_table().set_conditional_operator(oox_cond_rule->m_oOperator->GetValue());
			
				for (unsigned int i=0; i< oox_cond_rule->m_arrItems.size(); i++)
					convert(oox_cond_rule->m_arrItems[i]);
		}	
		ods_context->current_table().end_conditional_rule();
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CDataBar *oox_cond_databar)
{
	if (!oox_cond_databar)return;
	
	_CP_OPT(odf_types::color) color;
	convert(oox_cond_databar->m_oColor.GetPointer(), color);

	ods_context->current_table().set_conditional_databar_color(color);
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			//nullable<SimpleTypes::COnOff<>>					m_oShowValue;
	for (unsigned int i=0; i< oox_cond_databar->m_arrItems.size(); i++)
		convert(oox_cond_databar->m_arrItems[i]);
}
void XlsxConverter::convert(OOX::Spreadsheet::CColorScale *oox_cond_colorscale)
{
	if (!oox_cond_colorscale)return;
	
	for (unsigned int i=0; i< oox_cond_colorscale->m_arrItems.size(); i++)
	{
		if (!oox_cond_colorscale->m_arrItems[i])continue;
	
		OOX::EElementType type = oox_cond_colorscale->m_arrItems[i]->getType();
		if (type == OOX::et_x_ConditionalFormatValueObject)
		{
			convert(oox_cond_colorscale->m_arrItems[i]);
		}
		else
		{
			_CP_OPT(odf_types::color) color;
			convert(dynamic_cast<OOX::Spreadsheet::CColor*>(oox_cond_colorscale->m_arrItems[i]),color);
			ods_context->current_table().add_conditional_colorscale(color);
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CIconSet *oox_cond_iconset)
{
	if (!oox_cond_iconset)return;
	
	if (oox_cond_iconset->m_oIconSet.IsInit())
		ods_context->current_table().set_conditional_iconset(oox_cond_iconset->m_oIconSet->GetValue());
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			//nullable<SimpleTypes::COnOff<>>					m_oShowValue;
	for (unsigned int i=0; i< oox_cond_iconset->m_arrItems.size(); i++)
		convert(oox_cond_iconset->m_arrItems[i]);
}
void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormatValueObject *oox_cond_value)
{
	if (!oox_cond_value)return;

	std::wstring val;
	int type = 3;
	if (oox_cond_value->m_oVal.IsInit())	val = oox_cond_value->m_oVal.get2();
	if (oox_cond_value->m_oType.IsInit())	type = oox_cond_value->m_oType->GetValue();
	
	ods_context->current_table().set_conditional_value(type,val);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFormulaCF	*oox_cond_formula)
{
	if (!oox_cond_formula)return;
	ods_context->current_table().set_conditional_formula(oox_cond_formula->m_sText);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFilterColumn *oox_filter_column)
{
	//nullable<SimpleTypes::CUnsignedDecimalNumber<> > m_oColId;
	//nullable<SimpleTypes::COnOff<> > m_oHiddenButton;
	//nullable<SimpleTypes::COnOff<> > m_oShowButton;

	//nullable<CColorFilter > m_oColorFilter;
	//nullable<CDynamicFilter > m_oDynamicFilter;
	//nullable<CCustomFilters > m_oCustomFilters;
	//nullable<CFilters > m_oFilters;
	//nullable<CTop10 > m_oTop10;
}
void XlsxConverter::convert(OOX::Spreadsheet::CAutofilter *oox_filter)
{
	if (!oox_filter)return;
	
	std::wstring ref, sort_ref;	
	bool sort = false, caseSensitive = false;
	if (oox_filter->m_oRef.IsInit())
		ref = oox_filter->m_oRef->GetValue();
	if (oox_filter->m_oSortState.IsInit())
	{
		sort = true;
		if (oox_filter->m_oSortState->m_oRef.IsInit())
			sort_ref = 	oox_filter->m_oSortState->m_oRef->GetValue();
		if (oox_filter->m_oSortState->m_oCaseSensitive.IsInit() && oox_filter->m_oSortState->m_oCaseSensitive->GetValue()==1)
			caseSensitive = true;
	}
	ods_context->add_autofilter(ref);//target
	
	//for (unsigned int i=0; i < oox_filter->m_arrItems.size(); i++)//set items todooo
	//{
	//}
}

} // namespace Docx2Odt
