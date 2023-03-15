/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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
#include "../../../OOXML/XlsxFormat/Xlsx.h"
#include "../../../OOXML/XlsxFormat/XlsxFlat.h"
#include "../../../OOXML/XlsxFormat/Workbook/Workbook.h"
#include "../../../OOXML/XlsxFormat/Worksheets/Worksheet.h"
#include "../../../OOXML/XlsxFormat/Comments/Comments.h"
#include "../../../OOXML/XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../OOXML/XlsxFormat/Styles/Styles.h"
#include "../../../OOXML/XlsxFormat/CalcChain/CalcChain.h"
#include "../../../OOXML/XlsxFormat/ExternalLinks/ExternalLinks.h"
#include "../../../OOXML/XlsxFormat/ExternalLinks/ExternalLinkPath.h"
#include "../../../OOXML/XlsxFormat/Pivot/PivotTable.h"
#include "../../../OOXML/XlsxFormat/Pivot/PivotCacheDefinition.h"
#include "../../../OOXML/XlsxFormat/Pivot/PivotCacheRecords.h"

#include "../../../OOXML/DocxFormat/VmlDrawing.h"
#include "../../../OOXML/DocxFormat/Media/ActiveX.h"

#include "../Format/ods_conversion_context.h"

#include "../Format/odf_text_context.h"
#include "../Format/odf_drawing_context.h"

#include "../Format/styles.h"

#include "../Format/style_table_properties.h"
#include "../Format/style_text_properties.h"
#include "../Format/style_paragraph_properties.h"
#include "../Format/style_graphic_properties.h"
#include "../Format/style_page_layout_properties.h"

#include "../../Common/utils.h"

using namespace cpdoccore;

namespace Oox2Odf
{

XlsxConverter::XlsxConverter(const std::wstring & path, bool bTemplate) 
{
    const OOX::CPath oox_path(std::wstring(path.c_str()));

	xlsx_flat_document = NULL;
    xlsx_document = new OOX::Spreadsheet::CXlsx(oox_path);

	if (xlsx_document && xlsx_document->m_arWorksheets.empty() && !xlsx_document->m_pWorkbook)
	{
		delete xlsx_document; xlsx_document = NULL;
		xlsx_flat_document = new OOX::Spreadsheet::CXlsxFlat(oox_path);
	}
	if (xlsx_flat_document && xlsx_flat_document->m_arWorksheets.empty()/* && !xlsx_flat_document->m_pWorkbook*/)
	{
		delete xlsx_flat_document; xlsx_flat_document = NULL;
	}
	output_document = new odf_writer::package::odf_document(L"spreadsheet", bTemplate);
    ods_context     = new odf_writer::ods_conversion_context(output_document);

    xlsx_current_container = NULL;
}
XlsxConverter::~XlsxConverter() 
{
	if (ods_context)		delete ods_context;		ods_context		= NULL;
	if (xlsx_document)		delete xlsx_document;	xlsx_document	= NULL;
	if (xlsx_flat_document)	delete xlsx_flat_document;	xlsx_flat_document	= NULL;
	if (output_document)	delete output_document;	output_document = NULL;
}
odf_writer::odf_conversion_context* XlsxConverter::odf_context()
{
	return ods_context;
}
PPTX::Theme* XlsxConverter::oox_theme()
{
	if (xlsx_document)
		return xlsx_document->m_pTheme.GetPointer();
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
smart_ptr<OOX::File> XlsxConverter::find_file_by_id(const std::wstring & sId)
{
	smart_ptr<OOX::File> oFile;
	
	if (oox_current_child_document)
		oFile = oox_current_child_document->Find(sId);
	else if (xlsx_current_container)
		oFile = xlsx_current_container->Find(sId);
		
	return oFile;
}
std::wstring XlsxConverter::find_link_by_id (const std::wstring & sId, int type, bool & bExternal)
{
	bExternal = false;
	
	smart_ptr<OOX::File>	oFile;
	std::wstring			ref;

	if (oox_current_child_document)
	{
		oFile	= oox_current_child_document->Find(sId);
		ref		= OoxConverter::find_link_by(oFile, type, bExternal);
	}	
	if (!ref.empty()) return ref;

	if (xlsx_current_container)
	{
		oFile	= xlsx_current_container->Find(sId);
		ref		= OoxConverter::find_link_by(oFile, type, bExternal);
	}
	return ref;
}


bool XlsxConverter::convertDocument()
{
    if (!output_document)   return false;
    if (!ods_context)       return false;
   
	if (!xlsx_document && !xlsx_flat_document) return false;

	ods_context->start_document();

	convert_styles();

	convert_sheets();

	if (xlsx_document)		delete xlsx_document;		xlsx_document = NULL;
	if (xlsx_flat_document) delete xlsx_flat_document;	xlsx_flat_document = NULL;

	ods_context->end_document();
	
	return true;
}

void XlsxConverter::convert_sheets()
{
	if (!ods_context) return;

	OOX::Spreadsheet::CWorkbook *pWorkbook = NULL;
	if (xlsx_document)
	{	
		pWorkbook = xlsx_document->m_pWorkbook;
		if (!pWorkbook) return;

		if ((pWorkbook->m_oWorkbookProtection.IsInit()) && (pWorkbook->m_oWorkbookProtection->m_oLockStructure.IsInit()))
		{
			ods_context->set_tables_structure_lock(pWorkbook->m_oWorkbookProtection->m_oLockStructure->ToBool());
		}
		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*> &mapWorksheets = xlsx_document->m_mapWorksheets;
		
		xlsx_current_container = dynamic_cast<OOX::IFileContainer*>(pWorkbook);
		
		if(pWorkbook->m_oExternalReferences.IsInit())
		{	
			for (size_t i = 0; i < pWorkbook->m_oExternalReferences->m_arrItems.size(); i++)
			{
				OOX::Spreadsheet::CExternalReference *externalRef = dynamic_cast<OOX::Spreadsheet::CExternalReference*>(pWorkbook->m_oExternalReferences->m_arrItems[i]);
				if((externalRef) && (externalRef->m_oRid.IsInit()))
				{
					smart_ptr<OOX::File> file = find_file_by_id(externalRef->m_oRid->GetValue());
					
					smart_ptr<OOX::External> fileExternal = file.smart_dynamic_cast<OOX::External>();
					if (fileExternal.IsInit())
					{
						ods_context->add_external_reference(fileExternal->Uri().GetPath());
					}
					else
					{
						smart_ptr<OOX::Spreadsheet::CExternalLink> externalLink = file.smart_dynamic_cast<OOX::Spreadsheet::CExternalLink>();
						convert(externalLink.GetPointer());
					}
				}
			}
		}
		if(pWorkbook->m_oBookViews.IsInit())
		{	
			for (size_t i = 0; i < pWorkbook->m_oBookViews->m_arrItems.size(); i++)
			{
				convert(pWorkbook->m_oBookViews->m_arrItems[i]);
			}
		}
		if(pWorkbook->m_oSheets.IsInit())
		{				
			for(size_t i = 0, length = pWorkbook->m_oSheets->m_arrItems.size(); i < length; ++i)
			{
				OOX::Spreadsheet::CSheet* pSheet = pWorkbook->m_oSheets->m_arrItems[i];
					
				if(pSheet->m_oRid.IsInit())
				{
					std::wstring sSheetRId = pSheet->m_oRid.get2().ToString();
					std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>::iterator pFind = mapWorksheets.find(sSheetRId);
					
					if (pFind != mapWorksheets.end())
					{
						ods_context->start_sheet();
							ods_context->current_table()->set_table_name(*pSheet->m_oName);
							if (pSheet->m_oState.IsInit() && (	pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleHidden || 
																pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleVeryHidden))
								ods_context->current_table()->set_table_hidden(true);
							
							convert(pFind->second);
						ods_context->end_sheet();	
					}
				}
			}
		}
//----------------------------------------------------------------
		OoxConverter::convert(xlsx_document->m_pJsaProject);
	}
	else if (xlsx_flat_document)
	{
		pWorkbook = xlsx_flat_document->m_pWorkbook.GetPointer();
		if (!pWorkbook) return;		

		for(size_t i = 0; i < xlsx_flat_document->m_arWorksheets.size(); ++i)
		{
			OOX::Spreadsheet::CSheet* pSheet = NULL;
			if ((xlsx_flat_document->m_pWorkbook.IsInit()) && 
				(xlsx_flat_document->m_pWorkbook->m_oSheets.IsInit()) &&
				(xlsx_flat_document->m_pWorkbook->m_oSheets->m_arrItems.size() > i))
			{
				pSheet = xlsx_flat_document->m_pWorkbook->m_oSheets->m_arrItems[i];
			}
			ods_context->start_sheet();
				if ((pSheet) && (pSheet->m_oName.IsInit()))
				ods_context->current_table()->set_table_name(*pSheet->m_oName);
				//if (pSheet->m_oState.IsInit() && (	pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleHidden || 
				//									pSheet->m_oState->GetValue() == SimpleTypes::Spreadsheet::visibleVeryHidden))
				//	ods_context->current_table()->set_table_hidden(true);
				
				convert(xlsx_flat_document->m_arWorksheets[i]);
			ods_context->end_sheet();	
		}
	}
	if (pWorkbook->m_oDefinedNames.IsInit())
	{
		for (size_t i = 0; i < pWorkbook->m_oDefinedNames->m_arrItems.size(); i++)
		{
			convert(pWorkbook->m_oDefinedNames->m_arrItems[i]);
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
		std::wstring name = *oox_defined->m_oName;

		bool printable = false;
		if (name  == L"_xlnm.Print_Area")printable = true;

		ods_context->add_defined_expression (name, *oox_defined->m_oRef, sheet_id, printable);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (!oox_sheet) return;

	OOX::IFileContainer* old_container = xlsx_current_container;
	xlsx_current_container = dynamic_cast<OOX::IFileContainer*>(oox_sheet);
	
	if (oox_sheet->m_oDimension.IsInit())
	{
		ods_context->set_sheet_dimension(oox_sheet->m_oDimension->m_oRef.get());
	}

	convert(oox_sheet->m_oSheetFormatPr.GetPointer());
	convert(oox_sheet->m_oSheetPr.GetPointer());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Предобработка
	//гиперлинки 
	for (size_t hyp = 0; oox_sheet->m_oHyperlinks.IsInit() && hyp < oox_sheet->m_oHyperlinks->m_arrItems.size(); hyp++)
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

	//выносные части таблицы
	if (oox_sheet->m_oTableParts.IsInit())
	{
		for (size_t i=0 ; i < oox_sheet->m_oTableParts->m_arrItems.size(); i++)
		{
			OOX::Spreadsheet::CTablePart *oox_table_part = oox_sheet->m_oTableParts->m_arrItems[i];
			if (!oox_table_part)continue;
			if (!oox_table_part->m_oRId.IsInit())continue;

			smart_ptr<OOX::File> oFile = oox_sheet->Find(oox_table_part->m_oRId->GetValue());
			if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Table == oFile->type())
			{
				OOX::Spreadsheet::CTableFile* pTableFile = (OOX::Spreadsheet::CTableFile*)oFile.GetPointer();
						
				if ((pTableFile) && (pTableFile->m_oTable.IsInit()))
				{				
					oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(pTableFile);					
						convert(pTableFile->m_oTable.GetPointer());				
					oox_current_child_document = NULL;
				}
			}
		}
	}
	convert(oox_sheet->m_oDataValidations.GetPointer());
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	
	//колонки
		if (oox_sheet->m_oColBreaks.IsInit())
		{
			for (size_t i = 0; i < oox_sheet->m_oColBreaks->m_arrItems.size(); i++)
			{
				OOX::Spreadsheet::CBreak *break_ = dynamic_cast<OOX::Spreadsheet::CBreak*>(oox_sheet->m_oColBreaks->m_arrItems[i]);
				if (break_->m_oId.IsInit())
					ods_context->current_table()->add_column_break(break_->m_oId->GetValue());
			}
		}		
		ods_context->start_columns();
		for (size_t col = 0 ; oox_sheet->m_oCols.IsInit() && col < oox_sheet->m_oCols->m_arrItems.size(); col++)
		{
			convert(oox_sheet->m_oCols->m_arrItems[col]);
		}
		ods_context->end_columns();

	//мержи
	for (size_t mrg = 0 ; oox_sheet->m_oMergeCells.IsInit() && mrg < oox_sheet->m_oMergeCells->m_arrItems.size(); mrg++)
	{
		if (oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.IsInit())
			ods_context->add_merge_cells(oox_sheet->m_oMergeCells->m_arrItems[mrg]->m_oRef.get());
	}
	//строки
	if (oox_sheet->m_oSheetData.IsInit() )
	{
		if (oox_sheet->m_oRowBreaks.IsInit())
		{
			for (size_t i = 0; i < oox_sheet->m_oRowBreaks->m_arrItems.size(); i++)
			{
				OOX::Spreadsheet::CBreak *break_ = dynamic_cast<OOX::Spreadsheet::CBreak*>(oox_sheet->m_oRowBreaks->m_arrItems[i]);
				if (break_->m_oId.IsInit())
					ods_context->current_table()->add_row_break(break_->m_oId->GetValue());
			}
		}		
		ods_context->start_rows();
			for (size_t row = 0 ; row < oox_sheet->m_oSheetData->m_arrItems.size(); row++)
			{
				convert(oox_sheet->m_oSheetData->m_arrItems[row], row > 0 ? oox_sheet->m_oSheetData->m_arrItems[row - 1] : NULL);
				
				if ( (row > 0) && (oox_sheet->m_oSheetData->m_arrItems[row - 1] ))
				{
					delete oox_sheet->m_oSheetData->m_arrItems[row - 1];
					oox_sheet->m_oSheetData->m_arrItems[row - 1] = NULL;
				}
			}
		ods_context->end_rows();
		oox_sheet->m_oSheetData.reset();
	}

	if (oox_sheet->m_oDrawing.IsInit() && oox_sheet->m_oDrawing->m_oId.IsInit())
	{
		smart_ptr<OOX::File> oFile = oox_sheet->Find(oox_sheet->m_oDrawing->m_oId->GetValue());
		if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Drawings == oFile->type())
		{
			OOX::Spreadsheet::CDrawing* pDrawing = (OOX::Spreadsheet::CDrawing*)oFile.GetPointer();
			
			convert(pDrawing, oox_sheet);
		}
	}
	
	convert(oox_sheet->m_oOleObjects.GetPointer(), oox_sheet);
	convert(oox_sheet->m_oControls.GetPointer(), oox_sheet);

	//сортировки
	//convert(oox_sheet->m_oSortState.GetPointer());
	
	//автофильтры
	convert(oox_sheet->m_oAutofilter.GetPointer());

	//условное форматирование
	if (!oox_sheet->m_arrConditionalFormatting.empty() || oox_sheet->m_oExtLst.IsInit())
	{
		std::multimap<int, OOX::Spreadsheet::CConditionalFormatting*> mapSorted;

		std::vector<OOX::Spreadsheet::CConditionalFormatting*> arUnsorted;

		// sort by prioritet
		for (size_t fmt = 0; fmt < oox_sheet->m_arrConditionalFormatting.size(); fmt++)
		{
			OOX::Spreadsheet::CConditionalFormatting* cond_fmt = oox_sheet->m_arrConditionalFormatting[fmt];
			if (cond_fmt)
			{
				int priority = -1;
				for (size_t r = 0; r < cond_fmt->m_arrItems.size(); ++r)
				{
					if (cond_fmt->m_arrItems[r]->m_oPriority.IsInit())
					{
						priority = cond_fmt->m_arrItems[r]->m_oPriority->GetValue();
						break;
					}
				}
				if (priority >= 0)
					mapSorted.insert(std::make_pair(priority, cond_fmt));
				else
					arUnsorted.push_back(cond_fmt);
			}
		}

		if (oox_sheet->m_oExtLst.IsInit())
		{
			for (size_t ext = 0; ext < oox_sheet->m_oExtLst->m_arrExt.size(); ext++)
			{
				for (size_t fmt = 0; (oox_sheet->m_oExtLst->m_arrExt[ext]) && (fmt < oox_sheet->m_oExtLst->m_arrExt[ext]->m_arrConditionalFormatting.size()); fmt++)
				{
					OOX::Spreadsheet::CConditionalFormatting* cond_fmt = oox_sheet->m_oExtLst->m_arrExt[ext]->m_arrConditionalFormatting[fmt];
					if (cond_fmt)
					{
						int priority = -1;
						for (size_t r = 0; r < cond_fmt->m_arrItems.size(); ++r)
						{
							if (cond_fmt->m_arrItems[r]->m_oPriority.IsInit())
							{
								priority = cond_fmt->m_arrItems[r]->m_oPriority->GetValue();
								break;
							}
						}
						if (priority >= 0)
							mapSorted.insert(std::make_pair(priority, cond_fmt));
						else
							arUnsorted.push_back(cond_fmt);
					}
				}
			}
		}
//--------------------------------------------------------------------------
		ods_context->start_conditional_formats();
		
		for (size_t fmt =0; fmt < arUnsorted.size(); fmt++)
		{
			convert(arUnsorted[fmt]);
		}
		
		for (std::multimap<int, OOX::Spreadsheet::CConditionalFormatting*>::iterator it = mapSorted.begin(); 
			it != mapSorted.end(); ++it)
		{
			convert(it->second);
		}
		ods_context->end_conditional_formats();
	}
	OoxConverter::convert(oox_sheet->m_oExtLst.GetPointer());

	smart_ptr<OOX::File> oFile = oox_sheet->Find(OOX::Spreadsheet::FileTypes::PivotTable);
	if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::PivotTable == oFile->type())
	{
		smart_ptr<OOX::Spreadsheet::CPivotTableFile> oPivotTable = oFile.smart_dynamic_cast<OOX::Spreadsheet::CPivotTableFile>();
				
		convert(oPivotTable.GetPointer());
	}


/////////////////////////////////////////////////////////////////////////
	convert(oox_sheet->m_oSheetViews.GetPointer());
	convert(oox_sheet->m_oPageSetup.GetPointer());
	convert(oox_sheet->m_oPageMargins.GetPointer());
	convert(oox_sheet->m_oPrintOptions.GetPointer());
	convert(oox_sheet->m_oPicture.GetPointer());
	convert(oox_sheet->m_oSheetProtection.GetPointer());
	
	convert(oox_sheet->m_oLegacyDrawingHF.GetPointer());
	convert(oox_sheet->m_oHeaderFooter.GetPointer());
	
	xlsx_current_container = old_container;
}
void XlsxConverter::convert(OOX::Spreadsheet::CPivotTableFile *oox_pivot_table)
{
	if (!oox_pivot_table) return;
	if (!oox_pivot_table->m_oPivotTableDefinition.IsInit()) return;

	smart_ptr<OOX::File> oFile = oox_pivot_table->Find(OOX::Spreadsheet::FileTypes::PivotCacheDefinition);
	if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::PivotCacheDefinition == oFile->type())
	{
		smart_ptr<OOX::Spreadsheet::CPivotCacheDefinitionFile> oPivotCache = oFile.smart_dynamic_cast<OOX::Spreadsheet::CPivotCacheDefinitionFile>();
				
		convert(oox_pivot_table->m_oPivotTableDefinition.GetPointer(), oPivotCache->m_oPivotCashDefinition.GetPointer());
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CPivotTableDefinition *oox_pivot_table, OOX::Spreadsheet::CPivotCacheDefinition* oox_pivot_cache)
{
	if (!oox_pivot_table || !oox_pivot_cache) return;

	ods_context->start_pivot_table(oox_pivot_table->m_oName.IsInit() ? *oox_pivot_table->m_oName : L"");

	convert(oox_pivot_table->m_oLocation.GetPointer());
	if (oox_pivot_table->m_oPivotFields.IsInit())
	{
		for (size_t i = 0; i < oox_pivot_table->m_oPivotFields->m_arrItems.size(); i++)
		{
			convert(oox_pivot_table->m_oPivotFields->m_arrItems[i]);
		}
	}
	//if (oox_pivot_table->m_oDataFields.IsInit())
	//{
	//	for (size_t i = 0; i < oox_pivot_table->m_oDataFields->m_arrItems.size(); i++)
	//	{
	//		convert(oox_pivot_table->m_oDataFields->m_arrItems[i]);
	//	}
	//}
	//if (oox_pivot_table->m_oColFields.IsInit())
	//{
	//	for (size_t i = 0; i < oox_pivot_table->m_oColFields->m_arrItems.size(); i++)
	//	{
	//		convert(oox_pivot_table->m_oColFields->m_arrItems[i]);
	//	}
	//}
	//if (oox_pivot_table->m_oRowFields.IsInit())
	//{
	//	for (size_t i = 0; i < oox_pivot_table->m_oRowFields->m_arrItems.size(); i++)
	//	{
	//		convert(oox_pivot_table->m_oRowFields->m_arrItems[i]);
	//	}
	//}
	if (oox_pivot_table->m_oPageFields.IsInit())
	{
		for (size_t i = 0; i < oox_pivot_table->m_oPageFields->m_arrItems.size(); i++)
		{
			convert(oox_pivot_table->m_oPageFields->m_arrItems[i]);
		}
	}

	ods_context->end_pivot_table();
}
void XlsxConverter::convert(OOX::Spreadsheet::CLegacyDrawingHFWorksheet *oox_background)
{
	if (!oox_background) return;
	if (!oox_background->m_oId.IsInit()) return;

	smart_ptr<OOX::File> file = find_file_by_id(oox_background->m_oId->GetValue());
	smart_ptr<OOX::CVmlDrawing> vmlDrawing = file.smart_dynamic_cast<OOX::CVmlDrawing>();
	
	if (false == vmlDrawing.IsInit()) return;

	oox_current_child_document = dynamic_cast<OOX::IFileContainer*>(vmlDrawing.GetPointer());					
		
	for (boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator it = vmlDrawing->m_mapShapes.begin(); it!= vmlDrawing->m_mapShapes.end(); ++it)
	{
		OOX::Vml::CShape* pShape = dynamic_cast<OOX::Vml::CShape*>(it->second.pElement);

		if ((pShape) && (pShape->m_sId.IsInit()))
		{
			for (size_t i = 0; i < pShape->m_arrItems.size(); ++i)
			{
				OOX::Vml::CImageData* pImage = dynamic_cast<OOX::Vml::CImageData*>(pShape->m_arrItems[i]);
				if (pImage)
				{
					odf_writer::office_element_ptr fill_image_element;
					
					std::wstring pathImage, href, sID = pImage->m_oId.IsInit() ? pImage->m_rId->GetValue() : (pImage->m_oRelId.IsInit() ? pImage->m_oRelId->GetValue() : L"");
					
					bool bExternal = false;
					pathImage   = find_link_by_id(sID, 1, bExternal);
					href		= ods_context->add_image(pathImage, bExternal);

					if (false == href.empty())
					{
						odf_writer::create_element(L"style", L"background-image", fill_image_element, ods_context);

						odf_writer::style_background_image * fill_image = dynamic_cast<odf_writer::style_background_image*>(fill_image_element.get());
						if (!fill_image) return;

						fill_image->xlink_attlist_				= odf_types::common_xlink_attlist();
						fill_image->xlink_attlist_->type_		= odf_types::xlink_type::Simple;
						fill_image->xlink_attlist_->actuate_	= odf_types::xlink_actuate::OnLoad;	
						fill_image->xlink_attlist_->href_		= href;

						ods_context->add_header_footer_image(*pShape->m_sId, fill_image_element);
					}
				}
			}
		}				
	}
	oox_current_child_document = NULL;
}
void XlsxConverter::convert(OOX::Spreadsheet::CHeaderFooter *oox_header_footer)
{
	if (!oox_header_footer) return;

	bool bFirst = oox_header_footer->m_oDifferentFirst.IsInit() ? oox_header_footer->m_oDifferentFirst->ToBool() : false;
	bool bEven = oox_header_footer->m_oDifferentOddEven.IsInit() ? oox_header_footer->m_oDifferentOddEven->ToBool() : false;

	if (bEven || oox_header_footer->m_oOddHeader.IsInit())
	{
		if (true == ods_context->start_header(0))
		{
			convert(oox_header_footer->m_oOddHeader.GetPointer());
			ods_context->end_header_footer();
		}
	}
	if (bEven ||oox_header_footer->m_oOddFooter.IsInit())
	{
		if (true == ods_context->start_footer(0))
		{
			convert(oox_header_footer->m_oOddFooter.GetPointer());
			ods_context->end_header_footer();
		}
	}
	if (bEven ||oox_header_footer->m_oEvenHeader.IsInit())
	{
		if (true == ods_context->start_header(1))
		{
			convert(oox_header_footer->m_oEvenHeader.GetPointer());
			ods_context->end_header_footer();
		}
	}
	if (bEven ||oox_header_footer->m_oEvenFooter.IsInit())
	{
		if (true == ods_context->start_footer(1))
		{
			convert(oox_header_footer->m_oEvenFooter.GetPointer());
			ods_context->end_header_footer();
		}
	}
	if (bFirst || oox_header_footer->m_oFirstHeader.IsInit())
	{
		if (true == ods_context->start_header(2))
		{
			convert(oox_header_footer->m_oFirstHeader.GetPointer());
			ods_context->end_header_footer();
		}
	}
	if (bFirst || oox_header_footer->m_oFirstFooter.IsInit())
	{
		if (true == ods_context->start_footer(2))
		{
			convert(oox_header_footer->m_oFirstFooter.GetPointer());
			ods_context->end_header_footer();
		}
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CHeaderFooterElement	*oox_header_footer)
{
	if (!oox_header_footer) return;

	size_t pos = 0;
	odf_writer::style_text_properties current_text_props;
	
	int type_add = 0;
	while(pos < oox_header_footer->m_sText.size())
	{
		if (oox_header_footer->m_sText[pos] == L'&')
		{
			pos++;
			wchar_t comm = oox_header_footer->m_sText[pos];
			
			switch(comm)
			{
				case 'L':	
					{
						current_text_props.content_.clear();
						ods_context->start_header_footer_region(1); pos++;
					}break;
				case 'C':	
					{
						current_text_props.content_.clear();
						ods_context->start_header_footer_region(2); pos++; 
					}break;
				case 'R':	
					{
						current_text_props.content_.clear();						
						ods_context->start_header_footer_region(3); pos++; 
					}break;

				case 'A':	type_add = 1; pos++; break;
				case 'P':	type_add = 2; pos++; break;
				case 'N':	type_add = 3; pos++; break;
				case 'D':	type_add = 4; pos++; break;
				case 'T':	type_add = 5; pos++; break;
				case 'F':	type_add = 6; pos++; break;
				case 'Z':	type_add = 7; pos++; break;
				case '&':	type_add = 8; pos++; break;
				case 'G':	pos++; break;

				case 'E':	
				{
					if (current_text_props.content_.style_text_underline_type_)
						current_text_props.content_.style_text_underline_type_ = boost::none;
					else
						current_text_props.content_.style_text_underline_type_ = odf_types::line_type::Double; 
					pos++; 
				}break;
				case 'X':
				{
					if (current_text_props.content_.style_text_position_ && current_text_props.content_.style_text_position_->get_position().get_value() > 0)
						current_text_props.content_.style_text_position_ = boost::none;
					else
						current_text_props.content_.style_text_position_ = odf_types::text_position(+33.); 
					pos++; 
				}break;
				case 'Y':
				{
					if (current_text_props.content_.style_text_position_ && current_text_props.content_.style_text_position_->get_position().get_value() < 0)
						current_text_props.content_.style_text_position_ = boost::none;
					else
						current_text_props.content_.style_text_position_ = odf_types::text_position(-33.); 
					pos++; 
				}break;
				case 'B':
				{
					if (current_text_props.content_.fo_font_weight_)
						current_text_props.content_.fo_font_weight_ = boost::none;
					else
						current_text_props.content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold); 
					pos++; 
				}break;
				case 'I':	
				{
					if (!current_text_props.content_.fo_font_style_)
						current_text_props.content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
					else
						current_text_props.content_.fo_font_style_ = boost::none;							
					pos++; 
				}break;
				case 'U':
				{
					if (current_text_props.content_.style_text_underline_type_)
						current_text_props.content_.style_text_underline_type_ = boost::none;
					else
						current_text_props.content_.style_text_underline_type_ = odf_types::line_type(odf_types::line_type::Single); 
					pos++; 
				}break;
				case 'S':
				{
					if (current_text_props.content_.style_text_line_through_type_)
						current_text_props.content_.style_text_line_through_type_ = boost::none;
					else
						current_text_props.content_.style_text_line_through_type_ = odf_types::line_type(odf_types::line_type::Single); 
					pos++; 
				}break;
				case 'K':
				{
					pos++;
					std::wstring color = oox_header_footer->m_sText.substr(pos, 6); pos += 6;

					current_text_props.content_.fo_color_ = odf_types::color(L"#" + color); 
				}break;
				case '\"':
				{
					size_t pos1 = oox_header_footer->m_sText.find(L'\"', pos + 1); 
					std::wstring font = oox_header_footer->m_sText.substr(pos + 1, pos1 - pos - 1); pos = pos1;

					pos1 = font.find(L",");
					if (pos1 != std::wstring::npos)
					{
						std::wstring format = font.substr(pos1 + 1);
						font = font.substr(0, pos1);

						if (std::wstring::npos != format.find(L"Bold"))
							current_text_props.content_.fo_font_weight_ = odf_types::font_weight(odf_types::font_weight::WBold); 
						if (std::wstring::npos != format.find(L"Italic"))
							current_text_props.content_.fo_font_style_ = odf_types::font_style(odf_types::font_style::Italic);
						if (std::wstring::npos != format.find(L"Regular"))
						{
							current_text_props.content_.fo_font_weight_ = boost::none;
							current_text_props.content_.fo_font_style_ = boost::none;
						}
					}

					current_text_props.content_.fo_font_family_ = font;
					pos++;
				}break;
				default:
				{
					int font_size = 0;
					pos++;
					if (comm >= L'0' && comm <= L'9')
					{
						font_size = comm - L'0';
						wchar_t comm1 = oox_header_footer->m_sText[pos];
						if (comm1 >= L'0' && comm1 <= L'9')
						{
							pos++;
							font_size = (font_size * 10) + (comm1 - L'0');
						}
						else
						{
						}
					}
					if (font_size > 0)
					{
						current_text_props.content_.fo_font_size_ = odf_types::font_size(odf_types::length(font_size, odf_types::length::pt));
					}
				}break;
			}
		}
		size_t next = oox_header_footer->m_sText.find(L'&', pos);
		if (next == std::wstring::npos) next = oox_header_footer->m_sText.length();

		if (type_add > 0 || next - pos > 0)
		{
			ods_context->text_context()->get_styles_context()->create_style(L"", odf_types::style_family::Text, true, false, -1);					
			odf_writer::style_text_properties *text_properties = ods_context->text_context()->get_styles_context()->last_state()->get_text_properties();
			text_properties->content_.apply_from(current_text_props.content_);

			ods_context->text_context()->start_span(true);
			
			switch(type_add)
			{
				case 1:	ods_context->text_context()->add_text_sheet_name(L"???");	break;
				case 2: ods_context->text_context()->add_text_page_number(L"1");	break;
				case 3: ods_context->text_context()->add_text_page_count(L"99");	break;	
				case 4: ods_context->text_context()->add_text_date(L"00.00.0000");	break;	
				case 5: ods_context->text_context()->add_text_time(L"00:00");		break;	
				case 6: ods_context->text_context()->add_text_file_name(L"???");	break;	
				case 7: ods_context->text_context()->add_text_file_name(L"???");	break;
				case 8: ods_context->text_context()->add_text_content(L"&");		break;
			}
			type_add = 0;
			if (next - pos > 0)
			{
				std::wstring text = oox_header_footer->m_sText.substr(pos, next - pos);

				size_t pos2 = text.find(10);
				while(pos2 != std::wstring::npos)
				{
					std::wstring text1 = text.substr(0, pos2);
					text = text.substr(pos2 + 1);
					if (false == text1.empty())
					{
						ods_context->text_context()->add_text_content(text1);
					}
					ods_context->text_context()->end_span();
					ods_context->text_context()->end_paragraph();
					ods_context->text_context()->start_paragraph(false);
					ods_context->text_context()->start_span(true);

					pos2 = text.find(10);						
				}
				if (false == text.empty())
				{
					ods_context->text_context()->add_text_content(text);
				}	
			}
			ods_context->text_context()->end_span();

			pos = next;
		}
	}

}

void XlsxConverter::convert(OOX::Spreadsheet::CSheetProtection *oox_prot)
{
	if (!oox_prot) return;

	ods_context->current_table()->set_table_protection(true);

	if (oox_prot->m_oInsertColumns.IsInit())
	{
		ods_context->current_table()->set_table_protection_insert_columns(!oox_prot->m_oInsertColumns->ToBool());
	}
	if (oox_prot->m_oInsertRows.IsInit())
	{
		ods_context->current_table()->set_table_protection_insert_rows(!oox_prot->m_oInsertRows->ToBool());
	}
	if (oox_prot->m_oDeleteColumns.IsInit())
	{
		ods_context->current_table()->set_table_protection_delete_columns(!oox_prot->m_oDeleteColumns->ToBool());
	}
	if (oox_prot->m_oDeleteRows.IsInit())
	{
		ods_context->current_table()->set_table_protection_delete_rows(!oox_prot->m_oDeleteRows->ToBool());
	}
	if (oox_prot->m_oSelectLockedCells.IsInit())
	{
		ods_context->current_table()->set_table_protection_protected_cells(!oox_prot->m_oSelectLockedCells->ToBool());
	}
	if (oox_prot->m_oSelectUnlockedCells.IsInit())
	{
		ods_context->current_table()->set_table_protection_unprotected_cells(!oox_prot->m_oSelectUnlockedCells->ToBool());
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CDataValidations *oox_validations)
{
	if (!oox_validations) return;

	for (size_t i = 0; i < oox_validations->m_arrItems.size(); i++)
	{
		convert(oox_validations->m_arrItems[i]);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CDataValidation *oox_validation)
{
	if (!oox_validation) return;
	if (!oox_validation->m_oSqRef.IsInit()) return;

	if (false == ods_context->start_data_validation(*oox_validation->m_oSqRef, oox_validation->m_oType.IsInit() ? oox_validation->m_oType->GetValue() : 0)) return;

	if (oox_validation->m_oOperator.IsInit())
	{
		ods_context->set_data_validation_operator(oox_validation->m_oOperator->GetValue());
	}
	if (oox_validation->m_oAllowBlank.IsInit())
	{
		ods_context->set_data_validation_allow_empty(oox_validation->m_oAllowBlank->ToBool());
	}
	std::wstring formula_1, formula_2;

	if (oox_validation->m_oFormula1.IsInit()) formula_1 = oox_validation->m_oFormula1->m_sText;
	if (oox_validation->m_oFormula2.IsInit()) formula_2 = oox_validation->m_oFormula2->m_sText;

	ods_context->set_data_validation_content(formula_1, formula_2);
	
	ods_context->set_data_validation_error( oox_validation->m_oErrorTitle.IsInit() ? *oox_validation->m_oErrorTitle : L"", 
											oox_validation->m_oError.IsInit() ? *oox_validation->m_oError : L"",
											oox_validation->m_oShowErrorMessage.IsInit() ? oox_validation->m_oShowErrorMessage->ToBool() : true,
											oox_validation->m_oErrorStyle.IsInit() ? oox_validation->m_oErrorStyle->GetValue() : SimpleTypes::Spreadsheet::errorStyleStop);

	ods_context->set_data_validation_promt( oox_validation->m_oPromptTitle.IsInit() ? *oox_validation->m_oPromptTitle : L"", 
											oox_validation->m_oPrompt.IsInit() ? *oox_validation->m_oPrompt : L"", 
											oox_validation->m_oShowInputMessage.IsInit() ? oox_validation->m_oShowInputMessage->ToBool() : true);
	ods_context->end_data_validation();
}

void XlsxConverter::convert(OOX::Spreadsheet::CPictureWorksheet *oox_background)
{
	if (!oox_background) return;
	if (!oox_background->m_oId.IsInit())return;

	std::wstring pathImage, href, sID = oox_background->m_oId->GetValue();

	bool bExternal = false;

	pathImage   = find_link_by_id(sID, 1, bExternal);
	href		= ods_context->add_image(pathImage, bExternal);

	if (href.empty()) return;

	odf_writer::office_element_ptr fill_image_element;

	odf_writer::create_element(L"style", L"background-image", fill_image_element, ods_context);

	odf_writer::style_background_image * fill_image = dynamic_cast<odf_writer::style_background_image*>(fill_image_element.get());
	if (!fill_image) return;

	fill_image->xlink_attlist_				= odf_types::common_xlink_attlist();
	fill_image->xlink_attlist_->type_		= odf_types::xlink_type::Simple;
	fill_image->xlink_attlist_->actuate_	= odf_types::xlink_actuate::OnLoad;	
	fill_image->xlink_attlist_->href_		= href;

	odf_writer::style_page_layout_properties* page_props = ods_context->page_layout_context()->last_layout()->get_properties();
	page_props->add_child_element(fill_image_element);
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

	convert(oox_table_part->m_oTableColumns.GetPointer());
	OoxConverter::convert(oox_table_part->m_oExtLst.GetPointer());
	
	ods_context->end_table_part();
}

void XlsxConverter::convert(OOX::Spreadsheet::CTableColumns *oox_table_part_columns)
{
	if (!oox_table_part_columns) return;

	for (size_t i = 0; i < oox_table_part_columns->m_arrItems.size(); i++)
	{
		std::wstring name;
		if (oox_table_part_columns->m_arrItems[i]->m_oName.IsInit())
			name = *oox_table_part_columns->m_arrItems[i]->m_oName;
		
		ods_context->add_table_part_column(name);
	}
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
	if (oox_comment->m_bVisible.IsInit())
	{
		ods_context->set_comment_visible(*oox_comment->m_bVisible);
	}
	if (oox_comment->m_sFillColorRgb.IsInit())
	{
		ods_context->set_comment_color(*oox_comment->m_sFillColorRgb);
	}
	else
	{
		ods_context->set_comment_color(L"CCFFFF"); //default ms
	}
	if (oox_comment->m_oText.IsInit())
	{
		for(size_t i = 0; i < oox_comment->m_oText->m_arrItems.size(); ++i)
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
	std::wstring location;
	std::wstring display = oox_hyperlink->m_oDisplay.IsInit() ? oox_hyperlink->m_oDisplay.get() : L"";

	if (oox_hyperlink->m_oRid.IsInit() && oox_sheet->m_pCurRels.IsInit())
	{
		OOX::Rels::CRelationShip* oRels = NULL;
		oox_sheet->m_pCurRels->GetRel( OOX::RId(oox_hyperlink->m_oRid->GetValue()), &oRels);
		if(NULL != oRels && _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink") == oRels->Type() )
		{
			if(oRels->IsExternal())
				link= oRels->Target().GetPath();
		}
	}
	if (link.empty() && oox_hyperlink->m_oLink.IsInit())
	{
		link = *oox_hyperlink->m_oLink;
	}
	if (oox_hyperlink->m_oLocation.IsInit())
	{
		location = *oox_hyperlink->m_oLocation;
	}
	if (false == location.empty() || false == link.empty())
	{
		ods_context->add_hyperlink(ref, link, display, location);
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CRow *oox_row, OOX::Spreadsheet::CRow *oox_row_prev)
{
	if (oox_row == NULL)return;

	int row_number = oox_row->m_oR.IsInit() ? oox_row->m_oR->GetValue() : -1;

	if (oox_row_prev)
	{
		if (oox_row_prev->m_arrItems.empty() && 
			oox_row->m_arrItems.empty())
		{
			bool bEqual = true;

			if (oox_row->m_oCollapsed.IsInit())
			{
				if (oox_row_prev->m_oCollapsed.IsInit())
				{
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oCustomFormat.IsInit())
			{
				if (oox_row_prev->m_oCustomFormat.IsInit())
				{
					if (oox_row->m_oCustomFormat->GetValue() != oox_row_prev->m_oCustomFormat->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oCustomHeight.IsInit())
			{
				if (oox_row_prev->m_oCustomHeight.IsInit())
				{
					if (oox_row->m_oCustomHeight->GetValue() != oox_row_prev->m_oCustomHeight->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}			
			if (bEqual && oox_row->m_oHidden.IsInit())
			{
				if (oox_row_prev->m_oHidden.IsInit())
				{
					if (oox_row->m_oHidden->GetValue() != oox_row_prev->m_oHidden->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oHt.IsInit())
			{
				if (oox_row_prev->m_oHt.IsInit())
				{
					if (oox_row->m_oHt->GetValue() != oox_row_prev->m_oHt->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oOutlineLevel.IsInit())
			{
				if (oox_row_prev->m_oOutlineLevel.IsInit())
				{
					if (oox_row->m_oOutlineLevel->GetValue() != oox_row_prev->m_oOutlineLevel->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oPh.IsInit())
			{
				if (oox_row_prev->m_oPh.IsInit())
				{
					if (oox_row->m_oPh->GetValue() != oox_row_prev->m_oPh->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oR.IsInit())
			{
				if (oox_row_prev->m_oR.IsInit())
				{
					if (oox_row->m_oR->GetValue() != oox_row_prev->m_oR->GetValue() + 1) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oS.IsInit())
			{
				if (oox_row_prev->m_oS.IsInit())
				{
					if (oox_row->m_oS->GetValue() != oox_row_prev->m_oS->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oThickBot.IsInit())
			{
				if (oox_row_prev->m_oThickBot.IsInit())
				{
					if (oox_row->m_oThickBot->GetValue() != oox_row_prev->m_oThickBot->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oThickTop.IsInit())
			{
				if (oox_row_prev->m_oThickTop.IsInit())
				{
					if (oox_row->m_oThickTop->GetValue() != oox_row_prev->m_oThickTop->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}
			if (bEqual && oox_row->m_oDyDescent.IsInit())
			{
				if (oox_row_prev->m_oDyDescent.IsInit())
				{
					if (oox_row->m_oDyDescent->GetValue() != oox_row_prev->m_oDyDescent->GetValue()) bEqual = false;
				}
				else bEqual = false;
			}

			if (bEqual)
			{
				int repeated = 1;
				if ( ods_context->current_table()->is_row_comment(row_number, repeated) < 0 &&
					ods_context->current_table()->is_row_validation(row_number, repeated) < 0)
				{
					ods_context->add_row_repeated();
					return;
				}
			}
		}
	}

	bool _default = true;
	
	if (oox_row->m_oHt.IsInit() || oox_row->m_oCustomHeight.IsInit() || oox_row->m_oHidden.IsInit() || oox_row->m_oCollapsed.IsInit()) _default = false;
	
	int level = 0;
	if (oox_row->m_oOutlineLevel.IsInit())
	{
		level = oox_row->m_oOutlineLevel->GetValue();
	}

	ods_context->start_row(row_number, 1, level, _default);
	
	if (oox_row->m_oHidden.IsInit())		ods_context->current_table()->set_row_hidden(true);
	if (oox_row->m_oCollapsed.IsInit())		ods_context->current_table()->set_row_hidden(true);
	
	std::wstring style_cell_name;
	if (oox_row->m_oS.IsInit() && ( oox_row->m_oCustomFormat.IsInit() && oox_row->m_oCustomFormat->GetValue()==1))
	{
		int xfd_id = oox_row->m_oS->GetValue();

		style_cell_name  = ods_context->styles_context()->find_odf_style_name(xfd_id,odf_types::style_family::TableCell,false,true);
	}else
	{
		//style_cell_name  = ods_context->styles_context()->find_odf_style_name_default(odf_types::style_family::TableCell);
	}
	ods_context->current_table()->set_row_default_cell_style(style_cell_name );
	
	if (oox_row->m_oHt.IsInit())
	{
		double dyDescent = 0.25;
		double height = oox_row->m_oHt->GetValue();
		if (oox_row->m_oDyDescent.IsInit())
		{
			dyDescent = oox_row->m_oDyDescent->GetValue();
		}
		ods_context->current_table()->set_row_height(height);
	}
	if (false == _default)
	{
		if ((oox_row->m_oCustomHeight.IsInit() && oox_row->m_oCustomHeight->GetValue() == 1) ||
			(oox_row->m_oCustomFormat.IsInit() && oox_row->m_oCustomFormat->GetValue() == 1 && oox_row->m_oHt.IsInit()))
		{
			ods_context->current_table()->set_row_optimal_height(false);
		}
		else
			ods_context->current_table()->set_row_optimal_height(true);
	}

	for (size_t cell = 0 ; cell < oox_row->m_arrItems.size();cell++)
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

	int ifx_style = oox_cell->m_oStyle.IsInit() ? *oox_cell->m_oStyle : -1;

	ods_context->start_cell(ref, ifx_style);

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
            convert_sharing_string(XmlUtils::GetInteger(oox_cell->m_oValue->m_sText));
		}

		else
		{
			if (value_type >=0)		
				ods_context->current_table()->set_cell_type	(value_type);
			
			ods_context->current_table()->set_cell_value (oox_cell->m_oValue->m_sText);
			//ods_context->current_table()->set_cell_cache (oox_cell->m_oValue->m_sText);
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
	
	const OOX::Spreadsheet::CSharedStrings *SharedStrings= xlsx_document->m_pSharedStrings;
	if (!SharedStrings) return;

    if (number >= 0 && number < (int)SharedStrings->m_arrItems.size())

    convert(SharedStrings->m_arrItems[number]);
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
	ods_context->current_table()->set_cell_text( ods_context->text_context());
}
void XlsxConverter::convert(OOX::Spreadsheet::CExternalLink *oox_external_link)
{
	if (!oox_external_link) return;

	OOX::IFileContainer* old_container = xlsx_current_container;
	xlsx_current_container = dynamic_cast<OOX::IFileContainer*>(oox_external_link);
	
	if (oox_external_link->m_oExternalBook.IsInit())
	{
		if (oox_external_link->m_oExternalBook->m_oRid.IsInit())
		{
			smart_ptr<OOX::File> file = find_file_by_id(oox_external_link->m_oExternalBook->m_oRid->GetValue());
			
			smart_ptr<OOX::External> fileExternal = file.smart_dynamic_cast<OOX::External>();
			if (fileExternal.IsInit())
			{
				ods_context->add_external_reference(fileExternal->Uri().GetPath());
			}
		}
	}
	xlsx_current_container = old_container;
}

void XlsxConverter::convert(OOX::Spreadsheet::WritingElement  *oox_unknown)
{
	if (!oox_unknown)return;

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
		case OOX::et_x_ColorScale:
		{
			OOX::Spreadsheet::CColorScale *pB = dynamic_cast<OOX::Spreadsheet::CColorScale*>(oox_unknown);
			convert(pB);
		}break;
		case OOX::et_x_FormulaCF:
		{
			OOX::Spreadsheet::CFormulaCF *pF = dynamic_cast<OOX::Spreadsheet::CFormulaCF*>(oox_unknown);
			convert(pF);
		}break;		
		case OOX::et_x_ConditionalFormatValueObject:
		{
			OOX::Spreadsheet::CConditionalFormatValueObject *pF = dynamic_cast<OOX::Spreadsheet::CConditionalFormatValueObject*>(oox_unknown);
			convert(pF);
		}break;
		default:
		{
			OoxConverter::convert(oox_unknown);
		}
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CRun *oox_text_run)
{
	if (oox_text_run == NULL)return;
	
	convert(oox_text_run->m_oRPr.GetPointer());
	ods_context->text_context()->start_span(oox_text_run->m_oRPr.GetPointer() ? true : false);
	
	for(size_t i = 0; i < oox_text_run->m_arrItems.size(); ++i)
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

	ods_context->styles_context()->create_style(L"", odf_types::style_family::Text, automatic, root, -1);	
	
	odf_writer::style_text_properties *text_properties = ods_context->styles_context()->last_state()->get_text_properties();
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
            //nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oCondense;
            //nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oExtend;
            //nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oOutline;
            //nullable<ComplexTypes::Spreadsheet::COnOff2 >	m_oShadow;

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
			ods_context->current_table()->add_or_find_cell_shared_formula(formula,ref, ind);
		}
		else if (oox_formula->m_oT->GetValue() == SimpleTypes::Spreadsheet::cellformulatypeArray)
		{
			ods_context->current_table()->set_cell_array_formula(formula,ref);
		}
		else if (oox_formula->m_oT->GetValue() == SimpleTypes::Spreadsheet::cellformulatypeDataTable)
		{
		}
		else 
			ods_context->current_table()->set_cell_formula(formula);
	}else 
		ods_context->current_table()->set_cell_formula(formula);

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
		ods_context->current_table()->set_column_optimal_width(true);
	}
	else if (oox_column->m_oCustomWidth.IsInit() == false || 
			(oox_column->m_oCustomWidth.IsInit() == true && oox_column->m_oCustomWidth->GetValue() == 1))
	{
		ods_context->current_table()->set_column_optimal_width(false);
	}
	
	if (oox_column->m_oHidden.IsInit() && oox_column->m_oHidden->ToBool()) width = 0;

	if (width <0.01)
	{
		width = 0;
		ods_context->current_table()->set_column_hidden(true);
	}
	
	width = ods_context->convert_symbol_width(width);	
	ods_context->current_table()->set_column_width(width);

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
		ods_context->current_table()->set_column_default_cell_style(style_cell_name );
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
		ods_context->current_table()->set_table_tab_color(odf_color);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CWorkbookView *oox_book_views)
{
	if (!oox_book_views)return;
	const OOX::Spreadsheet::CWorkbook *Workbook= xlsx_document->m_pWorkbook;
	if (!Workbook) return;

	ods_context->settings_context()->start_view();

	if (oox_book_views->m_oActiveTab.IsInit())
	{
		int	table_id = oox_book_views->m_oActiveTab->GetValue();

		if (table_id >= 0 && table_id < (int)Workbook->m_oSheets->m_arrItems.size())
		{
			ods_context->settings_context()->add_property(L"ActiveTable", L"string", 
				XmlUtils::EncodeXmlString(*Workbook->m_oSheets->m_arrItems[table_id]->m_oName));
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

	for (size_t i =0; i < oox_sheet_views->m_arrItems.size(); i++)
	{
		OOX::Spreadsheet::CSheetView *sheet_view = oox_sheet_views->m_arrItems[i];
		if (!sheet_view) continue;

		int view_id = sheet_view->m_oWorkbookViewId.IsInit() ? sheet_view->m_oWorkbookViewId->GetValue() : -1;
		if (view_id < 0) continue;

		ods_context->start_table_view( view_id );
		
		if (sheet_view->m_oRightToLeft.IsInit() && sheet_view->m_oRightToLeft->GetValue()==1)
			ods_context->current_table()->set_table_rtl(true);

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
		int topLeftCellX = -1, topLeftCellY = -1;

		if (sheet_view->m_oTopLeftCell.IsInit())
		{
			std::wstring ref(sheet_view->m_oTopLeftCell.get());
			odf_writer::utils::parsing_ref (ref, topLeftCellX, topLeftCellY);
		}
		if (topLeftCellX > 0 && topLeftCellY > 0)
		{
			ods_context->settings_context()->add_property(L"PositionLeft", L"int", std::to_wstring(topLeftCellX - 1));
			ods_context->settings_context()->add_property(L"PositionTop", L"int", std::to_wstring(topLeftCellY - 1));
		}
		else
		{
			ods_context->settings_context()->add_property(L"PositionLeft", L"int", std::to_wstring(0));
			ods_context->settings_context()->add_property(L"PositionTop", L"int", std::to_wstring(0));
		}

		for (size_t j = 0; j < sheet_view->m_arrItems.size(); j++)
		{
			OOX::Spreadsheet::CSelection *selection = sheet_view->m_arrItems[j];

			if (selection->m_oActiveCell.IsInit())
			{
				std::wstring ref(selection->m_oActiveCell.get());
				odf_writer::utils::parsing_ref (ref, ActiveCellX, ActiveCellY);

				if (ActiveCellX > 0 && ActiveCellY > 0)
				{
					ods_context->settings_context()->add_property(L"CursorPositionX",	L"int", std::to_wstring(ActiveCellX - 1));
					ods_context->settings_context()->add_property(L"CursorPositionY",	L"int", std::to_wstring(ActiveCellY - 1));					
					ods_context->settings_context()->add_property(L"PositionRight",		L"int", std::to_wstring(0));
					ods_context->settings_context()->add_property(L"PositionBottom",	L"int", std::to_wstring(ActiveCellY > 30 ? ActiveCellY - 2 : 0));
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
				std::wstring sVal = std::to_wstring((int)sheet_view->m_oPane->m_oXSplit->GetValue());
				
				ods_context->settings_context()->add_property(L"HorizontalSplitMode", L"short", L"2");
				ods_context->settings_context()->add_property(L"HorizontalSplitPosition", L"int",  sVal);
				ods_context->settings_context()->add_property(L"PositionLeft", L"int",  L"0");
				ods_context->settings_context()->add_property(L"PositionRight", L"int",  sVal);
				bPaneX = true;
			}
			if (sheet_view->m_oPane->m_oYSplit.IsInit())
			{
				std::wstring sVal = std::to_wstring((int)sheet_view->m_oPane->m_oYSplit->GetValue());
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
		//nullable<SimpleTypes::Spreadsheet::CSheetViewType<>>	m_oView;
		//nullable<SimpleTypes::COnOff<>>						m_oWindowProtection;
		//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleNormal;
		//nullable<SimpleTypes::CUnsignedDecimalNumber<>>		m_oZoomScaleSheetLayoutView;		
		ods_context->end_table_view();
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CPrintOptions *oox_print_options)
{
	if (!oox_print_options) return;

	if (oox_print_options->m_oGridLines.IsInit())
	{
		ods_context->page_layout_context()->set_page_print_gridLines(oox_print_options->m_oGridLines->ToBool());
	}
	//if (oox_print_options->m_oGridLinesSet.IsInit()) дублирование
	//{
	//	ods_context->page_layout_context()->set_page_print_gridLinesSet(oox_print_options->m_oGridLines->ToBool());
	//}
	if (oox_print_options->m_oHeadings.IsInit())
	{
		ods_context->page_layout_context()->set_page_print_headings(oox_print_options->m_oHeadings->ToBool());
	}
	bool bHorizontal = false, bVertical = false;

	if (oox_print_options->m_oHorizontalCentered.IsInit())
	{
		bHorizontal = oox_print_options->m_oHorizontalCentered->ToBool();
	}
	if (oox_print_options->m_oVerticalCentered.IsInit())
	{
		bVertical = oox_print_options->m_oVerticalCentered->ToBool();
	}
	ods_context->page_layout_context()->set_page_centered(bHorizontal, bVertical);
}
void XlsxConverter::convert(OOX::Spreadsheet::CPageSetup *oox_page)
{
	if (!oox_page) return;

	int type = 1;	
	if (oox_page->m_oOrientation.IsInit())
	{
		type = (int)oox_page->m_oOrientation->GetValue();
	}
	ods_context->page_layout_context()->set_page_orientation(type);

	_CP_OPT(odf_types::length) width, height;
	if (oox_page->m_oPaperWidth.IsInit() && oox_page->m_oPaperHeight.IsInit())
	{
		double w	= oox_page->m_oPaperWidth->GetValue();
		double h	= oox_page->m_oPaperHeight->GetValue();
		int unit = oox_page->m_oPaperUnits.IsInit() ? oox_page->m_oPaperUnits->GetValue() : 0;

		switch(unit)
		{
		case 1:
			width	= odf_types::length(w, odf_types::length::cm);
			height	= odf_types::length(h, odf_types::length::cm);
		case 2:
			width	= odf_types::length(w, odf_types::length::inch);
			height	= odf_types::length(h, odf_types::length::inch);
		case 3:
			width	= odf_types::length(w, odf_types::length::pt);
			height	= odf_types::length(h, odf_types::length::pt);
		case 4:
			width	= odf_types::length(w, odf_types::length::px);
			height	= odf_types::length(h, odf_types::length::px);
		case 0:
		default:
			width	= odf_types::length(w, odf_types::length::mm);
			height	= odf_types::length(h, odf_types::length::mm);
		}
	}
	else if (oox_page->m_oPaperSize.IsInit())
	{
		switch(oox_page->m_oPaperSize->GetValue())
		{
		case SimpleTypes::Spreadsheet::pagesizeLetterPaper:
			width	= odf_types::length(8.5, odf_types::length::inch);
			height	= odf_types::length(11, odf_types::length::inch);
			break;
		case SimpleTypes::Spreadsheet::pagesizeA3Paper:
			width	= odf_types::length(297, odf_types::length::mm);
			height	= odf_types::length(420, odf_types::length::mm);
			break;
		case SimpleTypes::Spreadsheet::pagesizeA4Paper:
			width	= odf_types::length(210, odf_types::length::mm);
			height	= odf_types::length(297, odf_types::length::mm);
			break;
		default:
			break;
		//todooo
		}
	}
	ods_context->page_layout_context()->set_page_size(width, height);

	if (oox_page->m_oScale.IsInit())
	{
		ods_context->page_layout_context()->set_page_scale(oox_page->m_oScale->GetValue());
	}
	if ((oox_page->m_oUseFirstPageNumber.IsInit()) && (oox_page->m_oUseFirstPageNumber->ToBool()))
	{
		if (oox_page->m_oFirstPageNumber.IsInit())
			ods_context->page_layout_context()->set_page_first_page(oox_page->m_oFirstPageNumber->GetValue());
	}
}

void XlsxConverter::convert(OOX::Spreadsheet::CPageMargins *oox_page)
{
	if (!oox_page) return;
	_CP_OPT(odf_types::length) top, left, right, bottom, other;

	if (oox_page->m_oTop.IsInit())		top		= odf_types::length(oox_page->m_oTop->GetValue(), odf_types::length::pt);
	if (oox_page->m_oLeft.IsInit())		left	= odf_types::length(oox_page->m_oLeft->GetValue(), odf_types::length::pt);
	if (oox_page->m_oRight.IsInit())	right	= odf_types::length(oox_page->m_oRight->GetValue(), odf_types::length::pt);
	if (oox_page->m_oBottom.IsInit())	bottom	= odf_types::length(oox_page->m_oBottom->GetValue(), odf_types::length::pt);

	ods_context->page_layout_context()->set_page_margin(top, left, bottom, right);
	
	if (oox_page->m_oFooter.IsInit())
	{
		other = odf_types::length(oox_page->m_oFooter->GetValue(), odf_types::length::pt);
		ods_context->page_layout_context()->set_footer_size(other, other);
	}
	
	if (oox_page->m_oHeader.IsInit())
	{
		other = odf_types::length(oox_page->m_oHeader->GetValue(), odf_types::length::pt);
		ods_context->page_layout_context()->set_header_size(other, other);
	}

}
void XlsxConverter::convert(OOX::Spreadsheet::CSheetFormatPr *oox_sheet_format_pr)
{
	if (!oox_sheet_format_pr)return;
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
					width = *oox_sheet_format_pr->m_oDefaultColWidth;
					width = ods_context->convert_symbol_width(width) + 5 * 3 / 4.;
					//defaultColWidth = baseColumnWidth + {margin padding (2 pixels on each side, totalling 4 pixels)} + {gridline (1pixel)}
				}
				else if (oox_sheet_format_pr->m_oBaseColWidth.IsInit())
				{
					width = ods_context->convert_symbol_width(*oox_sheet_format_pr->m_oBaseColWidth);
				}
				else
				{
					width = ods_context->convert_symbol_width(8.43) + 5 * 3 / 4.;
				}
				ods_context->current_table()->defaut_column_width_ = width;//pt
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
					double height = *oox_sheet_format_pr->m_oDefaultRowHeight;

					ods_context->current_table()->defaut_row_height_ = height;//pt
					row_properties->style_table_row_properties_attlist_.style_row_height_ = odf_types::length(odf_types::length(height,odf_types::length::pt).get_value_unit(odf_types::length::cm),odf_types::length::cm);
				}
				//row_properties->style_table_row_properties_attlist_.style_use_optimal_row_height_ = true; - UTF-8''Отчет о развертывании-1.xlsx
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
	OOX::Spreadsheet::CStyles *xlsx_styles = NULL;
	
	if (xlsx_document)		xlsx_styles = xlsx_document->m_pStyles;
	if (xlsx_flat_document) xlsx_styles = xlsx_flat_document->m_pStyles.GetPointer();
	
	if (!xlsx_styles)return;
//todooo ?? стоит ли обращать на параметр Count ??
////////////форматы данных
	for (size_t i = 0; xlsx_styles->m_oNumFmts.IsInit() && i < xlsx_styles->m_oNumFmts->m_arrItems.size(); i++)
	{
		convert(xlsx_styles->m_oNumFmts->m_arrItems[i]);
	}
/////////////стили ячеек
	for (size_t i = 0; xlsx_styles->m_oCellStyleXfs.IsInit() && i < xlsx_styles->m_oCellStyleXfs->m_arrItems.size(); i++)
	{
		//automatical, root - noname - они тока для named
		convert(xlsx_styles->m_oCellStyleXfs->m_arrItems[i] , i, true, true);
	}
	for (size_t i = 0; xlsx_styles->m_oCellStyles.IsInit() && i < xlsx_styles->m_oCellStyles->m_arrItems.size(); i++)//styles.xml
	{
		//non automatical, root - named 
		convert(xlsx_styles->m_oCellStyles->m_arrItems[i]); 
	}	

	//кастомные стили ячеек
	for (size_t i = 0; xlsx_styles->m_oCellXfs.IsInit() && i < xlsx_styles->m_oCellXfs->m_arrItems.size(); i++)
	{
		//automatical, non root
		convert(xlsx_styles->m_oCellXfs->m_arrItems[i], i, true, false);
	}	

////////////стили условного форматирования 
	for (size_t i=0; xlsx_styles->m_oDxfs.IsInit() && i < xlsx_styles->m_oDxfs->m_arrItems.size(); i++)
	{
		convert(xlsx_styles->m_oDxfs->m_arrItems[i], i); 
	}
}



void XlsxConverter::convert(OOX::Spreadsheet::CFont *font, odf_writer::style_text_properties *text_properties)
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
	if (font->m_oItalic.IsInit() && (font->m_oItalic->m_oVal.ToBool() ==true)) font_italic = true;

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

	ods_context->add_font(font_name);
	ods_context->calculate_font_metrix(font_name, font_size, font_italic, font_bold);
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
	{//solid, gradient, none
		if (fill->m_oPatternFill->m_oFgColor.IsInit())
		{
			convert(fill->m_oPatternFill->m_oFgColor.GetPointer(), 
				cell_properties->content_.common_background_color_attlist_.fo_background_color_);
		}
		if (fill->m_oPatternFill->m_oBgColor.IsInit() && !cell_properties->content_.common_background_color_attlist_.fo_background_color_)
		{
			convert(fill->m_oPatternFill->m_oBgColor.GetPointer(), 
				cell_properties->content_.common_background_color_attlist_.fo_background_color_);
		}
		if (fill->m_oPatternFill->m_oPatternType.IsInit() && !cell_properties->content_.common_background_color_attlist_.fo_background_color_)
		{
			switch(fill->m_oPatternFill->m_oPatternType->GetValue())
			{
				case SimpleTypes::Spreadsheet::patterntypeNone:
					cell_properties->content_.common_background_color_attlist_.fo_background_color_ = 
						odf_types::background_color::Transparent;
						break;

				case SimpleTypes::Spreadsheet::patterntypeDarkDown:
				case SimpleTypes::Spreadsheet::patterntypeDarkGray:
				case SimpleTypes::Spreadsheet::patterntypeDarkGrid:
				case SimpleTypes::Spreadsheet::patterntypeDarkHorizontal:
				case SimpleTypes::Spreadsheet::patterntypeDarkTrellis:
				case SimpleTypes::Spreadsheet::patterntypeDarkUp:
				case SimpleTypes::Spreadsheet::patterntypeDarkVertical:
				case SimpleTypes::Spreadsheet::patterntypeGray0625:
				case SimpleTypes::Spreadsheet::patterntypeGray125:
				case SimpleTypes::Spreadsheet::patterntypeLightDown:
				case SimpleTypes::Spreadsheet::patterntypeLightGray:
				case SimpleTypes::Spreadsheet::patterntypeLightGrid:
				case SimpleTypes::Spreadsheet::patterntypeLightHorizontal:
				case SimpleTypes::Spreadsheet::patterntypeLightTrellis:
				case SimpleTypes::Spreadsheet::patterntypeLightUp:
				case SimpleTypes::Spreadsheet::patterntypeLightVertical:
				case SimpleTypes::Spreadsheet::patterntypeMediumGray:
					// todooo
					break;
			}
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
		ods_context->numbers_styles_context()->add_or_find(numFmt->m_oNumFmtId->GetValue(), *numFmt->m_oFormatCode);
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CAligment *aligment, odf_writer::style_paragraph_properties	* paragraph_properties
																 , odf_writer::style_table_cell_properties * cell_properties)
{
	if (!aligment)return;

	bool rtl = false;
	if (aligment->m_oReadingOrder.IsInit() && (*aligment->m_oReadingOrder == 1))
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
			cell_properties->content_.style_vertical_align_ = paragraph_properties->content_.style_vertical_align_;
	}
	if (aligment->m_oTextRotation.IsInit())
	{
		if (*aligment->m_oTextRotation <= 180 && *aligment->m_oTextRotation >= 0 )
		{
			cell_properties->content_.common_rotation_angle_attlist_.style_rotation_angle_ = *aligment->m_oTextRotation;
			cell_properties->content_.style_rotation_align_= odf_types::rotation_align(odf_types::rotation_align::Bottom);
		}
		else if (*aligment->m_oTextRotation == 0xff)//вертикальный текст
			cell_properties->content_.style_direction_ = odf_types::direction(odf_types::direction::Ttb);

	}
	_CP_OPT(odf_types::length) indent;
	if (aligment->m_oIndent.IsInit())
	{
		indent = odf_types::length(ods_context->convert_symbol_width(*aligment->m_oIndent), odf_types::length::pt);
	}
	if(aligment->m_oHorizontal.IsInit())
	{
		switch (aligment->m_oHorizontal->GetValue())
		{
			case SimpleTypes::Spreadsheet::horizontalalignmentCenter:
			{
				paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Center);
			}break;
			case SimpleTypes::Spreadsheet::horizontalalignmentJustify:
			{
				paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Justify);
			}break;
			case SimpleTypes::Spreadsheet::horizontalalignmentRight:
			{
				paragraph_properties->content_.fo_margin_right_ = indent;
				paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::End);
			}break;
			case SimpleTypes::Spreadsheet::horizontalalignmentFill:
			case SimpleTypes::Spreadsheet::horizontalalignmentLeft:
			default:
			{
				paragraph_properties->content_.fo_margin_left_ = indent;
				paragraph_properties->content_.fo_text_align_ = odf_types::text_align(odf_types::text_align::Start); 
			}break;
		}
		cell_properties->content_.style_text_align_source_ = odf_types::text_align_source(odf_types::text_align_source::Fix);
	}

	if (aligment->m_oWrapText.IsInit())
	{
		if (aligment->m_oWrapText->GetValue()) 
			cell_properties->content_.fo_wrap_option_ = odf_types::wrap_option(odf_types::wrap_option::Wrap);
	}
	if (aligment->m_oShrinkToFit.IsInit())
	{
		cell_properties->content_.style_shrink_to_fit_ = aligment->m_oShrinkToFit->ToBool();
	}

		//nullable<SimpleTypes::COnOff<>>									m_oJustifyLastLine;
		//nullable<SimpleTypes::CDecimalNumber<>>							m_oRelativeIndent;

}
void XlsxConverter::convert(OOX::Spreadsheet::CProtection *protection, odf_writer::style_table_cell_properties *cell_properties)
{
	if (!protection)return;
	if (!cell_properties)return;

	bool bHidden = protection->m_oHidden.IsInit() ? protection->m_oHidden->ToBool() : false;

	if (protection->m_oLocked.IsInit())
	{
		if (protection->m_oLocked->ToBool())
			cell_properties->content_.style_cell_protect_ = odf_types::style_cell_protect(odf_types::style_cell_protect::protected_);
		else 
			if (false == bHidden)
				cell_properties->content_.style_cell_protect_ = odf_types::style_cell_protect(odf_types::style_cell_protect::none);
	}
	if (bHidden)
	{
		if (cell_properties->content_.style_cell_protect_)
			cell_properties->content_.style_cell_protect_ = odf_types::style_cell_protect(odf_types::style_cell_protect::protected_formula_hidden);
		else
			cell_properties->content_.style_cell_protect_ = odf_types::style_cell_protect(odf_types::style_cell_protect::formula_hidden);
	}

}
void XlsxConverter::convert(OOX::Spreadsheet::CBorder *oox_border, odf_writer::style_table_cell_properties * table_cell_properties)
{
	if (!oox_border)return;

	std::wstring left,right,top,bottom, other;

	convert(oox_border->m_oBottom.GetPointer()	, bottom);
	convert(oox_border->m_oTop.GetPointer()		, top);
	convert(oox_border->m_oStart.GetPointer()	, left);
	convert(oox_border->m_oEnd.GetPointer()		, right);
	
	if (bottom == top && top == left && left == right && !bottom.empty())
	{
		table_cell_properties->content_.common_border_attlist_.fo_border_ = left;
	}
	else
	{
		if (!bottom.empty())table_cell_properties->content_.common_border_attlist_.fo_border_bottom_	= bottom;
		if (!top.empty())	table_cell_properties->content_.common_border_attlist_.fo_border_top_		= top;
		if (!left.empty())	table_cell_properties->content_.common_border_attlist_.fo_border_left_		= left;
		if (!right.empty())	table_cell_properties->content_.common_border_attlist_.fo_border_right_		= right;
	}

	convert(oox_border->m_oDiagonal.GetPointer(), other);
	
	if (oox_border->m_oDiagonalDown.IsInit() && !other.empty()) //and true???
		table_cell_properties->content_.style_diagonal_tl_br_= other;
	if (oox_border->m_oDiagonalUp.IsInit() && !other.empty()) //and true???
		table_cell_properties->content_.style_diagonal_bl_tr_= other;
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
	else
	{
		border_style = L"none";
		if (borderProp->m_oColor.IsInit())
			return;
	}

	if (borderProp->m_oColor.IsInit())
	{
		_CP_OPT(odf_types::color) odf_color;
		convert(borderProp->m_oColor.GetPointer(), odf_color);
		if (odf_color)
			border_color = L" #" + odf_color->get_hex_value();
		
		if (border_color.empty())border_color = L" #000000";
	}
	
	odf_border_prop = border_style + border_color;
}
void XlsxConverter::convert(OOX::Spreadsheet::CColor *color, _CP_OPT(odf_types::color) & odf_color)//стоит ли сюда тащить odf_writer type???
{
	if (!color)return;

	unsigned char ucA = 0, ucR =0, ucG =0, ucB = 0;
	bool result = false;
	
	if(color->m_oRgb.IsInit())//easy, faster,realy  !!
	{
		ucR = color->m_oRgb->Get_R(); 
		ucB = color->m_oRgb->Get_B(); 
		ucG = color->m_oRgb->Get_G(); 
		ucA = color->m_oRgb->Get_A(); 
		result = true;
	}
	
	if(color->m_oThemeColor.IsInit() && xlsx_document->m_pTheme.IsInit())
	{
		DWORD bgra = xlsx_document->m_pTheme->themeElements.clrScheme.GetARGBFromScheme(color->m_oThemeColor->ToString());
		
		ucB = (bgra & 0x0000FF); 
		ucG = (bgra & 0x00FF00)	>> 8; 
		ucR = (bgra & 0xFF0000)	>> 16; 
		ucA = bgra >> 24; 
		
		result = true;
	}

	if(color->m_oIndexed.IsInit())
	{
		OOX::Spreadsheet::CStyles * xlsx_styles = xlsx_document->m_pStyles;
	
		int ind = color->m_oIndexed->GetValue();

		if(xlsx_styles->m_oColors.IsInit() && xlsx_styles->m_oColors->m_oIndexedColors.IsInit() )
		{
			std::map<int, OOX::Spreadsheet::CRgbColor*>::iterator pFind = xlsx_styles->m_oColors->m_oIndexedColors->mapIndexedColors.find(ind);
			
			if (pFind != xlsx_styles->m_oColors->m_oIndexedColors->mapIndexedColors.end())
			{			
				ucR = pFind->second->m_oRgb->Get_R(); 
				ucB = pFind->second->m_oRgb->Get_B(); 
				ucG = pFind->second->m_oRgb->Get_G(); 
				ucA = pFind->second->m_oRgb->Get_A(); 
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
	convert(dxFmt->m_oProtection.GetPointer(), table_cell_properties); 
}
void XlsxConverter::convert(OOX::Spreadsheet::CXfs * xfc_style, int oox_id, bool automatic, bool root)
{
	OOX::Spreadsheet::CStyles *xlsx_styles = NULL;
	
	if (xlsx_document)		xlsx_styles = xlsx_document->m_pStyles;
	if (xlsx_flat_document) xlsx_styles = xlsx_flat_document->m_pStyles.GetPointer();
	
	if (!xlsx_styles)return;

	int id_parent	= xfc_style->m_oXfId.IsInit()		? xfc_style->m_oXfId->GetValue()	: -1; 
	int fill_id		= xfc_style->m_oFillId.IsInit()		? xfc_style->m_oFillId->GetValue()	: -1;
	int numFmt_id	= xfc_style->m_oNumFmtId.IsInit()	? xfc_style->m_oNumFmtId->GetValue(): -1; 
	int font_id		= xfc_style->m_oFontId.IsInit()		? xfc_style->m_oFontId->GetValue()	: -1;
	int border_id	= xfc_style->m_oBorderId.IsInit()	? xfc_style->m_oBorderId->GetValue(): -1;
		
	ods_context->styles_context()->create_style(L"", odf_types::style_family::TableCell, automatic, root, oox_id); 
	
	odf_writer::style_text_properties		* text_properties		= ods_context->styles_context()->last_state()->get_text_properties();
	odf_writer::style_table_cell_properties	* table_cell_properties = ods_context->styles_context()->last_state()->get_table_cell_properties();

	bool bApplyFont = xfc_style->m_oApplyFont.IsInit() ? xfc_style->m_oApplyFont->ToBool() : true;
	if (xlsx_styles->m_oFonts.IsInit() && font_id >=0 && (id_parent < 0 || bApplyFont))	
	{
		std::map<int, OOX::Spreadsheet::CFont*>::iterator pFind = xlsx_styles->m_oFonts->m_mapFonts.find(font_id);
		if (pFind != xlsx_styles->m_oFonts->m_mapFonts.end())
		{
			convert(pFind->second, text_properties); 
		}
	}
	bool bApplyFill = xfc_style->m_oApplyFill.IsInit() ? xfc_style->m_oApplyFill->ToBool() : true;
	if (xlsx_styles->m_oFills.IsInit() && fill_id >= 0 && (id_parent < 0 || bApplyFill))
	{
		std::map<int, OOX::Spreadsheet::CFill*>::iterator pFind = xlsx_styles->m_oFills->m_mapFills.find(fill_id);
		if (pFind != xlsx_styles->m_oFills->m_mapFills.end())
		{
			convert(pFind->second, table_cell_properties); 
		}
	}
	bool bApplyNumberFormat = xfc_style->m_oApplyNumberFormat.IsInit() ? xfc_style->m_oApplyNumberFormat->ToBool() : true;
	if (numFmt_id >= 0 && (id_parent < 0 || bApplyNumberFormat))	
	{
		ods_context->styles_context()->last_state()->set_data_style_name(ods_context->numbers_styles_context()->add_or_find(numFmt_id).style_name);
		ods_context->styles_context()->last_state()->set_number_format(numFmt_id);
	}
	bool bApplyBorder = xfc_style->m_oApplyBorder.IsInit() ? xfc_style->m_oApplyBorder->ToBool() : true;
	if (xlsx_styles->m_oBorders.IsInit() && border_id >=0 && (id_parent < 0 || bApplyBorder))	
	{
		std::map<int, OOX::Spreadsheet::CBorder*>::iterator pFind = xlsx_styles->m_oBorders->m_mapBorders.find(border_id);
		if (pFind != xlsx_styles->m_oBorders->m_mapBorders.end())
		{
			convert(pFind->second, table_cell_properties); 
		}
	}
	
	odf_writer::style_paragraph_properties	* paragraph_properties = ods_context->styles_context()->last_state()->get_paragraph_properties();
	paragraph_properties->content_.style_writing_mode_ = odf_types::writing_mode(odf_types::writing_mode::Page);

	if (xfc_style->m_oAligment.IsInit() && xfc_style->m_oApplyAlignment.IsInit())
	{
		convert(xfc_style->m_oAligment.GetPointer(), paragraph_properties, table_cell_properties);
	}
	if (xfc_style->m_oProtection.IsInit())
	{
		convert(xfc_style->m_oProtection.GetPointer(), table_cell_properties);
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

////////////////// 
	if (oox_anchor->m_oFrom.IsInit() || oox_anchor->m_oTo.IsInit() || 
		oox_anchor->m_oPos.IsInit() || oox_anchor->m_oExt.IsInit())
	{
		oox_table_position from={}, to={};
		
		double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

		if (oox_anchor->m_oFrom.IsInit())
		{
			convert(oox_anchor->m_oFrom.GetPointer(), &from);	
			ods_context->current_table()->convert_position(from, x1, y1);
		}
		else if (oox_anchor->m_oPos.IsInit()) 
		{
			if (oox_anchor->m_oPos->m_oX.IsInit())
				x1 = oox_anchor->m_oPos->m_oX->GetValue();
			if (oox_anchor->m_oPos->m_oY.IsInit())
				y1 = oox_anchor->m_oPos->m_oY->GetValue();
		}
		if (oox_anchor->m_oTo.IsInit())
		{
			convert(oox_anchor->m_oTo.GetPointer(), &to);
			ods_context->current_table()->convert_position(to, x2, y2);
		}
		else if (oox_anchor->m_oExt.IsInit())
		{
			if (oox_anchor->m_oExt->m_oCx.IsInit())
				x2 = x1 + oox_anchor->m_oExt->m_oCx->GetValue();
			if (oox_anchor->m_oExt->m_oCy.IsInit())
				y2 = y1 + oox_anchor->m_oExt->m_oCy->GetValue();
		}
		
		ods_context->drawing_context()->set_drawings_rect(x1, y1, x2 - x1, y2 - y1);
	}
		
//////////////
	odf_context()->drawing_context()->start_drawing();
		if (oox_anchor->m_oPos.IsInit())
		{
			_CP_OPT(double) x, y;

			if (oox_anchor->m_oPos->m_oX.IsInit())	x = oox_anchor->m_oPos->m_oX->GetValue();
			if (oox_anchor->m_oPos->m_oY.IsInit())	y = oox_anchor->m_oPos->m_oY->GetValue();
	
			odf_context()->drawing_context()->set_position( x, y);
		}

		if (oox_anchor->m_oExt.IsInit())
		{
			_CP_OPT(double) width, height;

			if (oox_anchor->m_oExt->m_oCx.IsInit())	width	= oox_anchor->m_oExt->m_oCx->GetValue();
			if (oox_anchor->m_oExt->m_oCy.IsInit())	height	= oox_anchor->m_oExt->m_oCy->GetValue();
			
			odf_context()->drawing_context()->set_size(	width, height);					
		}
		OoxConverter::convert(oox_anchor->m_oElement.GetPointer());
	odf_context()->drawing_context()->end_drawing();
}

void XlsxConverter::convert(OOX::Spreadsheet::CDrawing *oox_drawing, OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (!oox_drawing)return;
	
	OOX::IFileContainer* old_container = xlsx_current_container;
	xlsx_current_container = dynamic_cast<OOX::IFileContainer*>(oox_drawing);

	for (size_t i = 0; i < oox_drawing->m_arrItems.size(); i++)
	{
		OOX::Spreadsheet::CCellAnchor * oox_anchor = oox_drawing->m_arrItems[i];
		
		if (oox_anchor->m_bShapeOle) continue; 
		if (oox_anchor->m_bShapeControl) continue; 

		if (oox_sheet->m_oOleObjects.IsInit() && oox_anchor->m_nId.IsInit())
		{
            boost::unordered_map<unsigned int, OOX::Spreadsheet::COleObject*>::const_iterator pFind = oox_sheet->m_oOleObjects->m_mapOleObjects.find(oox_anchor->m_nId.get());
			if (pFind != oox_sheet->m_oOleObjects->m_mapOleObjects.end())
			{
				//??? перенести даные привязки 
				oox_anchor->m_bShapeOle = true;
				continue;
			}
		}
		if (oox_sheet->m_oControls.IsInit() && oox_anchor->m_nId.IsInit())
		{
            std::map<unsigned int, nullable<OOX::Spreadsheet::CControl>>::const_iterator pFind = oox_sheet->m_oControls->m_mapControls.find(oox_anchor->m_nId.get());
			if (pFind != oox_sheet->m_oControls->m_mapControls.end())
			{
				//??? перенести даные привязки 
				oox_anchor->m_bShapeControl = true;
				continue;
			}
		}
		convert(oox_anchor);
		ods_context->drawing_context()->clear();
	}

	xlsx_current_container = old_container;
}
void XlsxConverter::convert(OOX::Spreadsheet::COleObjects *oox_objects, OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (!oox_objects) return;

    for (boost::unordered_map<unsigned int, OOX::Spreadsheet::COleObject*>::const_iterator it = oox_objects->m_mapOleObjects.begin(); it != oox_objects->m_mapOleObjects.end(); ++it)
	{
		OOX::Spreadsheet::COleObject* object = it->second;

		bool bAnchor = false;
		std::wstring odf_ref_object, odf_ref_image;
		if (object->m_oObjectPr.IsInit())
		{
			if (object->m_oObjectPr->m_oAnchor.IsInit())
			{
				bAnchor = true;
				oox_table_position from = {}, to = {};
				
				convert(object->m_oObjectPr->m_oAnchor->m_oFrom.GetPointer(),	&from);	
				convert(object->m_oObjectPr->m_oAnchor->m_oTo.GetPointer(),		&to);

				double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
				ods_context->current_table()->convert_position(from, x1, y1);
				ods_context->current_table()->convert_position(to,	x2, y2);
				
				ods_context->drawing_context()->set_drawings_rect(x1, y1, x2 - x1, y2 - y1);
			}
		}
		bool bExternal = false;
		if (object->m_oRid.IsInit())
		{
			std::wstring sID = object->m_oRid->GetValue();
			
			std::wstring pathOle = find_link_by_id(sID, 4, bExternal);

			odf_ref_object = odf_context()->add_oleobject(pathOle);
		}
		if ((object->m_oObjectPr.IsInit()) && (object->m_oObjectPr->m_oRid.IsInit()))
		{
			std::wstring sID = object->m_oObjectPr->m_oRid->GetValue();
			
			std::wstring pathImage = find_link_by_id(sID, 1, bExternal);
					
			odf_ref_image = odf_context()->add_imageobject(pathImage);
		}
//--------------------------------------------------------------------------------------------------
		if ((!bAnchor || odf_ref_image.empty()) && object->m_oShapeId.IsInit())
		{
			std::wstring sShapeId = L"_x0000_s" + std::to_wstring(object->m_oShapeId->GetValue());

			smart_ptr<OOX::WritingElement>	oElement	= oox_sheet->FindVmlObject(sShapeId);
			smart_ptr<OOX::Vml::CShape> oShape		= oElement.smart_dynamic_cast<OOX::Vml::CShape>();
			
			for(size_t j = 0; (oShape.IsInit()) && (j < oShape->m_arrItems.size()); ++j)
			{
				OOX::WritingElement* pChildElemShape = oShape->m_arrItems[j];
				
				if(!bAnchor && OOX::et_v_ClientData == pChildElemShape->getType())
				{
					OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pChildElemShape);

					SimpleTypes::Spreadsheet::CCellAnchorType eAnchorType;
					eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);

					OOX::Spreadsheet::CCellAnchor *pCellAnchor = new OOX::Spreadsheet::CCellAnchor(eAnchorType);					
					if (pClientData->toCellAnchor(pCellAnchor))
					{
						oox_table_position from = {}, to = {};
						
						convert(pCellAnchor->m_oFrom.GetPointer(), &from);
						convert(pCellAnchor->m_oTo.GetPointer(), &to);
						
						double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
						ods_context->current_table()->convert_position(from, x1, y1);
						ods_context->current_table()->convert_position(to,	x2, y2);
						
						ods_context->drawing_context()->set_drawings_rect(x1, y1, x2 - x1, y2 - y1);
					}
					delete pCellAnchor;
				}
				if(OOX::et_v_imagedata == pChildElemShape->getType())
				{
					OOX::Vml::CImageData* pImageData = static_cast<OOX::Vml::CImageData*>(pChildElemShape);									
										
					std::wstring sIdImageFileCache;
					if (pImageData->m_oRelId.IsInit())		sIdImageFileCache = pImageData->m_oRelId->GetValue();
					else if (pImageData->m_rId.IsInit())	sIdImageFileCache = pImageData->m_rId->GetValue();
					else if (pImageData->m_rPict.IsInit())	sIdImageFileCache = pImageData->m_rPict->GetValue();
														
					if (!sIdImageFileCache.empty())
					{
						//ищем физический файл ( rId относительно vml_drawing)									
						smart_ptr<OOX::CVmlDrawing>	oVmlDrawing = oox_sheet->Find(oox_sheet->m_oLegacyDrawing->m_oId->GetValue()).smart_dynamic_cast<OOX::CVmlDrawing>();
						smart_ptr<OOX::File> pFile = oVmlDrawing->Find(sIdImageFileCache);
						
						if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
						{
							OOX::Image* pImageFileCache = static_cast<OOX::Image*>(pFile.GetPointer());
							
							if (pImageFileCache && odf_ref_image.empty())
							{
								odf_ref_image = odf_context()->add_imageobject(pImageFileCache->filename().GetPath());
							}
						}
					}
				}
			}
		}
//--------------------------------------------------------------------------------------------------
		ods_context->drawing_context()->start_drawing();

		ods_context->drawing_context()->start_object_ole(odf_ref_object);
		
		ods_context->drawing_context()->set_image_replacement(odf_ref_image);

		if (object->m_oProgId.IsInit())
		{
			ods_context->drawing_context()->set_program(*object->m_oProgId);
		}

		ods_context->drawing_context()->end_object_ole();
		ods_context->drawing_context()->end_drawing();

		ods_context->drawing_context()->clear();
	}
}


void XlsxConverter::convert(OOX::Spreadsheet::CControls *oox_controls, OOX::Spreadsheet::CWorksheet *oox_sheet)
{
	if (!oox_controls) return;

    for (std::map<unsigned int, nullable<OOX::Spreadsheet::CControl>>::const_iterator it = oox_controls->m_mapControls.begin(); it != oox_controls->m_mapControls.end(); ++it)
	{
		OOX::Spreadsheet::CControl* pControl = it->second.GetPointer();
		if (!pControl) continue;
	
		OOX::WritingElement* pShapeElem	= NULL;
		
		nullable<OOX::Spreadsheet::CCellAnchor>		oCellAnchor;
		nullable<OOX::Spreadsheet::CFormControlPr>	oFormControlPr;
        
		smart_ptr<OOX::File> pFileControl;

		if (pControl->m_oRid.IsInit())
		{			
			pFileControl = oox_sheet->Find(OOX::RId(pControl->m_oRid->GetValue()));// rId относительно sheet
		}
		if (false == pFileControl.IsInit()) continue;
		
		bool bSetAnchor = false;

		smart_ptr<OOX::Spreadsheet::CCtrlPropFile> pFileCtrlProp = pFileControl.smart_dynamic_cast<OOX::Spreadsheet::CCtrlPropFile>();
		
		if (pFileCtrlProp.IsInit())
		{
			oFormControlPr = pFileCtrlProp->m_oFormControlPr;
		}
		else
		{
			oFormControlPr.Init();
			smart_ptr<OOX::ActiveX_xml> pActiveX_xml = pFileControl.smart_dynamic_cast<OOX::ActiveX_xml>();

			if ((pActiveX_xml.IsInit()) && (pActiveX_xml->m_oObject.IsInit()))
			{
				if (pActiveX_xml->m_oObject->m_oObjectType.IsInit() == false)
				{
					OOX::ActiveXObjectImage* pImage = dynamic_cast<OOX::ActiveXObjectImage*>(pActiveX_xml->m_oObject.GetPointer());

					if (pImage)
					{
						//todooo
					}
					continue;
				}

				pActiveX_xml->m_oObject->toFormControlPr(oFormControlPr.GetPointer());
			}
		}		
		std::wstring sShapeId = L"_x0000_s" + std::to_wstring(pControl->m_oShapeId->GetValue());

		smart_ptr<OOX::WritingElement> oObjectElement = oox_sheet->FindVmlObject(sShapeId);
		smart_ptr<OOX::Vml::CShape> oShape = oObjectElement.smart_dynamic_cast<OOX::Vml::CShape>();

		SimpleTypes::Spreadsheet::CCellAnchorType eAnchorType;
		eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
		
		if (pControl->m_oControlPr.IsInit() && pControl->m_oControlPr->m_oAnchor.IsInit())
		{						
			const OOX::Spreadsheet::CExtAnchor& oAnchor = pControl->m_oControlPr->m_oAnchor.get();
			
			if (oAnchor.m_oFrom.IsInit() && oAnchor.m_oTo.IsInit())
			{
				if(oAnchor.m_oMoveWithCells.IsInit() && *oAnchor.m_oMoveWithCells)
					eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
				else if(oAnchor.m_oSizeWithCells.IsInit() && *oAnchor.m_oSizeWithCells)
					eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
				else
					eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
				
				oCellAnchor.reset(new OOX::Spreadsheet::CCellAnchor(eAnchorType));

				oCellAnchor->m_oFrom = oAnchor.m_oFrom.get();
				oCellAnchor->m_oTo = oAnchor.m_oTo.get();
				
				bSetAnchor = true;
			}
		}
		if (false == oCellAnchor.IsInit())
		{
			oCellAnchor.reset(new OOX::Spreadsheet::CCellAnchor(eAnchorType));
			oCellAnchor->m_bShapeControl = true;			
		}
		if ((oShape.IsInit()) && (OOX::et_v_shapetype != oShape->getType()))
		{
			for(size_t j = 0; j < oShape->m_arrItems.size(); ++j)
			{
				OOX::WritingElement* pChildElemShape = oShape->m_arrItems[j];
				
				if (OOX::et_v_ClientData == pChildElemShape->getType())
				{
					OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pChildElemShape);

					if (!bSetAnchor )
					{						
						bSetAnchor = pClientData->toCellAnchor(oCellAnchor.GetPointer());
					}
					pClientData->toFormControlPr(oFormControlPr.GetPointer());
				}
				else if (OOX::et_v_textbox == pChildElemShape->getType())
				{
					OOX::Vml::CTextbox* pTextbox = static_cast<OOX::Vml::CTextbox*>(pChildElemShape);

					oFormControlPr->m_oText = pTextbox->m_oText;
				}
			}	
		}
		if (!oCellAnchor.IsInit() || !oFormControlPr.IsInit())
		{
			continue;
		}
		if (!oFormControlPr->m_oObjectType.IsInit() )
		{
			continue;
		}
//---------------------------------------------
		std::wstring id = ods_context->controls_context()->start_control((int)oFormControlPr->m_oObjectType->GetValue(), oFormControlPr->m_oItemLst.IsInit());

		if (false == id.empty())
		{
			{
				oox_table_position from = {}, to = {};
				
				convert(oCellAnchor->m_oFrom.GetPointer(), &from);	
				convert(oCellAnchor->m_oTo.GetPointer(), &to);

				double x1 = 0, y1 = 0, x2 = 0, y2 = 0;
				ods_context->current_table()->convert_position(from, x1, y1);
				ods_context->current_table()->convert_position(to,	x2, y2);
				
				ods_context->drawing_context()->set_drawings_rect(x1, y1, x2 - x1, y2 - y1);
			}		
			ods_context->drawing_context()->start_drawing();
			ods_context->drawing_context()->start_control(id);
		
			if (pControl->m_oName.IsInit())
			{
				ods_context->controls_context()->set_name(*pControl->m_oName);
				ods_context->drawing_context()->set_name(*pControl->m_oName);
			}
//----------------------
			if (oFormControlPr->m_oText.IsInit())
			{
				ods_context->controls_context()->set_label(*oFormControlPr->m_oText);
			}
			if (oFormControlPr->m_oFillColor.IsInit())
			{
				ods_context->drawing_context()->start_area_properties(true);
					ods_context->drawing_context()->set_solid_fill(oFormControlPr->m_oFillColor->ToString());
				ods_context->drawing_context()->end_area_properties();
			}
			if (oFormControlPr->m_oBorderColor.IsInit())
			{
				ods_context->drawing_context()->start_line_properties();
					ods_context->drawing_context()->set_line_color(oFormControlPr->m_oBorderColor->ToString());
				ods_context->drawing_context()->end_line_properties();
			}
			if (oFormControlPr->m_oTextHAlign.IsInit())
			{
				ods_context->controls_context()->set_textHAlign(oFormControlPr->m_oTextHAlign->GetValue());
			}
			if (oFormControlPr->m_oTextVAlign.IsInit())
			{
				ods_context->controls_context()->set_textVAlign(oFormControlPr->m_oTextVAlign->GetValue());
			}
			if (oFormControlPr->m_oMin.IsInit())
			{
				ods_context->controls_context()->set_min_value(oFormControlPr->m_oMin->GetValue());
			}
			if (oFormControlPr->m_oMax.IsInit())
			{
				ods_context->controls_context()->set_max_value(oFormControlPr->m_oMax->GetValue());
			}
			if (oFormControlPr->m_oPage.IsInit())
			{
				ods_context->controls_context()->set_page_step(oFormControlPr->m_oPage->GetValue());
			}
			if (oFormControlPr->m_oInc.IsInit())
			{
				ods_context->controls_context()->set_step(oFormControlPr->m_oInc->GetValue());
			}
			if (oFormControlPr->m_oVal.IsInit())
			{
				ods_context->controls_context()->set_value(std::to_wstring(*oFormControlPr->m_oVal));
			}
			if (oFormControlPr->m_oHoriz.IsInit())
			{
				ods_context->controls_context()->set_horiz(*oFormControlPr->m_oHoriz);
			}
			if (oFormControlPr->m_oVerticalBar.IsInit())
			{
				ods_context->controls_context()->set_horiz(*oFormControlPr->m_oVerticalBar == false);
			}
			if (oFormControlPr->m_oFmlaLink.IsInit())
			{
				ods_context->controls_context()->set_linkedCell(*oFormControlPr->m_oFmlaLink);
			}
			if (oFormControlPr->m_oFmlaRange.IsInit())
			{
				ods_context->controls_context()->set_listFillRange(*oFormControlPr->m_oFmlaRange);
			}
			if (oFormControlPr->m_oChecked.IsInit())
			{
				ods_context->controls_context()->set_check_state(oFormControlPr->m_oChecked->GetValue());
			}
			if (oFormControlPr->m_oDropStyle.IsInit())
			{
				ods_context->controls_context()->set_drop_down(true);
			}
			if (oFormControlPr->m_oDropLines.IsInit())
			{
				ods_context->controls_context()->set_drop_size(oFormControlPr->m_oDropLines->GetValue());
			}
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oDx;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oSel;
			//nullable<SimpleTypes::Spreadsheet::CSelType<>>			m_oSelType;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>			m_oWidthMin;
			//nullable<SimpleTypes::Spreadsheet::CEditValidation<>>		m_oEditVal;
			//nullable_string		m_oFmlaGroup;
			//nullable_string		m_oFmlaTxbx;		
			//nullable_bool		m_oColored;
			//nullable_bool		m_oFirstButton;
			//nullable_bool		m_oJustLastX;
			//nullable_bool		m_oLockText;
			//nullable_string		m_oMultiSel;
			//nullable_bool		m_oNoThreeD;
			//nullable_bool		m_oNoThreeD2;
			//nullable_bool		m_oMultiLine;
			//nullable_bool		m_oVerticalBar;
			//nullable_bool		m_oPasswordEdit;
			//nullable<CListItems>							m_oItemLst;
//---------------------
			if (pControl->m_oControlPr->m_oLinkedCell.IsInit())
			{
				ods_context->controls_context()->set_linkedCell(*pControl->m_oControlPr->m_oLinkedCell);
			}
			if (pControl->m_oControlPr->m_oListFillRange.IsInit())
			{
				ods_context->controls_context()->set_listFillRange(*pControl->m_oControlPr->m_oListFillRange);
			}
			if (pControl->m_oControlPr->m_oMacro.IsInit())
			{
				ods_context->controls_context()->set_macro(*pControl->m_oControlPr->m_oMacro);
			}
			if (pControl->m_oControlPr->m_oDisabled.IsInit())
			{
				ods_context->controls_context()->set_disabled(*pControl->m_oControlPr->m_oDisabled);
			}
			if (pControl->m_oControlPr->m_oPrint.IsInit())
			{
				ods_context->controls_context()->set_printable(*pControl->m_oControlPr->m_oPrint);
			}
			if (pControl->m_oControlPr->m_oLocked.IsInit())
			{
			}
			//nullable_string						m_oAltText;
			//nullable_bool							m_oAutoFill;
			//nullable_bool							m_oAutoLine;
			//nullable_bool							m_oAutoPict;
			//nullable_bool							m_oDde;
			//nullable_bool							m_oDefaultSize;
			//nullable_string						m_oCf;
			//nullable_bool							m_oRecalcAlways;
//---------------------
			ods_context->drawing_context()->end_control();
			ods_context->drawing_context()->end_drawing();

			ods_context->drawing_context()->clear();
			ods_context->controls_context()->end_control();
		}
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

void XlsxConverter::convert(OOX::Spreadsheet::CSparklineGroups *sparkline)
{
	if (!sparkline)return;
}
void XlsxConverter::convert(OOX::Spreadsheet::CAltTextTable *alt_text)
{
	if (!alt_text)return;
}

void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormatting *oox_cond_fmt)
{
	if (!oox_cond_fmt)return;

	if (oox_cond_fmt->m_oSqRef.IsInit())	
	{
		ods_context->current_table()->start_conditional_format(oox_cond_fmt->m_oSqRef.get());

		for (size_t i=0; i< oox_cond_fmt->m_arrItems.size(); i++)
			convert(oox_cond_fmt->m_arrItems[i]);//rule
		
		ods_context->current_table()->end_conditional_format();
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormattingRule *oox_cond_rule)
{
	if (!oox_cond_rule)return;

	if (false == oox_cond_rule->m_oType.IsInit()) return;

	_CP_OPT(unsigned int) rank; 
	_CP_OPT(bool) bottom, percent;
	
	if (oox_cond_rule->m_oRank.IsInit()) rank = oox_cond_rule->m_oRank->GetValue();
	if (oox_cond_rule->m_oBottom.IsInit()) bottom = oox_cond_rule->m_oBottom->ToBool();
	if (oox_cond_rule->m_oPercent.IsInit()) percent = oox_cond_rule->m_oPercent->ToBool();

	ods_context->current_table()->start_conditional_rule(oox_cond_rule->m_oType->GetValue(), rank, bottom, percent);
	{
		if (oox_cond_rule->m_oDxfId.IsInit()) 
		{
			std::wstring odf_style_name = odf_context()->styles_context()->find_conditional_style_name(oox_cond_rule->m_oDxfId->GetValue(), odf_types::style_family::TableCell);

			ods_context->current_table()->set_conditional_style_name(odf_style_name);
		}
		else if (oox_cond_rule->m_oDxf.IsInit())
		{
			convert(oox_cond_rule->m_oDxf.GetPointer(), -1);
			
			std::wstring odf_style_name = ods_context->styles_context()->last_state()->get_name();
			ods_context->current_table()->set_conditional_style_name(odf_style_name);
		}
		if (oox_cond_rule->m_oOperator.IsInit()) 
			ods_context->current_table()->set_conditional_operator(oox_cond_rule->m_oOperator->GetValue());

		if (oox_cond_rule->m_oText.IsInit()) 
			ods_context->current_table()->set_conditional_text(*oox_cond_rule->m_oText);

		if (oox_cond_rule->m_oTimePeriod.IsInit())
			ods_context->current_table()->set_conditional_time(oox_cond_rule->m_oTimePeriod->GetValue());
		
		convert(oox_cond_rule->m_oIconSet.GetPointer());
		convert(oox_cond_rule->m_oColorScale.GetPointer());
		convert(oox_cond_rule->m_oDataBar.GetPointer());
		
		for (size_t i = 0; i < oox_cond_rule->m_arrFormula.size(); ++i)
			convert(oox_cond_rule->m_arrFormula[i].GetPointer());
	}	
	ods_context->current_table()->end_conditional_rule();
}
void XlsxConverter::convert(OOX::Spreadsheet::CDataBar *oox_cond_databar)
{
	if (!oox_cond_databar)return;
	
	_CP_OPT(odf_types::color) color;
	convert(oox_cond_databar->m_oColor.GetPointer(), color);

	ods_context->current_table()->set_conditional_databar_color(color);
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			//nullable<SimpleTypes::COnOff<>>					m_oShowValue;
	for (size_t i=0; i< oox_cond_databar->m_arrValues.size(); i++)
		convert(oox_cond_databar->m_arrValues[i].GetPointer());
}
void XlsxConverter::convert(OOX::Spreadsheet::CColorScale *oox_cond_colorscale)
{
	if (!oox_cond_colorscale)return;

	int index = 0;

	for (size_t i = 0; i < oox_cond_colorscale->m_arrValues.size(); ++i)
	{
		convert(oox_cond_colorscale->m_arrValues[i].GetPointer());
	}
	for (size_t i = 0; i < oox_cond_colorscale->m_arrColors.size(); ++i)
	{
		_CP_OPT(odf_types::color) color;
		convert(dynamic_cast<OOX::Spreadsheet::CColor*>(oox_cond_colorscale->m_arrColors[i].GetPointer()), color);

		ods_context->current_table()->add_conditional_colorscale( index++, color );
	}	
}
void XlsxConverter::convert(OOX::Spreadsheet::CIconSet *oox_cond_iconset)
{
	if (!oox_cond_iconset)return;
	
	if (oox_cond_iconset->m_oIconSet.IsInit())
		ods_context->current_table()->set_conditional_iconset(oox_cond_iconset->m_oIconSet->GetValue());
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMaxLength;
			//nullable<SimpleTypes::CUnsignedDecimalNumber<>>	m_oMinLength;
			//nullable<SimpleTypes::COnOff<>>					m_oShowValue;
	for (size_t i=0; i< oox_cond_iconset->m_arrValues.size(); i++)
	{
		convert(oox_cond_iconset->m_arrValues[i].GetPointer());
	}
}
void XlsxConverter::convert(OOX::Spreadsheet::CConditionalFormatValueObject *oox_cond_value)
{
	if (!oox_cond_value)return;

	std::wstring val;
	int type = 3;
	if (oox_cond_value->m_oType.IsInit())	type = oox_cond_value->m_oType->GetValue();
	
	if (oox_cond_value->m_oFormula.IsInit())	val = oox_cond_value->m_oFormula->m_sText;
	else if (oox_cond_value->m_oVal.IsInit())	val = *oox_cond_value->m_oVal;
	
	ods_context->current_table()->set_conditional_value(type, val);
}
void XlsxConverter::convert(OOX::Spreadsheet::CFormulaCF *oox_cond_formula)
{
	if (!oox_cond_formula)return;
	ods_context->current_table()->set_conditional_formula(oox_cond_formula->m_sText);
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
	
	//for (size_t i=0; i < oox_filter->m_arrItems.size(); i++)//set items todooo
	//{
	//}
}

} // namespace Docx2Odt
