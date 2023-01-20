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

#include <stdio.h>
#include "BinaryWriter.h"
#include "../Writer/BinaryReader.h"

#include "../../../../Common/OfficeFileFormats.h"
#include "../../../../Common/Base64.h"
#include "../../../../Common/OfficeFileErrorDescription.h"

#include "../../Presentation/FontCutter.h"
#include "../../../PPTXFormat/App.h"
#include "../../../PPTXFormat/Core.h"
#include "../../../PPTXFormat/Logic/HeadingVariant.h"

#include "../../../XlsxFormat/Xlsx.h"
#include "../../../XlsxFormat/XlsxFlat.h"

#include "../../../SystemUtility/SystemUtility.h"
#include "../../../DocxFormat/Media/OleObject.h"
#include "../../../DocxFormat/Media/ActiveX.h"
#include "../../../DocxFormat/Media/VbaProject.h"
#include "../../../DocxFormat/App.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/CustomXml.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinkPath.h"
#include "../../../XlsxFormat/Comments/ThreadedComments.h"
#include "../../../XlsxFormat/Slicer/SlicerCache.h"
#include "../../../XlsxFormat/Slicer/SlicerCacheExt.h"
#include "../../../XlsxFormat/Slicer/Slicer.h"
#include "../../../XlsxFormat/NamedSheetViews/NamedSheetViews.h"

namespace BinXlsxRW 
{

BinaryTableWriter::BinaryTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream):m_oBcw(oCBufferedStream)
{
}
void BinaryTableWriter::Write(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTableParts& oTableParts)
{
	int nCurPos = 0;
	for(size_t i = 0, length = oTableParts.m_arrItems.size(); i < length; ++i)
		WriteTablePart(oWorksheet, *oTableParts.m_arrItems[i]);
}
void BinaryTableWriter::WriteTablePart(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTablePart& oTablePart)
{
	int nCurPos = 0;
	if(oTablePart.m_oRId.IsInit())
	{
		smart_ptr<OOX::File> pFile = oWorksheet.Find(OOX::RId(oTablePart.m_oRId->GetValue()));
		if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::Table == pFile->type())
		{
			OOX::Spreadsheet::CTableFile* pTableFile = static_cast<OOX::Spreadsheet::CTableFile*>(pFile.GetPointer());
			if(pTableFile->m_oTable.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::Table);
				WriteTable(pTableFile);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
	}
}
void BinaryTableWriter::WriteTable(OOX::Spreadsheet::CTableFile* pTableFile)
{
	if (!pTableFile) return;

	OOX::Spreadsheet::CTable* pTable = pTableFile->m_oTable.GetPointer();
	if (!pTable) return;

	int nCurPos = 0;
	if(pTable->m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::Ref);
		m_oBcw.m_oStream.WriteStringW(pTable->m_oRef->ToString());
	}
	if(pTable->m_oHeaderRowCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::HeaderRowCount);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oHeaderRowCount->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTotalsRowCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TotalsRowCount);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oTotalsRowCount->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oDisplayName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::DisplayName);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oDisplayName);
	}
	if(pTable->m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::Name);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oName);
	}
	if(pTable->m_oComment.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::Comment);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oComment);
	}
	if(pTable->m_oConnectionId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::ConnectionId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oConnectionId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTableType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableType);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oTableType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oDataCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::DataCellStyle);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oDataCellStyle);
	}
	if(pTable->m_oDataDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::DataDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oDataDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oHeaderRowCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::HeaderRowCellStyle);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oHeaderRowCellStyle);
	}
	if(pTable->m_oHeaderRowDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::HeaderRowDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oHeaderRowDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oHeaderRowBorderDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::HeaderRowBorderDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oHeaderRowBorderDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::Id);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oInsertRow.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::InsertRow);
		m_oBcw.m_oStream.WriteBYTE(*pTable->m_oInsertRow ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oInsertRowShift.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::InsertRowShift);
		m_oBcw.m_oStream.WriteBYTE(*pTable->m_oInsertRowShift ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oPublished.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::Published);
		m_oBcw.m_oStream.WriteBYTE(*pTable->m_oPublished ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTableBorderDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableBorderDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oTableBorderDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTotalsRowBorderDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TotalsRowBorderDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oTotalsRowBorderDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTotalsRowCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::TotalsRowCellStyle);
		m_oBcw.m_oStream.WriteStringW(*pTable->m_oTotalsRowCellStyle);
	}
	if(pTable->m_oTotalsRowDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TotalsRowDxfId);
		m_oBcw.m_oStream.WriteLONG(pTable->m_oTotalsRowDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTotalsRowShown.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TotalsRowShown);
		m_oBcw.m_oStream.WriteBYTE(*pTable->m_oTotalsRowShown ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oAutoFilter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::AutoFilter);
		WriteAutoFilter(pTable->m_oAutoFilter.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oSortState.IsInit())
	{
		OOX::Spreadsheet::CSortState* pSortState = NULL;
		if(pTable->m_oSortState.IsInit())
			pSortState = pTable->m_oSortState.GetPointer();
		else
			pSortState = pTable->m_oAutoFilter->m_oSortState.GetPointer();

		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::SortState);
		WriteSortState(*pSortState);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTableColumns.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableColumns);
		WriteTableColumns(pTable->m_oTableColumns.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oTableStyleInfo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableStyleInfo);
		WriteTableStyleInfo(pTable->m_oTableStyleInfo.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pTable->m_oExtLst.IsInit())
	{
		for(size_t i = 0; i < pTable->m_oExtLst->m_arrExt.size(); ++i)
		{
			OOX::Drawing::COfficeArtExtension* pExt = pTable->m_oExtLst->m_arrExt[i];
			if(pExt->m_oAltTextTable.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::AltTextTable);
				WriteAltTextTable(pExt->m_oAltTextTable.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
	}
	smart_ptr<OOX::File> pFile = pTableFile->Find(OOX::Spreadsheet::FileTypes::QueryTable);
	OOX::Spreadsheet::CQueryTableFile *pQueryTableFile = dynamic_cast<OOX::Spreadsheet::CQueryTableFile*>(pFile.GetPointer());
	if ((pQueryTableFile) && (pQueryTableFile->m_oQueryTable.IsInit()))
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::QueryTable);
		WriteQueryTable(pQueryTableFile->m_oQueryTable.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);		
	}
}
void BinaryTableWriter::WriteQueryTable(const OOX::Spreadsheet::CQueryTable& oQueryTable)
{
	int nCurPos = 0;
	if(oQueryTable.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_QueryTable::Name);
		m_oBcw.m_oStream.WriteStringW(oQueryTable.m_oName.get());
	}
	if(oQueryTable.m_oConnectionId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ConnectionId);
		m_oBcw.m_oStream.WriteLONG(oQueryTable.m_oConnectionId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oAutoFormatId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::AutoFormatId);
		m_oBcw.m_oStream.WriteLONG(oQueryTable.m_oAutoFormatId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oGrowShrinkType.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_QueryTable::GrowShrinkType);
		m_oBcw.m_oStream.WriteStringW(oQueryTable.m_oGrowShrinkType.get());
	}
	if(oQueryTable.m_oAdjustColumnWidth.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::AdjustColumnWidth);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oAdjustColumnWidth ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oApplyAlignmentFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyAlignmentFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyAlignmentFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oApplyBorderFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyBorderFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyBorderFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}	
	if(oQueryTable.m_oApplyFontFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyFontFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyFontFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}	
	if(oQueryTable.m_oApplyNumberFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyNumberFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyNumberFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oApplyPatternFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyPatternFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyPatternFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oApplyWidthHeightFormats.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::ApplyWidthHeightFormats);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oApplyWidthHeightFormats ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oBackgroundRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::BackgroundRefresh);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oBackgroundRefresh ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oDisableEdit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::DisableEdit);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oDisableEdit ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oDisableRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::DisableRefresh);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oDisableRefresh ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oFillFormulas.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::FillFormulas);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oFillFormulas ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oFirstBackgroundRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::FirstBackgroundRefresh);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oFirstBackgroundRefresh ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oHeaders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::Headers);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oHeaders ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oIntermediate.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::Intermediate);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oIntermediate ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oPreserveFormatting.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::PreserveFormatting);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oPreserveFormatting ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oRefreshOnLoad.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::RefreshOnLoad);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTable.m_oRefreshOnLoad ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTable.m_oQueryTableRefresh.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTable::QueryTableRefresh);
		WriteQueryTableRefresh(oQueryTable.m_oQueryTableRefresh.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//m_oExtLst;
}
void BinaryTableWriter::WriteQueryTableRefresh(const OOX::Spreadsheet::CQueryTableRefresh& oQueryTableRefresh)
{
	int nCurPos = 0;
	if(oQueryTableRefresh.m_oNextId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::NextId);
		m_oBcw.m_oStream.WriteLONG(oQueryTableRefresh.m_oNextId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_FieldIdWrapped.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::FieldIdWrapped);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableRefresh.m_FieldIdWrapped ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_HeadersInLastRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::HeadersInLastRefresh);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableRefresh.m_HeadersInLastRefresh ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_HeadersInLastRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::HeadersInLastRefresh);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableRefresh.m_HeadersInLastRefresh ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_UnboundColumnsLeft.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::UnboundColumnsLeft);
		m_oBcw.m_oStream.WriteLONG(oQueryTableRefresh.m_UnboundColumnsLeft->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_UnboundColumnsLeft.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::UnboundColumnsLeft);
		m_oBcw.m_oStream.WriteLONG(oQueryTableRefresh.m_UnboundColumnsLeft->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_UnboundColumnsRight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::UnboundColumnsRight);
		m_oBcw.m_oStream.WriteLONG(oQueryTableRefresh.m_UnboundColumnsRight->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_oSortState.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::SortState);
		WriteSortState(oQueryTableRefresh.m_oSortState.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_oQueryTableFields.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::QueryTableFields);
		WriteQueryTableFields(oQueryTableRefresh.m_oQueryTableFields.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableRefresh.m_oQueryTableDeletedFields.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableRefresh::QueryTableDeletedFields);
		WriteQueryTableDeletedFields(oQueryTableRefresh.m_oQueryTableDeletedFields.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	//m_oExt
}
void BinaryTableWriter::WriteQueryTableFields(const OOX::Spreadsheet::CQueryTableFields& oQueryTableFields)
{
	for (size_t i = 0; i < oQueryTableFields.m_arrItems.size(); ++i)
	{
		if(oQueryTableFields.m_arrItems[i])
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::QueryTableField);
			WriteQueryTableField(*oQueryTableFields.m_arrItems[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryTableWriter::WriteQueryTableDeletedFields(const OOX::Spreadsheet::CQueryTableDeletedFields& oQueryTableDeletedFields)
{
	for (size_t i = 0; i < oQueryTableDeletedFields.m_arrItems.size(); ++i)
	{
		if(oQueryTableDeletedFields.m_arrItems[i])
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableDeletedField::QueryTableDeletedField);
			WriteQueryTableDeletedField(*oQueryTableDeletedFields.m_arrItems[i]);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryTableWriter::WriteQueryTableDeletedField(const OOX::Spreadsheet::CQueryTableDeletedField& oQueryTableDeletedField)
{
	if(oQueryTableDeletedField.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_QueryTableDeletedField::Name);
		m_oBcw.m_oStream.WriteStringW(oQueryTableDeletedField.m_oName.get());
	}
}
void BinaryTableWriter::WriteQueryTableField(const OOX::Spreadsheet::CQueryTableField& oQueryTableField)
{
	int nCurPos = 0;
	if(oQueryTableField.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_QueryTableField::Name);
		m_oBcw.m_oStream.WriteStringW(oQueryTableField.m_oName.get());
	}
	if(oQueryTableField.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::Id);
		m_oBcw.m_oStream.WriteLONG(oQueryTableField.m_oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableField.m_oTableColumnId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::TableColumnId);
		m_oBcw.m_oStream.WriteLONG(oQueryTableField.m_oTableColumnId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableField.m_oRowNumbers.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::RowNumbers);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableField.m_oRowNumbers ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableField.m_oFillFormulas.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::FillFormulas);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableField.m_oFillFormulas ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableField.m_oDataBound.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::DataBound);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableField.m_oDataBound ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oQueryTableField.m_oClipped.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_QueryTableField::Clipped);
		m_oBcw.m_oStream.WriteBYTE(*oQueryTableField.m_oClipped ? 1 : 0);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteAltTextTable(const OOX::Spreadsheet::CAltTextTable& oAltTextTable)		
{
	int nCurPos = 0;
	if(oAltTextTable.m_oAltText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_AltTextTable::AltText);
		m_oBcw.m_oStream.WriteStringW(oAltTextTable.m_oAltText.get());
	}
	if(oAltTextTable.m_oAltTextSummary.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_AltTextTable::AltTextSummary);
		m_oBcw.m_oStream.WriteStringW(oAltTextTable.m_oAltTextSummary.get());
	}
}
void BinaryTableWriter::WriteAutoFilter(const OOX::Spreadsheet::CAutofilter& oAutofilter)
{
	int nCurPos = 0;
	if(oAutofilter.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_AutoFilter::Ref);
		m_oBcw.m_oStream.WriteStringW(oAutofilter.m_oRef->ToString());
	}
	if(oAutofilter.m_arrItems.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_AutoFilter::FilterColumns);
		WriteFilterColumns(oAutofilter.m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oAutofilter.m_oSortState.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_AutoFilter::SortState);
		WriteSortState(oAutofilter.m_oSortState.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteFilterColumns(const std::vector<OOX::Spreadsheet::CFilterColumn *>& aFilterColumn)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aFilterColumn.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_AutoFilter::FilterColumn);
		WriteFilterColumn(*aFilterColumn[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteFilterColumn(OOX::Spreadsheet::CFilterColumn& oFilterColumn)
{
	int nCurPos = 0;
	if(oFilterColumn.m_oColId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::ColId);
		m_oBcw.m_oStream.WriteLONG(oFilterColumn.m_oColId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oFilters.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::Filters);
		WriteFilters(oFilterColumn.m_oFilters.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oCustomFilters.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::CustomFilters);
		WriteCustomFilters(oFilterColumn.m_oCustomFilters.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oDynamicFilter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::DynamicFilter);
		WriteDynamicFilter(oFilterColumn.m_oDynamicFilter.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oColorFilter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::ColorFilter);
		WriteColorFilter(oFilterColumn.m_oColorFilter.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oTop10.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::Top10);
		WriteTop10(oFilterColumn.m_oTop10.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oShowButton.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::ShowButton);
		m_oBcw.m_oStream.WriteBOOL(oFilterColumn.m_oShowButton->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oFilterColumn.m_oHiddenButton.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::HiddenButton);
		m_oBcw.m_oStream.WriteBOOL(oFilterColumn.m_oHiddenButton->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteFilters(const OOX::Spreadsheet::CFilters& oFilters)
{
	int nCurPos = 0;
	if(oFilters.m_oBlank.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::FiltersBlank);
		m_oBcw.m_oStream.WriteBOOL(oFilters.m_oBlank->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for(size_t i = 0, length = oFilters.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = oFilters.m_arrItems[i];
		if(OOX::et_x_Filter == we->getType())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::Filter);
			WriteFilter(*static_cast<OOX::Spreadsheet::CFilter*>(we));
			m_oBcw.WriteItemEnd(nCurPos);
		}
		else if(OOX::et_x_DateGroupItem == we->getType())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_FilterColumn::DateGroupItem);
			WriteDateGroupItem(*static_cast<OOX::Spreadsheet::CDateGroupItem*>(we));
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryTableWriter::WriteFilter(OOX::Spreadsheet::CFilter& oFilter)
{
	int nCurPos = 0;
	if(oFilter.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Filter::Val);
		m_oBcw.m_oStream.WriteStringW(oFilter.m_oVal.get2());
	}
}
void BinaryTableWriter::WriteDateGroupItem(OOX::Spreadsheet::CDateGroupItem& oDateGroupItem)
{
	int nCurPos = 0;
	if(oDateGroupItem.m_oDateTimeGrouping.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::DateTimeGrouping);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oDateGroupItem.m_oDateTimeGrouping->GetValue());
	}
	if(oDateGroupItem.m_oDay.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Day);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oDay->GetValue());
	}
	if(oDateGroupItem.m_oHour.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Hour);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oHour->GetValue());
	}
	if(oDateGroupItem.m_oMinute.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Minute);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oMinute->GetValue());
	}
	if(oDateGroupItem.m_oMonth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Month);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oMonth->GetValue());
	}
	if(oDateGroupItem.m_oSecond.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Second);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oSecond->GetValue());
	}
	if(oDateGroupItem.m_oYear.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DateGroupItem::Year);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oDateGroupItem.m_oYear->GetValue());
	}
}
void BinaryTableWriter::WriteCustomFilters(const OOX::Spreadsheet::CCustomFilters& oCustomFilters)
{
	int nCurPos = 0;
	if(oCustomFilters.m_oAnd.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CustomFilters::And);
		m_oBcw.m_oStream.WriteBOOL(oCustomFilters.m_oAnd->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oCustomFilters.m_arrItems.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CustomFilters::CustomFilters);
		WriteCustomFiltersItems(oCustomFilters.m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteCustomFiltersItems(const std::vector<OOX::Spreadsheet::CCustomFilter *>& aCustomFilters)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aCustomFilters.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CustomFilters::CustomFilter);
		WriteCustomFiltersItem(*aCustomFilters[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteCustomFiltersItem(OOX::Spreadsheet::CCustomFilter& oCustomFilter)
{
	int nCurPos = 0;
	if(oCustomFilter.m_oOperator.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CustomFilters::Operator);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oCustomFilter.m_oOperator->GetValue());
	}
	if(oCustomFilter.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CustomFilters::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oCustomFilter.m_oVal.get2());
	}
}
void BinaryTableWriter::WriteDynamicFilter(const OOX::Spreadsheet::CDynamicFilter& oDynamicFilter)
{
	if(oDynamicFilter.m_oType.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DynamicFilter::Type);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oDynamicFilter.m_oType->GetValue());
	}
	if(oDynamicFilter.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DynamicFilter::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oDynamicFilter.m_oVal->GetValue());
	}
	if(oDynamicFilter.m_oMaxVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DynamicFilter::MaxVal);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oDynamicFilter.m_oMaxVal->GetValue());
	}
}
void BinaryTableWriter::WriteColorFilter(const OOX::Spreadsheet::CColorFilter& oColorFilter)
{
	int nCurPos = 0;
	if(oColorFilter.m_oCellColor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ColorFilter::CellColor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oColorFilter.m_oCellColor->ToBool());
	}
	if(oColorFilter.m_oDxfId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ColorFilter::DxfId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oColorFilter.m_oDxfId->GetValue());
	}
}
void BinaryTableWriter::WriteTop10(const OOX::Spreadsheet::CTop10& oTop10)
{
	int nCurPos = 0;
	if(oTop10.m_oFilterVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Top10::FilterVal);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oTop10.m_oFilterVal->GetValue());
	}
	if(oTop10.m_oPercent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Top10::Percent);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTop10.m_oPercent->ToBool());
	}
	if(oTop10.m_oTop.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Top10::Top);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTop10.m_oTop->ToBool());
	}
	if(oTop10.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Top10::Val);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oTop10.m_oVal->GetValue());
	}
}
void BinaryTableWriter::WriteSortCondition(const OOX::Spreadsheet::CSortCondition& oSortCondition)
{
	int nCurPos = 0;
	if(oSortCondition.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_SortState::ConditionRef);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oSortCondition.m_oRef->ToString());
	}
	if(oSortCondition.m_oSortBy.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_SortState::ConditionSortBy);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oSortCondition.m_oSortBy->GetValue());
	}
	if(oSortCondition.m_oDescending.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_SortState::ConditionDescending);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oSortCondition.m_oDescending->ToBool());
	}
	if(oSortCondition.m_oDxfId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_SortState::ConditionDxfId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oSortCondition.m_oDxfId->GetValue());
	}
}
void BinaryTableWriter::WriteSortState(const OOX::Spreadsheet::CSortState& oSortState)
{
	int nCurPos = 0;
	if(oSortState.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_SortState::Ref);
		m_oBcw.m_oStream.WriteStringW(oSortState.m_oRef->ToString());
	}
	if(oSortState.m_oCaseSensitive.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SortState::CaseSensitive);
		m_oBcw.m_oStream.WriteBOOL(oSortState.m_oCaseSensitive->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSortState.m_oColumnSort.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SortState::ColumnSort);
		m_oBcw.m_oStream.WriteBOOL(oSortState.m_oColumnSort->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oSortState.m_oSortMethod.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SortState::SortMethod);
		m_oBcw.m_oStream.WriteBYTE(oSortState.m_oSortMethod->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_SortState::SortConditions);
	for(size_t i = 0, length = oSortState.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SortState::SortCondition);
		WriteSortCondition(*oSortState.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	m_oBcw.WriteItemEnd(nCurPos2);
}
void BinaryTableWriter::WriteTableColumn(const OOX::Spreadsheet::CTableColumn& oTableColumn)
{
	int nCurPos = 0;
	if(oTableColumn.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::Name);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oName);
	}
	if(oTableColumn.m_oTotalsRowLabel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::TotalsRowLabel);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oTotalsRowLabel);
	}
	if(oTableColumn.m_oTotalsRowFunction.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::TotalsRowFunction);
		m_oBcw.m_oStream.WriteBYTE(oTableColumn.m_oTotalsRowFunction->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oTotalsRowFormula.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::TotalsRowFormula);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oTotalsRowFormula);
	}
	if(oTableColumn.m_oDataDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::DataDxfId);
		m_oBcw.m_oStream.WriteLONG(oTableColumn.m_oDataDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oCalculatedColumnFormula.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::CalculatedColumnFormula);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oCalculatedColumnFormula);
	}
	if(oTableColumn.m_oDataCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::DataCellStyle);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oDataCellStyle);
	}
	if(oTableColumn.m_oHeaderRowCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::HeaderRowCellStyle);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oHeaderRowCellStyle);
	}
	if(oTableColumn.m_oHeaderRowDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::HeaderRowDxfId);
		m_oBcw.m_oStream.WriteLONG(oTableColumn.m_oHeaderRowDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::Id);
		m_oBcw.m_oStream.WriteLONG(oTableColumn.m_oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oQueryTableFieldId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::QueryTableFieldId);
		m_oBcw.m_oStream.WriteLONG(oTableColumn.m_oQueryTableFieldId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oTotalsRowCellStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::TotalsRowCellStyle);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oTotalsRowCellStyle);
	}
	if(oTableColumn.m_oTotalsRowDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::TotalsRowDxfId);
		m_oBcw.m_oStream.WriteLONG(oTableColumn.m_oTotalsRowDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableColumn.m_oUniqueName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::UniqueName);
		m_oBcw.m_oStream.WriteStringW(*oTableColumn.m_oUniqueName);
	}
}
void BinaryTableWriter::WriteTableColumns(const OOX::Spreadsheet::CTableColumns& oTableColumns)
{
	int nCurPos = 0;
	for(size_t i = 0, length = oTableColumns.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::TableColumn);
		WriteTableColumn(*oTableColumns.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryTableWriter::WriteTableStyleInfo(const OOX::Spreadsheet::CTableStyleInfo& oTableStyleInfo)
{
	if(oTableStyleInfo.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleInfo::Name);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oTableStyleInfo.m_oName.get2());
	}
	if(oTableStyleInfo.m_oShowColumnStripes.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleInfo::ShowColumnStripes);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTableStyleInfo.m_oShowColumnStripes->ToBool());
	}
	if(oTableStyleInfo.m_oShowRowStripes.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleInfo::ShowRowStripes);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTableStyleInfo.m_oShowRowStripes->ToBool());
	}
	if(oTableStyleInfo.m_oShowFirstColumn.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleInfo::ShowFirstColumn);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTableStyleInfo.m_oShowFirstColumn->ToBool());
	}
	if(oTableStyleInfo.m_oShowLastColumn.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleInfo::ShowLastColumn);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oTableStyleInfo.m_oShowLastColumn->ToBool());
	}
}

BinaryStyleTableWriter::BinaryStyleTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager):m_oBcw(oCBufferedStream),m_pEmbeddedFontsManager(pEmbeddedFontsManager)
{
}
void BinaryStyleTableWriter::Write(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteStylesContent(styles, pTheme, oFontProcessor);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryStyleTableWriter::WriteStylesContent(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos;
	OOX::Spreadsheet::CIndexedColors* pIndexedColors = NULL;
	if(styles.m_oColors.IsInit() && styles.m_oColors->m_oIndexedColors.IsInit())
	{
		pIndexedColors = styles.m_oColors->m_oIndexedColors.operator ->();
	}
//borders
	if(styles.m_oBorders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Borders);
		WriteBorders(styles.m_oBorders.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Fills
	if(styles.m_oFills.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Fills);
		WriteFills(styles.m_oFills.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Fonts
	if(styles.m_oFonts.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Fonts);
		WriteFonts(styles.m_oFonts.get(), pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//NumFmts
	if(styles.m_oNumFmts.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::NumFmts);
		WriteNumFmts(styles.m_oNumFmts.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//CellStyleXfs
	if(styles.m_oCellStyleXfs.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::CellStyleXfs);
		WriteCellStyleXfs(styles.m_oCellStyleXfs.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//CellXfs
	if(styles.m_oCellXfs.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::CellXfs);
		WriteCellXfs(styles.m_oCellXfs.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//CellStyles
	if(styles.m_oCellStyles.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::CellStyles);
		WriteCellStyles(styles.m_oCellStyles.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Dxfs
	if(styles.m_oDxfs.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Dxfs);
		WriteDxfs(styles.m_oDxfs.get(), pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//TableStyles
	if(styles.m_oTableStyles.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::TableStyles);
		WriteTableStyles(styles.m_oTableStyles.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (styles.m_oExtLst.IsInit())
	{
		for(size_t i = 0; i < styles.m_oExtLst->m_arrExt.size(); ++i)
		{
			OOX::Drawing::COfficeArtExtension* pExt = styles.m_oExtLst->m_arrExt[i];
			if ( pExt->m_oDxfs.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::ExtDxfs);
				WriteDxfs(pExt->m_oDxfs.get(), pIndexedColors, pTheme, oFontProcessor);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			else if ( pExt->m_oSlicerStyles.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::SlicerStyles);
				m_oBcw.m_oStream.WriteRecord2(0, pExt->m_oSlicerStyles);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
	}
}
void BinaryStyleTableWriter::WriteBorders(const OOX::Spreadsheet::CBorders& borders, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
{
	int nCurPos = 0;
	for(size_t i = 0, length = borders.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CBorder* pBorder = borders.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Border);
		WriteBorder(*pBorder, pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteBorder(const OOX::Spreadsheet::CBorder& border, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
{
	int nCurPos = 0;
	//Bottom
	if(false != border.m_oBottom.IsInit() && false != border.m_oBottom->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Bottom);
		WriteBorderProp(border.m_oBottom.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Diagonal
	if(false != border.m_oDiagonal.IsInit() && false != border.m_oDiagonal->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Diagonal);
		WriteBorderProp(border.m_oDiagonal.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//End
	if(false != border.m_oEnd.IsInit() && false != border.m_oEnd->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::End);
		WriteBorderProp(border.m_oEnd.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Horizontal
	if(false != border.m_oHorizontal.IsInit() && false != border.m_oHorizontal->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Horizontal);
		WriteBorderProp(border.m_oHorizontal.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Start
	if(false != border.m_oStart.IsInit() && false != border.m_oStart->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Start);
		WriteBorderProp(border.m_oStart.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Top
	if(false != border.m_oTop.IsInit() && false != border.m_oTop->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Top);
		WriteBorderProp(border.m_oTop.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Vertical
	if(false != border.m_oVertical.IsInit() && false != border.m_oVertical->m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Vertical);
		WriteBorderProp(border.m_oVertical.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//DiagonalDown
	if(false != border.m_oDiagonalDown.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::DiagonalDown);
		m_oBcw.m_oStream.WriteBOOL(border.m_oDiagonalDown->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//DiagonalUp
	if(false != border.m_oDiagonalUp.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::DiagonalUp);
		m_oBcw.m_oStream.WriteBOOL(border.m_oDiagonalUp->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Outline
	if(false != border.m_oOutline.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerBorderTypes::Outline);
		m_oBcw.m_oStream.WriteBOOL(border.m_oOutline->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteBorderProp(const OOX::Spreadsheet::CBorderProp& borderProp, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
{
	int nCurPos = 0;
	//Color
	if(false != borderProp.m_oColor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerBorderPropTypes::Color);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteColor(borderProp.m_oColor.get(), pIndexedColors);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Style
	if(false != borderProp.m_oStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerBorderPropTypes::Style);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)borderProp.m_oStyle->GetValue());
	}
}
void BinaryStyleTableWriter::WriteCellStyleXfs(const OOX::Spreadsheet::CCellStyleXfs& cellStyleXfs)
{
	int nCurPos = 0;
	for(size_t i = 0, length = cellStyleXfs.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = cellStyleXfs.m_arrItems[i];
		if(OOX::et_x_Xfs == we->getType())
		{
			OOX::Spreadsheet::CXfs* pXfs = static_cast<OOX::Spreadsheet::CXfs*>(we);
			nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Xfs);
			WriteXfs(*pXfs);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryStyleTableWriter::WriteCellXfs(const OOX::Spreadsheet::CCellXfs& cellXfs)
{
	int nCurPos = 0;
	for(size_t i = 0, length = cellXfs.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = cellXfs.m_arrItems[i];
		if(OOX::et_x_Xfs == we->getType())
		{
			OOX::Spreadsheet::CXfs* pXfs = static_cast<OOX::Spreadsheet::CXfs*>(we);
			nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Xfs);
			WriteXfs(*pXfs);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryStyleTableWriter::WriteXfs(const OOX::Spreadsheet::CXfs& xfs)
{
	int nCurPos = 0;
	//ApplyAlignment
	if (false != xfs.m_oApplyAlignment.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyAlignment);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyAlignment->ToBool());
	}
	//ApplyBorder
	if(false != xfs.m_oApplyBorder.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyBorder);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyBorder->ToBool());
	}
	//ApplyFill
	if (false != xfs.m_oApplyFill.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyFill);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyFill->ToBool());
	}
	//ApplyFont
	if(false != xfs.m_oApplyFont.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyFont);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyFont->ToBool());
	}
	//ApplyNumberFormat
	if (false != xfs.m_oApplyNumberFormat.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyNumberFormat);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyNumberFormat->ToBool());
	}
	//ApplyProtection
	if (false != xfs.m_oApplyProtection.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyProtection);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyProtection->ToBool());
	}
	//BorderId
	if (false != xfs.m_oBorderId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::BorderId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(xfs.m_oBorderId->GetValue());
	}
	//FillId
	if (false != xfs.m_oFillId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::FillId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(xfs.m_oFillId->GetValue());
	}
	//FontId
	if (false != xfs.m_oFontId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::FontId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(xfs.m_oFontId->GetValue());
	}
	//NumFmtId
	if (false != xfs.m_oNumFmtId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::NumFmtId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(xfs.m_oNumFmtId->GetValue());
	}
	//QuotePrefix
	if (false != xfs.m_oQuotePrefix.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::QuotePrefix);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oQuotePrefix->ToBool());
	}
	//PivotButton
	if (false != xfs.m_oPivotButton.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::PivotButton);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(xfs.m_oPivotButton->ToBool());
	}
	//XfId
	if (false != xfs.m_oXfId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::XfId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(xfs.m_oXfId->GetValue());
	}
	//Aligment
	if (false != xfs.m_oAligment.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::Aligment);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteAligment(xfs.m_oAligment.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Protection 
	if (false != xfs.m_oProtection.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::Protection);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteProtection(xfs.m_oProtection.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteProtection(const OOX::Spreadsheet::CProtection& protection)
{
	int nCurPos = 0;
	if (false != protection.m_oHidden.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectionTypes::Hidden);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oHidden->ToBool());
	}
	if (false != protection.m_oLocked.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectionTypes::Locked);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oLocked->ToBool());
	}
}
void BinaryStyleTableWriter::WriteAligment(const OOX::Spreadsheet::CAligment& aligment)
{
	int nCurPos = 0;
	//Horizontal
	if(false != aligment.m_oHorizontal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::Horizontal);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(aligment.m_oHorizontal->GetValue());
	}
	//Indent
	if(false != aligment.m_oIndent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::Indent);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*aligment.m_oIndent);
	}
	//RelativeIndent
	if(false != aligment.m_oRelativeIndent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::RelativeIndent);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*aligment.m_oRelativeIndent);
	}
	//ShrinkToFit
	if(false != aligment.m_oShrinkToFit.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::ShrinkToFit);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(aligment.m_oShrinkToFit->ToBool());
	}
	//TextRotation
	if(false != aligment.m_oTextRotation.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::TextRotation);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*aligment.m_oTextRotation);
	}
	//Vertical
	if(false != aligment.m_oVertical.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::Vertical);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(aligment.m_oVertical->GetValue());
	}
	//WrapText
	if(false != aligment.m_oWrapText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::WrapText);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(aligment.m_oWrapText->ToBool());
	}
}
void BinaryStyleTableWriter::WriteFills(const OOX::Spreadsheet::CFills& fills, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
{
	int nCurPos = 0;
	for(size_t i = 0, length = fills.m_arrItems.size(); i < length; ++i)
	{
			OOX::Spreadsheet::CFill* pFill = fills.m_arrItems[i];
			nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Fill);
			WriteFill(*pFill, pIndexedColors);
			m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteFill(const OOX::Spreadsheet::CFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors)
{
	int nCurPos = 0;
	if (fill.m_oPatternFill.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::Pattern);
		WritePatternFill(fill.m_oPatternFill.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oGradientFill.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::Gradient);
		WriteGradientFill(fill.m_oGradientFill.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WritePatternFill(const OOX::Spreadsheet::CPatternFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors)
{
	int nCurPos = 0;
	if (fill.m_oPatternType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::PatternType);
		m_oBcw.m_oStream.WriteBYTE(fill.m_oPatternType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oFgColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::PatternFgColor);
		m_oBcw.WriteColor(fill.m_oFgColor.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oBgColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::PatternBgColor);
		m_oBcw.WriteColor(fill.m_oBgColor.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteGradientFill(const OOX::Spreadsheet::CGradientFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors)
{
	int nCurPos = 0;
	if (fill.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientType);
		m_oBcw.m_oStream.WriteBYTE(fill.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oLeft.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientLeft);
		m_oBcw.m_oStream.WriteDoubleReal(fill.m_oLeft->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oTop.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientTop);
		m_oBcw.m_oStream.WriteDoubleReal(fill.m_oTop->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oRight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientRight);
		m_oBcw.m_oStream.WriteDoubleReal(fill.m_oRight->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oBottom.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientBottom);
		m_oBcw.m_oStream.WriteDoubleReal(fill.m_oBottom->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (fill.m_oDegree.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientDegree);
		m_oBcw.m_oStream.WriteDoubleReal(fill.m_oDegree->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for(size_t i = 0, length = fill.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientStop);
		WriteGradientFillStop(*fill.m_arrItems[i], pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteGradientFillStop(const OOX::Spreadsheet::CGradientStop& stop, OOX::Spreadsheet::CIndexedColors* pIndexedColors)
{
	int nCurPos = 0;
	if (stop.m_oPosition.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientStopPosition);
		m_oBcw.m_oStream.WriteDoubleReal(stop.m_oPosition->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (stop.m_oColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::GradientStopColor);
		m_oBcw.WriteColor(stop.m_oColor.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteFonts(const OOX::Spreadsheet::CFonts& fonts, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos = 0;
	for(size_t i = 0, length = fonts.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CFont* pFont = fonts.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Font);
		WriteFont(*pFont, pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteFont(const OOX::Spreadsheet::CFont& font, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* theme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos = 0;
	//Bold
	if(false != font.m_oBold.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Bold);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(font.m_oBold->m_oVal.ToBool());
	}
	//Color
	if(false != font.m_oColor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Color);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteColor(font.m_oColor.get(), pIndexedColors);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Italic
	if(false != font.m_oItalic.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Italic);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(font.m_oItalic->m_oVal.ToBool());
	}
	//RFont
	if(font.m_oRFont.IsInit() && font.m_oRFont->m_sVal.IsInit())
	{
		//подбираем шрифт
        std::wstring sFont = oFontProcessor.getFont(font.m_oScheme, font.m_oRFont, font.m_oCharset, font.m_oFamily, theme);
		
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::RFont);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sFont);

		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckFont(sFont, oFontProcessor.getFontManager());
	}
	//Scheme
	if(font.m_oScheme.IsInit() && font.m_oScheme->m_oFontScheme.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Scheme);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)font.m_oScheme->m_oFontScheme->GetValue());
	}
	//Strike
	if(false != font.m_oStrike.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Strike);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(font.m_oStrike->m_oVal.ToBool());
	}
	//Sz
	if(false != font.m_oSz.IsInit() && font.m_oSz->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Sz);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(font.m_oSz->m_oVal->GetValue());
	}
	//Underline
	if(false != font.m_oUnderline.IsInit())
	{
		SimpleTypes::Spreadsheet::EUnderline eType = SimpleTypes::Spreadsheet::underlineSingle;
		if(font.m_oUnderline->m_oUnderline.IsInit())
			eType = font.m_oUnderline->m_oUnderline->GetValue();
		
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Underline);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(eType);
	}
	//VertAlign
	if(false != font.m_oVertAlign.IsInit() && font.m_oVertAlign->m_oVerticalAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::VertAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(font.m_oVertAlign->m_oVerticalAlign->GetValue());
	}
}
void BinaryStyleTableWriter::WriteNumFmts(const OOX::Spreadsheet::CNumFmts& numFmts)
{
	int nCurPos = 0;
	for(size_t i = 0, length = numFmts.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CNumFmt* pNumFmt = numFmts.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::NumFmt);
		WriteNumFmt(*pNumFmt);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteNumFmt(const OOX::Spreadsheet::CNumFmt& numFmt)
{
	int nCurPos = 0;
	//FormatCode
	if(numFmt.m_oFormatCode.IsInit())
	{
		std::wstring& sFormatCode = *numFmt.m_oFormatCode;
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumFmtTypes::FormatCode);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sFormatCode);

		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckString(sFormatCode);
	}
	if(numFmt.m_oFormatCode16.IsInit())
	{
		std::wstring& sFormatCode = *numFmt.m_oFormatCode16;
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumFmtTypes::FormatCode16);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sFormatCode);

		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckString(sFormatCode);
	}
	//NumFmtId
	if(numFmt.m_oNumFmtId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerNumFmtTypes::NumFmtId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(numFmt.m_oNumFmtId->GetValue());
	}
}
void BinaryStyleTableWriter::WriteCellStyles(const OOX::Spreadsheet::CCellStyles& oCellStyles)
{
	int nCurPos = 0;
	for (size_t i = 0, nLength = oCellStyles.m_arrItems.size(); i < nLength; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = oCellStyles.m_arrItems[i];
		if (OOX::et_x_CellStyle == we->getType())
		{
			OOX::Spreadsheet::CCellStyle* pCellStyle = static_cast<OOX::Spreadsheet::CCellStyle*>(we);
			nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::CellStyle);
			WriteCellStyle(*pCellStyle);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryStyleTableWriter::WriteCellStyle(const OOX::Spreadsheet::CCellStyle& oCellStyle)
{
	int nCurPos = 0;
	if (oCellStyle.m_oBuiltinId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::BuiltinId);
		m_oBcw.m_oStream.WriteLONG(oCellStyle.m_oBuiltinId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCellStyle.m_oCustomBuiltin.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::CustomBuiltin);
		m_oBcw.m_oStream.WriteBOOL(oCellStyle.m_oCustomBuiltin->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCellStyle.m_oHidden.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::Hidden);
		m_oBcw.m_oStream.WriteBOOL(oCellStyle.m_oHidden->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCellStyle.m_oILevel.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::ILevel);
		m_oBcw.m_oStream.WriteLONG(oCellStyle.m_oILevel->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCellStyle.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CellStyle::Name);
		m_oBcw.m_oStream.WriteStringW(*oCellStyle.m_oName);
	}
	if (oCellStyle.m_oXfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::XfId);
		m_oBcw.m_oStream.WriteLONG(oCellStyle.m_oXfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteDxfs(const OOX::Spreadsheet::CDxfs& oDxfs, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos = 0;
	for(size_t i = 0, length = oDxfs.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CDxf* pDxf = oDxfs.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Dxf);
		WriteDxf(*pDxf, pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteDxf(const OOX::Spreadsheet::CDxf& oDxf, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos = 0;
	if(oDxf.m_oAlignment.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Dxf::Alignment);
		WriteAligment(oDxf.m_oAlignment.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDxf.m_oBorder.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Dxf::Border);
		WriteBorder(oDxf.m_oBorder.get(), pIndexedColors, pTheme);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDxf.m_oFill.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Dxf::Fill);
		WriteFill(oDxf.m_oFill.get(), pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDxf.m_oFont.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Dxf::Font);
		WriteFont(oDxf.m_oFont.get(), pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oDxf.m_oNumFmt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Dxf::NumFmt);
		WriteNumFmt(oDxf.m_oNumFmt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTableStyles(const OOX::Spreadsheet::CTableStyles& oTableStyles)
{
	int nCurPos = 0;
	if(oTableStyles.m_oDefaultTableStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyles::DefaultTableStyle);
		m_oBcw.m_oStream.WriteStringW(oTableStyles.m_oDefaultTableStyle.get2());
	}
	if(oTableStyles.m_oDefaultPivotStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyles::DefaultPivotStyle);
		m_oBcw.m_oStream.WriteStringW(oTableStyles.m_oDefaultPivotStyle.get2());
	}
	if(oTableStyles.m_arrItems.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyles::TableStyles);
		WriteTableCustomStyles(oTableStyles.m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTableCustomStyles(const std::vector<OOX::Spreadsheet::CTableStyle *>& aTableStyles)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aTableStyles.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CTableStyle* pTableStyle = aTableStyles[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyles::TableStyle);
		WriteTableStyle(*pTableStyle);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTableStyle(const OOX::Spreadsheet::CTableStyle& oTableStyle)
{
	int nCurPos = 0;
	if(oTableStyle.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyle::Name);
		m_oBcw.m_oStream.WriteStringW(oTableStyle.m_oName.get2());
	}
	if(oTableStyle.m_oPivot.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyle::Pivot);
		m_oBcw.m_oStream.WriteBOOL(oTableStyle.m_oPivot->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableStyle.m_oTable.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyle::Table);
		m_oBcw.m_oStream.WriteBOOL(oTableStyle.m_oTable->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableStyle.m_arrItems.size() > 0)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyle::Elements);
		WriteTableStyleElements(oTableStyle.m_arrItems);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oTableStyle.m_oDisplayName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyle::DisplayName);
		m_oBcw.m_oStream.WriteStringW(oTableStyle.m_oDisplayName.get2());
	}
}
void BinaryStyleTableWriter::WriteTableStyleElements(const std::vector<OOX::Spreadsheet::CTableStyleElement *>& aTableStyles)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aTableStyles.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyle::Element);
		WriteTableStyleElement(*aTableStyles[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryStyleTableWriter::WriteTableStyleElement(const OOX::Spreadsheet::CTableStyleElement& oTableStyleElement)
{
	int nCurPos = 0;
	if(oTableStyleElement.m_oType.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleElement::Type);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)oTableStyleElement.m_oType->GetValue());
	}
	if(oTableStyleElement.m_oSize.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleElement::Size);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oTableStyleElement.m_oSize->GetValue());
	}
	if(oTableStyleElement.m_oDxfId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_TableStyleElement::DxfId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oTableStyleElement.m_oDxfId->GetValue());
	}
}

BinarySharedStringTableWriter::BinarySharedStringTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager):m_oBcw(oCBufferedStream),m_pEmbeddedFontsManager(pEmbeddedFontsManager)
{
}
void BinarySharedStringTableWriter::Write(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteSharedStrings(sharedString, pIndexedColors, pTheme, oFontProcessor);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinarySharedStringTableWriter::WriteSharedStrings(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos;
	for(size_t i = 0, length = sharedString.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = sharedString.m_arrItems[i];
		if(OOX::et_x_Si == we->getType())
		{
			OOX::Spreadsheet::CSi* pSi = static_cast<OOX::Spreadsheet::CSi*>(we);
			nCurPos = m_oBcw.WriteItemStart(c_oSerSharedStringTypes::Si);
			WriteSharedString(*pSi, pIndexedColors, pTheme, oFontProcessor);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
}
void BinarySharedStringTableWriter::WriteSharedString(OOX::Spreadsheet::CSi& si, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos;
	for(size_t i = 0, length = si.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = si.m_arrItems[i];
		if(OOX::et_x_r == we->getType())
		{
			OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(we);
			nCurPos = m_oBcw.WriteItemStart(c_oSerSharedStringTypes::Run);
				WriteRun(*pRun, pIndexedColors, pTheme, oFontProcessor);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		else if(OOX::et_x_t == we->getType())
		{
			OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
			m_oBcw.m_oStream.WriteBYTE(c_oSerSharedStringTypes::Text);
			m_oBcw.m_oStream.WriteStringW(pText->m_sText);

			if(NULL != m_pEmbeddedFontsManager)
				m_pEmbeddedFontsManager->CheckString(pText->m_sText);
		}
	}
}
void BinarySharedStringTableWriter::WriteRun(OOX::Spreadsheet::CRun& run, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos;
//rPr
	if(run.m_oRPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerSharedStringTypes::RPr);
		WriteRPr(run.m_oRPr.get(), pIndexedColors, pTheme, oFontProcessor);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for(size_t i = 0, length = run.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::WritingElement* we = run.m_arrItems[i];
		if(OOX::et_x_t == we->getType())
		{
			OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(we);
			m_oBcw.m_oStream.WriteBYTE(c_oSerSharedStringTypes::Text);
			m_oBcw.m_oStream.WriteStringW(pText->m_sText);

			if(NULL != m_pEmbeddedFontsManager)
				m_pEmbeddedFontsManager->CheckString(pText->m_sText);
		}
	}
}
void BinarySharedStringTableWriter::WriteRPr(const OOX::Spreadsheet::CRPr& rPr, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
{
	int nCurPos = 0;
	//Bold
	if(false != rPr.m_oBold.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Bold);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr.m_oBold->m_oVal.ToBool());
	}
	//Color
	if(false != rPr.m_oColor.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Color);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		m_oBcw.WriteColor(rPr.m_oColor.get(), pIndexedColors);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Italic
	if(false != rPr.m_oItalic.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Italic);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr.m_oItalic->m_oVal.ToBool());
	}
	//RFont
	if(false != rPr.m_oRFont.IsInit() && rPr.m_oRFont->m_sVal.IsInit())
	{
        std::wstring sFont = oFontProcessor.getFont(rPr.m_oScheme, rPr.m_oRFont, rPr.m_oCharset, rPr.m_oFamily, pTheme);
		
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::RFont);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(sFont);

		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckFont(sFont, oFontProcessor.getFontManager());
	}
	//Scheme
	if(rPr.m_oScheme.IsInit() && rPr.m_oScheme->m_oFontScheme.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Scheme);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE((BYTE)rPr.m_oScheme->m_oFontScheme->GetValue());
	}
	//Strike
	if(false != rPr.m_oStrike.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Strike);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(rPr.m_oStrike->m_oVal.ToBool());
	}
	//Sz
	if(false != rPr.m_oSz.IsInit() && rPr.m_oSz->m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Sz);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(rPr.m_oSz->m_oVal->GetValue());
	}
	//Underline
	if(false != rPr.m_oUnderline.IsInit())
	{
		SimpleTypes::Spreadsheet::EUnderline eType = SimpleTypes::Spreadsheet::underlineSingle;
		if(rPr.m_oUnderline->m_oUnderline.IsInit())
			eType = rPr.m_oUnderline->m_oUnderline->GetValue();
		
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::Underline);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(eType);
	}
	//VertAlign
	if(false != rPr.m_oVertAlign.IsInit() && rPr.m_oVertAlign->m_oVerticalAlign.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFontTypes::VertAlign);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(rPr.m_oVertAlign->m_oVerticalAlign->GetValue());
	}
}


BinaryWorkbookTableWriter::BinaryWorkbookTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, OOX::Document *pDocument) 
: m_oBcw(oCBufferedStream), m_pXlsx(NULL), m_pXlsxFlat(NULL)
{
	m_pXlsx		= dynamic_cast<OOX::Spreadsheet::CXlsx*>(pDocument);
	m_pXlsxFlat	= dynamic_cast<OOX::Spreadsheet::CXlsxFlat*>(pDocument);
}
void BinaryWorkbookTableWriter::Write(OOX::Spreadsheet::CWorkbook& workbook)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteWorkbook(workbook);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryWorkbookTableWriter::WriteWorkbook(OOX::Spreadsheet::CWorkbook& workbook)
{
	int nCurPos;
//WorkbookPr
	if(workbook.m_oWorkbookPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::WorkbookPr);
		WriteWorkbookPr(workbook.m_oWorkbookPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//WorkbookPr
	if (workbook.m_oWorkbookProtection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::Protection);
		WriteProtection(workbook.m_oWorkbookProtection.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//BookViews
	if(workbook.m_oBookViews.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::BookViews);
		WriteBookViews(workbook.m_oBookViews.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//DefinedNames
	if(workbook.m_oDefinedNames.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::DefinedNames);
		WriteDefinedNames(workbook.m_oDefinedNames.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (workbook.m_oCalcPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::CalcPr);
		WriteCalcPr(workbook.m_oCalcPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//ExternalReferences
	if(workbook.m_oExternalReferences.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalReferences);
		WriteExternalReferences(workbook.m_oExternalReferences.get(), workbook);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Ext
	if (workbook.m_oExtLst.IsInit())
	{
		for(size_t i = 0; i < workbook.m_oExtLst->m_arrExt.size(); ++i)
		{
			OOX::Drawing::COfficeArtExtension* pExt = workbook.m_oExtLst->m_arrExt[i];
			if ( pExt->m_oSlicerCaches.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::SlicerCaches);
				WriteSlicerCaches(workbook, pExt->m_oSlicerCaches.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			else if ( pExt->m_oSlicerCachesExt.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::SlicerCachesExt);
				WriteSlicerCaches(workbook, pExt->m_oSlicerCachesExt.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
	}
//Write VbaProject
	if (m_pXlsx && NULL != m_pXlsx->m_pVbaProject)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::VbaProject);

		m_oBcw.m_oStream.StartRecord(0);
        m_pXlsx->m_pVbaProject->toPPTY(&m_oBcw.m_oStream);
        m_oBcw.m_oStream.EndRecord();

        m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Write JsaProject
	if (m_pXlsx && NULL != m_pXlsx->m_pJsaProject)
	{
		BYTE* pData = NULL;
		DWORD nBytesCount;
		if(NSFile::CFileBinary::ReadAllBytes(m_pXlsx->m_pJsaProject->filename().GetPath(), &pData, nBytesCount))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::JsaProject);
			m_oBcw.m_oStream.WriteBYTEArray(pData, nBytesCount);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
//Workbook Comments
	if (m_pXlsx && NULL != m_pXlsx->m_pWorkbookComments)
	{
		BYTE* pData = NULL;
		DWORD nBytesCount;
		if(NSFile::CFileBinary::ReadAllBytes(m_pXlsx->m_pWorkbookComments->m_oReadPath.GetPath(), &pData, nBytesCount))
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::Comments);
			m_oBcw.m_oStream.WriteBYTEArray(pData, nBytesCount);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
	smart_ptr<OOX::File> pFile = workbook.Find(OOX::Spreadsheet::FileTypes::Connections);
	OOX::Spreadsheet::CConnectionsFile *pConnectionFile = dynamic_cast<OOX::Spreadsheet::CConnectionsFile*>(pFile.GetPointer());
	if ((pConnectionFile) && (pConnectionFile->m_oConnections.IsInit()))
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::Connections);
		WriteConnections(pConnectionFile->m_oConnections.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);		
	}
	if (workbook.m_oAppName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::AppName);
		m_oBcw.m_oStream.WriteStringW3(*workbook.m_oAppName);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if (workbook.m_oOleSize.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::OleSize);
		m_oBcw.m_oStream.WriteStringW3(*workbook.m_oOleSize);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteProtection(const OOX::Spreadsheet::CWorkbookProtection& protection)
{
	if (protection.m_oWorkbookAlgorithmName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::AlgorithmName);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(protection.m_oWorkbookAlgorithmName->GetValue());
	}
	if (protection.m_oWorkbookSpinCount.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::SpinCount);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG(protection.m_oWorkbookSpinCount->GetValue());
	}
	if (protection.m_oWorkbookHashValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::HashValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oWorkbookHashValue);
	}
	if (protection.m_oWorkbookSaltValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::SaltValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oWorkbookSaltValue);
	}
	if (protection.m_oLockStructure.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::LockStructure);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oLockStructure->ToBool());
	}
	if (protection.m_oLockWindows.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::LockWindows);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oLockWindows->ToBool());
	}
	if (protection.m_oPassword.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookProtection::Password);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oPassword);
	}
}
void BinaryWorkbookTableWriter::WriteWorkbookPr(const OOX::Spreadsheet::CWorkbookPr& workbookPr)
{
//Date1904
	if(workbookPr.m_oDate1904.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookPrTypes::Date1904);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(workbookPr.m_oDate1904->ToBool());
	}
//DateCompatibility
	if(workbookPr.m_oDateCompatibility.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookPrTypes::DateCompatibility);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(workbookPr.m_oDateCompatibility->ToBool());
	}
	if(workbookPr.m_oHidePivotFieldList.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookPrTypes::HidePivotFieldList);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(workbookPr.m_oHidePivotFieldList->ToBool());
	}
	if(workbookPr.m_oShowPivotChartFilter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookPrTypes::ShowPivotChartFilter);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(workbookPr.m_oShowPivotChartFilter->ToBool());
	}
}
void BinaryWorkbookTableWriter::WriteConnectionTextFields(const OOX::Spreadsheet::CTextFields& textFields)
{
	for (size_t i = 0; i < textFields.m_arrItems.size(); ++i)
	{
		OOX::Spreadsheet::CTextField* pTextField = static_cast<OOX::Spreadsheet::CTextField*>(textFields.m_arrItems[i]);
		
		int nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::TextField);
		WriteConnectionTextField(*pTextField);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteConnectionTextField(const OOX::Spreadsheet::CTextField& textField)
{
	int nCurPos;
	if (textField.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::TextFieldType);
		m_oBcw.m_oStream.WriteLONG(textField.m_oType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	
	if (textField.m_oPosition.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::TextFieldPosition);
		m_oBcw.m_oStream.WriteLONG(*textField.m_oPosition);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteBookViews(const OOX::Spreadsheet::CBookViews& bookViews)
{
	int nCurPos;
	if (false == bookViews.m_arrItems.empty())
	{
		OOX::Spreadsheet::CWorkbookView* pWorkbookView = static_cast<OOX::Spreadsheet::CWorkbookView*>(bookViews.m_arrItems[0]);
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::WorkbookView);
		WriteWorkbookView(*pWorkbookView);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteWorkbookView(const OOX::Spreadsheet::CWorkbookView& workbookView)
{
//ActiveTab
	if(workbookView.m_oActiveTab.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookViewTypes::ActiveTab);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(workbookView.m_oActiveTab->GetValue());
	}
}
void BinaryWorkbookTableWriter::WriteDefinedNames(const OOX::Spreadsheet::CDefinedNames& definedNames)
{
	int nCurPos;
	for(size_t i = 0, length = definedNames.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CDefinedName* pDefinedName = definedNames.m_arrItems[i];
		//DefinedName
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::DefinedName);
		WriteDefinedName(*pDefinedName);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteCalcPr(const OOX::Spreadsheet::CCalcPr& CCalcPr)
{
	int nCurPos = 0;
	if(CCalcPr.m_oCalcId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::CalcId);
		m_oBcw.m_oStream.WriteULONG(CCalcPr.m_oCalcId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oCalcMode.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::CalcMode);
		m_oBcw.m_oStream.WriteBYTE((BYTE)CCalcPr.m_oCalcMode->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oFullCalcOnLoad.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::FullCalcOnLoad);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oFullCalcOnLoad->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oRefMode.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::RefMode);
		m_oBcw.m_oStream.WriteBYTE((BYTE)CCalcPr.m_oRefMode->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oIterate.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::Iterate);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oIterate->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oIterateCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::IterateCount);
		m_oBcw.m_oStream.WriteULONG(CCalcPr.m_oIterateCount->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oIterateDelta.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::IterateDelta);
		m_oBcw.m_oStream.WriteDoubleReal(CCalcPr.m_oIterateDelta->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oFullPrecision.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::FullPrecision);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oFullPrecision->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oCalcCompleted.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::CalcCompleted);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oCalcCompleted->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oCalcOnSave.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::CalcOnSave);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oCalcOnSave->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oConcurrentCalc.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::ConcurrentCalc);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oConcurrentCalc->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oConcurrentManualCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::ConcurrentManualCount);
		m_oBcw.m_oStream.WriteULONG(CCalcPr.m_oConcurrentManualCount->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(CCalcPr.m_oForceFullCalc.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCalcPrTypes::ForceFullCalc);
		m_oBcw.m_oStream.WriteBOOL(CCalcPr.m_oForceFullCalc->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteConnections(const OOX::Spreadsheet::CConnections& connections)
{
	for (size_t i = 0; i < connections.m_arrItems.size(); ++i)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Connection);
		WriteConnection(*connections.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteConnection(const OOX::Spreadsheet::CConnection& connection)
{
	int nCurPos = 0;
	if(connection.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Type);
		m_oBcw.m_oStream.WriteULONG(*connection.m_oType);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::Name);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oName);
	}
	if(connection.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Id);
		m_oBcw.m_oStream.WriteULONG(connection.m_oId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (connection.m_oUId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::UId);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oUId);
	}
	if(connection.m_oCredentials.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Credentials);
		m_oBcw.m_oStream.WriteULONG(connection.m_oCredentials->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oBackground.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Background);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oBackground ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oDeleted.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Deleted);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oDeleted ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oDescription.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::Description);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oDescription);
	}
	if(connection.m_oInterval.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::Interval);
		m_oBcw.m_oStream.WriteLONG(*connection.m_oInterval);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(connection.m_oKeepAlive.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::KeepAlive);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oKeepAlive ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oMinRefreshableVersion.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::MinRefreshableVersion);
		m_oBcw.m_oStream.WriteLONG(*connection.m_oMinRefreshableVersion);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oNew.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::New);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oNew ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oOdcFile.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::OdcFile);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oOdcFile);
	}
	if(connection.m_oOdcFile.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::OdcFile);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oOdcFile);
	}
	if(connection.m_oOnlyUseConnectionFile.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::OnlyUseConnectionFile);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oOnlyUseConnectionFile ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oReconnectionMethod.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::ReconnectionMethod);
		m_oBcw.m_oStream.WriteLONG(*connection.m_oReconnectionMethod);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oRefreshedVersion.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::RefreshedVersion);
		m_oBcw.m_oStream.WriteLONG(*connection.m_oRefreshedVersion);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oRefreshOnLoad.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::RefreshOnLoad);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oRefreshOnLoad ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oSaveData.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::SaveData);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oSaveData ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oSavePassword.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::SavePassword);
		m_oBcw.m_oStream.WriteBYTE(*connection.m_oSavePassword ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oSingleSignOnId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::SingleSignOnId);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oSingleSignOnId);
	}
	if(connection.m_oSourceFile.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::SourceFile);
		m_oBcw.m_oStream.WriteStringW(*connection.m_oSourceFile);
	}
	if(connection.m_oDbPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::DbPr);
		WriteConnectionDbPr(connection.m_oDbPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oOlapPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::OlapPr);
		WriteConnectionOlapPr(connection.m_oOlapPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oTextPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::TextPr);
		WriteConnectionTextPr(connection.m_oTextPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oWebPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::WebPr);
		WriteConnectionWebPr(connection.m_oWebPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(connection.m_oExtLst.IsInit())
	{
		for (size_t i = 0; i < connection.m_oExtLst->m_arrExt.size(); ++i)
		{
			if (connection.m_oExtLst->m_arrExt[i]->m_oConnection.IsInit())
			{
				if(connection.m_oExtLst->m_arrExt[i]->m_oConnection->m_oIdExt.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerConnectionsTypes::IdExt);
					m_oBcw.m_oStream.WriteStringW(*connection.m_oExtLst->m_arrExt[i]->m_oConnection->m_oIdExt);
				}
				if (connection.m_oExtLst->m_arrExt[i]->m_oConnection->m_oRangePr.IsInit())
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerConnectionsTypes::RangePr);
					WriteConnectionRangePr(connection.m_oExtLst->m_arrExt[i]->m_oConnection->m_oRangePr.get());
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
		}
	}
}
void BinaryWorkbookTableWriter::WriteConnectionDbPr(const OOX::Spreadsheet::CDbPr& dbPr)
{
	int nCurPos = 0;
	if(dbPr.m_oConnection.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDbPrTypes::Connection);
		m_oBcw.m_oStream.WriteStringW(*dbPr.m_oConnection);
	}
	if(dbPr.m_oCommand.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDbPrTypes::Command);
		m_oBcw.m_oStream.WriteStringW(*dbPr.m_oCommand);
	}
	if(dbPr.m_oCommandType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDbPrTypes::CommandType);
		m_oBcw.m_oStream.WriteLONG(*dbPr.m_oCommandType);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(dbPr.m_oServerCommand.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDbPrTypes::ServerCommand);
		m_oBcw.m_oStream.WriteStringW(*dbPr.m_oServerCommand);
	}
}
void BinaryWorkbookTableWriter::WriteConnectionOlapPr(const OOX::Spreadsheet::COlapPr& olapPr)
{
	int nCurPos = 0;
	if(olapPr.m_oLocalConnection.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerOlapPrTypes::LocalConnection);
		m_oBcw.m_oStream.WriteStringW(*olapPr.m_oLocalConnection);
	}
	if(olapPr.m_oRowDrillCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::RowDrillCount);
		m_oBcw.m_oStream.WriteLONG(*olapPr.m_oRowDrillCount);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(olapPr.m_oLocal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::Local);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oLocal ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}		
	if(olapPr.m_oLocalRefresh.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::LocalRefresh);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oLocalRefresh ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(olapPr.m_oSendLocale.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::SendLocale);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oSendLocale ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(olapPr.m_oServerNumberFormat.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::ServerNumberFormat);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oServerNumberFormat ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(olapPr.m_oServerFont.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::ServerFont);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oServerFont ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(olapPr.m_oServerFontColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerOlapPrTypes::ServerFontColor);
		m_oBcw.m_oStream.WriteBYTE(*olapPr.m_oServerFontColor ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteConnectionTextPr(const OOX::Spreadsheet::CTextPr& textPr)
{
	int nCurPos = 0;
	if(textPr.m_oCharacterSet.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerTextPrTypes::CharacterSet);
		m_oBcw.m_oStream.WriteStringW(*textPr.m_oCharacterSet);
	}
	if(textPr.m_oSourceFile.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerTextPrTypes::SourceFile);
		m_oBcw.m_oStream.WriteStringW(*textPr.m_oSourceFile);
	}
	if(textPr.m_oDecimal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerTextPrTypes::Decimal);
		m_oBcw.m_oStream.WriteStringW(*textPr.m_oDecimal);
	}	
	if(textPr.m_oDelimiter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerTextPrTypes::Decimal);
		m_oBcw.m_oStream.WriteStringW(*textPr.m_oDelimiter);
	}	
	if(textPr.m_oThousands.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerTextPrTypes::Thousands);
		m_oBcw.m_oStream.WriteStringW(*textPr.m_oThousands);
	}	
	if(textPr.m_oFirstRow.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::FirstRow);
		m_oBcw.m_oStream.WriteLONG(*textPr.m_oFirstRow);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(textPr.m_oQualifier.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Qualifier);
		m_oBcw.m_oStream.WriteLONG(textPr.m_oQualifier->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(textPr.m_oFileType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::FileType);
		m_oBcw.m_oStream.WriteLONG(textPr.m_oFileType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if(textPr.m_oPrompt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Prompt);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oPrompt ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oDelimited.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Delimited);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oDelimited ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oTab.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Tab);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oTab ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oSpace.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Space);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oSpace ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oComma.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Comma);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oComma ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oSemicolon.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Semicolon);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oSemicolon ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(textPr.m_oConsecutive.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::Consecutive);
		m_oBcw.m_oStream.WriteBYTE(*textPr.m_oConsecutive ? 1 : 0);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (textPr.m_oCodePage.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::CodePage);
		m_oBcw.m_oStream.WriteLONG(*textPr.m_oCodePage);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (textPr.m_oTextFields.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerTextPrTypes::TextFields);
		WriteConnectionTextFields(textPr.m_oTextFields.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteConnectionRangePr(const OOX::Spreadsheet::CRangePr& rangePr)
{
	int nCurPos = 0;
	if (rangePr.m_oSourceName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRangePrTypes::SourceName);
		m_oBcw.m_oStream.WriteStringW(*rangePr.m_oSourceName);
	}
}
void BinaryWorkbookTableWriter::WriteConnectionWebPr(const OOX::Spreadsheet::CWebPr& webPr)
{
	int nCurPos = 0;
	if (webPr.m_oUrl.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Url);
		m_oBcw.m_oStream.WriteStringW3(*webPr.m_oUrl);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oPost.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Post);
		m_oBcw.m_oStream.WriteStringW3(*webPr.m_oPost);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oEditPage.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::EditPage);
		m_oBcw.m_oStream.WriteStringW3(*webPr.m_oEditPage);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if (webPr.m_oXml.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Xml);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oXml);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if (webPr.m_oSourceData.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::SourceData);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oSourceData);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oConsecutive.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Consecutive);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oConsecutive);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oFirstRow.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::FirstRow);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oFirstRow);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oXl97.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Xl97);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oXl97);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oTextDates.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::TextDates);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oTextDates);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oXl2000.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::Xl2000);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oXl2000);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oHtmlTables.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::HtmlTables);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oHtmlTables);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (webPr.m_oHtmlFormat.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWebPrTypes::HtmlFormat);
		m_oBcw.m_oStream.WriteBOOL(*webPr.m_oHtmlTables);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalReferences(const OOX::Spreadsheet::CExternalReferences& externalReferences, OOX::Spreadsheet::CWorkbook& workbook)
{
	for (size_t i = 0, length = externalReferences.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CExternalReference* pExternalReference = externalReferences.m_arrItems[i];
		if (!pExternalReference) continue;

		if (false == pExternalReference->m_oRid.IsInit()) continue;

		OOX::Spreadsheet::CExternalLink* pExternalLink = NULL;

		smart_ptr<OOX::File> pFile = workbook.Find(OOX::RId(pExternalReference->m_oRid->GetValue()));

		if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::ExternalLinks == pFile->type())			
			pExternalLink = static_cast<OOX::Spreadsheet::CExternalLink*>(pFile.operator ->());
		
		if (!pExternalLink) continue;

		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalReference);

		if (pExternalLink->m_oFileKey.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalFileKey);
			m_oBcw.m_oStream.WriteStringW3(*pExternalLink->m_oFileKey);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		if (pExternalLink->m_oInstanceId.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalInstanceId);
			m_oBcw.m_oStream.WriteStringW3(*pExternalLink->m_oInstanceId);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
		if (pExternalLink->m_oExternalBook.IsInit())
		{
			std::wstring sLink;
			if (pExternalLink->m_oExternalBook->m_oRid.IsInit())
			{
				smart_ptr<OOX::File> pFile = pExternalLink->Find(OOX::RId(pExternalLink->m_oExternalBook->m_oRid.get().GetValue()));
				if (pFile.IsInit() && OOX::FileTypes::ExternalLinkPath == pFile->type())
				{
					OOX::Spreadsheet::ExternalLinkPath* pLinkFile = static_cast<OOX::Spreadsheet::ExternalLinkPath*>(pFile.operator ->());
					sLink = pLinkFile->Uri().GetPath();
				}
			}
			if (!sLink.empty())
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalBook);
				WriteExternalBook(pExternalLink->m_oExternalBook.get(), sLink);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		else if (pExternalLink->m_oOleLink.IsInit())
		{
			std::wstring sLink;
			if (pExternalLink->m_oOleLink->m_oRid.IsInit())
			{
				smart_ptr<OOX::File> pFile = pExternalLink->Find(OOX::RId(pExternalLink->m_oOleLink->m_oRid.get().GetValue()));
				if (pFile.IsInit() && OOX::FileTypes::OleObject == pFile->type())
				{
					OOX::OleObject* pLinkFile = static_cast<OOX::OleObject*>(pFile.operator ->());
					sLink = pLinkFile->filename().GetPath();
				}
			}
			if (!sLink.empty())
			{
				int nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::OleLink);
				WriteOleLink(pExternalLink->m_oOleLink.get(), sLink);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		else if (pExternalLink->m_oDdeLink.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::DdeLink);
			WriteDdeLink(pExternalLink->m_oDdeLink.get());
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}

		m_oBcw.WriteItemWithLengthEnd(nCurPos2);
	}
}
void BinaryWorkbookTableWriter::WriteExternalBook(const OOX::Spreadsheet::CExternalBook& externalBook, const std::wstring& sLink)
{
	int nCurPos = 0;
	nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::Id);
	m_oBcw.m_oStream.WriteStringW3(sLink);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);

	if (externalBook.m_oSheetNames.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetNames);
		WriteExternalSheetNames(externalBook.m_oSheetNames.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (externalBook.m_oDefinedNames.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedNames);
		WriteExternalDefinedNames(externalBook.m_oDefinedNames.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (externalBook.m_oSheetDataSet.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataSet);
		WriteExternalSheetDataSet(externalBook.m_oSheetDataSet.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalSheetNames(const OOX::Spreadsheet::CExternalSheetNames& sheetNames)
{
	int nCurPos = 0;
	for (size_t i = 0, length = sheetNames.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetName);
		m_oBcw.m_oStream.WriteStringW3(sheetNames.m_arrItems[i]->ToString2());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalDefinedNames(const OOX::Spreadsheet::CExternalDefinedNames& definedNames)
{
	int nCurPos = 0;
	for (size_t i = 0, length = definedNames.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedName);
		WriteExternalDefinedName(*definedNames.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalDefinedName(const OOX::Spreadsheet::CExternalDefinedName& definedName)
{
	int nCurPos = 0;
	if (definedName.m_oName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedNameName);
		m_oBcw.m_oStream.WriteStringW3(definedName.m_oName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (definedName.m_oRefersTo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedNameRefersTo);
		m_oBcw.m_oStream.WriteStringW3(definedName.m_oRefersTo.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (definedName.m_oSheetId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedNameSheetId);
		m_oBcw.m_oStream.WriteULONG(definedName.m_oSheetId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalSheetDataSet(const OOX::Spreadsheet::CExternalSheetDataSet& sheetDataSet)
{
	int nCurPos = 0;
	for (size_t i = 0, length = sheetDataSet.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetData);
		WriteExternalSheetData(*sheetDataSet.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalSheetData(const OOX::Spreadsheet::CExternalSheetData& sheetData)
{
	int nCurPos = 0;
	if(sheetData.m_oSheetId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataSheetId);
		m_oBcw.m_oStream.WriteULONG(sheetData.m_oSheetId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(sheetData.m_oRefreshError.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRefreshError);
		m_oBcw.m_oStream.WriteBOOL(sheetData.m_oRefreshError->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for (size_t i = 0, length = sheetData.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRow);
		WriteExternalRow(*sheetData.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalRow(const OOX::Spreadsheet::CExternalRow& row)
{
	int nCurPos = 0;
	if(row.m_oR.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRowR);
		m_oBcw.m_oStream.WriteULONG(row.m_oR->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for (size_t i = 0, length = row.m_arrItems.size(); i < length; ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRowCell);
		WriteExternalCell(*row.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteExternalCell(const OOX::Spreadsheet::CExternalCell& cell)
{
	int nCurPos = 0;
	if(cell.m_oRef.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRowCellRef);
		m_oBcw.m_oStream.WriteStringW3(cell.m_oRef.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cell.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRowCellType);
		m_oBcw.m_oStream.WriteBYTE(cell.m_oType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(cell.m_oValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetDataRowCellValue);
		m_oBcw.m_oStream.WriteStringW3(cell.m_oValue->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteOleLink(const OOX::Spreadsheet::COleLink& oleLink, const std::wstring& sLink)
{
	int nCurPos = 0;
	nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::Id);
	m_oBcw.m_oStream.WriteStringW3(sLink);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);

	if (oleLink.m_oProgId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::ProgId);
		m_oBcw.m_oStream.WriteStringW3(oleLink.m_oProgId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oleLink.m_oOleItems.IsInit())
	{
		for(size_t i = 0; i < oleLink.m_oOleItems->m_arrItems.size(); ++i)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::OleItem);
			WriteOleItem(*oleLink.m_oOleItems->m_arrItems[i]);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
}
void BinaryWorkbookTableWriter::WriteOleItem(const OOX::Spreadsheet::COleItem& oleItem)
{
	int nCurPos = 0;
	if (oleItem.m_oName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::Name);
		m_oBcw.m_oStream.WriteStringW3(oleItem.m_oName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oleItem.m_oIcon.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::Icon);
		m_oBcw.m_oStream.WriteBOOL(oleItem.m_oIcon->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oleItem.m_oAdvise.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::Advise);
		m_oBcw.m_oStream.WriteBOOL(oleItem.m_oAdvise->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oleItem.m_oPreferPic.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OleLinkTypes::PreferPic);
		m_oBcw.m_oStream.WriteBOOL(oleItem.m_oPreferPic->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteDdeLink(const OOX::Spreadsheet::CDdeLink& ddeLink)
{
	int nCurPos = 0;
	if (ddeLink.m_oDdeService.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeService);
		m_oBcw.m_oStream.WriteStringW3(ddeLink.m_oDdeService.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeLink.m_oDdeTopic.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeTopic);
		m_oBcw.m_oStream.WriteStringW3(ddeLink.m_oDdeTopic.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeLink.m_oDdeItems.IsInit())
	{
		for(size_t i = 0; i < ddeLink.m_oDdeItems->m_arrItems.size(); ++i)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeItem);
			WriteDdeItem(*ddeLink.m_oDdeItems->m_arrItems[i]);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
}
void BinaryWorkbookTableWriter::WriteDdeItem(const OOX::Spreadsheet::CDdeItem& ddeItem)
{
	int nCurPos = 0;
	if (ddeItem.m_oName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::Name);
		m_oBcw.m_oStream.WriteStringW3(ddeItem.m_oName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeItem.m_oOle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::Ole);
		m_oBcw.m_oStream.WriteBOOL(ddeItem.m_oOle->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeItem.m_oAdvise.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::Advise);
		m_oBcw.m_oStream.WriteBOOL(ddeItem.m_oAdvise->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeItem.m_oPreferPic.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::PreferPic);
		m_oBcw.m_oStream.WriteBOOL(ddeItem.m_oPreferPic->ToBool());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeItem.m_oDdeValues.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValues);
		WriteDdeValues(ddeItem.m_oDdeValues.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteDdeValues(const OOX::Spreadsheet::CDdeValues& ddeValues)
{
	int nCurPos = 0;
	if (ddeValues.m_oRows.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValuesRows);
		m_oBcw.m_oStream.WriteULONG(ddeValues.m_oRows->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (ddeValues.m_oCols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValuesCols);
		m_oBcw.m_oStream.WriteULONG(ddeValues.m_oCols->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for(size_t i = 0; i < ddeValues.m_arrItems.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValue);
		WriteDdeValue(*ddeValues.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteDdeValue(const OOX::Spreadsheet::CDdeValue& ddeValue)
{
	int nCurPos = 0;
	if (ddeValue.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValueType);
		m_oBcw.m_oStream.WriteBYTE(ddeValue.m_oType->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	for (size_t i = 0; i < ddeValue.m_arrItems.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DdeLinkTypes::DdeValueVal);
		m_oBcw.m_oStream.WriteStringW3(ddeValue.m_arrItems[i]->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorkbookTableWriter::WriteDefinedName(const OOX::Spreadsheet::CDefinedName& definedName)
{
	int nCurPos = 0;
	//Name
	if(definedName.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDefinedNameTypes::Name);
		m_oBcw.m_oStream.WriteStringW(*definedName.m_oName);
	}
	//Ref
	if(definedName.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDefinedNameTypes::Ref);
		m_oBcw.m_oStream.WriteStringW(*definedName.m_oRef);
	}
	//LocalSheetId
	if(definedName.m_oLocalSheetId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerDefinedNameTypes::LocalSheetId);
		m_oBcw.m_oStream.WriteLONG(definedName.m_oLocalSheetId->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);		
	}
    //Hidden
    if(definedName.m_oHidden.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSerDefinedNameTypes::Hidden);
        m_oBcw.m_oStream.WriteBOOL(definedName.m_oHidden->ToBool());
        m_oBcw.WriteItemWithLengthEnd(nCurPos);
    }
	//Comment
	if (definedName.m_oComment.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerDefinedNameTypes::Comment);
		m_oBcw.m_oStream.WriteStringW(*definedName.m_oComment);
	}
}
void BinaryWorkbookTableWriter::WriteSlicerCaches(OOX::Spreadsheet::CWorkbook& workbook, const OOX::Spreadsheet::CSlicerCaches& oSlicerCaches)
{
	int nCurPos = 0;
	for(size_t i = 0; i < oSlicerCaches.m_oSlicerCache.size(); ++i)
	{
		if(oSlicerCaches.m_oSlicerCache[i].m_oRId.IsInit())
		{
			smart_ptr<OOX::File> pFile = workbook.Find(OOX::RId(oSlicerCaches.m_oSlicerCache[i].m_oRId->GetValue()));
			if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::SlicerCache == pFile->type())
			{
				OOX::Spreadsheet::CSlicerCacheFile* pSlicerCacheFile = static_cast<OOX::Spreadsheet::CSlicerCacheFile*>(pFile.GetPointer());
				if(pSlicerCacheFile->m_oSlicerCacheDefinition.IsInit())
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::SlicerCache);
					m_oBcw.m_oStream.WriteRecord2(0, pSlicerCacheFile->m_oSlicerCacheDefinition);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
		}
	}
}

BinaryPersonTableWriter::BinaryPersonTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream) : m_oBcw(oCBufferedStream)
{
}
void BinaryPersonTableWriter::Write(OOX::Spreadsheet::CPersonList& oPersonList)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WritePersonList(oPersonList);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryPersonTableWriter::WritePersonList(OOX::Spreadsheet::CPersonList& oPersonList)
{
	int nCurPos = 0;
	for(size_t i = 0; i < oPersonList.m_arrItems.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Person::person);
		WritePerson(*oPersonList.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryPersonTableWriter::WritePerson(OOX::Spreadsheet::CPerson& oPerson)
{
	int nCurPos = 0;
	if(oPerson.id.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Person::id);
		m_oBcw.m_oStream.WriteStringW3(oPerson.id.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oPerson.providerId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Person::providerId);
		m_oBcw.m_oStream.WriteStringW3(oPerson.providerId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oPerson.userId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Person::userId);
		m_oBcw.m_oStream.WriteStringW3(oPerson.userId.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oPerson.displayName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Person::displayName);
		m_oBcw.m_oStream.WriteStringW3(oPerson.displayName.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}

BinaryWorksheetTableWriter::BinaryWorksheetTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter) :
  m_oBcw(oCBufferedStream), m_pEmbeddedFontsManager(pEmbeddedFontsManager), m_pIndexedColors(pIndexedColors), m_pTheme(pTheme), m_oFontProcessor(oFontProcessor), m_pOfficeDrawingConverter(pOfficeDrawingConverter)
{
}
void BinaryWorksheetTableWriter::Write(OOX::Spreadsheet::CWorkbook& workbook,  std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteWorksheets(workbook, mapWorksheets);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryWorksheetTableWriter::Write(OOX::Spreadsheet::CWorkbook& workbook, std::vector<OOX::Spreadsheet::CWorksheet*>& arrWorksheets)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();

    for(size_t i = 0; i < arrWorksheets.size(); ++i)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Worksheet);
			WriteWorksheet(workbook.m_oSheets->m_arrItems[i], *arrWorksheets[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}

	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryWorksheetTableWriter::WriteWorksheets(OOX::Spreadsheet::CWorkbook& workbook, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets)
{
	int nCurPos;
	//определяем порядок следования .. излишне с vector
	if(workbook.m_oSheets.IsInit())
	{
        for(size_t i = 0; i < workbook.m_oSheets->m_arrItems.size(); ++i)
		{
            OOX::Spreadsheet::CSheet* pSheet = workbook.m_oSheets->m_arrItems[i];

            if(pSheet->m_oRid.IsInit())
			{
				std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>::const_iterator pFind = mapWorksheets.find(pSheet->m_oRid->GetValue());
				if(mapWorksheets.end() != pFind)
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Worksheet);
					WriteWorksheet(pSheet, *pFind->second);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
		}
	}
}
void BinaryWorksheetTableWriter::WriteWorksheet(OOX::Spreadsheet::CSheet* pSheet, OOX::Spreadsheet::CWorksheet& oWorksheet)
{
	int nCurPos;
	//WorksheetProp
	if (pSheet)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::WorksheetProp);
		WriteWorksheetProp(*pSheet);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Protection
	if (oWorksheet.m_oSheetProtection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Protection);
		WriteProtection(oWorksheet.m_oSheetProtection.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Cols
	if(oWorksheet.m_oCols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Cols);
		WriteCols(oWorksheet.m_oCols.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Dimension
	if(oWorksheet.m_oDimension.IsInit() && oWorksheet.m_oDimension->m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetsTypes::Dimension);
		m_oBcw.m_oStream.WriteStringW(*oWorksheet.m_oDimension->m_oRef);
	}
	//SheetViews
	if(oWorksheet.m_oSheetViews.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetViews);
		WriteSheetViews(oWorksheet.m_oSheetViews.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}	
	if (oWorksheet.m_oProtectedRanges.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::ProtectedRanges);
		WriteProtectedRanges(oWorksheet.m_oProtectedRanges.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//SheetFormatPr
	if(oWorksheet.m_oSheetFormatPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetFormatPr);
		WriteSheetFormatPr(oWorksheet.m_oSheetFormatPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//PageMargins
	if(oWorksheet.m_oPageMargins.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::PageMargins);
		WritePageMargins(oWorksheet.m_oPageMargins.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//PageSetup
	if(oWorksheet.m_oPageSetup.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::PageSetup);
		WritePageSetup(oWorksheet.m_oPageSetup.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//PrintOptions
	if(oWorksheet.m_oPrintOptions.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::PrintOptions);
		WritePrintOptions(oWorksheet.m_oPrintOptions.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Hyperlinks
	if(oWorksheet.m_oHyperlinks.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Hyperlinks);
		WriteHyperlinks(oWorksheet.m_oHyperlinks.get(), oWorksheet);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//MergeCells
	if(oWorksheet.m_oMergeCells.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::MergeCells);
		WriteMergeCells(oWorksheet.m_oMergeCells.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//SheetData
	if(oWorksheet.m_oSheetData.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetData);
		WriteSheetData(oWorksheet.m_oSheetData.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	// ConditionalFormatting
	if ( !oWorksheet.m_arrConditionalFormatting.empty() )
    {
		WriteConditionalFormattings(oWorksheet.m_arrConditionalFormatting, oWorksheet.m_mapConditionalFormattingEx, false);
	}
    if (oWorksheet.m_oExtLst.IsInit())
    {
        for(size_t i = 0; i < oWorksheet.m_oExtLst->m_arrExt.size(); ++i)
        {
            OOX::Drawing::COfficeArtExtension* pExt = oWorksheet.m_oExtLst->m_arrExt[i];
			if ( !pExt->m_arrConditionalFormatting.empty() )
			{
				WriteConditionalFormattings(pExt->m_arrConditionalFormatting, oWorksheet.m_mapConditionalFormattingEx, true);
			}
			else if ( pExt->m_oSlicerList.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Slicers);
				WriteSlicers(oWorksheet, pExt->m_oSlicerList.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			else if ( pExt->m_oSlicerListExt.IsInit() )
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SlicersExt);
				WriteSlicers(oWorksheet, pExt->m_oSlicerListExt.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
        }
    }
	// DataValidations (with ext)
	if ( oWorksheet.m_oDataValidations.IsInit() )
    {
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::DataValidations);
		WriteDataValidations(oWorksheet.m_oDataValidations.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
//Drawing
	 smart_ptr<OOX::CVmlDrawing> currentVmlDrawing;
	 smart_ptr<OOX::Spreadsheet::CDrawing> currentDrawing;

    if (oWorksheet.m_oLegacyDrawing.IsInit() &&
                    oWorksheet.m_oLegacyDrawing->m_oId.IsInit())
	{
        smart_ptr<OOX::File> oFile = oWorksheet.Find(oWorksheet.m_oLegacyDrawing->m_oId->GetValue());
		currentVmlDrawing = oFile.smart_dynamic_cast<OOX::CVmlDrawing>();
	}
	if (oWorksheet.m_oDrawing.IsInit() && oWorksheet.m_oDrawing->m_oId.IsInit())
	{
		smart_ptr<OOX::File> oFile = oWorksheet.Find(oWorksheet.m_oDrawing->m_oId->GetValue());
		currentDrawing = oFile.smart_dynamic_cast<OOX::Spreadsheet::CDrawing>();
	}
	
	WriteControls(oWorksheet, currentVmlDrawing.GetPointer());

	smart_ptr<OOX::IFileContainer> oldRels;
	if (currentDrawing.IsInit())
	{
		oldRels = m_pOfficeDrawingConverter->GetRels();
		m_pOfficeDrawingConverter->SetRels(currentDrawing.smart_dynamic_cast<OOX::IFileContainer>());
	}
	if (currentDrawing.IsInit() || currentVmlDrawing.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawings);
			WriteDrawings(oWorksheet, currentDrawing.GetPointer(), currentVmlDrawing.GetPointer());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oldRels.IsInit())
	{
		m_pOfficeDrawingConverter->SetRels(oldRels);
	}

	if (oWorksheet.m_oLegacyDrawingHF.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::LegacyDrawingHF);
		WriteLegacyDrawingHF(oWorksheet);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Autofilter
	if (oWorksheet.m_oAutofilter.IsInit())
	{
		BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Autofilter);
		oBinaryTableWriter.WriteAutoFilter(oWorksheet.m_oAutofilter.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//TableParts
	if (oWorksheet.m_oTableParts.IsInit())
	{
		BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::TableParts);
		oBinaryTableWriter.Write(oWorksheet, oWorksheet.m_oTableParts.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	smart_ptr<OOX::File> pFile = oWorksheet.Find(OOX::Spreadsheet::FileTypes::QueryTable);
	OOX::Spreadsheet::CQueryTableFile *pQueryTableFile = dynamic_cast<OOX::Spreadsheet::CQueryTableFile*>(pFile.GetPointer());
	if ((pQueryTableFile) && (pQueryTableFile->m_oQueryTable.IsInit()))
	{
		BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::QueryTable);
		oBinaryTableWriter.WriteQueryTable(pQueryTableFile->m_oQueryTable.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);		
	}
	pFile = oWorksheet.Find(OOX::Spreadsheet::FileTypes::NamedSheetView);
	OOX::Spreadsheet::CNamedSheetViewFile *pNamedSheetViewFile = dynamic_cast<OOX::Spreadsheet::CNamedSheetViewFile*>(pFile.GetPointer());
	if ((pNamedSheetViewFile) && (pNamedSheetViewFile->m_oNamedSheetViews.IsInit()))
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::NamedSheetView);
		m_oBcw.m_oStream.StartRecord(0);
		pNamedSheetViewFile->m_oNamedSheetViews->toPPTY(&m_oBcw.m_oStream);
		m_oBcw.m_oStream.EndRecord();
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Comments
	if (false == oWorksheet.m_mapComments.empty())
	{
		bool bIsEmpty = true;
		int nCurPos = 0;

        for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = oWorksheet.m_mapComments.begin(); it != oWorksheet.m_mapComments.end(); ++it)
		{
			if(it->second->IsValid())
			{
				bIsEmpty = false;
				break;
			}
		}
		if(false == bIsEmpty)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Comments);
			WriteComments(oWorksheet.m_mapComments);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	if (oWorksheet.m_oSheetPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetPr);
		WriteSheetPr(oWorksheet.m_oSheetPr.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oWorksheet.m_oHeaderFooter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::HeaderFooter);
		WritemHeaderFooter(oWorksheet.m_oHeaderFooter.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oWorksheet.m_oRowBreaks.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::RowBreaks);
		WritemRowColBreaks(oWorksheet.m_oRowBreaks.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oWorksheet.m_oColBreaks.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::ColBreaks);
		WritemRowColBreaks(oWorksheet.m_oColBreaks.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oWorksheet.m_oCellWatches.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::CellWatches);
		WriteCellWatches(oWorksheet.m_oCellWatches.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if (oWorksheet.m_oPicture.IsInit() && oWorksheet.m_oPicture->m_oId.IsInit())
	{
		smart_ptr<OOX::File> pFile = oWorksheet.Find(oWorksheet.m_oPicture->m_oId->GetValue());
		if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
		{
			OOX::Image* pImageFileCache = static_cast<OOX::Image*>(pFile.GetPointer());
			OOX::CPath pathImage = pImageFileCache->filename();
			std::wstring	additionalPath;
			int				additionalType = 0;
			double dX = -1.0; //mm
			double dY = -1.0;
			double dW = -1.0; //mm
			double dH = -1.0;
			NSShapeImageGen::CMediaInfo oId = m_pOfficeDrawingConverter->m_pBinaryWriter->m_pCommon->m_pMediaManager->WriteImage(pathImage.GetPath(), dX, dY, dW, dH, additionalPath, additionalType);
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Picture);
			m_oBcw.m_oStream.WriteStringW3(oId.GetPath2());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	if (oWorksheet.m_oSortState.IsInit())
	{
		BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SortState);
		oBinaryTableWriter.WriteSortState(oWorksheet.m_oSortState.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
    if (oWorksheet.m_oExtLst.IsInit())
    {
        for(size_t i = 0; i < oWorksheet.m_oExtLst->m_arrExt.size(); ++i)
        {
            OOX::Drawing::COfficeArtExtension* pExt = oWorksheet.m_oExtLst->m_arrExt[i];
            if( pExt->m_oSparklineGroups.IsInit() )
            {
                nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SparklineGroups);
                WriteSparklineGroups(pExt->m_oSparklineGroups.get());
                m_oBcw.WriteItemWithLengthEnd(nCurPos);
            }
        }
    }
	//pFile = oWorksheet.Find(OOX::Spreadsheet::FileTypes::PivotTable);
	//OOX::Spreadsheet::CPivotTableFile *pPivotTableFile = dynamic_cast<OOX::Spreadsheet::CPivotTableFile*>(pFile.GetPointer());
	//if ((pPivotTableFile) && (pPivotTableFile->m_oPivotTable.IsInit()))
	//{
	//	BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
	//	nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::PivotTable);
	//	oBinaryTableWriter.WriteQueryTable(pPivotTableFile->m_oPivotTable.get());
	//	m_oBcw.WriteItemWithLengthEnd(nCurPos);		
	//}
}
void BinaryWorksheetTableWriter::WriteWorksheetProp(OOX::Spreadsheet::CSheet& oSheet)
{
	//Name
	if(oSheet.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetPropTypes::Name);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*oSheet.m_oName);
	}
	//SheetId
	if(oSheet.m_oSheetId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetPropTypes::SheetId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oSheet.m_oSheetId->GetValue());
	}
	//State
	if(oSheet.m_oState.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetPropTypes::State);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oSheet.m_oState->GetValue());
	}
}
void BinaryWorksheetTableWriter::WriteCellWatches(const OOX::Spreadsheet::CCellWatches& cellWatches)
{
	for (size_t nIndex = 0, nLength = cellWatches.m_arrItems.size(); nIndex < nLength; ++nIndex)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::CellWatch);
		WriteCellWatch(*cellWatches.m_arrItems[nIndex]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCellWatch(const OOX::Spreadsheet::CCellWatch& cellWatch)
{
	if (cellWatch.m_oR.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetsTypes::CellWatchR);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*cellWatch.m_oR);
	}
}
void BinaryWorksheetTableWriter::WriteProtectedRanges(const OOX::Spreadsheet::CProtectedRanges& protectedRanges)
{
	for (size_t nIndex = 0, nLength = protectedRanges.m_arrItems.size(); nIndex < nLength; ++nIndex)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::ProtectedRange);
		WriteProtectedRange(*protectedRanges.m_arrItems[nIndex]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

void BinaryWorksheetTableWriter::WriteProtectedRange(const OOX::Spreadsheet::CProtectedRange& protectedRange)
{
	if (protectedRange.m_oAlgorithmName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::AlgorithmName);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(protectedRange.m_oAlgorithmName->GetValue());
	}
	if (protectedRange.m_oSpinCount.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::SpinCount);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG(protectedRange.m_oSpinCount->GetValue());
	}
	if (protectedRange.m_oHashValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::HashValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protectedRange.m_oHashValue);
	}
	if (protectedRange.m_oSaltValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::SaltValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protectedRange.m_oSaltValue);
	}
	if (protectedRange.m_oName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::Name);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protectedRange.m_oName);
	}
	if (protectedRange.m_oSqref.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::SqRef);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protectedRange.m_oSqref);
	}
	for (size_t i = 0; i < protectedRange.m_arSecurityDescriptors.size(); ++i)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerProtectedRangeTypes::SecurityDescriptor);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(protectedRange.m_arSecurityDescriptors[i]);
	}
}
void BinaryWorksheetTableWriter::WriteProtection(const OOX::Spreadsheet::CSheetProtection& protection)
{
	if (protection.m_oAlgorithmName.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::AlgorithmName);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(protection.m_oAlgorithmName->GetValue());
	}	
	if (protection.m_oSpinCount.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::SpinCount);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteULONG(protection.m_oSpinCount->GetValue());
	}
	if (protection.m_oHashValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::HashValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oHashValue);
	}
	if (protection.m_oSaltValue.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::SaltValue);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oSaltValue);
	}
	if (protection.m_oPassword.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Password);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(*protection.m_oPassword);
	}
	if (protection.m_oAutoFilter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::AutoFilter);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oAutoFilter->ToBool());
	}
	if (protection.m_oContent.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Content);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oContent->ToBool());
	}
	if (protection.m_oDeleteColumns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::DeleteColumns);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oDeleteColumns->ToBool());
	}
	if (protection.m_oDeleteRows.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::DeleteRows);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oDeleteRows->ToBool());
	}
	if (protection.m_oFormatCells.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::FormatCells);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oFormatCells->ToBool());
	}
	if (protection.m_oFormatColumns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::FormatColumns);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oFormatColumns->ToBool());
	}
	if (protection.m_oFormatRows.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::FormatRows);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oFormatRows->ToBool());
	}
	if (protection.m_oInsertColumns.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::InsertColumns);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oInsertColumns->ToBool());
	}
	if (protection.m_oInsertHyperlinks.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::InsertHyperlinks);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oInsertHyperlinks->ToBool());
	}
	if (protection.m_oInsertRows.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::InsertRows);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oInsertRows->ToBool());
	}
	if (protection.m_oObjects.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Objects);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oObjects->ToBool());
	}
	if (protection.m_oPivotTables.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::PivotTables);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oPivotTables->ToBool());
	}
	if (protection.m_oScenarios.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Scenarios);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oScenarios->ToBool());
	}
	if (protection.m_oSelectLockedCells.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::SelectLockedCells);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oSelectLockedCells->ToBool());
	}
	if (protection.m_oSelectUnlockedCells.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::SelectUnlockedCell);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oSelectUnlockedCells->ToBool());
	}
	if (protection.m_oSheet.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Sheet);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oSheet->ToBool());
	}
	if (protection.m_oSort.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetProtection::Sort);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(protection.m_oSort->ToBool());
	}
}
void BinaryWorksheetTableWriter::WriteCols(const OOX::Spreadsheet::CCols& oCols)
{
	int nCurPos;
	for(size_t i = 0, length = oCols.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CCol* pCol = oCols.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Col);
		WriteCol(*pCol);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCol(const OOX::Spreadsheet::CCol& oCol)
{
	//BestFit
	if(oCol.m_oBestFit.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::BestFit);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCol.m_oBestFit->ToBool());
	}
	//Hidden
	if(oCol.m_oHidden.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Hidden);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCol.m_oHidden->ToBool());
	}
	//Max
	if(oCol.m_oMax.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Max);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oCol.m_oMax->GetValue());
	}
	//Min
	if(oCol.m_oMin.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Min);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oCol.m_oMin->GetValue());
	}
	//Style
	if(oCol.m_oStyle.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Style);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oCol.m_oStyle->GetValue());
	}
	//Width
	if(oCol.m_oWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Width);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oCol.m_oWidth->GetValue());
	}
    //CustomWidth
    if(oCol.m_oCustomWidth.IsInit())
    {
        m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::CustomWidth);
        m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
        m_oBcw.m_oStream.WriteBYTE(oCol.m_oCustomWidth->ToBool());
    }
    if(oCol.m_oOutlineLevel.IsInit())
    {
        m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::OutLevel);
        m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
        m_oBcw.m_oStream.WriteLONG(oCol.m_oOutlineLevel->GetValue());
    }
	if(oCol.m_oCollapsed.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetColTypes::Collapsed);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCol.m_oCollapsed->ToBool());
	}
}

void BinaryWorksheetTableWriter::WriteSheetViews(const OOX::Spreadsheet::CSheetViews& oSheetViews)
{
	int nCurPos = 0;
	for (size_t nIndex = 0, nLength = oSheetViews.m_arrItems.size(); nIndex < nLength; ++nIndex)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetView);
		WriteSheetView(*oSheetViews.m_arrItems[nIndex]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

void BinaryWorksheetTableWriter::WriteSheetView(const OOX::Spreadsheet::CSheetView& oSheetView)
{
	int nCurPos = 0;
	if (oSheetView.m_oColorId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ColorId);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oColorId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oDefaultGridColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::DefaultGridColor);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oDefaultGridColor->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oRightToLeft.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::RightToLeft);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oRightToLeft->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowFormulas.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowFormulas);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowFormulas->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowGridLines.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowGridLines);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowGridLines->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowOutlineSymbols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowOutlineSymbols);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowOutlineSymbols->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowRowColHeaders.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowRowColHeaders);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowRowColHeaders->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowRuler.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowRuler);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowRuler->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowWhiteSpace.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowWhiteSpace);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowWhiteSpace->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oShowZeros.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ShowZeros);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oShowZeros->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oTabSelected.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::TabSelected);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oTabSelected->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oTopLeftCell.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::TopLeftCell);
		m_oBcw.m_oStream.WriteStringW4(*oSheetView.m_oTopLeftCell);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oView.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::View);
		m_oBcw.m_oStream.WriteBYTE(oSheetView.m_oView->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oWindowProtection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::WindowProtection);
		m_oBcw.m_oStream.WriteBOOL(oSheetView.m_oWindowProtection->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oWorkbookViewId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::WorkbookViewId);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oWorkbookViewId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oZoomScale.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ZoomScale);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oZoomScale->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oZoomScaleNormal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ZoomScaleNormal);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oZoomScaleNormal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oZoomScalePageLayoutView.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ZoomScalePageLayoutView);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oZoomScalePageLayoutView->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetView.m_oZoomScaleSheetLayoutView.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::ZoomScaleSheetLayoutView);
		m_oBcw.m_oStream.WriteLONG(oSheetView.m_oZoomScaleSheetLayoutView->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	if (oSheetView.m_oPane.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::Pane);
		WritePane(oSheetView.m_oPane.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for(size_t i = 0; i < oSheetView.m_arrItems.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetView::Selection);
		WriteSelection(*oSheetView.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

void BinaryWorksheetTableWriter::WritePane(const OOX::Spreadsheet::CPane& oPane)
{
	int nCurPos = 0;
	if (oPane.m_oActivePane.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Pane::ActivePane);
		m_oBcw.m_oStream.WriteBYTE(oPane.m_oActivePane->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//State
	if (oPane.m_oState.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Pane::State);
		m_oBcw.m_oStream.WriteStringW4(oPane.m_oState->ToString());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//TopLeftCell
	if (oPane.m_oTopLeftCell.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Pane::TopLeftCell);
		m_oBcw.m_oStream.WriteStringW(*oPane.m_oTopLeftCell);
	}
	//XSplit
	if (oPane.m_oXSplit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Pane::XSplit);
		m_oBcw.m_oStream.WriteDoubleReal(oPane.m_oXSplit->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//YSplit
	if (oPane.m_oYSplit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Pane::YSplit);
		m_oBcw.m_oStream.WriteDoubleReal(oPane.m_oYSplit->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteSelection(const OOX::Spreadsheet::CSelection& oSelection)
{
	int nCurPos = 0;
	if (oSelection.m_oActiveCell.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Selection::ActiveCell);
		m_oBcw.m_oStream.WriteStringW4(oSelection.m_oActiveCell.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSelection.m_oActiveCellId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Selection::ActiveCellId);
		m_oBcw.m_oStream.WriteLONG(oSelection.m_oActiveCellId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSelection.m_oPane.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Selection::Pane);
		m_oBcw.m_oStream.WriteBYTE(oSelection.m_oPane->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSelection.m_oSqref.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Selection::Sqref);
		m_oBcw.m_oStream.WriteStringW4(oSelection.m_oSqref.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

void BinaryWorksheetTableWriter::WriteSheetFormatPr(const OOX::Spreadsheet::CSheetFormatPr& oSheetFormatPr)
{
	//DefaultColWidth
	if(oSheetFormatPr.m_oDefaultColWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::DefaultColWidth);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(*oSheetFormatPr.m_oDefaultColWidth);
	}
	//DefaultRowHeight
	if(oSheetFormatPr.m_oDefaultRowHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::DefaultRowHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(*oSheetFormatPr.m_oDefaultRowHeight);
	}
	//BaseColWidth
	if(oSheetFormatPr.m_oBaseColWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::BaseColWidth);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*oSheetFormatPr.m_oBaseColWidth);
	}
	//CustomHeight
	if(oSheetFormatPr.m_oCustomHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::CustomHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(*oSheetFormatPr.m_oCustomHeight);
	}
	//ZeroHeight
	if(oSheetFormatPr.m_oZeroHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::ZeroHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(*oSheetFormatPr.m_oZeroHeight);
	}
	//OutlineLevelCol
	if(oSheetFormatPr.m_oOutlineLevelCol.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::OutlineLevelCol);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*oSheetFormatPr.m_oOutlineLevelCol);
	}
	//OutlineLevelRow
	if(oSheetFormatPr.m_oOutlineLevelRow.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::OutlineLevelRow);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(*oSheetFormatPr.m_oOutlineLevelRow);
	}
}
void BinaryWorksheetTableWriter::WritePageMargins(const OOX::Spreadsheet::CPageMargins& oPageMargins)
{
	//Left
	if(oPageMargins.m_oLeft.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Left);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oLeft->ToMm());
	}
	//Top
	if(oPageMargins.m_oTop.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Top);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oTop->ToMm());
	}
	//Right
	if(oPageMargins.m_oRight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Right);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oRight->ToMm());
	}
	//Bottom
	if(oPageMargins.m_oBottom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Bottom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oBottom->ToMm());
	}
	//Header
	if(oPageMargins.m_oHeader.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Header);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oHeader->ToMm());
	}
	//Footer
	if(oPageMargins.m_oFooter.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageMargins::Footer);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageMargins.m_oFooter->ToMm());
	}
}
void BinaryWorksheetTableWriter::WritePageSetup(const OOX::Spreadsheet::CPageSetup& oPageSetup)
{
	//PageSize
	if(oPageSetup.m_oPaperSize.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperSize);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oPaperSize->GetValue());
	}
	if(oPageSetup.m_oBlackAndWhite.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::BlackAndWhite);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPageSetup.m_oBlackAndWhite->ToBool());
	}
	if(oPageSetup.m_oCellComments.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::CellComments);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oCellComments->GetValue());
	}
	if(oPageSetup.m_oCopies.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Copies);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oCopies->GetValue());
	}
	if(oPageSetup.m_oDraft.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Draft);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPageSetup.m_oDraft->ToBool());
	}
	if(oPageSetup.m_oErrors.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Errors);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oErrors->GetValue());
	}
	if(oPageSetup.m_oFirstPageNumber.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::FirstPageNumber);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oFirstPageNumber->GetValue());
	}
	if(oPageSetup.m_oFitToHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::FitToHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oFitToHeight->GetValue());
	}
	if(oPageSetup.m_oFitToWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::FitToWidth);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oFitToWidth->GetValue());
	}
	if(oPageSetup.m_oHorizontalDpi.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::HorizontalDpi);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oHorizontalDpi->GetValue());
	}
	//Orientation
	if(oPageSetup.m_oOrientation.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Orientation);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oOrientation->GetValue());
	}
	if(oPageSetup.m_oPageOrder.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PageOrder);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oPageOrder->GetValue());
	}
	if(oPageSetup.m_oPaperHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageSetup.m_oPaperHeight->GetValue());
	}
	if(oPageSetup.m_oPaperWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperWidth);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageSetup.m_oPaperWidth->GetValue());
	}
	if(oPageSetup.m_oPaperWidth.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperWidth);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPageSetup.m_oPaperWidth->GetValue());
	}
	if(oPageSetup.m_oPaperUnits.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperUnits);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oPaperUnits->GetValue());
	}
	if(oPageSetup.m_oScale.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Scale);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oScale->GetValue());
	}
	if(oPageSetup.m_oUseFirstPageNumber.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::UseFirstPageNumber);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPageSetup.m_oUseFirstPageNumber->ToBool());
	}
	if(oPageSetup.m_oUsePrinterDefaults.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::UsePrinterDefaults);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPageSetup.m_oUsePrinterDefaults->ToBool());
	}
	if(oPageSetup.m_oVerticalDpi.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::VerticalDpi);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oPageSetup.m_oVerticalDpi->GetValue());
	}

}
void BinaryWorksheetTableWriter::WritePrintOptions(const OOX::Spreadsheet::CPrintOptions& oPrintOptions)
{
	//GridLines
	if(oPrintOptions.m_oGridLines.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PrintOptions::GridLines);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPrintOptions.m_oGridLines->ToBool());
	}
	//Headings
	if(oPrintOptions.m_oHeadings.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_PrintOptions::Headings);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oPrintOptions.m_oHeadings->ToBool());
	}
}
void BinaryWorksheetTableWriter::WriteHyperlinks(const OOX::Spreadsheet::CHyperlinks& oHyperlinks, OOX::Spreadsheet::CWorksheet& oWorksheet)
{
	int nCurPos;
	for(size_t i = 0, length = oHyperlinks.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CHyperlink* pHyperlink = oHyperlinks.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Hyperlink);
		WriteHyperlink(*pHyperlink, oWorksheet);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
};
void BinaryWorksheetTableWriter::WriteHyperlink(const OOX::Spreadsheet::CHyperlink& oHyperlink, OOX::Spreadsheet::CWorksheet& oWorksheet)
{
	std::wstring sRef;
	std::wstring sHyperlink;
	std::wstring sLocation;
	std::wstring sDisplay;

	if(oHyperlink.m_oRef.IsInit())
		sRef = oHyperlink.m_oRef.get();
	if(oHyperlink.m_oRid.IsInit() && oWorksheet.m_pCurRels.IsInit())
	{
		OOX::Rels::CRelationShip* oRels = NULL;
		oWorksheet.m_pCurRels->GetRel( OOX::RId(oHyperlink.m_oRid->GetValue()), &oRels);
		if(NULL != oRels && _T("http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink") == oRels->Type() )
		{
			if(oRels->IsExternal())
				sHyperlink = oRels->Target().GetPath();
		}
	}
	else if (oHyperlink.m_oLink.IsInit())
	{
		sHyperlink = *oHyperlink.m_oLink;
	}
	if(oHyperlink.m_oLocation.IsInit())
		sLocation = *oHyperlink.m_oLocation;
	if (oHyperlink.m_oDisplay.IsInit())
		sDisplay = *oHyperlink.m_oDisplay;

	if (!sRef.empty() && (!sHyperlink.empty() || !sLocation.empty() || !sDisplay.empty()))
	{
		//Ref
		m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Ref);
		m_oBcw.m_oStream.WriteStringW(sRef);
		//Hyperlink
		if (!sHyperlink.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Hyperlink);
			m_oBcw.m_oStream.WriteStringW(sHyperlink);
		}
		//Location
		if (!sLocation.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Location);
			m_oBcw.m_oStream.WriteStringW(sLocation);
		}
		//Tooltip
		if(oHyperlink.m_oTooltip.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Tooltip);
			m_oBcw.m_oStream.WriteStringW(*oHyperlink.m_oTooltip);
		}
		//Display
		if(oHyperlink.m_oDisplay.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Display);
			m_oBcw.m_oStream.WriteStringW(*oHyperlink.m_oDisplay);
		}
	}
}
void BinaryWorksheetTableWriter::WriteMergeCells(const OOX::Spreadsheet::CMergeCells& oMergeCells)
{
	for(size_t i = 0, length = oMergeCells.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CMergeCell* pMergeCell = oMergeCells.m_arrItems[i];
		if(pMergeCell->m_oRef.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetsTypes::MergeCell);
			m_oBcw.m_oStream.WriteStringW(*pMergeCell->m_oRef);
		}
	}
}
void BinaryWorksheetTableWriter::WriteSheetData(const OOX::Spreadsheet::CSheetData& oSheetData)
{
	int nCurPos;
	if(oSheetData.m_oXlsbPos.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::XlsbPos);
		m_oBcw.m_oStream.WriteLONG(oSheetData.m_oXlsbPos->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	else
	{
		for(size_t i = 0, length = oSheetData.m_arrItems.size(); i < length; ++i)
		{
			OOX::Spreadsheet::CRow* pRow = oSheetData.m_arrItems[i];
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Row);
			WriteRow(*pRow);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryWorksheetTableWriter::WriteRow(const OOX::Spreadsheet::CRow& oRows)
{
	int nCurPos;
	//Row
	if(oRows.m_oR.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Row);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oRows.m_oR->GetValue());
	}
	//Style
	if(oRows.m_oS.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Style);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oRows.m_oS->GetValue());
	}
	//Height
	if(oRows.m_oHt.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Height);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oRows.m_oHt->GetValue());
	}
	//Hidden
	if(oRows.m_oHidden.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Hidden);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oRows.m_oHidden->ToBool());
	}
	//CustomHeight
	if(oRows.m_oCustomHeight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::CustomHeight);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oRows.m_oCustomHeight->ToBool());
	}
    if(oRows.m_oOutlineLevel.IsInit())
    {
        m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::OutLevel);
        m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
        m_oBcw.m_oStream.WriteLONG(oRows.m_oOutlineLevel->GetValue());
    }
	if(oRows.m_oCollapsed.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Collapsed);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oRows.m_oCollapsed->ToBool());
	}
    if(oRows.m_arrItems.size() > 0)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Cells);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteCells(oRows);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCells(const OOX::Spreadsheet::CRow& oRows)
{
	int nCurPos;
	for(size_t i = 0, length = oRows.m_arrItems.size(); i < length; ++i)
	{
		OOX::Spreadsheet::CCell* oCell =oRows.m_arrItems[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSerRowTypes::Cell);
		WriteCell(*oCell);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCell(const OOX::Spreadsheet::CCell& oCell)
{
	int nCurPos;
//Ref
	int nRow = 0;
	int nCol = 0;
	if (oCell.isInitRef() && oCell.getRowCol(nRow, nCol))
	{
		// Пишем теперь не строку, а 2 числа (чтобы не парсить на JavaScript, т.к. на C++ быстрее парсинг). Ускорение открытия файла.
		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::RefRowCol);
		m_oBcw.m_oStream.WriteLONG(nRow);
		m_oBcw.m_oStream.WriteLONG(nCol);
		m_oBcw.WriteItemEnd(nCurPos);

		//m_oBcw.m_oStream.WriteBYTE(c_oSerCellTypes::Ref);
		//m_oBcw.m_oStream.WriteStringW(oCell.m_oRef.get2());
	}
	//Style
	if(oCell.m_oStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::Style);
		m_oBcw.m_oStream.WriteLONG(*oCell.m_oStyle);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Type
	if(oCell.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::Type);
		m_oBcw.m_oStream.WriteBYTE(oCell.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	//Formula
	if(oCell.m_oFormula.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::Formula);
		WriteFormula(oCell.m_oFormula.get2());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Value
	if (oCell.m_oValue.IsInit() && !oCell.m_oValue->ToString().empty())
	{

        double dValue = 0;
        try
        {
            dValue = XmlUtils::GetDouble(oCell.m_oValue->ToString());
        }
        catch(...)
        {   //1.3912059045063478e-310
            //Lighting Load Calculation.xls
        }

		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::Value);
		m_oBcw.m_oStream.WriteDoubleReal(dValue);
		m_oBcw.WriteItemEnd(nCurPos);
	}
//ValueCache	
	if (oCell.m_oCacheValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::ValueCache);
		m_oBcw.m_oStream.WriteStringW3(*oCell.m_oCacheValue);
		m_oBcw.WriteItemEnd(nCurPos);		
	}
}
void BinaryWorksheetTableWriter::WriteFormula(OOX::Spreadsheet::CFormula& oFormula)
{
	//Aca
	if(oFormula.m_oAca.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Aca);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oAca->ToBool());
	}
	//Bx
	if(oFormula.m_oBx.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Bx);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oBx->ToBool());
	}
	//Ca
	if(oFormula.m_oCa.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Ca);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oCa->ToBool());
	}
	//Del1
	if(oFormula.m_oDel1.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Del1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oDel1->ToBool());
	}
	//Del2
	if(oFormula.m_oDel2.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Del2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oDel2->ToBool());
	}
	//Dt2D
	if(oFormula.m_oDt2D.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Dt2D);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oDt2D->ToBool());
	}
	//Dtr
	if(oFormula.m_oDtr.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Dtr);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oFormula.m_oDtr->ToBool());
	}
	//R1
	if(oFormula.m_oR1.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::R1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oFormula.m_oR1.get2());
	}
	//R2
	if(oFormula.m_oR2.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::R2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oFormula.m_oR2.get2());
	}
	//Ref
	if(oFormula.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Ref);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oFormula.m_oRef.get2());
	}
	//Si
	if(oFormula.m_oSi.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Si);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFormula.m_oSi->GetValue());
	}
	//T
	if(oFormula.m_oT.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::T);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oFormula.m_oT->GetValue());
	}
	//Text
	if(!oFormula.m_sText.empty())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerFormulaTypes::Text);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oFormula.m_sText);

		if(NULL != m_pEmbeddedFontsManager)
			m_pEmbeddedFontsManager->CheckString(oFormula.m_sText);
	}
}

void BinaryWorksheetTableWriter::WriteOleObjects(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::CVmlDrawing *pVmlDrawing)
{
	if(false == oWorksheet.m_oOleObjects.IsInit()) return;

	for (boost::unordered_map<unsigned int, OOX::Spreadsheet::COleObject*>::const_iterator it = oWorksheet.m_oOleObjects->m_mapOleObjects.begin(); it != oWorksheet.m_oOleObjects->m_mapOleObjects.end(); ++it)
	{
		OOX::Spreadsheet::COleObject*	pOleObject	= it->second;
		OOX::WritingElement*			pShapeElem	= NULL;
		
		if (!pOleObject) continue;
		
		nullable<OOX::Spreadsheet::CCellAnchor>	oCellAnchor;        

		smart_ptr<OOX::OleObject>	pFileOleObject;
		nullable<OOX::RId>			pRId;

		if (pOleObject->m_oRid.IsInit())
		{
			pRId = new OOX::RId( pOleObject->m_oRid->GetValue());

			//ищем физический файл ( rId относительно sheet)
			smart_ptr<OOX::File> pFile = oWorksheet.Find(pRId.get());
			pFileOleObject = pFile.smart_dynamic_cast<OOX::OleObject>();
		}
		if (false == pFileOleObject.IsInit()) continue;
		
		if (!( pFileOleObject->IsExist() || pFileOleObject->IsExternal() )) continue;
		
		std::wstring sShapeId = L"_x0000_s" + std::to_wstring(pOleObject->m_oShapeId->GetValue());
		if (pVmlDrawing)
		{
            boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = pVmlDrawing->m_mapShapes.find(sShapeId);
			
			if (pFind != pVmlDrawing->m_mapShapes.end())
			{
				pFind->second.bUsed = true;
				pShapeElem	= pFind->second.pElement;
			}			
		}
		SimpleTypes::Spreadsheet::CCellAnchorType eAnchorType;
		eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);		
		bool bSetAnchor = false;
		if (pOleObject->m_oObjectPr.IsInit() && pOleObject->m_oObjectPr->m_oAnchor.IsInit() && pOleObject->m_oObjectPr->m_oRid.IsInit())
		{
			const OOX::Spreadsheet::CExtAnchor& oAnchor = pOleObject->m_oObjectPr->m_oAnchor.get();

			if (oAnchor.m_oFrom.IsInit() && oAnchor.m_oTo.IsInit())
			{
				if (oAnchor.m_oMoveWithCells.IsInit() && *oAnchor.m_oMoveWithCells)
					eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
				else if (oAnchor.m_oSizeWithCells.IsInit() && *oAnchor.m_oSizeWithCells)
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
		}

		oCellAnchor->m_bShapeOle = true;

		PPTX::Logic::Pic *olePic = new PPTX::Logic::Pic;

		olePic->oleObject.Init();
		olePic->blipFill.blip.Init();

		if (pOleObject->m_oProgId.IsInit())
			olePic->oleObject->m_sProgId = pOleObject->m_oProgId.get();
		
		if (pOleObject->m_oObjectPr.IsInit() && pOleObject->m_oObjectPr->m_oAnchor.IsInit())
		{
			olePic->oleObject->m_oMoveWithCells = pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells;
			olePic->oleObject->m_oSizeWithCells = pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells;
		}

		olePic->oleObject->m_oId = pRId;
		olePic->oleObject->m_OleObjectFile = pFileOleObject;

		if (pOleObject->m_oDvAspect.IsInit())
		{
			olePic->oleObject->m_oDrawAspect = (BYTE)pOleObject->m_oDvAspect->GetValue();
		}

		if (olePic->oleObject->m_OleObjectFile.IsInit())
		{
			olePic->blipFill.blip->oleFilepathBin = olePic->oleObject->m_OleObjectFile->filename().GetPath();
		}

		OOX::Image*		pImageFileCache = NULL;
		std::wstring	sIdImageFileCache;
		if ((NULL != pShapeElem) && (OOX::et_v_shapetype != pShapeElem->getType()))
		{
			OOX::Vml::CShape* pShape = static_cast<OOX::Vml::CShape*>(pShapeElem);
			for (size_t j = 0; (pShape) && (j < pShape->m_arrItems.size()); ++j)
			{
				OOX::WritingElement* pChildElemShape = pShape->m_arrItems[j];
				if (!bSetAnchor && OOX::et_v_ClientData == pChildElemShape->getType())
				{
					OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pChildElemShape);
					pClientData->toCellAnchor(oCellAnchor.GetPointer());

					olePic->oleObject->m_oMoveWithCells = pClientData->m_oMoveWithCells;
					olePic->oleObject->m_oSizeWithCells = pClientData->m_oSizeWithCells;
				}
				if (OOX::et_v_imagedata == pChildElemShape->getType())
				{
					OOX::Vml::CImageData* pImageData = static_cast<OOX::Vml::CImageData*>(pChildElemShape);

					if (pImageData->m_oRelId.IsInit())		sIdImageFileCache = pImageData->m_oRelId->GetValue();
					else if (pImageData->m_rId.IsInit())	sIdImageFileCache = pImageData->m_rId->GetValue();
					else if (pImageData->m_rPict.IsInit())	sIdImageFileCache = pImageData->m_rPict->GetValue();

					if (!sIdImageFileCache.empty())
					{
						//ищем физический файл ( rId относительно vml_drawing)									
						smart_ptr<OOX::File> pFile = pVmlDrawing->Find(sIdImageFileCache);

						if (pFile.IsInit() && (OOX::FileTypes::Image == pFile->type()))
						{
							pImageFileCache = static_cast<OOX::Image*>(pFile.GetPointer());
						}
					}
				}
			}
		}
		if (pImageFileCache == NULL && pOleObject->m_oObjectPr.IsInit() && pOleObject->m_oObjectPr->m_oRid.IsInit())
		{
			sIdImageFileCache = pOleObject->m_oObjectPr->m_oRid->GetValue();

			smart_ptr<OOX::File> pFile = oWorksheet.Find(sIdImageFileCache);
			if (pFile.IsInit() && (OOX::FileTypes::Image == pFile->type()))
			{
				pImageFileCache = static_cast<OOX::Image*>(pFile.GetPointer());
			}
		}
		if (pImageFileCache)
		{
			OOX::CPath pathImage = pImageFileCache->filename();

			if (olePic->oleObject->m_OleObjectFile.IsInit())
			{
				olePic->oleObject->m_OleObjectFile->set_filename_cache(pathImage);
			}

			olePic->blipFill.blip->embed = new OOX::RId(sIdImageFileCache); //ваще то тут не важно что - приоритет у того что ниже..
			olePic->blipFill.blip->oleFilepathImage = pathImage.GetPath();
		}
		
		oCellAnchor->m_oElement = new PPTX::Logic::SpTreeElem();
		oCellAnchor->m_oElement->InitElem(olePic);
//-----------------------------------------------------------------------------------------------------
		if (oCellAnchor.IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
			WriteDrawing(oWorksheet, pDrawing, oCellAnchor.GetPointer(), pVmlDrawing, pOleObject);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryWorksheetTableWriter::WriteControls(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::CVmlDrawing *pVmlDrawing)
{
	if (false == oWorksheet.m_oControls.IsInit()) return;
	if (oWorksheet.m_oControls->m_mapControls.empty()) return;

	int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Controls);

	for (std::map<unsigned int, nullable<OOX::Spreadsheet::CControl>>::const_iterator it = oWorksheet.m_oControls->m_mapControls.begin(); it != oWorksheet.m_oControls->m_mapControls.end(); ++it)
	{
		OOX::Spreadsheet::CControl* pControl = it->second.GetPointer();
		
		if (!pControl) continue;
		
		nullable<OOX::Spreadsheet::CCellAnchor> oCellAnchor;
		OOX::Spreadsheet::CFormControlPr oFormControlPr;
		OOX::Spreadsheet::CFormControlPr* pFormControlPr = &oFormControlPr;
        
		smart_ptr<OOX::File> pFileControl;

		if (pControl->m_oRid.IsInit())
		{			
			pFileControl = oWorksheet.Find(OOX::RId(pControl->m_oRid->GetValue()));// rId относительно sheet
		}
		if (false == pFileControl.IsInit()) continue;
		
		bool bSetAnchor = false;

		smart_ptr<OOX::Spreadsheet::CCtrlPropFile> pFileCtrlProp = pFileControl.smart_dynamic_cast<OOX::Spreadsheet::CCtrlPropFile>();
		
		if (pFileCtrlProp.IsInit())
		{
			pFormControlPr = pFileCtrlProp->m_oFormControlPr.GetPointer();
		}
		else
		{
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

				pActiveX_xml->m_oObject->toFormControlPr(pFormControlPr);
			}
		}
		
		std::wstring sShapeId = L"_x0000_s" + std::to_wstring(pControl->m_oShapeId->GetValue());
		OOX::Vml::CShape* pShape = NULL;
		boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind;
		
		if (pVmlDrawing)
		{
			pFind = pVmlDrawing->m_mapShapes.find(sShapeId);

			if (pFind != pVmlDrawing->m_mapShapes.end() && !pFind->second.bUsed)
			{
				pShape = dynamic_cast<OOX::Vml::CShape*>(pFind->second.pElement);
			}
		}
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
			oCellAnchor->m_bShapeOle= true;			
		}
		if ((pShape) && (OOX::et_v_shapetype != pShape->getType()))
		{
			for(size_t j = 0; (pShape) && (j < pShape->m_arrItems.size()); ++j)
			{
				OOX::WritingElement* pChildElemShape = pShape->m_arrItems[j];
				if (OOX::et_v_ClientData == pChildElemShape->getType())
				{
					OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pChildElemShape);

					if (!bSetAnchor )
					{						
						bSetAnchor = pClientData->toCellAnchor(oCellAnchor.GetPointer());
					}
					pClientData->toFormControlPr(pFormControlPr);
				}
				else if (OOX::et_v_textbox == pChildElemShape->getType())
				{
					OOX::Vml::CTextbox* pTextbox = static_cast<OOX::Vml::CTextbox*>(pChildElemShape);

					pFormControlPr->m_oText = pTextbox->m_oText;
				}
			}	
		}

		if (!bSetAnchor)
		{
			continue;
		}
		if (pShape)
			pFind->second.bUsed = true;
//-----------------------------------------------------------------------------------------------------
		int nCurPos2 = m_oBcw.WriteItemStart(c_oSerControlTypes::Control);
			int nCurPos3 = m_oBcw.WriteItemStart(c_oSerControlTypes::ControlAnchor);
				WriteCellAnchor(oCellAnchor.GetPointer());
			m_oBcw.WriteItemEnd(nCurPos3);

			if(pControl->m_oName.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::Name);
				m_oBcw.m_oStream.WriteStringW(*pControl->m_oName);
			}
			WriteControlPr(pControl->m_oControlPr.GetPointer(), pFormControlPr);
		m_oBcw.WriteItemEnd(nCurPos2);
	}
	m_oBcw.WriteItemEnd(nCurPos);
}

void BinaryWorksheetTableWriter::WriteControlPr(OOX::Spreadsheet::CControlPr* pControlPr, OOX::Spreadsheet::CFormControlPr* pFormControlPr)
{
	if (!pControlPr) return;
	if (!pFormControlPr) return;

	int nCurPos = 0;

	if (pFormControlPr->m_oObjectType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::ObjectType);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oObjectType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oAltText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::AltText);
		m_oBcw.m_oStream.WriteStringW(*pControlPr->m_oAltText);
	}
	if (pControlPr->m_oAutoFill.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::AutoFill);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oAutoFill);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oAutoLine.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::AutoLine);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oAutoLine);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oAutoPict.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::AutoPict);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oAutoPict);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oDefaultSize.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::DefaultSize);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oDefaultSize);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oDisabled.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Disabled);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oDisabled);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oLocked.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Locked);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oLocked);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oPrint.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Print);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oPrint);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pControlPr->m_oRecalcAlways.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::RecalcAlways);
		m_oBcw.m_oStream.WriteBOOL(*pControlPr->m_oRecalcAlways);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pControlPr->m_oMacro.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::Macro);
		m_oBcw.m_oStream.WriteStringW(*pControlPr->m_oMacro);
	}
	if(pFormControlPr->m_oFmlaGroup.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::FmlaGroup);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oFmlaGroup);
	}
	if(pFormControlPr->m_oFmlaLink.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::FmlaLink);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oFmlaLink);
	}
	if(pFormControlPr->m_oFmlaRange.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::FmlaRange);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oFmlaRange);
	}
	if(pFormControlPr->m_oFmlaTxbx.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::FmlaTxbx);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oFmlaTxbx);
	}
	if (pFormControlPr->m_oDropLines.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::DropLines);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oDropLines->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oChecked.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Checked);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oChecked->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oDropStyle.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::DropStyle);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oDropStyle->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oDx.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Dx);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oDx->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oInc.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Inc);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oInc->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oMin.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Min);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oMin->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oMax.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Max);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oMax->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oPage.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Page);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oPage->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oSel.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Sel);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oSel->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oSelType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::SelType);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oSelType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oTextHAlign.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::TextHAlign);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oTextHAlign->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oTextVAlign.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::TextVAlign);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oTextVAlign->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Val);
		m_oBcw.m_oStream.WriteLONG(*pFormControlPr->m_oVal);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oWidthMin.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::WidthMin);
		m_oBcw.m_oStream.WriteLONG(pFormControlPr->m_oWidthMin->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oEditVal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::EditVal);
		m_oBcw.m_oStream.WriteBYTE(pFormControlPr->m_oEditVal->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oColored.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Colored);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oColored);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oFirstButton.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::FirstButton);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oFirstButton);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oHoriz.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::Horiz);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oHoriz);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oJustLastX.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::JustLastX);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oJustLastX);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oLockText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::LockText);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oLockText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oNoThreeD.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::NoThreeD);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oNoThreeD);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oNoThreeD2.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::NoThreeD2);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oNoThreeD2);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(pFormControlPr->m_oMultiSel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::MultiSel);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oMultiSel);
	}
	if (pFormControlPr->m_oMultiLine.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::MultiLine);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oMultiLine);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oVerticalBar.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::VerticalBar);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oVerticalBar);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oPasswordEdit.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::PasswordEdit);
		m_oBcw.m_oStream.WriteBOOL(*pFormControlPr->m_oPasswordEdit);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (pFormControlPr->m_oText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::Text);
		m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oText);

	}
	if(pFormControlPr->m_oItemLst.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSerControlTypes::ItemLst);
		for (size_t i = 0; i < pFormControlPr->m_oItemLst->m_arrItems.size(); ++i)
		{
			if((pFormControlPr->m_oItemLst->m_arrItems[i].IsInit()) && 
				(pFormControlPr->m_oItemLst->m_arrItems[i]->m_oVal.IsInit()))
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerControlTypes::Item);
				m_oBcw.m_oStream.WriteStringW(*pFormControlPr->m_oItemLst->m_arrItems[i]->m_oVal);
			}
		}
		m_oBcw.WriteItemEnd(nCurPos);
	}
}

void BinaryWorksheetTableWriter::WriteDrawings(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::CVmlDrawing *pVmlDrawing)
{
	for (size_t i = 0; pDrawing && i < pDrawing->m_arrItems.size(); ++i)
	{
		OOX::Spreadsheet::CCellAnchor* pCellAnchor = pDrawing->m_arrItems[i];

//we use legacyDrawing or objectPr in OleObject so skip shape in drawing
	
		if (!pCellAnchor->m_bShapeOle && !pCellAnchor->m_bShapeControl && pCellAnchor->isValid())
		{
			if(oWorksheet.m_oOleObjects.IsInit() && pCellAnchor->m_nId.IsInit())
			{
                boost::unordered_map<unsigned int, OOX::Spreadsheet::COleObject*>::const_iterator pFind = oWorksheet.m_oOleObjects->m_mapOleObjects.find(pCellAnchor->m_nId.get());
				if (pFind != oWorksheet.m_oOleObjects->m_mapOleObjects.end())
				{
					pCellAnchor->m_bShapeOle = true;
					continue;
				}
			}
			if(oWorksheet.m_oControls.IsInit() && pCellAnchor->m_nId.IsInit())
			{
                std::map<unsigned int, nullable<OOX::Spreadsheet::CControl>>::const_iterator pFind = oWorksheet.m_oControls->m_mapControls.find(pCellAnchor->m_nId.get());
				if (pFind != oWorksheet.m_oControls->m_mapControls.end())
				{
					pCellAnchor->m_bShapeControl = true;
					continue;
				}
			}
			int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
			WriteDrawing(oWorksheet, pDrawing, pCellAnchor, pVmlDrawing);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	WriteOleObjects(oWorksheet, pDrawing, pVmlDrawing);

	if (NULL != pVmlDrawing)
	{
        boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator it = pVmlDrawing->m_mapShapes.begin();

        for (; it != pVmlDrawing->m_mapShapes.end(); it++)
		{
			if (it->second.bUsed == false)
			{//Bonetti Martínez. cálculo estructural de pilotes y pilas.xlsx						
				OOX::WritingElement* pElem = it->second.pElement;
				if(OOX::et_v_shapetype != pElem->getType())
				{
					OOX::Vml::CShape* pShape = static_cast<OOX::Vml::CShape*>(pElem);
					for(size_t j = 0; j < pShape->m_arrItems.size(); ++j)
					{
						OOX::WritingElement* pElemShape = pShape->m_arrItems[j];
						if(OOX::et_v_ClientData == pElemShape->getType())
						{
							//преобразуем ClientData в CellAnchor
							OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pElemShape);
							
							SimpleTypes::Spreadsheet::CCellAnchorType eAnchorType;
							eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);

							OOX::Spreadsheet::CCellAnchor *pCellAnchor = new OOX::Spreadsheet::CCellAnchor(eAnchorType);
							if (pClientData->toCellAnchor(pCellAnchor))
							{
								pCellAnchor->m_sVmlSpId.Init();
								pCellAnchor->m_sVmlSpId->append(it->first);

								int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
								WriteDrawing(oWorksheet, pDrawing, pCellAnchor, pVmlDrawing, NULL);
								m_oBcw.WriteItemEnd(nCurPos);								
							}
							delete pCellAnchor;
						}
					}
				}
				it->second.bUsed = true;
			}
		}
	}
}
void BinaryWorksheetTableWriter::WriteCellAnchor(OOX::Spreadsheet::CCellAnchor* pCellAnchor)
{
	if (!pCellAnchor) return;

//Type
	int nCurPos;
	nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::Type);
	m_oBcw.m_oStream.WriteBYTE(pCellAnchor->m_oAnchorType.GetValue());
	m_oBcw.WriteItemEnd(nCurPos);

	if(pCellAnchor->m_oEditAs.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::EditAs);
		m_oBcw.m_oStream.WriteBYTE(pCellAnchor->m_oEditAs->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//From
	if(pCellAnchor->m_oFrom.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::From);
		WriteFromTo(pCellAnchor->m_oFrom.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//To
	if(pCellAnchor->m_oTo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::To);
		WriteFromTo(pCellAnchor->m_oTo.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Pos
	if(pCellAnchor->m_oPos.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::Pos);
		WritePos(pCellAnchor->m_oPos.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//Ext
	if(pCellAnchor->m_oExt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::Ext);
		WriteExt(pCellAnchor->m_oExt.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
//ClientData
	if (pCellAnchor->m_oClientData.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::ClientData);
		WriteClientData(pCellAnchor->m_oClientData.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteDrawing(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::Spreadsheet::CCellAnchor* pCellAnchor, OOX::CVmlDrawing *pVmlDrawing, OOX::Spreadsheet::COleObject* pOleObject)
{
	if (!pCellAnchor) return;

	if (pCellAnchor->m_oElement.IsInit() == false && 
		pCellAnchor->m_sVmlSpId.IsInit() == false) return;

	WriteCellAnchor(pCellAnchor);

	if (pCellAnchor->m_sVmlSpId.IsInit() && pVmlDrawing)
	{
        boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator pFind = pVmlDrawing->m_mapShapes.find(pCellAnchor->m_sVmlSpId.get2());
		
		if (pFind != pVmlDrawing->m_mapShapes.end() && !pFind->second.bUsed)
		{
			pFind->second.bUsed = true;
			
			std::wstring sVmlXml = L"<v:object>";
			sVmlXml += pFind->second.sXml; //add vml shape xml
			
			if (pCellAnchor->m_bShapeOle && NULL != pOleObject)
			{
				//ищем физический файл, потому что rId относительно sheet.xml, а SetRelsPath(pVmlDrawing
				smart_ptr<OOX::File> pFile = oWorksheet.Find(OOX::RId(pOleObject->m_oRid->GetValue()));
				pOleObject->m_OleObjectFile = pFile.smart_dynamic_cast<OOX::OleObject>();
			
				NSStringUtils::CStringBuilder writer;
				pOleObject->toXMLPptx(writer, L"");
					
				sVmlXml += writer.GetData();
			}
			sVmlXml += L"</v:object>";

            smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
			m_pOfficeDrawingConverter->SetRels(pVmlDrawing);

			std::wstring* bstrOutputXml = NULL;
			
			m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingType::pptxDrawing);			
			int nCurPos = m_oBcw.WriteItemWithLengthStart();			
				m_pOfficeDrawingConverter->AddObject(sVmlXml, &bstrOutputXml);			
				m_pOfficeDrawingConverter->SetRels(oldRels);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);					
			RELEASEOBJECT(bstrOutputXml);
		}
	}
	else if (pCellAnchor->m_oElement.IsInit())
	{
		smart_ptr<OOX::IFileContainer> oldRels = m_oBcw.m_oStream.GetRels();
		m_oBcw.m_oStream.SetRels(pDrawing);

		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingType::pptxDrawing);
		int nCurPos = m_oBcw.WriteItemWithLengthStart();
			m_oBcw.m_oStream.StartRecord(0);
			m_oBcw.m_oStream.WriteRecord2(1, pCellAnchor->m_oElement->GetElem());
			m_oBcw.m_oStream.EndRecord();
		m_oBcw.WriteItemWithLengthEnd(nCurPos);

		m_oBcw.m_oStream.SetRels(oldRels);
	}
}
void BinaryWorksheetTableWriter::WriteLegacyDrawingHF(const OOX::Spreadsheet::CWorksheet& oWorksheet)
{
	int nCurPos = 0;
	const OOX::Spreadsheet::CLegacyDrawingHFWorksheet& oLegacyDrawingHF = oWorksheet.m_oLegacyDrawingHF.get();
	if(oLegacyDrawingHF.m_oCfe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Cfe);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oCfe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oCff.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Cff);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oCff->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oCfo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Cfo);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oCfo->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oChe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Che);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oChe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oChf.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Chf);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oChf->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oCho.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Cho);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oCho->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLfe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lfe);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLfe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLff.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lff);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLff->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLfo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lfo);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLfo->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLhe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lhe);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLhe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLhf.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lhf);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLhf->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oLho.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Lho);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oLho->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRfe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rfe);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRfe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRff.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rff);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRff->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRfo.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rfo);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRfo->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRhe.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rhe);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRhe->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRhf.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rhf);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRhf->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oRho.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Rho);
		m_oBcw.m_oStream.WriteLONG(oLegacyDrawingHF.m_oRho->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if(oLegacyDrawingHF.m_oId.IsInit())
	{
		smart_ptr<OOX::File> oFileV = oWorksheet.Find(oLegacyDrawingHF.m_oId->GetValue());
		if (oFileV.IsInit() && OOX::FileTypes::VmlDrawing == oFileV->type())
		{
			OOX::CVmlDrawing* pVmlDrawing = (OOX::CVmlDrawing*)oFileV.GetPointer();
			smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
			m_pOfficeDrawingConverter->SetRels(pVmlDrawing);
			m_pOfficeDrawingConverter->ClearShapeTypes();
			nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Drawings);
			WriteLegacyDrawingHFDrawings(pVmlDrawing);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
			m_pOfficeDrawingConverter->SetRels(oldRels);
		}
	}
}
void BinaryWorksheetTableWriter::WriteLegacyDrawingHFDrawings(OOX::CVmlDrawing* pVmlDrawing)
{
	for (size_t i = 0; i < pVmlDrawing->m_arrShapeTypes.size(); ++i)
	{
		m_pOfficeDrawingConverter->AddShapeType(pVmlDrawing->m_arrShapeTypes[i].sXml);
	}
	boost::unordered_map<std::wstring, OOX::CVmlDrawing::_vml_shape>::iterator it = pVmlDrawing->m_mapShapes.begin();
	for (; it != pVmlDrawing->m_mapShapes.end(); it++)
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::Drawing);
		WriteLegacyDrawingHFDrawing(it->second);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteLegacyDrawingHFDrawing(const OOX::CVmlDrawing::_vml_shape& oVmlShape)
{
		OOX::Vml::CVmlCommonElements* common = dynamic_cast<OOX::Vml::CVmlCommonElements*>(oVmlShape.pElement);
		std::wstring sId;
		if (common)
		{
			if (common->m_sId.IsInit())
			{
				sId = *common->m_sId;
			}
		}
		else
		{
			OOX::Vml::CGroup *group = dynamic_cast<OOX::Vml::CGroup*>(oVmlShape.pElement);
			if (group)
			{
				if (group->m_sId.IsInit())
				{
					sId = *group->m_sId;
				}
			}
		}
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::DrawingId);
		m_oBcw.m_oStream.WriteStringW3(sId);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);

		std::wstring sVmlXml = L"<v:object>";
		sVmlXml += oVmlShape.sXml; //add vml shape xml
		sVmlXml += L"</v:object>";

		std::wstring* bstrOutputXml = NULL;
		nCurPos = m_oBcw.WriteItemStart(c_oSer_LegacyDrawingHF::DrawingShape);
		m_pOfficeDrawingConverter->AddObject(sVmlXml, &bstrOutputXml);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
		RELEASEOBJECT(bstrOutputXml);
}

void BinaryWorksheetTableWriter::WriteFromTo(const OOX::Spreadsheet::CFromTo& oFromTo)
{
	//Col
	if(oFromTo.m_oCol.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingFromToType::Col);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFromTo.m_oCol->GetValue());
	}
	//ColOff
	if(oFromTo.m_oColOff.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingFromToType::ColOff);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oColOff->ToMm());
	}
	//Row
	if(oFromTo.m_oRow.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingFromToType::Row);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oFromTo.m_oRow->GetValue());
	}
	//ColOff
	if(oFromTo.m_oRowOff.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingFromToType::RowOff);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oFromTo.m_oRowOff->ToMm());
	}
}
void BinaryWorksheetTableWriter::WritePos(const OOX::Spreadsheet::CPos& oPos)
{
	//X
	if(oPos.m_oX.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::X);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPos.m_oX->ToMm());
	}
	//Y
	if(oPos.m_oY.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingPosType::Y);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oPos.m_oY->ToMm());
	}
}
void BinaryWorksheetTableWriter::WriteClientData(const OOX::Spreadsheet::CClientData& oClientData)
{
	if (oClientData.fLocksWithSheet.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingClientDataType::fLocksWithSheet);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(*oClientData.fLocksWithSheet);
	}
	if (oClientData.fPrintsWithSheet.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingClientDataType::fPrintsWithSheet);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(*oClientData.fPrintsWithSheet);
	}
}

void BinaryWorksheetTableWriter::WriteExt(const OOX::Spreadsheet::CExt& oExt)
{
//Cx
	if(oExt.m_oCx.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cx);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCx->ToMm());
	}
//Cy
	if(oExt.m_oCy.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingExtType::Cy);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oExt.m_oCy->ToMm());
	}
}
void BinaryWorksheetTableWriter::WriteComments(std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>& mapComments)
{
	int nCurPos = 0;

    for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = mapComments.begin(); it != mapComments.end(); ++it)
	{
		if(it->second->IsValid())
		{
			OOX::Spreadsheet::CCommentItem& oComment = *it->second;
			std::vector<SerializeCommon::CommentData*> aCommentDatas;
			
			getSavedComment(oComment, aCommentDatas);
			
			//записываем тот обьект, который был в бинарнике, подменяем только текст, который мог быть отредактирован в Excel
			
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Comment);
				WriteComment(oComment, aCommentDatas);
			m_oBcw.WriteItemEnd(nCurPos);

			for(size_t i = 0, length = aCommentDatas.size(); i < length; ++i)
			{
				RELEASEOBJECT(aCommentDatas[i]);
			}
			aCommentDatas.clear();
		}
	}
}
void BinaryWorksheetTableWriter::getSavedComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aDatas)
{
	if(oComment.m_sGfxdata.IsInit())
	{
        const std::wstring& sGfxData = *oComment.m_sGfxdata;
		std::wstring sSignatureBase64Old(_T("WExTV"));//XLST
		std::wstring sSignatureBase64(_T("WExTM"));//XLS2
		//compatibility with fact that previously used long that can be 4 or 8 byte
		bool bCompatibility = sSignatureBase64Old == sGfxData.substr(0, sSignatureBase64Old.length());
		bool isComment = (sSignatureBase64 == sGfxData.substr(0, sSignatureBase64.length())) || bCompatibility;
		if(isComment)
		{
			std::string sSignature("XLS2");
            int nSignatureSize = (int)sSignature.length();
			int nDataLengthSize = bCompatibility ? sizeof(long) : sizeof(_INT32);
			int nJunkSize = 2;
            std::string sGfxDataA = std::string(sGfxData.begin(), sGfxData.end());
            int nDataSize = (int)sGfxDataA.length();
			BYTE* pBuffer = new BYTE[nDataSize];
            if(false != Base64::Base64Decode((const char*)sGfxDataA.c_str(), (int)sGfxDataA.length(), pBuffer, &nDataSize))
			{
				int nLength;
				if (bCompatibility)
				{
					nLength = *((long*)(pBuffer + nSignatureSize));
				}
				else
				{
					nLength = *((_INT32*)(pBuffer + nSignatureSize));
				}
				NSBinPptxRW::CBinaryFileReader oBufferedStream;
				oBufferedStream.Init((BYTE*)pBuffer, nSignatureSize + nDataLengthSize, nLength);

				BinaryCommentReader oBinaryCommentReader(oBufferedStream, NULL);
				oBinaryCommentReader.ReadExternal(nLength, &aDatas);
			}
			RELEASEARRAYOBJECTS(pBuffer);
		}
	}
}
void BinaryWorksheetTableWriter::WriteComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas)
{
	int nCurPos = 0;
	int nRow = 0;
	int nCol = 0;
	if(oComment.m_nRow.IsInit())
	{
		nRow = oComment.m_nRow.get();
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Row);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(nRow);
	}
	if(oComment.m_nCol.IsInit())
	{
		nCol = oComment.m_nCol.get();
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Col);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(nCol);
	}
	m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::CommentDatas);
	m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
	nCurPos = m_oBcw.WriteItemWithLengthStart();
	WriteCommentData(oComment, aCommentDatas);
	m_oBcw.WriteItemWithLengthEnd(nCurPos);

	if(oComment.m_nLeft.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Left);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nLeft.get());
	}
	if(oComment.m_nLeftOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::LeftOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nLeftOffset.get());
	}
	if(oComment.m_nTop.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Top);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nTop.get());
	}
	if(oComment.m_nTopOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::TopOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nTopOffset.get());
	}
	if(oComment.m_nRight.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Right);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nRight.get());
	}
	if(oComment.m_nRightOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::RightOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nRightOffset.get());
	}
	if(oComment.m_nBottom.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::Bottom);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nBottom.get());
	}
	if(oComment.m_nBottomOffset.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::BottomOffset);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oComment.m_nBottomOffset.get());
	}
	if(oComment.m_dLeftMM.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::LeftMM);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oComment.m_dLeftMM.get());
	}
	if(oComment.m_dTopMM.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::TopMM);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oComment.m_dTopMM.get());
	}
	if(oComment.m_dWidthMM.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::WidthMM);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oComment.m_dWidthMM.get());
	}
	if(oComment.m_dHeightMM.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::HeightMM);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
		m_oBcw.m_oStream.WriteDoubleReal(oComment.m_dHeightMM.get());
	}
	if(oComment.m_bMove.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::MoveWithCells);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oComment.m_bMove.get());
	}
	if(oComment.m_bSize.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::SizeWithCells);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oComment.m_bSize.get());
	}
	if(NULL != oComment.m_pThreadedComment)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_Comments::ThreadedComment);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		nCurPos = m_oBcw.WriteItemWithLengthStart();
		WriteThreadedComment(*oComment.m_pThreadedComment, oComment.m_bThreadedCommentCopy);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCommentData(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas)
{
	bool isThreadedComment = NULL != oComment.m_pThreadedComment;
	int nCurPos = 0;
	if(aCommentDatas.size() > 0)
	{
		for(size_t i = 0, length = aCommentDatas.size(); i < length; ++i)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_Comments::CommentData);
			if(0 == i)
				WriteCommentDataContent(&oComment, aCommentDatas[i], isThreadedComment);
			else
				WriteCommentDataContent(NULL, aCommentDatas[i], isThreadedComment);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	else
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_Comments::CommentData);
		WriteCommentDataContent(&oComment, NULL, isThreadedComment);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCommentDataContent(OOX::Spreadsheet::CCommentItem* pComment, SerializeCommon::CommentData* pCommentData, bool isThreadedComment)
{
	int nCurPos = 0;
	if(NULL != pCommentData && !pCommentData->sText.empty())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Text);
		m_oBcw.m_oStream.WriteStringW(pCommentData->sText);
	}
	else if(NULL != pComment && pComment->m_oText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Text);
		m_oBcw.m_oStream.WriteStringW(pComment->m_oText->ToString());
	}
	if(NULL != pCommentData)
	{
		if(!pCommentData->sTime.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Time);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sTime);
		}
		if(!pCommentData->sOOTime.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::OOTime);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sOOTime);
		}
		if (!pCommentData->sUserId.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserId);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sUserId);
		}
		if (!pCommentData->sUserName.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserName);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sUserName);
		}
		if (!pCommentData->sUserData.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserData);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sUserData);
		}
		if (!pCommentData->sQuoteText.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::QuoteText);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sQuoteText);
		}
		if(pCommentData->bSolved)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentData::Solved);
			m_oBcw.m_oStream.WriteBOOL(pCommentData->Solved);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(pCommentData->bDocument)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentData::Document);
			m_oBcw.m_oStream.WriteBOOL(pCommentData->Document);
			m_oBcw.WriteItemEnd(nCurPos);
		}
		if(pCommentData->aReplies.size() > 0)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentData::Replies);
			WriteCommentReplies(pCommentData->aReplies, isThreadedComment);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	else if(NULL != pComment)
	{
		if(pComment->m_sAuthor.IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserName);
			m_oBcw.m_oStream.WriteStringW(*pComment->m_sAuthor);
		}
	}
	if(!isThreadedComment)
	{
		if(pCommentData && !pCommentData->sGuid.empty())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Guid);
			m_oBcw.m_oStream.WriteStringW(pCommentData->sGuid);
		}
		else
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Guid);
			m_oBcw.m_oStream.WriteStringW(L"{" + XmlUtils::GenerateGuid() + L"}");
		}
	}
}
void BinaryWorksheetTableWriter::WriteCommentReplies(std::vector<SerializeCommon::CommentData*>& aReplies, bool isThreadedComment)
{
	int nCurPos = 0;
	for(size_t i = 0, length = aReplies.size(); i < length; i++)
	{
		SerializeCommon::CommentData* pReply = aReplies[i];
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentData::Reply);
		WriteCommentDataContent(NULL, pReply, isThreadedComment);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteThreadedComment(OOX::Spreadsheet::CThreadedComment& oThreadedComment, bool bThreadedCommentCopy)
{
	int nCurPos = 0;

	if(oThreadedComment.dT.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::dT);
		m_oBcw.m_oStream.WriteStringW3(oThreadedComment.dT->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oThreadedComment.personId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::personId);
		m_oBcw.m_oStream.WriteStringW3(oThreadedComment.personId->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(bThreadedCommentCopy || !oThreadedComment.id.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::id);
		m_oBcw.m_oStream.WriteStringW3(L"{" + XmlUtils::GenerateGuid() + L"}");
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	else
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::id);
		m_oBcw.m_oStream.WriteStringW3(oThreadedComment.id->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oThreadedComment.done.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::done);
		m_oBcw.m_oStream.WriteBOOL(oThreadedComment.done.get());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oThreadedComment.m_oText.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::text);
		m_oBcw.m_oStream.WriteStringW3(oThreadedComment.m_oText->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oThreadedComment.m_oMentions.IsInit())
	{
		for(size_t i = 0; i < oThreadedComment.m_oMentions->m_arrItems.size(); ++i)
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::mention);
			WriteThreadedCommentMention(*oThreadedComment.m_oMentions->m_arrItems[i]);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);
		}
	}
	for(size_t i = 0; i < oThreadedComment.m_arrReplies.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::reply);
		WriteThreadedComment(*oThreadedComment.m_arrReplies[i], bThreadedCommentCopy);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteThreadedCommentMention(OOX::Spreadsheet::CThreadedCommentMention& oMention)
{
	int nCurPos = 0;
	if(oMention.mentionpersonId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::mentionpersonId);
		m_oBcw.m_oStream.WriteStringW3(oMention.mentionpersonId->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oMention.mentionId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::mentionId);
		m_oBcw.m_oStream.WriteStringW3(oMention.mentionId->ToString());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oMention.startIndex.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::startIndex);
		m_oBcw.m_oStream.WriteULONG(oMention.startIndex->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	if(oMention.length.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ThreadedComment::length);
		m_oBcw.m_oStream.WriteULONG(oMention.length->GetValue());
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteSheetPr(const OOX::Spreadsheet::CSheetPr& oSheetPr)
{
	int nCurPos = 0;
	if (oSheetPr.m_oCodeName.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::CodeName);
		m_oBcw.m_oStream.WriteStringW4(*oSheetPr.m_oCodeName);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oEnableFormatConditionsCalculation.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::EnableFormatConditionsCalculation);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oEnableFormatConditionsCalculation->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oFilterMode.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::FilterMode);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oFilterMode->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oPublished.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::Published);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oPublished->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oSyncHorizontal.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::SyncHorizontal);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oSyncHorizontal->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oSyncRef.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::SyncRef);
		m_oBcw.m_oStream.WriteStringW4(*oSheetPr.m_oSyncRef);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oSyncVertical.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::SyncVertical);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oSyncVertical->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oTransitionEntry.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::TransitionEntry);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oTransitionEntry->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oTransitionEvaluation.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::TransitionEvaluation);
		m_oBcw.m_oStream.WriteBOOL(oSheetPr.m_oTransitionEvaluation->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	if (oSheetPr.m_oTabColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::TabColor);
		m_oBcw.WriteColor(oSheetPr.m_oTabColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oPageSetUpPr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::PageSetUpPr);
		WritePageSetUpPr(oSheetPr.m_oPageSetUpPr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSheetPr.m_oOutlinePr.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::OutlinePr);
		WriteOutlinePr(oSheetPr.m_oOutlinePr.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteOutlinePr(const OOX::Spreadsheet::COutlinePr& oOutlinePr)
{
	int nCurPos = 0;
	if (oOutlinePr.m_oApplyStyles.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::ApplyStyles);
		m_oBcw.m_oStream.WriteBOOL(oOutlinePr.m_oApplyStyles->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oOutlinePr.m_oShowOutlineSymbols.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::ShowOutlineSymbols);
		m_oBcw.m_oStream.WriteBOOL(oOutlinePr.m_oShowOutlineSymbols->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oOutlinePr.m_oSummaryBelow.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::SummaryBelow);
		m_oBcw.m_oStream.WriteBOOL(oOutlinePr.m_oSummaryBelow->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oOutlinePr.m_oSummaryRight.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::SummaryRight);
		m_oBcw.m_oStream.WriteBOOL(oOutlinePr.m_oSummaryRight->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WritePageSetUpPr(const OOX::Spreadsheet::CPageSetUpPr& oPageSetUpPr)
{
	int nCurPos = 0;
	if (oPageSetUpPr.m_oAutoPageBreaks.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::AutoPageBreaks);
		m_oBcw.m_oStream.WriteBOOL(oPageSetUpPr.m_oAutoPageBreaks->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oPageSetUpPr.m_oFitToPage.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::FitToPage);
		m_oBcw.m_oStream.WriteBOOL(oPageSetUpPr.m_oFitToPage->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WritemHeaderFooter(const OOX::Spreadsheet::CHeaderFooter& oHeaderFooter)
{
	int nCurPos = 0;
	if (oHeaderFooter.m_oAlignWithMargins.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::AlignWithMargins);
		m_oBcw.m_oStream.WriteBOOL(oHeaderFooter.m_oAlignWithMargins->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oDifferentFirst.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::DifferentFirst);
		m_oBcw.m_oStream.WriteBOOL(oHeaderFooter.m_oDifferentFirst->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oDifferentOddEven.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::DifferentOddEven);
		m_oBcw.m_oStream.WriteBOOL(oHeaderFooter.m_oDifferentOddEven->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oScaleWithDoc.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::ScaleWithDoc);
		m_oBcw.m_oStream.WriteBOOL(oHeaderFooter.m_oScaleWithDoc->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oEvenFooter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::EvenFooter);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oEvenFooter->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oEvenHeader.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::EvenHeader);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oEvenHeader->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oFirstFooter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::FirstFooter);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oFirstFooter->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oFirstHeader.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::FirstHeader);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oFirstHeader->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oOddFooter.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::OddFooter);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oOddFooter->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oHeaderFooter.m_oOddHeader.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_HeaderFooter::OddHeader);
		m_oBcw.m_oStream.WriteStringW3(oHeaderFooter.m_oOddHeader->m_sText);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WritemRowColBreaks(const OOX::Spreadsheet::CRowColBreaks& oRowColBreaks)
{
	int nCurPos = 0;
	if (oRowColBreaks.m_oCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Count);
		m_oBcw.m_oStream.WriteLONG(oRowColBreaks.m_oCount->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oRowColBreaks.m_oManualBreakCount.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::ManualBreakCount);
		m_oBcw.m_oStream.WriteLONG(oRowColBreaks.m_oManualBreakCount->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	for (size_t i = 0; i < oRowColBreaks.m_arrItems.size(); ++i)
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Break);
		WritemBreak(*oRowColBreaks.m_arrItems[i]);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WritemBreak(const OOX::Spreadsheet::CBreak& oBreak)
{
	int nCurPos = 0;
	if (oBreak.m_oId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Id);
		m_oBcw.m_oStream.WriteLONG(oBreak.m_oId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oBreak.m_oMan.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Man);
		m_oBcw.m_oStream.WriteBOOL(oBreak.m_oMan->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oBreak.m_oMax.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Max);
		m_oBcw.m_oStream.WriteLONG(oBreak.m_oMax->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oBreak.m_oMin.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Min);
		m_oBcw.m_oStream.WriteLONG(oBreak.m_oMin->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oBreak.m_oPt.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Pt);
		m_oBcw.m_oStream.WriteBOOL(oBreak.m_oPt->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteConditionalFormattings(std::vector<OOX::Spreadsheet::CConditionalFormatting*>& arrConditionalFormatting,
	std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt)
{
	int nCurPos = 0;
	for (size_t nIndex = 0, nLength = arrConditionalFormatting.size(); nIndex < nLength; ++nIndex)
	{
		if (arrConditionalFormatting[nIndex]->IsUsage()) continue;

		nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::ConditionalFormatting);
		WriteConditionalFormatting(*arrConditionalFormatting[nIndex], mapCFRuleEx, isExt);
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteConditionalFormatting(const OOX::Spreadsheet::CConditionalFormatting& oConditionalFormatting,
	std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt)
{
	int nCurPos = 0;

	if (oConditionalFormatting.m_oPivot.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormatting::Pivot);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormatting.m_oPivot->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormatting.m_oSqRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormatting::SqRef);
		m_oBcw.m_oStream.WriteStringW(oConditionalFormatting.m_oSqRef.get());
	}

	if (false == oConditionalFormatting.m_arrItems.empty())
	{
		for (size_t i = 0, length = oConditionalFormatting.m_arrItems.size(); i < length; ++i)
		{
			int nCurPos1 = m_oBcw.WriteItemStart(c_oSer_ConditionalFormatting::ConditionalFormattingRule);
			WriteConditionalFormattingRule(*oConditionalFormatting.m_arrItems[i], mapCFRuleEx, isExt);
			m_oBcw.WriteItemEnd(nCurPos1);
		}			
	}
}
void BinaryWorksheetTableWriter::WriteConditionalFormattingRule(const OOX::Spreadsheet::CConditionalFormattingRule& oConditionalFormattingRule,
	std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>& mapCFRuleEx, bool isExt)
{
	std::map<std::wstring, OOX::Spreadsheet::CConditionalFormattingRule*>::iterator pFind;
	if (oConditionalFormattingRule.m_oExtId.IsInit())
	{
		 pFind = mapCFRuleEx.find(oConditionalFormattingRule.m_oExtId.get2());

		 if (pFind != mapCFRuleEx.end())
		 {
			 const OOX::Spreadsheet::CConditionalFormattingRule newRule = 
				 OOX::Spreadsheet::CConditionalFormattingRule::Merge(oConditionalFormattingRule, *pFind->second);

			 pFind->second->bUsage = true;
			 return WriteConditionalFormattingRule(newRule, mapCFRuleEx, true);
		 }
	}
	int nCurPos = 0;

	if (oConditionalFormattingRule.m_oAboveAverage.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::AboveAverage);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormattingRule.m_oAboveAverage->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oBottom.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Bottom);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormattingRule.m_oBottom->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oDxfId.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::DxfId);
		m_oBcw.m_oStream.WriteLONG(oConditionalFormattingRule.m_oDxfId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oEqualAverage.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::EqualAverage);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormattingRule.m_oEqualAverage->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oOperator.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Operator);
		m_oBcw.m_oStream.WriteBYTE(oConditionalFormattingRule.m_oOperator->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oPercent.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Percent);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormattingRule.m_oPercent->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oPriority.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Priority);
		m_oBcw.m_oStream.WriteLONG(oConditionalFormattingRule.m_oPriority->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oRank.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Rank);
		m_oBcw.m_oStream.WriteLONG(oConditionalFormattingRule.m_oRank->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oStdDev.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::StdDev);
		m_oBcw.m_oStream.WriteLONG(oConditionalFormattingRule.m_oStdDev->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oStopIfTrue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::StopIfTrue);
		m_oBcw.m_oStream.WriteBOOL(oConditionalFormattingRule.m_oStopIfTrue->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oText.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::Text);
		m_oBcw.m_oStream.WriteStringW(oConditionalFormattingRule.m_oText.get2());
	}
	if (oConditionalFormattingRule.m_oTimePeriod.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::strTimePeriod);
		m_oBcw.m_oStream.WriteStringW(oConditionalFormattingRule.m_oTimePeriod->ToString());
		
		//m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::TimePeriod);
		//m_oBcw.m_oStream.WriteBYTE(oConditionalFormattingRule.m_oTimePeriod->GetValue());
	}
	if (oConditionalFormattingRule.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Type);
		m_oBcw.m_oStream.WriteBYTE(oConditionalFormattingRule.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oConditionalFormattingRule.m_oDxf.IsInit())
	{
		BinaryStyleTableWriter oBinaryStyleTableWriter(m_oBcw.m_oStream, m_pEmbeddedFontsManager);
		
		int nCurPos1 = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Dxf);
		oBinaryStyleTableWriter.WriteDxf(oConditionalFormattingRule.m_oDxf.get(), m_pIndexedColors, m_pTheme, m_oFontProcessor);
		m_oBcw.WriteItemEnd(nCurPos1);
	}

	if (oConditionalFormattingRule.m_oColorScale.IsInit())
	{
		int nCurPos1 = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::ColorScale);
		WriteColorScale(oConditionalFormattingRule.m_oColorScale.get());
		m_oBcw.WriteItemEnd(nCurPos1);
	}
	if (oConditionalFormattingRule.m_oDataBar.IsInit())
	{
		int nCurPos1 = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::DataBar);
		WriteDataBar(oConditionalFormattingRule.m_oDataBar.get());
		m_oBcw.WriteItemEnd(nCurPos1);
	}
	for (size_t i = 0; i < oConditionalFormattingRule.m_arrFormula.size(); ++i)
	{
		if (oConditionalFormattingRule.m_arrFormula[i].IsInit())
		{
			m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::FormulaCF);
			m_oBcw.m_oStream.WriteStringW(oConditionalFormattingRule.m_arrFormula[i]->m_sText);
		}
	}
	if (oConditionalFormattingRule.m_oIconSet.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::IconSet);
		WriteIconSet(oConditionalFormattingRule.m_oIconSet.get());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::IsExt);
	m_oBcw.m_oStream.WriteBOOL(isExt);
	m_oBcw.WriteItemEnd(nCurPos);
}

void BinaryWorksheetTableWriter::WriteColorScale(const OOX::Spreadsheet::CColorScale& oColorScale)
{
	// ToDo более правильно заделать виртуальную функцию, которая будет писать без привидения типов

	for (size_t i = 0, length = oColorScale.m_arrValues.size(); i < length; ++i)
	{
		if (oColorScale.m_arrValues[i].IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRuleColorScale::CFVO);
			WriteCFVO(oColorScale.m_arrValues[i].get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}

	for (size_t i = 0, length = oColorScale.m_arrColors.size(); i < length; ++i)
	{
		if (oColorScale.m_arrColors[i].IsInit())
		{
			int nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRuleColorScale::Color);
			m_oBcw.WriteColor(oColorScale.m_arrColors[i].get(), m_pIndexedColors);
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryWorksheetTableWriter::WriteDataBar(const OOX::Spreadsheet::CDataBar& oDataBar)
{
	int nCurPos = 0;

	if (oDataBar.m_oMaxLength.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::MaxLength);
		m_oBcw.m_oStream.WriteLONG(oDataBar.m_oMaxLength->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oMinLength.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::MinLength);
		m_oBcw.m_oStream.WriteLONG(oDataBar.m_oMinLength->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oShowValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::ShowValue);
		m_oBcw.m_oStream.WriteBOOL(oDataBar.m_oShowValue->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oGradient.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::GradientEnabled);
		m_oBcw.m_oStream.WriteBOOL(oDataBar.m_oGradient->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oNegativeBarColorSameAsPositive.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::NegativeBarColorSameAsPositive);
		m_oBcw.m_oStream.WriteBOOL(oDataBar.m_oNegativeBarColorSameAsPositive->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oNegativeBarBorderColorSameAsPositive.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::NegativeBarBorderColorSameAsPositive);
		m_oBcw.m_oStream.WriteBOOL(oDataBar.m_oNegativeBarBorderColorSameAsPositive->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oAxisPosition.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::AxisPosition);
		m_oBcw.m_oStream.WriteLONG(oDataBar.m_oAxisPosition->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oDirection.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::Direction);
		m_oBcw.m_oStream.WriteLONG(oDataBar.m_oDirection->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::Color);
		m_oBcw.WriteColor(oDataBar.m_oColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oBorderColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::BorderColor);
		m_oBcw.WriteColor(oDataBar.m_oBorderColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}			
	if (oDataBar.m_oAxisColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::AxisColor);
		m_oBcw.WriteColor(oDataBar.m_oAxisColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}	
	if (oDataBar.m_oNegativeFillColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::NegativeColor);
		m_oBcw.WriteColor(oDataBar.m_oNegativeFillColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oDataBar.m_oNegativeBorderColor.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::NegativeBorderColor);
		m_oBcw.WriteColor(oDataBar.m_oNegativeBorderColor.get(), m_pIndexedColors);
		m_oBcw.WriteItemEnd(nCurPos);
	}
	
	for (size_t i = 0, length = oDataBar.m_arrValues.size(); i < length; ++i)
	{
		if (oDataBar.m_arrValues[i].IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::CFVO);
			WriteCFVO(oDataBar.m_arrValues[i].get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryWorksheetTableWriter::WriteIconSet(const OOX::Spreadsheet::CIconSet& oIconSet)
{
	int nCurPos = 0;

	if (oIconSet.m_oIconSet.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::IconSet);
		m_oBcw.m_oStream.WriteBYTE(oIconSet.m_oIconSet->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oIconSet.m_oPercent.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::Percent);
		m_oBcw.m_oStream.WriteBOOL(oIconSet.m_oPercent->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oIconSet.m_oReverse.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::Reverse);
		m_oBcw.m_oStream.WriteBOOL(oIconSet.m_oReverse->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oIconSet.m_oShowValue.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::ShowValue);
		m_oBcw.m_oStream.WriteBOOL(oIconSet.m_oShowValue->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}

	for (size_t i = 0, length = oIconSet.m_arrValues.size(); i < length; ++i)
	{
		if (oIconSet.m_arrValues[i].IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::CFVO);
			WriteCFVO(oIconSet.m_arrValues[i].get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
	for (size_t i = 0, length = oIconSet.m_arrIconSets.size(); i < length; ++i)
	{
		if (oIconSet.m_arrIconSets[i].IsInit())
		{
			nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::CFIcon);
			WriteCFIcon(oIconSet.m_arrIconSets[i].get());
			m_oBcw.WriteItemEnd(nCurPos);
		}
	}
}
void BinaryWorksheetTableWriter::WriteCFIcon(const OOX::Spreadsheet::CConditionalFormatIconSet& oCFIcon)
{
	if (oCFIcon.m_oIconSet.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIcon::iconSet);
		m_oBcw.m_oStream.WriteLONG(oCFIcon.m_oIconSet->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCFIcon.m_oIconId.IsInit())
	{
		int nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIcon::iconId);
		m_oBcw.m_oStream.WriteLONG(oCFIcon.m_oIconId->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
}
void BinaryWorksheetTableWriter::WriteCFVO(const OOX::Spreadsheet::CConditionalFormatValueObject& oCFVO)
{
	int nCurPos = 0;
	if (oCFVO.m_oGte.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingValueObject::Gte);
		m_oBcw.m_oStream.WriteBOOL(oCFVO.m_oGte->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCFVO.m_oType.IsInit())
	{
		nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingValueObject::Type);
		m_oBcw.m_oStream.WriteBYTE(oCFVO.m_oType->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oCFVO.m_oVal.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingValueObject::Val);
		m_oBcw.m_oStream.WriteStringW(oCFVO.m_oVal.get2());
	}
	if (oCFVO.m_oFormula.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingValueObject::Formula);
		m_oBcw.m_oStream.WriteStringW(oCFVO.m_oFormula->m_sText);
	}
}
void BinaryWorksheetTableWriter::WriteSparklineGroups(const OOX::Spreadsheet::CSparklineGroups& oSparklineGroups)
{
    int nCurPos = 0;
    for(size_t i = 0; i < oSparklineGroups.m_arrItems.size(); ++i)
    {
        OOX::Spreadsheet::CSparklineGroup* pSparklineGroup = oSparklineGroups.m_arrItems[i];
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::SparklineGroup);
        WriteSparklineGroup(*pSparklineGroup);
        m_oBcw.WriteItemEnd(nCurPos);
    }
}
void BinaryWorksheetTableWriter::WriteSparklineGroup(const OOX::Spreadsheet::CSparklineGroup& oSparklineGroup)
{
    int nCurPos = 0;
    if (oSparklineGroup.m_oManualMax.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ManualMax);
        m_oBcw.m_oStream.WriteDoubleReal(oSparklineGroup.m_oManualMax->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);

    }
    if (oSparklineGroup.m_oManualMin.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ManualMin);
        m_oBcw.m_oStream.WriteDoubleReal(oSparklineGroup.m_oManualMin->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oLineWeight.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::LineWeight);
        m_oBcw.m_oStream.WriteDoubleReal(oSparklineGroup.m_oLineWeight->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oType.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Type);
        m_oBcw.m_oStream.WriteBYTE(oSparklineGroup.m_oType->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oDateAxis.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::DateAxis);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oDateAxis->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oDisplayEmptyCellsAs.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::DisplayEmptyCellsAs);
        m_oBcw.m_oStream.WriteBYTE(*oSparklineGroup.m_oDisplayEmptyCellsAs);
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oMarkers.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Markers);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oMarkers->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oHigh.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::High);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oHigh->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oLow.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Low);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oLow->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oFirst.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::First);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oFirst->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oLast.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Last);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oLast->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oNegative.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Negative);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oNegative->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oDisplayXAxis.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::DisplayXAxis);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oDisplayXAxis->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oDisplayHidden.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::DisplayHidden);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oDisplayHidden->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oMinAxisType.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::MinAxisType);
        m_oBcw.m_oStream.WriteBYTE(oSparklineGroup.m_oMinAxisType->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oMaxAxisType.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::MaxAxisType);
        m_oBcw.m_oStream.WriteBYTE(oSparklineGroup.m_oMaxAxisType->GetValue());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oRightToLeft.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::RightToLeft);
        m_oBcw.m_oStream.WriteBOOL(oSparklineGroup.m_oRightToLeft->ToBool());
        m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oSparklineGroup.m_oColorSeries.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorSeries);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorSeries.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
    }
	if (oSparklineGroup.m_oColorNegative.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorNegative);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorNegative.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorAxis.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorAxis);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorAxis.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorMarkers.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorMarkers);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorMarkers.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorFirst.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorFirst);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorFirst.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorLast.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorLast);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorLast.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorHigh.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorHigh);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorHigh.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
	if (oSparklineGroup.m_oColorLow.IsInit())
	{
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::ColorLow);
        m_oBcw.WriteColor(oSparklineGroup.m_oColorLow.get(), m_pIndexedColors);
        m_oBcw.WriteItemEnd(nCurPos);
	}
    if (oSparklineGroup.m_oRef.IsInit())
    {
        m_oBcw.m_oStream.WriteBYTE(c_oSer_Sparkline::Ref);
		m_oBcw.m_oStream.WriteStringW(oSparklineGroup.m_oRef.get());
    }
    if (oSparklineGroup.m_oSparklines.IsInit())
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Sparklines);
        WriteSparklines(oSparklineGroup.m_oSparklines.get());
        m_oBcw.WriteItemEnd(nCurPos);
    }
}
void BinaryWorksheetTableWriter::WriteDataValidations(const OOX::Spreadsheet::CDataValidations& oDataValidations)
{
	int nCurPos = 0;
    if (oDataValidations.m_oDisablePrompts.IsInit())
    {
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DataValidation::DisablePrompts);
		m_oBcw.m_oStream.WriteBOOL(oDataValidations.m_oDisablePrompts->ToBool());
		m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oDataValidations.m_oXWindow.IsInit())
    {
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DataValidation::XWindow);
		m_oBcw.m_oStream.WriteLONG(oDataValidations.m_oXWindow->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
    }
    if (oDataValidations.m_oYWindow.IsInit())
    {
		nCurPos = m_oBcw.WriteItemStart(c_oSer_DataValidation::YWindow);
		m_oBcw.m_oStream.WriteLONG(oDataValidations.m_oYWindow->GetValue());
		m_oBcw.WriteItemEnd(nCurPos);
    }

	nCurPos = m_oBcw.WriteItemStart(c_oSer_DataValidation::DataValidations);
	WriteDataValidationsContent(oDataValidations);
	m_oBcw.WriteItemEnd(nCurPos);
}
void BinaryWorksheetTableWriter::WriteDataValidationsContent(const OOX::Spreadsheet::CDataValidations& oDataValidations)
{
	for(size_t i = 0; i < oDataValidations.m_arrItems.size(); ++i)
    {
        int nCurPos = m_oBcw.WriteItemStart(c_oSer_DataValidation::DataValidation);
        WriteDataValidation(*oDataValidations.m_arrItems[i]);
        m_oBcw.WriteItemEnd(nCurPos);
    }
}
void BinaryWorksheetTableWriter::WriteDataValidation(const OOX::Spreadsheet::CDataValidation& oDataValidation)
{
    if (oDataValidation.m_oType.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Type);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oDataValidation.m_oType->GetValue());
    }
    if (oDataValidation.m_oAllowBlank.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::AllowBlank);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oDataValidation.m_oAllowBlank->ToBool());
    }
    if (oDataValidation.m_oError.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Error);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oError.get());
    }
    if (oDataValidation.m_oErrorTitle.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ErrorTitle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oErrorTitle.get());
    }
    if (oDataValidation.m_oErrorStyle.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ErrorStyle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oDataValidation.m_oErrorStyle->GetValue());
    }
    if (oDataValidation.m_oImeMode.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ImeMode);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oDataValidation.m_oImeMode->GetValue());
    }
    if (oDataValidation.m_oOperator.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Operator);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBYTE(oDataValidation.m_oOperator->GetValue());
    }
    if (oDataValidation.m_oPrompt.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Promt);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oPrompt.get());
    }
    if (oDataValidation.m_oPromptTitle.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::PromptTitle);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oPromptTitle.get());
    }
    if (oDataValidation.m_oShowDropDown.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ShowDropDown);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oDataValidation.m_oShowDropDown->ToBool());
    }
    if (oDataValidation.m_oShowErrorMessage.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ShowErrorMessage);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oDataValidation.m_oShowErrorMessage->ToBool());
    }
    if (oDataValidation.m_oShowInputMessage.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::ShowInputMessage);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oDataValidation.m_oShowInputMessage->ToBool());
    }
    if (oDataValidation.m_oSqRef.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::SqRef);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oSqRef.get());
    }
    if (oDataValidation.m_oFormula1.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Formula1);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oFormula1->m_sText);
    }
    if (oDataValidation.m_oFormula2.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE(c_oSer_DataValidation::Formula2);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oDataValidation.m_oFormula2->m_sText);
    }
}
void BinaryWorksheetTableWriter::WriteSparklines(const OOX::Spreadsheet::CSparklines& oSparklines)
{
    int nCurPos = 0;
    for(size_t i = 0; i < oSparklines.m_arrItems.size(); ++i)
    {
        nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Sparkline);
        WriteSparkline(*oSparklines.m_arrItems[i]);
        m_oBcw.WriteItemEnd(nCurPos);
    }
}
void BinaryWorksheetTableWriter::WriteSparkline(const OOX::Spreadsheet::CSparkline& oSparkline)
{
    int nCurPos = 0;
    if (oSparkline.m_oRef.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE		(c_oSer_Sparkline::SparklineRef);
		m_oBcw.m_oStream.WriteStringW	(oSparkline.m_oRef.get());
    }
    if (oSparkline.m_oSqRef.IsInit())
    {
		m_oBcw.m_oStream.WriteBYTE		(c_oSer_Sparkline::SparklineSqRef);
		m_oBcw.m_oStream.WriteStringW	(oSparkline.m_oSqRef.get());
    }
}
void BinaryWorksheetTableWriter::WriteSlicers(OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CSlicerRefs& oSlicers)
{
	int nCurPos = 0;
	for(size_t i = 0; i < oSlicers.m_oSlicer.size(); ++i)
	{
		if(oSlicers.m_oSlicer[i].m_oRId.IsInit())
		{
			smart_ptr<OOX::File> pFile = oWorksheet.Find(OOX::RId(oSlicers.m_oSlicer[i].m_oRId->GetValue()));
			if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::Slicer == pFile->type())
			{
				OOX::Spreadsheet::CSlicerFile* pSlicerFile = static_cast<OOX::Spreadsheet::CSlicerFile*>(pFile.GetPointer());
				if(pSlicerFile->m_oSlicers.IsInit())
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Slicer);
					m_oBcw.m_oStream.WriteRecord2(0, pSlicerFile->m_oSlicers);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------------------------------------------
BinaryCalcChainTableWriter::BinaryCalcChainTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream) : m_oBcw(oCBufferedStream)
{
}
void BinaryCalcChainTableWriter::Write(OOX::Spreadsheet::CCalcChain& pCalcChain)
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteCalcChainTableContent(pCalcChain);
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryCalcChainTableWriter::WriteCalcChainTableContent(OOX::Spreadsheet::CCalcChain& pCalcChain)
{
	int nCurPos;
	for(size_t i = 0, length = pCalcChain.m_arrItems.size(); i < length; ++i)
	{
		//media
		nCurPos = m_oBcw.WriteItemStart(c_oSer_CalcChainType::CalcChainItem);
		WriteCalcChain(*pCalcChain.m_arrItems[i]);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}
void BinaryCalcChainTableWriter::WriteCalcChain(OOX::Spreadsheet::CCalcCell& oCalcCell)
{
	//Array
	if(oCalcCell.m_oArray.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::Array);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCalcCell.m_oArray->ToBool());
	}
	//SheetId
	if(oCalcCell.m_oSheetId.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::SheetId);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
		m_oBcw.m_oStream.WriteLONG(oCalcCell.m_oSheetId->GetValue());
	}
	//DependencyLevel
	if(oCalcCell.m_oDependencyLevel.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::DependencyLevel);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCalcCell.m_oDependencyLevel->ToBool());
	}
	//Ref
	if(oCalcCell.m_oRef.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::Ref);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
		m_oBcw.m_oStream.WriteStringW(oCalcCell.m_oRef.get2());
	}
	//ChildChain
	if(oCalcCell.m_oChildChain.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::ChildChain);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCalcCell.m_oChildChain->ToBool());
	}
	//NewThread
	if(oCalcCell.m_oNewThread.IsInit())
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_CalcChainType::NewThread);
		m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
		m_oBcw.m_oStream.WriteBOOL(oCalcCell.m_oNewThread->ToBool());
	}
}
//-----------------------------------------------------------------------------------------------------
BinaryCustomsTableWriter::BinaryCustomsTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream) : m_oBcw(oCBufferedStream)
{
}
void BinaryCustomsTableWriter::Write(OOX::IFileContainer *pContainer)
{
	if (!pContainer) return;

	int nStart = m_oBcw.WriteItemWithLengthStart();

	std::vector<smart_ptr<OOX::File>>& container = pContainer->GetContainer();
	for (size_t k = 0; k < container.size(); ++k)
	{
		if (OOX::FileTypes::CustomXml == container[k]->type())
		{
			OOX::CCustomXML* pCustomXml = dynamic_cast<OOX::CCustomXML*>(container[k].GetPointer());
			if (pCustomXml->m_bUsed) continue;

			int nCurPos = m_oBcw.WriteItemStart(c_oSerCustoms::Custom);

			std::vector<smart_ptr<OOX::File>>& containerCustom = pCustomXml->GetContainer();
			for (size_t i = 0; i < containerCustom.size(); ++i)
			{
				if (OOX::FileTypes::CustomXmlProps == containerCustom[i]->type())
				{
					OOX::CCustomXMLProps* pCustomXmlProps = dynamic_cast<OOX::CCustomXMLProps*>(containerCustom[i].GetPointer());

					int nCurPos1 = m_oBcw.WriteItemStart(c_oSerCustoms::ItemId);
					m_oBcw.m_oStream.WriteStringW3(pCustomXmlProps->m_oItemID.ToString());
					m_oBcw.WriteItemEnd(nCurPos1);

					if (pCustomXmlProps->m_oShemaRefs.IsInit())
					{
						for (size_t j = 0; j < pCustomXmlProps->m_oShemaRefs->m_arrItems.size(); ++j)
						{
							nCurPos1 = m_oBcw.WriteItemStart(c_oSerCustoms::Uri);
							m_oBcw.m_oStream.WriteStringW3(pCustomXmlProps->m_oShemaRefs->m_arrItems[j]->m_sUri);
							m_oBcw.WriteItemEnd(nCurPos1);
						}
					}
				}
			}

			int nCurPos2 = m_oBcw.WriteItemStart(c_oSerCustoms::Content);
			m_oBcw.m_oStream.WriteStringA(pCustomXml->m_sXmlA);
			m_oBcw.WriteItemEnd(nCurPos2);

			m_oBcw.WriteItemEnd(nCurPos);
			pCustomXml->m_bUsed = true;
		}
	}
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
//------------------------------------------------------------------------------------------------------
BinaryOtherTableWriter::BinaryOtherTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, PPTX::Theme* pTheme, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
	:	m_oBcw					(oCBufferedStream), 
		m_pEmbeddedFontsManager	(pEmbeddedFontsManager),
		m_pTheme				(pTheme),
		m_pOfficeDrawingConverter(pOfficeDrawingConverter)
{
}
void BinaryOtherTableWriter::Write()
{
	int nStart = m_oBcw.WriteItemWithLengthStart();
	WriteOtherTableContent();
	m_oBcw.WriteItemWithLengthEnd(nStart);
}
void BinaryOtherTableWriter::WriteOtherTableContent()
{
	int nCurPos;
	//EmbeddedFonts
	if(NULL != m_pEmbeddedFontsManager)
	{
		EmbeddedBinaryWriter oEmbeddedBinaryWriter(m_oBcw.m_oStream);
		nCurPos = m_oBcw.WriteItemStart(c_oSer_OtherType::EmbeddedFonts);
		m_pEmbeddedFontsManager->WriteEmbeddedFonts<EmbeddedBinaryWriter>(&oEmbeddedBinaryWriter);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
	//Theme
	if(NULL != m_pTheme)
	{
		m_oBcw.m_oStream.WriteBYTE(c_oSer_OtherType::Theme);
		
		int nCurPos = m_oBcw.WriteItemWithLengthStart();
			m_pTheme->toPPTY(&m_oBcw.m_oStream);
		m_oBcw.WriteItemWithLengthEnd(nCurPos);
	}
}

BinaryFileWriter::BinaryFileWriter(DocWrapper::FontProcessor& oFontProcessor) : m_oBcw(NULL), m_oFontProcessor(oFontProcessor)
{
	m_nLastFilePos = 0;
	m_nLastFilePosOffset = 0;
	m_nRealTableCount = 0;
}
BinaryFileWriter::~BinaryFileWriter()
{
	RELEASEOBJECT(m_oBcw);
}
_UINT32 BinaryFileWriter::Open(const std::wstring& sInputDir, const std::wstring& sFileDst, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager,
    NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bIsNoBase64)
{
	_UINT32 result = 0;

	OOX::CPath path(sFileDst);
//создаем папку для media
    std::wstring mediaDir = path.GetDirectory() + L"media";
	NSDirectory::CreateDirectory(mediaDir);

    pOfficeDrawingConverter->SetDstPath(path.GetDirectory() + FILE_SEPARATOR_STR + L"word");
    pOfficeDrawingConverter->SetMediaDstPath(mediaDir);

	NSBinPptxRW::CBinaryFileWriter& oBufferedStream = *pOfficeDrawingConverter->m_pBinaryWriter;

	m_oBcw = new BinaryCommonWriter(oBufferedStream);

	// File Type
	BYTE fileType;
	UINT nCodePage;
	std::wstring sDelimiter;
	BYTE saveFileType;
	SerializeCommon::ReadFileType(sXMLOptions, fileType, nCodePage, sDelimiter, saveFileType);

	m_nLastFilePosOffset = 0;
	OOX::Spreadsheet::CXlsx *pXlsx = NULL;
	OOX::Spreadsheet::CXlsxFlat *pXlsxFlat = NULL;
	switch(fileType)
	{
		case BinXlsxRW::c_oFileTypes::CSV:
		{
			CSVReader csvReader;

			pXlsx = new OOX::Spreadsheet::CXlsx();
			result = csvReader.Read(sInputDir, *pXlsx, nCodePage, sDelimiter);
		}break;
		case BinXlsxRW::c_oFileTypes::XLSX:
        case BinXlsxRW::c_oFileTypes::XLSB:
		default:
		{
			if (bIsNoBase64 && BinXlsxRW::c_oFileTypes::JSON != saveFileType)
			{
                if(fileType == BinXlsxRW::c_oFileTypes::XLSB)
                    pXlsx = new OOX::Spreadsheet::CXlsb();
                else
                    pXlsx = new OOX::Spreadsheet::CXlsx();
				pXlsx->m_bNeedCalcChain = false;

				NSBinPptxRW::CXlsbBinaryWriter oXlsbWriter;
				oXlsbWriter.CreateFileW(sFileDst);
				//write dummy header and main table
				oXlsbWriter.WriteStringUtf8(WriteFileHeader(0, g_nFormatVersionNoBase64));
				oXlsbWriter.WriteReserved(GetMainTableSize());
				int nDataStartPos = oXlsbWriter.GetPositionAbsolute();
				
				if (fileType == 1)
				{
					pXlsx->m_pXlsbWriter = &oXlsbWriter;
				}
				//parse
				pXlsx->Read(OOX::CPath(sInputDir));

				pXlsx->m_pXlsbWriter = NULL;
				oXlsbWriter.CloseFile();
				m_nLastFilePosOffset = oXlsbWriter.GetPositionAbsolute() - nDataStartPos;
			}
			else
			{
				pXlsx = new OOX::Spreadsheet::CXlsx(OOX::CPath(sInputDir));
			}
		}break;
	}		
	if (0 != result && AVS_FILEUTILS_ERROR_CONVERT_ROWLIMITS != result)
	{
		RELEASEOBJECT(pXlsx);
		return result;
	}
	if (pXlsx && !pXlsx->m_pWorkbook && pXlsx->m_arWorksheets.empty())
	{
		delete pXlsx; pXlsx = NULL;
		pXlsxFlat = new OOX::Spreadsheet::CXlsxFlat(OOX::CPath(sInputDir));

		if (pXlsxFlat && pXlsxFlat->m_arWorksheets.empty())
		{
			delete pXlsxFlat; pXlsxFlat = NULL;
		}
	}
	if (pXlsx)
	{
		pXlsx->PrepareWorkbook();

		if (NULL == pXlsx->m_pWorkbook)
		{
			RELEASEOBJECT(pXlsx);
			return AVS_FILEUTILS_ERROR_CONVERT;
        }

        if (fileType == BinXlsxRW::c_oFileTypes::XLSB)
        {
            dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsx)->PrepareSi();
            dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsx)->PrepareTableFormula();
            dynamic_cast<OOX::Spreadsheet::CXlsb*>(pXlsx)->ReadSheetData();
        }
	}

	if (BinXlsxRW::c_oFileTypes::JSON == saveFileType)
	{
//todo 46 временно CP_UTF8
		
		CSVWriter oCSVWriter;
		oCSVWriter.Xlsx2Csv(sFileDst, *pXlsx, 46, std::wstring(L","), true);
	}
	else
	{
		if (bIsNoBase64)
		{
			oBufferedStream.WriteStringUtf8(WriteFileHeader(0, g_nFormatVersionNoBase64));
		}
		int nHeaderLen = oBufferedStream.GetPosition();
		
		WriteMainTableStart(oBufferedStream);
		WriteContent(pXlsx ? dynamic_cast<OOX::Document*>(pXlsx) : dynamic_cast<OOX::Document*>(pXlsxFlat), pEmbeddedFontsManager, pOfficeDrawingConverter);
		WriteMainTableEnd();

		BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
		int nBinBufferLen = oBufferedStream.GetPosition();
		if (bIsNoBase64)
		{
			int nMidPoint = nHeaderLen + GetMainTableSize();
			
			NSFile::CFileBinary oFile;
			if(0 != m_nLastFilePosOffset)
			{
				oFile.OpenFile(sFileDst, true);
			}
			else
			{
				oFile.CreateFileW(sFileDst);
			}
			//write header and main table
			oFile.WriteFile(pbBinBuffer, nMidPoint);
			//skip xlsb records written on xml reading
			oFile.SeekFile(0, SEEK_END);
			//write other records
			oFile.WriteFile(pbBinBuffer + nMidPoint, nBinBufferLen - nMidPoint);
			oFile.CloseFile();
		}
		else
		{
			int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nBinBufferLen, Base64::B64_BASE64_FLAG_NOCRLF);
			BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen+64];
			if(true == Base64_1::Base64Encode(pbBinBuffer, nBinBufferLen, pbBase64Buffer, &nBase64BufferLen))
			{
				NSFile::CFileBinary oFile;
				oFile.CreateFileW(sFileDst);
				oFile.WriteStringUTF8(WriteFileHeader(nBinBufferLen, g_nFormatVersion));
				oFile.WriteFile(pbBase64Buffer, nBase64BufferLen);
				oFile.CloseFile();
			}
			else
			{
				result = AVS_FILEUTILS_ERROR_CONVERT;
			}
			RELEASEARRAYOBJECTS(pbBase64Buffer);
		}
	}

	RELEASEOBJECT(pXlsx);
	RELEASEOBJECT(pXlsxFlat);

	return result;
}
void BinaryFileWriter::WriteBinaryTable(const BYTE* pBuffer, size_t size)
{
	int nCurPos = m_oBcw->m_oStream.GetPosition();

	m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
	m_oBcw->m_oStream.WriteBYTEArray(pBuffer, size);
	m_nLastFilePos = m_oBcw->m_oStream.GetPosition();

	m_oBcw->m_oStream.SetPosition(nCurPos);
}

void BinaryFileWriter::WriteContent(OOX::Document *pDocument, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
{
	OOX::Spreadsheet::CXlsx *pXlsx = dynamic_cast<OOX::Spreadsheet::CXlsx *>(pDocument);
	OOX::Spreadsheet::CXlsxFlat *pXlsxFlat = dynamic_cast<OOX::Spreadsheet::CXlsxFlat *>(pDocument);

	OOX::Spreadsheet::CStyles *pStyles = NULL;
	OOX::Spreadsheet::CWorkbook *pWorkbook = NULL;
	OOX::Spreadsheet::CSharedStrings *pSharedStrings = NULL;

	if (pXlsx)
	{
		pWorkbook = pXlsx->m_pWorkbook;
		pStyles = pXlsx->m_pStyles;
		pSharedStrings = pXlsx->m_pSharedStrings;
	}
	if (pXlsxFlat)
	{
		pWorkbook = pXlsxFlat->m_pWorkbook.GetPointer();
		pStyles = pXlsxFlat->m_pStyles.GetPointer();
		pSharedStrings = pXlsxFlat->m_pSharedStrings.GetPointer();
	}

	int nCurPos;
//SharedString
	OOX::Spreadsheet::CIndexedColors* pIndexedColors = NULL;
	
	if( pStyles && pStyles->m_oColors.IsInit() && pStyles->m_oColors->m_oIndexedColors.IsInit())
	{
		pIndexedColors = pStyles->m_oColors->m_oIndexedColors.operator ->();
	}

	if(pXlsx && pXlsx->m_pApp)
	{
		nCurPos = this->WriteTableStart(c_oSerTableTypes::App);
		pXlsx->m_pApp->ToPptxApp()->toPPTY(&m_oBcw->m_oStream);
		this->WriteTableEnd(nCurPos);
	}

	if(pXlsx && pXlsx->m_pCore)
	{
		nCurPos = this->WriteTableStart(c_oSerTableTypes::Core);
		pXlsx->m_pCore->ToPptxCore()->toPPTY(&m_oBcw->m_oStream);
		this->WriteTableEnd(nCurPos);
	}

	if (pXlsx)
	{
		smart_ptr<OOX::File> pFile = pXlsx->Find(OOX::FileTypes::CustomProperties);
		PPTX::CustomProperties *pCustomProperties = dynamic_cast<PPTX::CustomProperties*>(pFile.GetPointer());
		if (pCustomProperties)
		{
			nCurPos = this->WriteTableStart(c_oSerTableTypes::CustomProperties);
			pCustomProperties->toPPTY(&m_oBcw->m_oStream);
			this->WriteTableEnd(nCurPos);
		}
	}

	if(pSharedStrings)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::SharedStrings);
		BinarySharedStringTableWriter oBinarySharedStringTableWriter(m_oBcw->m_oStream, pEmbeddedFontsManager);
		oBinarySharedStringTableWriter.Write(*pSharedStrings, pIndexedColors, pXlsx ? pXlsx->GetTheme() : NULL, m_oFontProcessor);
		WriteTableEnd(nCurPos);
	}

	if(pWorkbook && pWorkbook->m_pPersonList)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::PersonList);
		BinaryPersonTableWriter oBinaryPersonTableWriter(m_oBcw->m_oStream);
		oBinaryPersonTableWriter.Write(*pXlsx->m_pWorkbook->m_pPersonList);
		WriteTableEnd(nCurPos);
	}
//Styles
	if(pStyles)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Styles);
		BinaryStyleTableWriter oBinaryStyleTableWriter(m_oBcw->m_oStream, pEmbeddedFontsManager);
		oBinaryStyleTableWriter.Write(*pStyles, pXlsx ? pXlsx->GetTheme() : NULL, m_oFontProcessor);
		WriteTableEnd(nCurPos);
	}
////CalcChain
	//OOX::Spreadsheet::CCalcChain* pCalcChain = oXlsx.GetCalcChain();
	//if(NULL != pCalcChain)
	//{
	//	nCurPos = WriteTableStart(c_oSerTableTypes::CalcChain);
	//	BinaryCalcChainTableWriter oBinaryCalcChainTableWriter(oBufferedStream);
	//	oBinaryCalcChainTableWriter.Write(*pCalcChain);
	//	WriteTableEnd(nCurPos);
	//}

//Workbook
	if ( pWorkbook)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Workbook);
		BinaryWorkbookTableWriter oBinaryWorkbookTableWriter(m_oBcw->m_oStream, pXlsx ? dynamic_cast<OOX::Document*>(pXlsx) : dynamic_cast<OOX::Document*>(pXlsxFlat));
		oBinaryWorkbookTableWriter.Write(*pWorkbook);
		WriteTableEnd(nCurPos);
	}
	if(pXlsx)
	{
	//Worksheets
		nCurPos = WriteTableStart(c_oSerTableTypes::Worksheets);
		BinaryWorksheetTableWriter oBinaryWorksheetTableWriter(m_oBcw->m_oStream, pEmbeddedFontsManager, pIndexedColors, pXlsx->GetTheme(), m_oFontProcessor, pOfficeDrawingConverter);
		oBinaryWorksheetTableWriter.Write(*pXlsx->m_pWorkbook, pXlsx->m_mapWorksheets); 
		WriteTableEnd(nCurPos);
	
	//OtherTable
		nCurPos = WriteTableStart(c_oSerTableTypes::Other);
		BinaryOtherTableWriter oBinaryOtherTableWriter(m_oBcw->m_oStream, pEmbeddedFontsManager, pXlsx->GetTheme(), pOfficeDrawingConverter);
		oBinaryOtherTableWriter.Write();
		WriteTableEnd(nCurPos);

	//Customs from Workbook (todooo - другие)
		nCurPos = WriteTableStart(c_oSerTableTypes::Customs);
		BinaryCustomsTableWriter oBinaryCustomsTableWriter(m_oBcw->m_oStream);
		oBinaryCustomsTableWriter.Write(pXlsx->m_pWorkbook);
		WriteTableEnd(nCurPos);
	}
	else if (pXlsxFlat)
	{
		nCurPos = WriteTableStart(c_oSerTableTypes::Worksheets);
		BinaryWorksheetTableWriter oBinaryWorksheetTableWriter(m_oBcw->m_oStream, pEmbeddedFontsManager, pIndexedColors, NULL, m_oFontProcessor, pOfficeDrawingConverter);
		oBinaryWorksheetTableWriter.Write(*pXlsxFlat->m_pWorkbook.GetPointer(), pXlsxFlat->m_arWorksheets); 
		WriteTableEnd(nCurPos);
	}
}

std::wstring BinaryFileWriter::WriteFileHeader(int nDataSize, int version)
{
    std::wstring sHeader = std::wstring(g_sFormatSignature) + L";v" + std::to_wstring(version)+ L";" + std::to_wstring(nDataSize) + L";";
	return sHeader;
}
void BinaryFileWriter::WriteMainTableStart(NSBinPptxRW::CBinaryFileWriter &oBufferedStream)
{
	if (!m_oBcw)
		m_oBcw = new BinaryCommonWriter(oBufferedStream);

	m_nRealTableCount = 0;
	m_nMainTableStart = m_oBcw->m_oStream.GetPosition();
	//вычисляем с какой позиции можно писать таблицы
	m_nLastFilePos = m_nMainTableStart + GetMainTableSize();
	//Write mtLen
	m_oBcw->m_oStream.WriteBYTE(0);
}
int BinaryFileWriter::GetMainTableSize()
{
	return 128 * 5;//128 items of 5 bytes
}
void BinaryFileWriter::WriteMainTableEnd()
{
//Количество таблиц
	m_oBcw->m_oStream.SetPosition(m_nMainTableStart);
	m_oBcw->m_oStream.WriteBYTE(m_nRealTableCount);

	m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
}
int BinaryFileWriter::WriteTableStart(BYTE type, int nStartPos)
{
	if(-1 != nStartPos)
		m_oBcw->m_oStream.SetPosition(nStartPos);
	//Write mtItem
	//Write mtiType
	m_oBcw->m_oStream.WriteBYTE(type);
	//Write mtiOffBits
	m_oBcw->m_oStream.WriteLONG(m_nLastFilePos + m_nLastFilePosOffset);

	//Write table
	//Запоминаем позицию в MainTable
	int nCurPos = m_oBcw->m_oStream.GetPosition();
	//Seek в свободную область
	m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
	return nCurPos;
}
void BinaryFileWriter::WriteTableEnd(int nCurPos)
{
	//сдвигаем позицию куда можно следующую таблицу
	m_nLastFilePos = m_oBcw->m_oStream.GetPosition();
	m_nRealTableCount++;
	//Seek вобратно в MainTable
	m_oBcw->m_oStream.SetPosition(nCurPos);
}

}
