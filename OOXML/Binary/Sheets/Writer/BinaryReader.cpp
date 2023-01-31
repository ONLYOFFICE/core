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

#include "BinaryReader.h"

#include "../../../../Common/Base64.h"
#include "../../../../Common/ATLDefine.h"
#include "../../../../Common/OfficeFileErrorDescription.h"

#include "../../../../DesktopEditor/common/Path.h"
#include "../../../../DesktopEditor/common/Directory.h"

#include "../Writer/CSVWriter.h"
#include "BinaryCommonReader.h"

#include "../../Document/BinReader/DefaultThemeWriter.h"

#include "../../../PPTXFormat/Theme.h"
#include "../../../../MsBinaryFile/Common/Vml/toVmlConvert.h"
#include "../../../PPTXFormat/App.h"
#include "../../../PPTXFormat/Core.h"
#include "../../../PPTXFormat/Logic/HeadingVariant.h"

#include "../../../XlsxFormat/Worksheets/Sparkline.h"
#include "../../../XlsxFormat/Drawing/Drawing.h"
#include "../../../XlsxFormat/Comments/Comments.h"
#include "../../../XlsxFormat/SharedStrings/SharedStrings.h"
#include "../../../XlsxFormat/Styles/Styles.h"
#include "../../../XlsxFormat/CalcChain/CalcChain.h"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinks.h"
#include "../../../XlsxFormat/ExternalLinks/ExternalLinkPath.h"
#include "../../../XlsxFormat/WorkbookComments.h"
#include "../../../XlsxFormat/Table/Connections.h"
#include "../../../XlsxFormat/Controls/Controls.h"

#include "../../../DocxFormat/Media/VbaProject.h"
#include "../../../DocxFormat/Media/JsaProject.h"
#include "../../../DocxFormat/VmlDrawing.h"
#include "../../../DocxFormat/App.h"
#include "../../../DocxFormat/Core.h"
#include "../../../DocxFormat/CustomXml.h"

#include "../../../XlsxFormat/Comments/ThreadedComments.h"
#include "../../../XlsxFormat/Slicer/SlicerCache.h"
#include "../../../XlsxFormat/Slicer/SlicerCacheExt.h"
#include "../../../XlsxFormat/Slicer/Slicer.h"
#include "../../../XlsxFormat/NamedSheetViews/NamedSheetViews.h"

namespace BinXlsxRW 
{
	#define SEEK_TO_POS_START(type) \
		pFind = mapPos.find(type); \
		if (pFind != mapPos.end()) \
		{ \
			for (size_t i = 0; i < pFind->second.size() - 1; i+=2) \
			{ \
				nPos = pFind->second[i]; \
				length = pFind->second[i + 1]; \
				m_oBufferedStream.Seek(nPos); \

#define SEEK_TO_POS_END(elem) \
				elem.toXML(oStreamWriter); \
			} \
		}

#define SEEK_TO_POS_END2() \
			} \
		}

#define SEEK_TO_POS_ELSE() \
		else \
		{

#define SEEK_TO_POS_ELSE_END() \
		}

Binary_CommonReader2::Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_poBufferedStream(poBufferedStream)
{
}
int Binary_CommonReader2::ReadColor(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CColor* pColor = static_cast<OOX::Spreadsheet::CColor*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ColorObjectType::Type == type)
	{
		BYTE byteColorType = m_poBufferedStream.GetUChar();
		if(c_oSer_ColorType::Auto == byteColorType)
		{
			pColor->m_oAuto.Init();
			pColor->m_oAuto->SetValue(SimpleTypes::onoffTrue);
		}
	}
	else if(c_oSer_ColorObjectType::Rgb == type)
	{
		pColor->m_oRgb.Init();
		pColor->m_oRgb->FromInt(m_poBufferedStream.GetLong());
	}
	else if(c_oSer_ColorObjectType::Theme == type)
	{
		pColor->m_oThemeColor.Init();
		pColor->m_oThemeColor->SetValue((SimpleTypes::Spreadsheet::EThemeColor)m_poBufferedStream.GetUChar());
	}
	else if(c_oSer_ColorObjectType::Tint == type)
	{
		pColor->m_oTint.Init();
		pColor->m_oTint->SetValue(m_poBufferedStream.GetDoubleReal());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

BinaryTableReader::BinaryTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorksheet* pCurWorksheet):Binary_CommonReader(oBufferedStream), m_pCurWorksheet(pCurWorksheet)
{
}
int BinaryTableReader::Read(long length, OOX::Spreadsheet::CTableParts* pTableParts)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadTablePart, pTableParts);
	return res;
}
int BinaryTableReader::ReadQueryTable(long length, OOX::Spreadsheet::CQueryTable* pQueryTable)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadQueryTableContent, pQueryTable);
	return res;
}
int BinaryTableReader::ReadQueryTableContent(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTable* pQueryTable = static_cast<OOX::Spreadsheet::CQueryTable*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTable::ConnectionId == type)
	{
		pQueryTable->m_oConnectionId.Init();
		pQueryTable->m_oConnectionId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTable::Name == type)
	{
		pQueryTable->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_QueryTable::AutoFormatId == type)
	{
		pQueryTable->m_oAutoFormatId.Init();
		pQueryTable->m_oAutoFormatId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTable::GrowShrinkType == type)
	{
		pQueryTable->m_oGrowShrinkType = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_QueryTable::AdjustColumnWidth == type)
	{
		pQueryTable->m_oAdjustColumnWidth = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyAlignmentFormats == type)
	{
		pQueryTable->m_oApplyAlignmentFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyBorderFormats == type)
	{
		pQueryTable->m_oApplyBorderFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyFontFormats == type)
	{
		pQueryTable->m_oApplyFontFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyNumberFormats == type)
	{
		pQueryTable->m_oApplyNumberFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyPatternFormats == type)
	{
		pQueryTable->m_oApplyPatternFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::ApplyWidthHeightFormats == type)
	{
		pQueryTable->m_oApplyWidthHeightFormats = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::BackgroundRefresh == type)
	{
		pQueryTable->m_oBackgroundRefresh = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::DisableEdit == type)
	{
		pQueryTable->m_oDisableEdit = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::DisableRefresh == type)
	{
		pQueryTable->m_oDisableRefresh = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::FillFormulas == type)
	{
		pQueryTable->m_oFillFormulas = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::FirstBackgroundRefresh == type)
	{
		pQueryTable->m_oFirstBackgroundRefresh = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::Headers == type)
	{
		pQueryTable->m_oHeaders = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::Intermediate == type)
	{
		pQueryTable->m_oIntermediate = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::PreserveFormatting == type)
	{
		pQueryTable->m_oPreserveFormatting = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::RefreshOnLoad == type)
	{
		pQueryTable->m_oRefreshOnLoad = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::RemoveDataOnSave == type)
	{
		pQueryTable->m_oRemoveDataOnSave = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::RowNumbers == type)
	{
		pQueryTable->m_oRowNumbers = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTable::QueryTableRefresh == type)
	{
		pQueryTable->m_oQueryTableRefresh.Init();
		READ1_DEF(length, res, this->ReadQueryTableRefresh, pQueryTable->m_oQueryTableRefresh.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadQueryTableRefresh(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTableRefresh* pQueryTableRefresh = static_cast<OOX::Spreadsheet::CQueryTableRefresh*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTableRefresh::NextId == type)
	{
		pQueryTableRefresh->m_oNextId.Init();
		pQueryTableRefresh->m_oNextId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableRefresh::MinimumVersion == type)
	{
		pQueryTableRefresh->m_oMinimumVersion.Init();
		pQueryTableRefresh->m_oMinimumVersion->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableRefresh::UnboundColumnsLeft == type)
	{
		pQueryTableRefresh->m_UnboundColumnsLeft.Init();
		pQueryTableRefresh->m_UnboundColumnsLeft->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableRefresh::UnboundColumnsRight == type)
	{
		pQueryTableRefresh->m_UnboundColumnsRight.Init();
		pQueryTableRefresh->m_UnboundColumnsRight->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableRefresh::FieldIdWrapped == type)
	{
		pQueryTableRefresh->m_FieldIdWrapped = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableRefresh::HeadersInLastRefresh == type)
	{
		pQueryTableRefresh->m_HeadersInLastRefresh = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableRefresh::PreserveSortFilterLayout == type)
	{
		pQueryTableRefresh->m_PreserveSortFilterLayout = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableRefresh::SortState == type)
	{
		pQueryTableRefresh->m_oSortState.Init();
		READ1_DEF(length, res, this->ReadSortState, pQueryTableRefresh->m_oSortState.GetPointer());
	}
	else if(c_oSer_QueryTableRefresh::QueryTableFields == type)
	{
		pQueryTableRefresh->m_oQueryTableFields.Init();
		READ1_DEF(length, res, this->ReadQueryTableFields, pQueryTableRefresh->m_oQueryTableFields.GetPointer());
	}
	else if(c_oSer_QueryTableRefresh::QueryTableDeletedFields == type)
	{
		pQueryTableRefresh->m_oQueryTableDeletedFields.Init();
		READ1_DEF(length, res, this->ReadQueryTableDeletedFields, pQueryTableRefresh->m_oQueryTableDeletedFields.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadQueryTableFields(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTableFields* pQueryTableFields = static_cast<OOX::Spreadsheet::CQueryTableFields*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTableField::QueryTableField == type)
	{
		OOX::Spreadsheet::CQueryTableField* pQueryTableField = new OOX::Spreadsheet::CQueryTableField();
		READ1_DEF(length, res, this->ReadQueryTableField, pQueryTableField);
		
		pQueryTableFields->m_arrItems.push_back(pQueryTableField);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadQueryTableDeletedField(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTableDeletedField* pQueryTableDeletedField = static_cast<OOX::Spreadsheet::CQueryTableDeletedField*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTableDeletedField::Name == type)
	{
		pQueryTableDeletedField->m_oName = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadQueryTableDeletedFields(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTableDeletedFields* pQueryTableDeletedFields = static_cast<OOX::Spreadsheet::CQueryTableDeletedFields*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTableDeletedField::QueryTableDeletedField == type)
	{
		OOX::Spreadsheet::CQueryTableDeletedField* pQueryTableDeletedField = new OOX::Spreadsheet::CQueryTableDeletedField();
		READ1_DEF(length, res, this->ReadQueryTableDeletedField, pQueryTableDeletedField);
		
		pQueryTableDeletedFields->m_arrItems.push_back(pQueryTableDeletedField);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}		
int BinaryTableReader::ReadQueryTableField(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CQueryTableField* pQueryTableField = static_cast<OOX::Spreadsheet::CQueryTableField*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if(c_oSer_QueryTableField::Name == type)
	{
		pQueryTableField->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_QueryTableField::Id == type)
	{
		pQueryTableField->m_oId.Init();
		pQueryTableField->m_oId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableField::TableColumnId == type)
	{
		pQueryTableField->m_oTableColumnId.Init();
		pQueryTableField->m_oTableColumnId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_QueryTableField::RowNumbers == type)
	{
		pQueryTableField->m_oRowNumbers = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableField::FillFormulas == type)
	{
		pQueryTableField->m_oFillFormulas = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableField::DataBound == type)
	{
		pQueryTableField->m_oDataBound = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_QueryTableField::Clipped == type)
	{
		pQueryTableField->m_oClipped = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int BinaryTableReader::ReadTablePart(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableParts* pTableParts = static_cast<OOX::Spreadsheet::CTableParts*>(poResult);
	if(c_oSer_TablePart::Table == type)
	{
		OOX::Spreadsheet::CTableFile* pTable = new OOX::Spreadsheet::CTableFile(NULL);
		pTable->m_oTable.Init();
		READ1_DEF(length, res, this->ReadTable, pTable);

		OOX::Spreadsheet::CTablePart* pTablePart = new OOX::Spreadsheet::CTablePart();
		NSCommon::smart_ptr<OOX::File> pTableFile(pTable);
		const OOX::RId oRId = m_pCurWorksheet->Add(pTableFile);
		pTablePart->m_oRId.Init();
		pTablePart->m_oRId->SetValue(oRId.get());
		pTableParts->m_arrItems.push_back(pTablePart);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryTableReader::ReadTable(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
		
	OOX::Spreadsheet::CTableFile* pTableFile = static_cast<OOX::Spreadsheet::CTableFile*>(poResult);
	if (!pTableFile) return c_oSerConstants::ReadUnknown;

	OOX::Spreadsheet::CTable* pTable = pTableFile->m_oTable.GetPointer();
	if (!pTable) return c_oSerConstants::ReadUnknown;

	if(c_oSer_TablePart::Ref == type)
	{
		pTable->m_oRef.Init();
		pTable->m_oRef->SetValue(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_TablePart::HeaderRowCount == type)
	{
		pTable->m_oHeaderRowCount.Init();
		pTable->m_oHeaderRowCount->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::TotalsRowCount == type)
	{
		pTable->m_oTotalsRowCount.Init();
		pTable->m_oTotalsRowCount->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::DisplayName == type)
	{
		pTable->m_oDisplayName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::Name == type)
	{
		pTable->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::Comment == type)
	{
		pTable->m_oComment = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::ConnectionId == type)
	{
		pTable->m_oConnectionId.Init();
		pTable->m_oConnectionId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::TableType == type)
	{
		pTable->m_oTableType.Init();
		pTable->m_oTableType->SetValue((SimpleTypes::Spreadsheet::ETableType)m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::DataCellStyle == type)
	{
		pTable->m_oDataCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::DataDxfId == type)
	{
		pTable->m_oDataDxfId.Init();
		pTable->m_oDataDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::HeaderRowCellStyle == type)
	{
		pTable->m_oHeaderRowCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::HeaderRowDxfId == type)
	{
		pTable->m_oHeaderRowDxfId.Init();
		pTable->m_oHeaderRowDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::HeaderRowBorderDxfId == type)
	{
		pTable->m_oHeaderRowBorderDxfId.Init();
		pTable->m_oHeaderRowBorderDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::Id == type)
	{
		pTable->m_oId.Init();
		pTable->m_oId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::InsertRow == type)
	{
		pTable->m_oInsertRow = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_TablePart::InsertRowShift == type)
	{
		pTable->m_oInsertRowShift = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_TablePart::Published == type)
	{
		pTable->m_oPublished = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_TablePart::TableBorderDxfId == type)
	{
		pTable->m_oTableBorderDxfId.Init();
		pTable->m_oTableBorderDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::TotalsRowBorderDxfId == type)
	{
		pTable->m_oTotalsRowBorderDxfId.Init();
		pTable->m_oTotalsRowBorderDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::TotalsRowCellStyle == type)
	{
		pTable->m_oTotalsRowCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TablePart::TotalsRowDxfId == type)
	{
		pTable->m_oTotalsRowDxfId.Init();
		pTable->m_oTotalsRowDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TablePart::TotalsRowShown == type)
	{
		pTable->m_oTotalsRowShown = m_oBufferedStream.GetBool();
	}
	else if(c_oSer_TablePart::AutoFilter == type)
	{
		pTable->m_oAutoFilter.Init();
		READ1_DEF(length, res, this->ReadAutoFilter, pTable->m_oAutoFilter.GetPointer());
	}
	else if(c_oSer_TablePart::SortState == type)
	{
		pTable->m_oSortState.Init();
		READ1_DEF(length, res, this->ReadSortState, pTable->m_oSortState.GetPointer());
	}
	else if(c_oSer_TablePart::TableColumns == type)
	{
		pTable->m_oTableColumns.Init();
		READ1_DEF(length, res, this->ReadTableColumns, pTable->m_oTableColumns.GetPointer());
	}
	else if(c_oSer_TablePart::TableStyleInfo == type)
	{
		pTable->m_oTableStyleInfo.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadTableStyleInfo, pTable->m_oTableStyleInfo.GetPointer());
	}
	else if(c_oSer_TablePart::AltTextTable == type)
	{
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oAltTextTable.Init();

		READ1_DEF(length, res, this->ReadAltTextTable, pOfficeArtExtension->m_oAltTextTable.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri->append(_T("{504A1905-F514-4f6f-8877-14C23A59335A}"));
		pOfficeArtExtension->m_sAdditionalNamespace = _T("xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"");
		pTable->m_oExtLst.Init();
		pTable->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	}
	else if(c_oSer_TablePart::QueryTable == type)
	{
		smart_ptr<OOX::Spreadsheet::CQueryTableFile> pQueryTableFile(new OOX::Spreadsheet::CQueryTableFile(NULL));
		pQueryTableFile->m_oQueryTable.Init();
		READ1_DEF(length, res, this->ReadQueryTableContent, pQueryTableFile->m_oQueryTable.GetPointer());

		smart_ptr<OOX::File> oFile = pQueryTableFile.smart_dynamic_cast<OOX::File>();
		pTableFile->Add(oFile);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadAltTextTable(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CAltTextTable* pAltTextTable = static_cast<OOX::Spreadsheet::CAltTextTable*>(poResult);
	if(c_oSer_AltTextTable::AltText == type)
	{
		pAltTextTable->m_oAltText.Init();
		pAltTextTable->m_oAltText->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_AltTextTable::AltTextSummary == type)
	{
		pAltTextTable->m_oAltTextSummary.Init();
		pAltTextTable->m_oAltTextSummary->append(m_oBufferedStream.GetString3(length));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadAutoFilter(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CAutofilter* pAutofilter = static_cast<OOX::Spreadsheet::CAutofilter*>(poResult);
	if(c_oSer_AutoFilter::Ref == type)
	{
		pAutofilter->m_oRef.Init();
		pAutofilter->m_oRef->SetValue(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_AutoFilter::FilterColumns == type)
	{
		READ1_DEF(length, res, this->ReadFilterColumns, poResult);
	}
	else if(c_oSer_AutoFilter::SortState == type)
	{
		pAutofilter->m_oSortState.Init();
		READ1_DEF(length, res, this->ReadSortState, pAutofilter->m_oSortState.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadFilterColumns(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CAutofilter* pAutofilter = static_cast<OOX::Spreadsheet::CAutofilter*>(poResult);
	if(c_oSer_AutoFilter::FilterColumn == type)
	{
		OOX::Spreadsheet::CFilterColumn* pFilterColumn = new OOX::Spreadsheet::CFilterColumn();
		READ1_DEF(length, res, this->ReadFilterColumn, pFilterColumn);
		pAutofilter->m_arrItems.push_back(pFilterColumn);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadFilterColumnExternal(OOX::Spreadsheet::CFilterColumn* pFilterColumn)
{
	int res = c_oSerConstants::ReadOk;
	ULONG length = m_oBufferedStream.GetULong();
	READ1_DEF(length, res, this->ReadFilterColumn, pFilterColumn);
	return res;
}
int BinaryTableReader::ReadFilterColumn(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CFilterColumn* pFilterColumn = static_cast<OOX::Spreadsheet::CFilterColumn*>(poResult);
	if(c_oSer_FilterColumn::ColId == type)
	{
		pFilterColumn->m_oColId.Init();
		pFilterColumn->m_oColId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_FilterColumn::Filters == type)
	{
		pFilterColumn->m_oFilters.Init();
		READ1_DEF(length, res, this->ReadFilterFilters, pFilterColumn->m_oFilters.GetPointer());
	}
	else if(c_oSer_FilterColumn::CustomFilters == type)
	{
		pFilterColumn->m_oCustomFilters.Init();
		READ1_DEF(length, res, this->ReadCustomFilters, pFilterColumn->m_oCustomFilters.GetPointer());
	}
	else if(c_oSer_FilterColumn::DynamicFilter == type)
	{
		pFilterColumn->m_oDynamicFilter.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadDynamicFilter, pFilterColumn->m_oDynamicFilter.GetPointer());
	}
	else if(c_oSer_FilterColumn::ColorFilter == type)
	{
		pFilterColumn->m_oColorFilter.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColorFilter, pFilterColumn->m_oColorFilter.GetPointer());
	}
	else if(c_oSer_FilterColumn::Top10 == type)
	{
		pFilterColumn->m_oTop10.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadTop10, pFilterColumn->m_oTop10.GetPointer());
	}
	else if(c_oSer_FilterColumn::HiddenButton == type)
	{
		pFilterColumn->m_oHiddenButton.Init();
		pFilterColumn->m_oHiddenButton->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_FilterColumn::ShowButton == type)
	{
		pFilterColumn->m_oShowButton.Init();
		pFilterColumn->m_oShowButton->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadFilterFilters(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CFilters* pFilters = static_cast<OOX::Spreadsheet::CFilters*>(poResult);
	if(c_oSer_FilterColumn::Filter == type)
	{
		OOX::Spreadsheet::CFilter* pFilter = new OOX::Spreadsheet::CFilter();
		READ1_DEF(length, res, this->ReadFilterFilter, pFilter);
		pFilters->m_arrItems.push_back(pFilter);
	}
	else if(c_oSer_FilterColumn::DateGroupItem == type)
	{
		OOX::Spreadsheet::CDateGroupItem* pDateGroupItem = new OOX::Spreadsheet::CDateGroupItem();
		READ2_DEF_SPREADSHEET(length, res, this->ReadDateGroupItem, pDateGroupItem);
		pFilters->m_arrItems.push_back(pDateGroupItem);
	}
	else if(c_oSer_FilterColumn::FiltersBlank == type)
	{
		pFilters->m_oBlank.Init();
		pFilters->m_oBlank->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadFilterFilter(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CFilter* pFilters = static_cast<OOX::Spreadsheet::CFilter*>(poResult);
	if(c_oSer_Filter::Val == type)
	{
		pFilters->m_oVal.Init();
		pFilters->m_oVal->append(m_oBufferedStream.GetString4(length));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadDateGroupItem(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CDateGroupItem* pDateGroupItem = static_cast<OOX::Spreadsheet::CDateGroupItem*>(poResult);
	if(c_oSer_DateGroupItem::DateTimeGrouping == type)
	{
		pDateGroupItem->m_oDateTimeGrouping.Init();
		pDateGroupItem->m_oDateTimeGrouping->SetValue((SimpleTypes::Spreadsheet::EDateTimeGroup)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_DateGroupItem::Day == type)
	{
		pDateGroupItem->m_oDay.Init();
		pDateGroupItem->m_oDay->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DateGroupItem::Hour == type)
	{
		pDateGroupItem->m_oHour.Init();
		pDateGroupItem->m_oHour->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DateGroupItem::Minute == type)
	{
		pDateGroupItem->m_oMinute.Init();
		pDateGroupItem->m_oMinute->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DateGroupItem::Month == type)
	{
		pDateGroupItem->m_oMonth.Init();
		pDateGroupItem->m_oMonth->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DateGroupItem::Second == type)
	{
		pDateGroupItem->m_oSecond.Init();
		pDateGroupItem->m_oSecond->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DateGroupItem::Year == type)
	{
		pDateGroupItem->m_oYear.Init();
		pDateGroupItem->m_oYear->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadCustomFilters(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCustomFilters* pCustomFilters = static_cast<OOX::Spreadsheet::CCustomFilters*>(poResult);
	if(c_oSer_CustomFilters::And == type)
	{
		pCustomFilters->m_oAnd.Init();
		pCustomFilters->m_oAnd->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_CustomFilters::CustomFilters == type)
	{
		READ1_DEF(length, res, this->ReadCustomFilter, poResult);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadCustomFilter(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCustomFilters* pCustomFilters = static_cast<OOX::Spreadsheet::CCustomFilters*>(poResult);
	if(c_oSer_CustomFilters::CustomFilter == type)
	{
		OOX::Spreadsheet::CCustomFilter* pCustomFilter = new OOX::Spreadsheet::CCustomFilter();
		READ2_DEF_SPREADSHEET(length, res, this->ReadCustomFiltersItem, pCustomFilter);
		pCustomFilters->m_arrItems.push_back(pCustomFilter);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadCustomFiltersItem(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCustomFilter* pCustomFilter = static_cast<OOX::Spreadsheet::CCustomFilter*>(poResult);
	if(c_oSer_CustomFilters::Operator == type)
	{
		pCustomFilter->m_oOperator.Init();
		pCustomFilter->m_oOperator->SetValue((SimpleTypes::Spreadsheet::ECustomFilter)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_CustomFilters::Val == type)
	{
		pCustomFilter->m_oVal.Init();
		pCustomFilter->m_oVal->append(m_oBufferedStream.GetString4(length));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadDynamicFilter(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CDynamicFilter* pDynamicFilter = static_cast<OOX::Spreadsheet::CDynamicFilter*>(poResult);
	if(c_oSer_DynamicFilter::Type == type)
	{
		pDynamicFilter->m_oType.Init();
		pDynamicFilter->m_oType->SetValue((SimpleTypes::Spreadsheet::EDynamicFilterType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_DynamicFilter::Val == type)
	{
		pDynamicFilter->m_oVal.Init();
		pDynamicFilter->m_oVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_DynamicFilter::MaxVal == type)
	{
		pDynamicFilter->m_oMaxVal.Init();
		pDynamicFilter->m_oMaxVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadColorFilter(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CColorFilter* pColorFilter = static_cast<OOX::Spreadsheet::CColorFilter*>(poResult);
	if(c_oSer_ColorFilter::CellColor == type)
	{
		pColorFilter->m_oCellColor.Init();
		pColorFilter->m_oCellColor->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ColorFilter::DxfId == type)
	{
		pColorFilter->m_oDxfId.Init();
		pColorFilter->m_oDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadTop10(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTop10* pTop10 = static_cast<OOX::Spreadsheet::CTop10*>(poResult);
	if(c_oSer_Top10::FilterVal == type)
	{
		pTop10->m_oFilterVal.Init();
		pTop10->m_oFilterVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_Top10::Percent == type)
	{
		pTop10->m_oPercent.Init();
		pTop10->m_oPercent->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_Top10::Top == type)
	{
		pTop10->m_oTop.Init();
		pTop10->m_oTop->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_Top10::Val == type)
	{
		pTop10->m_oVal.Init();
		pTop10->m_oVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadSortState(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CSortState* pSortState = static_cast<OOX::Spreadsheet::CSortState*>(poResult);
	if(c_oSer_SortState::Ref == type)
	{
		pSortState->m_oRef.Init();
		pSortState->m_oRef->SetValue(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_SortState::CaseSensitive == type)
	{
		pSortState->m_oCaseSensitive.Init();
		pSortState->m_oCaseSensitive->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SortState::ColumnSort == type)
	{
		pSortState->m_oColumnSort.Init();
		pSortState->m_oColumnSort->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SortState::SortMethod == type)
	{
		pSortState->m_oSortMethod.Init();
		pSortState->m_oSortMethod->SetValue((SimpleTypes::Spreadsheet::ESortMethod)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_SortState::SortConditions == type)
	{
		READ1_DEF(length, res, this->ReadSortConditions, pSortState);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadSortConditions(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CSortState* pSortState = static_cast<OOX::Spreadsheet::CSortState*>(poResult);
	if(c_oSer_SortState::SortCondition == type)
	{
		OOX::Spreadsheet::CSortCondition* pSortCondition = new OOX::Spreadsheet::CSortCondition();
		READ2_DEF_SPREADSHEET(length, res, this->ReadSortCondition, pSortCondition);
		pSortState->m_arrItems.push_back(pSortCondition);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadSortConditionExternal(OOX::Spreadsheet::CSortCondition* pSortCondition)
{
	int res = c_oSerConstants::ReadOk;
	ULONG length = m_oBufferedStream.GetULong();
	READ2_DEF_SPREADSHEET(length, res, this->ReadSortCondition, pSortCondition);
	return res;
}
int BinaryTableReader::ReadSortCondition(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CSortCondition* pSortCondition = static_cast<OOX::Spreadsheet::CSortCondition*>(poResult);
	if(c_oSer_SortState::ConditionRef == type)
	{
		pSortCondition->m_oRef.Init();
		pSortCondition->m_oRef->SetValue(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_SortState::ConditionSortBy == type)
	{
		pSortCondition->m_oSortBy.Init();
		pSortCondition->m_oSortBy->SetValue((SimpleTypes::Spreadsheet::ESortBy)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_SortState::ConditionDescending == type)
	{
		pSortCondition->m_oDescending.Init();
		pSortCondition->m_oDescending->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SortState::ConditionDxfId == type)
	{
		pSortCondition->m_oDxfId.Init();
		pSortCondition->m_oDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadTableColumns(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableColumns* pTableColumns = static_cast<OOX::Spreadsheet::CTableColumns*>(poResult);
	if(c_oSer_TableColumns::TableColumn == type)
	{
		OOX::Spreadsheet::CTableColumn* pTableColumn = new OOX::Spreadsheet::CTableColumn();
		READ1_DEF(length, res, this->ReadTableColumn, pTableColumn);
		
		if (!pTableColumn->m_oId.IsInit())
		{
			pTableColumn->m_oId.Init();
			pTableColumn->m_oId->SetValue((unsigned int)pTableColumns->m_arrItems.size() + 1);
		}
		pTableColumns->m_arrItems.push_back(pTableColumn);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadTableColumn(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableColumn* pTableColumn = static_cast<OOX::Spreadsheet::CTableColumn*>(poResult);
	
	if(c_oSer_TableColumns::Name == type)
	{
		pTableColumn->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::TotalsRowLabel == type)
	{
		pTableColumn->m_oTotalsRowLabel = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::TotalsRowFunction == type)
	{
		pTableColumn->m_oTotalsRowFunction.Init();
		pTableColumn->m_oTotalsRowFunction->SetValue((SimpleTypes::Spreadsheet::ETotalsRowFunction)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_TableColumns::TotalsRowFormula == type)
	{
		pTableColumn->m_oTotalsRowFormula = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::DataDxfId == type)
	{
		pTableColumn->m_oDataDxfId.Init();
		pTableColumn->m_oDataDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableColumns::CalculatedColumnFormula == type)
	{
		pTableColumn->m_oCalculatedColumnFormula = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::DataCellStyle == type)
	{
		pTableColumn->m_oDataCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::HeaderRowCellStyle == type)
	{
		pTableColumn->m_oHeaderRowCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::HeaderRowDxfId == type)
	{
		pTableColumn->m_oHeaderRowDxfId.Init();
		pTableColumn->m_oHeaderRowDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableColumns::Id == type)
	{
		pTableColumn->m_oId.Init();
		pTableColumn->m_oId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableColumns::QueryTableFieldId == type)
	{
		pTableColumn->m_oQueryTableFieldId.Init();
		pTableColumn->m_oQueryTableFieldId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableColumns::TotalsRowCellStyle == type)
	{
		pTableColumn->m_oTotalsRowCellStyle = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_TableColumns::TotalsRowDxfId == type)
	{
		pTableColumn->m_oTotalsRowDxfId.Init();
		pTableColumn->m_oTotalsRowDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableColumns::UniqueName == type)
	{
		pTableColumn->m_oUniqueName = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryTableReader::ReadTableStyleInfo(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyleInfo* pTableStyleInfo = static_cast<OOX::Spreadsheet::CTableStyleInfo*>(poResult);
	if(c_oSer_TableStyleInfo::Name == type)
	{
		pTableStyleInfo->m_oName.Init();
		pTableStyleInfo->m_oName->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_TableStyleInfo::ShowColumnStripes == type)
	{
		pTableStyleInfo->m_oShowColumnStripes.Init();
		pTableStyleInfo->m_oShowColumnStripes->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_TableStyleInfo::ShowRowStripes == type)
	{
		pTableStyleInfo->m_oShowRowStripes.Init();
		pTableStyleInfo->m_oShowRowStripes->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_TableStyleInfo::ShowFirstColumn == type)
	{
		pTableStyleInfo->m_oShowFirstColumn.Init();
		pTableStyleInfo->m_oShowFirstColumn->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_TableStyleInfo::ShowLastColumn == type)
	{
		pTableStyleInfo->m_oShowLastColumn.Init();
		pTableStyleInfo->m_oShowLastColumn->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

BinarySharedStringTableReader::BinarySharedStringTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CSharedStrings& oSharedStrings):Binary_CommonReader(oBufferedStream), m_oSharedStrings(oSharedStrings), m_oBcr(oBufferedStream)
{
}
int BinarySharedStringTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadSharedStringTableContent, this);
	
	m_oSharedStrings.m_oCount.Init();
	m_oSharedStrings.m_oCount->SetValue((unsigned int)m_oSharedStrings.m_nCount);
	
	m_oSharedStrings.m_oUniqueCount.Init();
	m_oSharedStrings.m_oUniqueCount->SetValue((unsigned int)m_oSharedStrings.m_nCount);
	return res;
};
int BinarySharedStringTableReader::ReadSharedStringTableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerSharedStringTypes::Si == type)
	{
		OOX::Spreadsheet::CSi* pSi = new OOX::Spreadsheet::CSi();
		READ1_DEF(length, res, this->ReadSi, pSi);
		m_oSharedStrings.AddSi(pSi);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinarySharedStringTableReader::ReadSi(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSi* pSi = static_cast<OOX::Spreadsheet::CSi*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerSharedStringTypes::Run == type)
	{
		OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
		READ1_DEF(length, res, this->ReadRun, pRun);
		pSi->m_arrItems.push_back(pRun);
	}
	else if(c_oSerSharedStringTypes::Text == type)
	{
		std::wstring sText(m_oBufferedStream.GetString4(length));
		OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
		pText->m_sText = sText;
		if(std::wstring::npos != sText.find(_T(" ")))
		{
			pText->m_oSpace.Init();
			pText->m_oSpace->SetValue(SimpleTypes::xmlspacePreserve);
		}
		pSi->m_arrItems.push_back(pText);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinarySharedStringTableReader::ReadRun(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerSharedStringTypes::RPr == type)
	{
		pRun->m_oRPr.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadRPr, pRun->m_oRPr.GetPointer());
	}
	else if(c_oSerSharedStringTypes::Text == type)
	{
		std::wstring sText(m_oBufferedStream.GetString4(length));
		OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
		pText->m_sText = sText;
		if(std::wstring::npos != sText.find(_T(" ")))
		{
			pText->m_oSpace.Init();
			pText->m_oSpace->SetValue(SimpleTypes::xmlspacePreserve);
		}
		pRun->m_arrItems.push_back(pText);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinarySharedStringTableReader::ReadRPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRPr* pFont = static_cast<OOX::Spreadsheet::CRPr* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFontTypes::Bold == type)
	{
		pFont->m_oBold.Init();
		pFont->m_oBold->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::Color == type)
	{
		pFont->m_oColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pFont->m_oColor.GetPointer());
	}
	else if(c_oSerFontTypes::Italic == type)
	{
		pFont->m_oItalic.Init();
		pFont->m_oItalic->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::RFont == type)
	{
		pFont->m_oRFont.Init();
		pFont->m_oRFont->m_sVal = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerFontTypes::Scheme == type)
	{
		pFont->m_oScheme.Init();
		pFont->m_oScheme->m_oFontScheme.Init();
		pFont->m_oScheme->m_oFontScheme->SetValue((SimpleTypes::Spreadsheet::EFontScheme)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFontTypes::Strike == type)
	{
		pFont->m_oStrike.Init();
		pFont->m_oStrike->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::Sz == type)
	{
		pFont->m_oSz.Init();
		pFont->m_oSz->m_oVal.Init();
		pFont->m_oSz->m_oVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFontTypes::Underline == type)
	{
		pFont->m_oUnderline.Init();
		pFont->m_oUnderline->m_oUnderline.Init();
		pFont->m_oUnderline->m_oUnderline->SetValue((SimpleTypes::Spreadsheet::EUnderline)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFontTypes::VertAlign == type)
	{
		pFont->m_oVertAlign.Init();
		pFont->m_oVertAlign->m_oVerticalAlign.Init();
		pFont->m_oVertAlign->m_oVerticalAlign->SetValue((SimpleTypes::EVerticalAlignRun)m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinarySharedStringTableReader::ReadColor(BYTE type, long length, void* poResult)
{
	return m_oBcr.ReadColor(type, length, poResult);
}

BinaryStyleTableReader::BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CStyles& oStyles):Binary_CommonReader(oBufferedStream), m_oStyles(oStyles), m_oBcr(oBufferedStream)
{
}
int BinaryStyleTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadStyleTableContent, this);
	return res;
};
int BinaryStyleTableReader::ReadStyleTableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Borders == type)
	{
		m_oStyles.m_oBorders.Init();
		READ1_DEF(length, res, this->ReadBorders, poResult);
		
		m_oStyles.m_oBorders->m_oCount.Init();
		m_oStyles.m_oBorders->m_oCount->SetValue((unsigned int)m_oStyles.m_oBorders->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::Fills == type)
	{
		m_oStyles.m_oFills.Init();
		READ1_DEF(length, res, this->ReadFills, poResult);
		
		m_oStyles.m_oFills->m_oCount.Init();
		m_oStyles.m_oFills->m_oCount->SetValue((unsigned int)m_oStyles.m_oFills->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::Fonts == type)
	{
		m_oStyles.m_oFonts.Init();
		READ1_DEF(length, res, this->ReadFonts, poResult);
		
		m_oStyles.m_oFonts->m_oCount.Init();
		m_oStyles.m_oFonts->m_oCount->SetValue((unsigned int)m_oStyles.m_oFonts->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::NumFmts == type)
	{
		m_oStyles.m_oNumFmts.Init();
		READ1_DEF(length, res, this->ReadNumFmts, poResult);
		
		m_oStyles.m_oNumFmts->m_oCount.Init();
		m_oStyles.m_oNumFmts->m_oCount->SetValue((unsigned int)m_oStyles.m_oNumFmts->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::CellStyleXfs == type)
	{
		m_oStyles.m_oCellStyleXfs.Init();
		READ1_DEF(length, res, this->ReadCellStyleXfs, poResult);
		
		m_oStyles.m_oCellStyleXfs->m_oCount.Init();
		m_oStyles.m_oCellStyleXfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellStyleXfs->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::CellXfs == type)
	{
		m_oStyles.m_oCellXfs.Init();
		READ1_DEF(length, res, this->ReadCellXfs, poResult);
		
		m_oStyles.m_oCellXfs->m_oCount.Init();
		m_oStyles.m_oCellXfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellXfs->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::CellStyles == type)
	{
		m_oStyles.m_oCellStyles.Init();
		READ1_DEF(length, res, this->ReadCellStyles, poResult);
		
		m_oStyles.m_oCellStyles->m_oCount.Init();
		m_oStyles.m_oCellStyles->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellStyles->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::Dxfs == type)
	{
		m_oStyles.m_oDxfs.Init();
		READ1_DEF(length, res, this->ReadDxfs, m_oStyles.m_oDxfs.GetPointer());
		
		m_oStyles.m_oDxfs->m_oCount.Init();
		m_oStyles.m_oDxfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oDxfs->m_arrItems.size());
	}
	else if(c_oSerStylesTypes::TableStyles == type)
	{
		m_oStyles.m_oTableStyles.Init();
		READ1_DEF(length, res, this->ReadTableStyles, m_oStyles.m_oTableStyles.GetPointer());
		if(false == m_oStyles.m_oTableStyles->m_oCount.IsInit())
		{
			m_oStyles.m_oTableStyles->m_oCount.Init();
			m_oStyles.m_oTableStyles->m_oCount->SetValue(0);
		}
	}
	else if(c_oSerStylesTypes::ExtDxfs == type)
	{
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();

		pOfficeArtExtension->m_oDxfs.Init();
		READ1_DEF(length, res, this->ReadDxfs, pOfficeArtExtension->m_oDxfs.GetPointer());

		pOfficeArtExtension->m_oDxfs->m_oCount.Init();
		pOfficeArtExtension->m_oDxfs->m_oCount->SetValue((unsigned int)pOfficeArtExtension->m_oDxfs->m_arrItems.size());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{46F421CA-312F-682f-3DD2-61675219B42D}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_oStyles.m_oExtLst.IsInit() == false)
			m_oStyles.m_oExtLst.Init();
		m_oStyles.m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	}
	else if(c_oSerStylesTypes::SlicerStyles == type)
	{
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSlicerStyles.Init();

		m_oBufferedStream.GetUChar();//type
		pOfficeArtExtension->m_oSlicerStyles->fromPPTY(&m_oBufferedStream);

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{EB79DEF2-80B8-43e5-95BD-54CBDDF9020C}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_oStyles.m_oExtLst.IsInit() == false)
			m_oStyles.m_oExtLst.Init();
		m_oStyles.m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadBorders(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Border == type)
	{
		OOX::Spreadsheet::CBorder* pBorder = new OOX::Spreadsheet::CBorder();
		READ1_DEF(length, res, this->ReadBorder, pBorder);
		m_oStyles.m_oBorders->m_arrItems.push_back(pBorder);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadBorder(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CBorder* pBorder = static_cast<OOX::Spreadsheet::CBorder*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerBorderTypes::Bottom == type)
	{
		pBorder->m_oBottom.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oBottom.GetPointer());
	}
	else if(c_oSerBorderTypes::Diagonal == type)
	{
		pBorder->m_oDiagonal.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oDiagonal.GetPointer());
	}
	else if(c_oSerBorderTypes::End == type)
	{
		pBorder->m_oEnd.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oEnd.GetPointer());
	}
	else if(c_oSerBorderTypes::Horizontal == type)
	{
		pBorder->m_oHorizontal.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oHorizontal.GetPointer());
	}
	else if(c_oSerBorderTypes::Start == type)
	{
		pBorder->m_oStart.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oStart.GetPointer());
	}
	else if(c_oSerBorderTypes::Top == type)
	{
		pBorder->m_oTop.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oTop.GetPointer());
	}
	else if(c_oSerBorderTypes::Vertical == type)
	{
		pBorder->m_oVertical.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadBorderProp, pBorder->m_oVertical.GetPointer());
	}
	else if(c_oSerBorderTypes::DiagonalDown == type)
	{
		pBorder->m_oDiagonalDown.Init();
		bool bDD = m_oBufferedStream.GetBool();
		pBorder->m_oDiagonalDown->SetValue((false != bDD) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerBorderTypes::DiagonalUp == type)
	{
		pBorder->m_oDiagonalUp.Init();
		bool bDU = m_oBufferedStream.GetBool();
		pBorder->m_oDiagonalUp->SetValue((false != bDU) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadBorderProp(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CBorderProp* pBorderProp = static_cast<OOX::Spreadsheet::CBorderProp*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerBorderPropTypes::Style == type)
	{
		pBorderProp->m_oStyle.Init();
		pBorderProp->m_oStyle->SetValue((SimpleTypes::Spreadsheet::EBorderStyle)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerBorderPropTypes::Color == type)
	{
		pBorderProp->m_oColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pBorderProp->m_oColor.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadColor(BYTE type, long length, void* poResult)
{
	return m_oBcr.ReadColor(type, length, poResult);
}
int BinaryStyleTableReader::ReadFills(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Fill == type)
	{
		OOX::Spreadsheet::CFill* pFill = new OOX::Spreadsheet::CFill();
		READ1_DEF(length, res, this->ReadFill, pFill);
		m_oStyles.m_oFills->m_arrItems.push_back(pFill);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadFill(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CFill* pFill = static_cast<OOX::Spreadsheet::CFill* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFillTypes::Pattern == type)
	{
		pFill->m_oPatternFill.Init();
		READ1_DEF(length, res, this->ReadPatternFill, pFill->m_oPatternFill.GetPointer());
	}
	else if(c_oSerFillTypes::Gradient == type)
	{
		pFill->m_oGradientFill.Init();
		READ1_DEF(length, res, this->ReadGradientFill, pFill->m_oGradientFill.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadPatternFill(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPatternFill* pPatternFill = static_cast<OOX::Spreadsheet::CPatternFill* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFillTypes::PatternBgColor_deprecated == type)
	{
		pPatternFill->m_oPatternType.Init();
		pPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeSolid);

		LONG colorPos = m_oBufferedStream.GetPos();
		pPatternFill->m_oFgColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pPatternFill->m_oFgColor.GetPointer());
		//todo copy
		m_oBufferedStream.Seek(colorPos);
		pPatternFill->m_oBgColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pPatternFill->m_oBgColor.GetPointer());
	}
	else if(c_oSerFillTypes::PatternType == type)
	{
		pPatternFill->m_oPatternType.Init();
		pPatternFill->m_oPatternType->SetValue((SimpleTypes::Spreadsheet::EPatternType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFillTypes::PatternFgColor == type)
	{
		pPatternFill->m_oFgColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pPatternFill->m_oFgColor.GetPointer());
	}
	else if(c_oSerFillTypes::PatternBgColor == type)
	{
		pPatternFill->m_oBgColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pPatternFill->m_oBgColor.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadGradientFill(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CGradientFill* pGradientFill = static_cast<OOX::Spreadsheet::CGradientFill* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFillTypes::GradientType == type)
	{
		pGradientFill->m_oType.Init();
		pGradientFill->m_oType->SetValue((SimpleTypes::Spreadsheet::EGradientType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFillTypes::GradientLeft == type)
	{
		pGradientFill->m_oLeft.Init();
		pGradientFill->m_oLeft->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientTop == type)
	{
		pGradientFill->m_oTop.Init();
		pGradientFill->m_oTop->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientRight == type)
	{
		pGradientFill->m_oRight.Init();
		pGradientFill->m_oRight->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientBottom == type)
	{
		pGradientFill->m_oBottom.Init();
		pGradientFill->m_oBottom->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientDegree == type)
	{
		pGradientFill->m_oDegree.Init();
		pGradientFill->m_oDegree->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientStop == type)
	{
		OOX::Spreadsheet::CGradientStop* pGradientStop = new OOX::Spreadsheet::CGradientStop();
		READ1_DEF(length, res, this->ReadGradientFillStop, pGradientStop);
		pGradientFill->m_arrItems.push_back(pGradientStop);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadGradientFillStop(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CGradientStop* pGradientStop = static_cast<OOX::Spreadsheet::CGradientStop* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFillTypes::GradientStopPosition == type)
	{
		pGradientStop->m_oPosition.Init();
		pGradientStop->m_oPosition->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFillTypes::GradientStopColor == type)
	{
		pGradientStop->m_oColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pGradientStop->m_oColor.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadFonts(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Font == type)
	{
		OOX::Spreadsheet::CFont* pFont = new OOX::Spreadsheet::CFont();
		READ2_DEF_SPREADSHEET(length, res, this->ReadFont, pFont);
		m_oStyles.m_oFonts->m_arrItems.push_back(pFont);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadFont(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CFont* pFont = static_cast<OOX::Spreadsheet::CFont* >(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFontTypes::Bold == type)
	{
		pFont->m_oBold.Init();
		pFont->m_oBold->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::Color == type)
	{
		pFont->m_oColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pFont->m_oColor.GetPointer());
	}
	else if(c_oSerFontTypes::Italic == type)
	{
		pFont->m_oItalic.Init();
		pFont->m_oItalic->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::RFont == type)
	{
		std::wstring sFontName(m_oBufferedStream.GetString4(length));
		pFont->m_oRFont.Init();
		pFont->m_oRFont->m_sVal = sFontName;
	}
	else if(c_oSerFontTypes::Scheme == type)
	{
		pFont->m_oScheme.Init();
		pFont->m_oScheme->m_oFontScheme.Init();
		pFont->m_oScheme->m_oFontScheme->SetValue((SimpleTypes::Spreadsheet::EFontScheme)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFontTypes::Strike == type)
	{
		pFont->m_oStrike.Init();
		pFont->m_oStrike->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFontTypes::Sz == type)
	{
		pFont->m_oSz.Init();
		pFont->m_oSz->m_oVal.Init();
		pFont->m_oSz->m_oVal->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerFontTypes::Underline == type)
	{
		pFont->m_oUnderline.Init();
		pFont->m_oUnderline->m_oUnderline.Init();
		pFont->m_oUnderline->m_oUnderline->SetValue((SimpleTypes::Spreadsheet::EUnderline)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFontTypes::VertAlign == type)
	{
		pFont->m_oVertAlign.Init();
		pFont->m_oVertAlign->m_oVerticalAlign.Init();
		pFont->m_oVertAlign->m_oVerticalAlign->SetValue((SimpleTypes::EVerticalAlignRun)m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadNumFmts(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::NumFmt == type)
	{
		OOX::Spreadsheet::CNumFmt* pNumFmt = new OOX::Spreadsheet::CNumFmt();
		READ2_DEF_SPREADSHEET(length, res, this->ReadNumFmt, pNumFmt);
		m_oStyles.m_oNumFmts->m_arrItems.push_back(pNumFmt);
		
		if (pNumFmt->m_oNumFmtId.IsInit())
		{
			m_oStyles.m_oNumFmts->m_mapNumFmtIndex.insert(std::make_pair(pNumFmt->m_oNumFmtId->GetValue(), m_oStyles.m_oNumFmts->m_arrItems.size() - 1));
		}

	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadNumFmt(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CNumFmt* pNumFmt = static_cast<OOX::Spreadsheet::CNumFmt*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerNumFmtTypes::FormatCode == type)
	{
		pNumFmt->m_oFormatCode = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerNumFmtTypes::FormatCode16 == type)
	{
		pNumFmt->m_oFormatCode16 = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerNumFmtTypes::NumFmtId == type)
	{
		pNumFmt->m_oNumFmtId.Init();
		pNumFmt->m_oNumFmtId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadCellStyleXfs(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Xfs == type)
	{
		OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
		READ2_DEF_SPREADSHEET(length, res, this->ReadXfs, pXfs);
		m_oStyles.m_oCellStyleXfs->m_arrItems.push_back(pXfs);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadCellXfs(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::Xfs == type)
	{
		OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
		READ2_DEF_SPREADSHEET(length, res, this->ReadXfs, pXfs);
		m_oStyles.m_oCellXfs->m_arrItems.push_back(pXfs);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadXfs(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CXfs* pXfs = static_cast<OOX::Spreadsheet::CXfs*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerXfsTypes::ApplyAlignment == type)
	{
		pXfs->m_oApplyAlignment.Init();
		pXfs->m_oApplyAlignment->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::ApplyBorder == type)
	{
		pXfs->m_oApplyBorder.Init();
		pXfs->m_oApplyBorder->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::ApplyFill == type)
	{
		pXfs->m_oApplyFill.Init();
		pXfs->m_oApplyFill->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::ApplyFont == type)
	{
		pXfs->m_oApplyFont.Init();
		pXfs->m_oApplyFont->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::ApplyNumberFormat == type)
	{
		pXfs->m_oApplyNumberFormat.Init();
		pXfs->m_oApplyNumberFormat->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if (c_oSerXfsTypes::ApplyProtection == type)
	{
		pXfs->m_oApplyProtection.Init();
		pXfs->m_oApplyProtection->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::BorderId == type)
	{
		pXfs->m_oBorderId.Init();
		pXfs->m_oBorderId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerXfsTypes::FillId == type)
	{
		pXfs->m_oFillId.Init();
		pXfs->m_oFillId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerXfsTypes::FontId == type)
	{
		pXfs->m_oFontId.Init();
		pXfs->m_oFontId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerXfsTypes::NumFmtId == type)
	{
		pXfs->m_oNumFmtId.Init();
		pXfs->m_oNumFmtId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerXfsTypes::QuotePrefix == type)
	{
		pXfs->m_oQuotePrefix.Init();
		pXfs->m_oQuotePrefix->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::PivotButton == type)
	{
		pXfs->m_oPivotButton.Init();
		pXfs->m_oPivotButton->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerXfsTypes::Aligment == type)
	{
		pXfs->m_oAligment.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadAligment, pXfs->m_oAligment.GetPointer());
	}
	else if (c_oSerXfsTypes::Protection == type)
	{
		pXfs->m_oProtection.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadProtection, pXfs->m_oProtection.GetPointer());
	}
	else if (c_oSerXfsTypes::XfId == type)
	{
		pXfs->m_oXfId.Init();
		pXfs->m_oXfId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadProtection(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CProtection* pProtection = static_cast<OOX::Spreadsheet::CProtection*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if (c_oSerProtectionTypes::Hidden == type)
	{
		pProtection->m_oHidden.Init();
		pProtection->m_oHidden->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if (c_oSerProtectionTypes::Locked == type)
	{
		pProtection->m_oLocked.Init();
		pProtection->m_oLocked->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else
		res = c_oSerConstants::ReadUnknown;
		return res;
}
int BinaryStyleTableReader::ReadAligment(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CAligment* pAligment = static_cast<OOX::Spreadsheet::CAligment*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerAligmentTypes::Horizontal == type)
	{
		pAligment->m_oHorizontal.Init();
		pAligment->m_oHorizontal->SetValue((SimpleTypes::Spreadsheet::EHorizontalAlignment)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerAligmentTypes::Indent == type)
	{
		pAligment->m_oIndent = m_oBufferedStream.GetLong();
	}
	else if(c_oSerAligmentTypes::RelativeIndent == type)
	{
		pAligment->m_oRelativeIndent = m_oBufferedStream.GetLong();
	}
	else if(c_oSerAligmentTypes::ShrinkToFit == type)
	{
		pAligment->m_oShrinkToFit.Init();
		pAligment->m_oShrinkToFit->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerAligmentTypes::TextRotation == type)
	{
		pAligment->m_oTextRotation = m_oBufferedStream.GetLong();
	}
	else if(c_oSerAligmentTypes::Vertical == type)
	{
		pAligment->m_oVertical.Init();
		pAligment->m_oVertical->SetValue((SimpleTypes::Spreadsheet::EVerticalAlignment)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerAligmentTypes::WrapText == type)
	{
		pAligment->m_oWrapText.Init();
		pAligment->m_oWrapText->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
};
int BinaryStyleTableReader::ReadDxfs(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CDxfs* pDxfs = static_cast<OOX::Spreadsheet::CDxfs*>(poResult);
	if(c_oSerStylesTypes::Dxf == type)
	{
		OOX::Spreadsheet::CDxf* pDxf = new OOX::Spreadsheet::CDxf();
		READ1_DEF(length, res, this->ReadDxf, pDxf);
		pDxfs->m_arrItems.push_back(pDxf);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadDxfExternal(OOX::Spreadsheet::CDxf* pDxf)
{
	int res = c_oSerConstants::ReadOk;
	ULONG length = m_oBufferedStream.GetULong();
	READ1_DEF(length, res, this->ReadDxf, pDxf);
	return res;
}
int BinaryStyleTableReader::ReadDxf(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CDxf* pDxf = static_cast<OOX::Spreadsheet::CDxf*>(poResult);
	if(c_oSer_Dxf::Alignment == type)
	{
		pDxf->m_oAlignment.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadAligment, pDxf->m_oAlignment.GetPointer());
	}
	else if(c_oSer_Dxf::Border == type)
	{
		pDxf->m_oBorder.Init();
		READ1_DEF(length, res, this->ReadBorder, pDxf->m_oBorder.GetPointer());
	}
	else if(c_oSer_Dxf::Fill == type)
	{
		pDxf->m_oFill.Init();
		READ1_DEF(length, res, this->ReadFill, pDxf->m_oFill.GetPointer());
	}
	else if(c_oSer_Dxf::Font == type)
	{
		pDxf->m_oFont.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadFont, pDxf->m_oFont.GetPointer());
	}
	else if(c_oSer_Dxf::NumFmt == type)
	{
		pDxf->m_oNumFmt.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadNumFmt, pDxf->m_oNumFmt.GetPointer());
		//todooo в m_mapNumFmtIndex
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadCellStyles(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerStylesTypes::CellStyle == type)
	{
		OOX::Spreadsheet::CCellStyle* pCellStyle = new OOX::Spreadsheet::CCellStyle();
		READ1_DEF(length, res, this->ReadCellStyle, pCellStyle);
		m_oStyles.m_oCellStyles->m_arrItems.push_back(pCellStyle);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadCellStyle(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCellStyle* pCellStyle = static_cast<OOX::Spreadsheet::CCellStyle*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_CellStyle::BuiltinId == type)
	{
		pCellStyle->m_oBuiltinId.Init();
		pCellStyle->m_oBuiltinId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_CellStyle::CustomBuiltin == type)
	{
		pCellStyle->m_oCustomBuiltin.Init();
		pCellStyle->m_oCustomBuiltin->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_CellStyle::Hidden == type)
	{
		pCellStyle->m_oHidden.Init();
		pCellStyle->m_oHidden->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_CellStyle::ILevel == type)
	{
		pCellStyle->m_oILevel.Init();
		pCellStyle->m_oILevel->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_CellStyle::Name == type)
	{
		pCellStyle->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_CellStyle::XfId == type)
	{
		pCellStyle->m_oXfId.Init();
		pCellStyle->m_oXfId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadTableStyles(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyles* pTableStyles = static_cast<OOX::Spreadsheet::CTableStyles*>(poResult);
	if(c_oSer_TableStyles::DefaultTableStyle == type)
	{
		pTableStyles->m_oDefaultTableStyle.Init();
		pTableStyles->m_oDefaultTableStyle->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_TableStyles::DefaultPivotStyle == type)
	{
		pTableStyles->m_oDefaultPivotStyle.Init();
		pTableStyles->m_oDefaultPivotStyle->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_TableStyles::TableStyles == type)
	{
		READ1_DEF(length, res, this->ReadTableCustomStyles, pTableStyles);
		
		pTableStyles->m_oCount.Init();
		pTableStyles->m_oCount->SetValue((unsigned int)pTableStyles->m_arrItems.size());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadTableCustomStyles(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyles* pTableStyles = static_cast<OOX::Spreadsheet::CTableStyles*>(poResult);
	if(c_oSer_TableStyles::TableStyle == type)
	{
		OOX::Spreadsheet::CTableStyle* pTableStyle = new OOX::Spreadsheet::CTableStyle();
		READ1_DEF(length, res, this->ReadTableCustomStyle, pTableStyle);
		pTableStyles->m_arrItems.push_back(pTableStyle);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadTableCustomStyle(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyle* pTableStyle = static_cast<OOX::Spreadsheet::CTableStyle*>(poResult);
	if(c_oSer_TableStyle::Name == type)
	{
		pTableStyle->m_oName.Init();
		pTableStyle->m_oName->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_TableStyle::Pivot == type)
	{
		pTableStyle->m_oPivot.Init();
		pTableStyle->m_oPivot->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_TableStyle::Table == type)
	{
		pTableStyle->m_oTable.Init();
		pTableStyle->m_oTable->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_TableStyle::Elements == type)
	{
		READ1_DEF(length, res, this->ReadTableCustomStyleElements, pTableStyle);
		
		pTableStyle->m_oCount.Init();
		pTableStyle->m_oCount->SetValue((unsigned int)pTableStyle->m_arrItems.size());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadTableCustomStyleElements(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyle* pTableStyle = static_cast<OOX::Spreadsheet::CTableStyle*>(poResult);
	if(c_oSer_TableStyle::Element == type)
	{
		OOX::Spreadsheet::CTableStyleElement* pTableStyleElement = new OOX::Spreadsheet::CTableStyleElement();
		READ2_DEF_SPREADSHEET(length, res, this->ReadTableCustomStyleElement, pTableStyleElement);
		pTableStyle->m_arrItems.push_back(pTableStyleElement);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryStyleTableReader::ReadTableCustomStyleElement(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CTableStyleElement* pTableStyleElement = static_cast<OOX::Spreadsheet::CTableStyleElement*>(poResult);
	if(c_oSer_TableStyleElement::Type == type)
	{
		pTableStyleElement->m_oType.Init();
		pTableStyleElement->m_oType->SetValue((SimpleTypes::Spreadsheet::ETableStyleType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_TableStyleElement::Size == type)
	{
		pTableStyleElement->m_oSize.Init();
		pTableStyleElement->m_oSize->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_TableStyleElement::DxfId == type)
	{
		pTableStyleElement->m_oDxfId.Init();
		pTableStyleElement->m_oDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

BinaryWorkbookTableReader::BinaryWorkbookTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook, boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>& mapPivotCacheDefinitions, const std::wstring& sDestinationDir, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
	: Binary_CommonReader(oBufferedStream), m_oWorkbook(oWorkbook), m_mapPivotCacheDefinitions(mapPivotCacheDefinitions), m_sDestinationDir(sDestinationDir), m_pOfficeDrawingConverter(pOfficeDrawingConverter)
{
}
int BinaryWorkbookTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadWorkbookTableContent, this);

	if (!m_bMacroRead)
		m_oWorkbook.m_bMacroEnabled = false;
	return res;
}
int BinaryWorkbookTableReader::ReadWorkbookTableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookTypes::WorkbookPr == type)
	{
		m_oWorkbook.m_oWorkbookPr.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadWorkbookPr, poResult);
	}
	else if (c_oSerWorkbookTypes::Protection == type)
	{
		m_oWorkbook.m_oWorkbookProtection.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadProtection, poResult);
	}
	else if(c_oSerWorkbookTypes::BookViews == type)
	{
		m_oWorkbook.m_oBookViews.Init();
		READ1_DEF(length, res, this->ReadBookViews, poResult);
	}
	else if(c_oSerWorkbookTypes::DefinedNames == type)
	{
		m_oWorkbook.m_oDefinedNames.Init();
		READ1_DEF(length, res, this->ReadDefinedNames, poResult);
	}
	else if(c_oSerWorkbookTypes::CalcPr == type)
	{
		m_oWorkbook.m_oCalcPr.Init();
		READ1_DEF(length, res, this->ReadCalcPr, m_oWorkbook.m_oCalcPr.GetPointer());
	}
	else if(c_oSerWorkbookTypes::ExternalReferences == type)
	{
		m_oWorkbook.m_oExternalReferences.Init();
		READ1_DEF(length, res, this->ReadExternalReferences, poResult);
	}
	else if(c_oSerWorkbookTypes::PivotCaches == type)
	{
		m_oWorkbook.m_oPivotCachesXml.Init();
		m_oWorkbook.m_oPivotCachesXml->append(L"<pivotCaches>");
		READ1_DEF(length, res, this->ReadPivotCaches, poResult);
		m_oWorkbook.m_oPivotCachesXml->append(L"</pivotCaches>");
	}
	else if (c_oSerWorkbookTypes::AppName == type)
	{
		m_oWorkbook.m_oAppName = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorkbookTypes::OleSize == type)
	{
		m_oWorkbook.m_oOleSize = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerWorkbookTypes::VbaProject == type)
	{
		m_bMacroRead = true;
		m_oBufferedStream.Skip(1); //skip type

		if (m_oWorkbook.m_bMacroEnabled)
		{
			smart_ptr<OOX::VbaProject> oFileVbaProject(new OOX::VbaProject(NULL));

			oFileVbaProject->fromPPTY(&m_oBufferedStream);

			smart_ptr<OOX::File> oFile = oFileVbaProject.smart_dynamic_cast<OOX::File>();
			const OOX::RId oRId = m_oWorkbook.Add(oFile);
		}
		else
		{
			m_oBufferedStream.SkipRecord();
		}
    }
	else if(c_oSerWorkbookTypes::JsaProject == type)
	{
		BYTE* pData = m_oBufferedStream.GetPointer(length);
		OOX::CPath oJsaProject = OOX::FileTypes::JsaProject.DefaultFileName();
		std::wstring filePath = m_sDestinationDir  + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + oJsaProject.GetPath();

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath);
		oFile.WriteFile(pData, length);
		oFile.CloseFile();

		smart_ptr<OOX::JsaProject> oFileJsaProject(new OOX::JsaProject(NULL));
		smart_ptr<OOX::File> oFileJsaProjectFile = oFileJsaProject.smart_dynamic_cast<OOX::File>();
		m_oWorkbook.Add(oFileJsaProjectFile);
		m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(oJsaProject.GetExtention(false));
	}
	else if(c_oSerWorkbookTypes::Comments == type)
	{
		BYTE* pData = m_oBufferedStream.GetPointer(length);
		OOX::CPath oWorkbookComments = OOX::Spreadsheet::FileTypes::WorkbookComments.DefaultFileName();
		std::wstring filePath = m_sDestinationDir  + FILE_SEPARATOR_STR + _T("xl") + FILE_SEPARATOR_STR + oWorkbookComments.GetPath();

		NSFile::CFileBinary oFile;
		oFile.CreateFileW(filePath);
		oFile.WriteFile(pData, length);
		oFile.CloseFile();

		smart_ptr<OOX::Spreadsheet::WorkbookComments> oFileWorkbookComments(new OOX::Spreadsheet::WorkbookComments(NULL));
		smart_ptr<OOX::File> oFileWorkbookCommentsFile = oFileWorkbookComments.smart_dynamic_cast<OOX::File>();
		m_oWorkbook.Add(oFileWorkbookCommentsFile);
		m_pOfficeDrawingConverter->m_pImageManager->m_pContentTypes->AddDefault(oWorkbookComments.GetExtention(false));
	}
	else if(c_oSerWorkbookTypes::Connections == type)
	{
		smart_ptr<OOX::Spreadsheet::CConnectionsFile> oConnection(new OOX::Spreadsheet::CConnectionsFile(NULL));
		oConnection->m_oConnections.Init();
		READ1_DEF(length, res, this->ReadConnections, oConnection->m_oConnections.GetPointer());

		smart_ptr<OOX::File> oFile = oConnection.smart_dynamic_cast<OOX::File>();
		m_oWorkbook.Add(oFile);
	}
	else if(c_oSerWorkbookTypes::SlicerCaches == type)
	{
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSlicerCaches.Init();

		READ1_DEF(length, res, this->ReadSlicerCaches, pOfficeArtExtension->m_oSlicerCaches.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{BBE1A952-AA13-448e-AADC-164F8A28A991}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_oWorkbook.m_oExtLst.IsInit() == false)
			m_oWorkbook.m_oExtLst.Init();
		m_oWorkbook.m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	}
	else if(c_oSerWorkbookTypes::SlicerCachesExt == type)
	{
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSlicerCachesExt.Init();

		READ1_DEF(length, res, this->ReadSlicerCaches, pOfficeArtExtension->m_oSlicerCachesExt.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{46BE6895-7355-4a93-B00E-2C351335B9C9}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"";

		if (m_oWorkbook.m_oExtLst.IsInit() == false)
			m_oWorkbook.m_oExtLst.Init();
		m_oWorkbook.m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnections(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConnections* pConnections = static_cast<OOX::Spreadsheet::CConnections*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerConnectionsTypes::Connection == type)
	{
		OOX::Spreadsheet::CConnection* pConnection = new OOX::Spreadsheet::CConnection();
		READ1_DEF(length, res, this->ReadConnection, pConnection);
		pConnections->m_arrItems.push_back(pConnection);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnection(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConnection* pConnection = static_cast<OOX::Spreadsheet::CConnection*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerConnectionsTypes::Type == type)
	{
		pConnection->m_oType = m_oBufferedStream.GetLong();
	}
	else if(c_oSerConnectionsTypes::Name == type)
	{
		pConnection->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::Id == type)
	{
		pConnection->m_oId.Init();
		pConnection->m_oId->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSerConnectionsTypes::UId == type)
	{
		pConnection->m_oUId = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::Credentials == type)
	{
		pConnection->m_oCredentials.Init();
		pConnection->m_oCredentials->SetValue((SimpleTypes::Spreadsheet::ECredMethod)m_oBufferedStream.GetLong());
	}
	else if(c_oSerConnectionsTypes::Background == type)
	{
		pConnection->m_oBackground = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::Deleted == type)
	{
		pConnection->m_oDeleted = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::Description == type)
	{
		pConnection->m_oDescription = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::Interval == type)
	{
		pConnection->m_oInterval = m_oBufferedStream.GetLong();
	}
	else if(c_oSerConnectionsTypes::KeepAlive == type)
	{
		pConnection->m_oKeepAlive = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::MinRefreshableVersion == type)
	{
		pConnection->m_oMinRefreshableVersion = m_oBufferedStream.GetLong();
	}
	else if(c_oSerConnectionsTypes::New == type)
	{
		pConnection->m_oNew = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::OdcFile == type)
	{
		pConnection->m_oOdcFile = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::OnlyUseConnectionFile == type)
	{
		pConnection->m_oOnlyUseConnectionFile = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::ReconnectionMethod == type)
	{
		pConnection->m_oReconnectionMethod = m_oBufferedStream.GetLong();
	}
	else if(c_oSerConnectionsTypes::RefreshedVersion == type)
	{
		pConnection->m_oRefreshedVersion = m_oBufferedStream.GetLong();
	}
	else if(c_oSerConnectionsTypes::RefreshOnLoad == type)
	{
		pConnection->m_oRefreshOnLoad = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::SaveData == type)
	{
		pConnection->m_oSaveData = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::SavePassword == type)
	{
		pConnection->m_oSavePassword = m_oBufferedStream.GetBool();
	}
	else if(c_oSerConnectionsTypes::SingleSignOnId == type)
	{
		pConnection->m_oSingleSignOnId = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::SourceFile == type)
	{
		pConnection->m_oSourceFile = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::DbPr == type)
	{
		pConnection->m_oDbPr.Init();
		READ1_DEF(length, res, this->ReadConnectionDbPr, pConnection->m_oDbPr.GetPointer());
	}
	else if(c_oSerConnectionsTypes::OlapPr == type)
	{
		pConnection->m_oOlapPr.Init();
		READ1_DEF(length, res, this->ReadConnectionOlapPr, pConnection->m_oOlapPr.GetPointer());
	}
	else if(c_oSerConnectionsTypes::TextPr == type)
	{
		pConnection->m_oTextPr.Init();
		READ1_DEF(length, res, this->ReadConnectionTextPr, pConnection->m_oTextPr.GetPointer());
	}
	else if(c_oSerConnectionsTypes::WebPr == type)
	{
		pConnection->m_oWebPr.Init();
		READ1_DEF(length, res, this->ReadConnectionWebPr, pConnection->m_oWebPr.GetPointer());
	}
	else if(c_oSerConnectionsTypes::IdExt == type)
	{
		pConnection->m_oIdExt = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerConnectionsTypes::RangePr == type)
	{
		pConnection->m_oRangePr.Init();
		READ1_DEF(length, res, this->ReadConnectionRangePr, pConnection->m_oRangePr.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionDbPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDbPr* pDbPr = static_cast<OOX::Spreadsheet::CDbPr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerDbPrTypes::CommandType == type)
	{
		pDbPr->m_oCommandType = m_oBufferedStream.GetLong();
	}
	else if(c_oSerDbPrTypes::Connection == type)
	{
		pDbPr->m_oConnection = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerDbPrTypes::Command == type)
	{
		pDbPr->m_oCommand = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerDbPrTypes::ServerCommand == type)
	{
		pDbPr->m_oServerCommand = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionOlapPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::COlapPr* pOlapPr = static_cast<OOX::Spreadsheet::COlapPr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerOlapPrTypes::RowDrillCount == type)
	{
		pOlapPr->m_oRowDrillCount = m_oBufferedStream.GetLong();
	}
	else if(c_oSerOlapPrTypes::LocalConnection == type)
	{
		pOlapPr->m_oLocalConnection = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerOlapPrTypes::Local == type)
	{
		pOlapPr->m_oLocal = m_oBufferedStream.GetBool();
	}
	else if(c_oSerOlapPrTypes::LocalRefresh == type)
	{
		pOlapPr->m_oLocalRefresh = m_oBufferedStream.GetBool();
	}
	else if(c_oSerOlapPrTypes::SendLocale == type)
	{
		pOlapPr->m_oSendLocale = m_oBufferedStream.GetBool();
	}
	else if(c_oSerOlapPrTypes::ServerNumberFormat == type)
	{
		pOlapPr->m_oServerNumberFormat = m_oBufferedStream.GetBool();
	}
	else if(c_oSerOlapPrTypes::ServerFont == type)
	{
		pOlapPr->m_oServerFont = m_oBufferedStream.GetBool();
	}
	else if(c_oSerOlapPrTypes::ServerFontColor == type)
	{
		pOlapPr->m_oServerFontColor = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionRangePr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRangePr* pRangePr = static_cast<OOX::Spreadsheet::CRangePr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerRangePrTypes::SourceName == type)
	{
		pRangePr->m_oSourceName = m_oBufferedStream.GetString4(length);
	}
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionTextPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CTextPr* pTextPr = static_cast<OOX::Spreadsheet::CTextPr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerTextPrTypes::CharacterSet == type)
	{
		pTextPr->m_oCharacterSet = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerTextPrTypes::SourceFile == type)
	{
		pTextPr->m_oSourceFile = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerTextPrTypes::Decimal == type)
	{
		pTextPr->m_oDecimal = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerTextPrTypes::Delimiter == type)
	{
		pTextPr->m_oDelimiter = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerTextPrTypes::Thousands == type)
	{
		pTextPr->m_oThousands = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerTextPrTypes::FirstRow == type)
	{
		pTextPr->m_oFirstRow = m_oBufferedStream.GetLong();
	}
	else if (c_oSerTextPrTypes::CodePage == type)
	{
		pTextPr->m_oCodePage = m_oBufferedStream.GetLong();
	}
	else if(c_oSerTextPrTypes::Qualifier == type)
	{
		pTextPr->m_oQualifier.Init();
		pTextPr->m_oQualifier->SetValue((SimpleTypes::Spreadsheet::EQualifier)m_oBufferedStream.GetLong());
	}
	else if(c_oSerTextPrTypes::FileType == type)
	{
		pTextPr->m_oFileType.Init();
		pTextPr->m_oFileType->SetValue((SimpleTypes::Spreadsheet::EFileType)m_oBufferedStream.GetLong());
	}
	else if(c_oSerTextPrTypes::Prompt == type)
	{
		pTextPr->m_oPrompt = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Delimited == type)
	{
		pTextPr->m_oDelimited = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Tab == type)
	{
		pTextPr->m_oTab = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Space == type)
	{
		pTextPr->m_oSpace = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Comma == type)
	{
		pTextPr->m_oComma = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Semicolon == type)
	{
		pTextPr->m_oSemicolon = m_oBufferedStream.GetBool();
	}
	else if(c_oSerTextPrTypes::Consecutive == type)
	{
		pTextPr->m_oConsecutive = m_oBufferedStream.GetBool();
	}
	else if (c_oSerTextPrTypes::TextFields == type)
	{
		pTextPr->m_oTextFields.Init();
		READ1_DEF(length, res, this->ReadConnectionTextFields, pTextPr->m_oTextFields.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionTextFields(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CTextFields* pTextFields = static_cast<OOX::Spreadsheet::CTextFields*>(poResult);

	int res = c_oSerConstants::ReadOk;

	if (c_oSerTextPrTypes::TextField == type)
	{
		pTextFields->m_arrItems.push_back(new OOX::Spreadsheet::CTextField());
		READ1_DEF(length, res, this->ReadConnectionTextField, pTextFields->m_arrItems.back());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionTextField(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CTextField* pTextField = static_cast<OOX::Spreadsheet::CTextField*>(poResult);

	int res = c_oSerConstants::ReadOk;

	if (c_oSerTextPrTypes::TextFieldType == type)
	{
		pTextField->m_oType.Init();
		pTextField->m_oType->SetValue((SimpleTypes::Spreadsheet::EExternalConnectionType)m_oBufferedStream.GetLong());
	}
	else if (c_oSerTextPrTypes::TextFieldPosition == type)
	{
		pTextField->m_oPosition = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadConnectionWebPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CWebPr* pWebPr = static_cast<OOX::Spreadsheet::CWebPr*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWebPrTypes::Url == type)
	{
		pWebPr->m_oUrl = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerWebPrTypes::Post == type)
	{
		pWebPr->m_oPost = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerWebPrTypes::EditPage == type)
	{
		pWebPr->m_oEditPage = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerWebPrTypes::HtmlFormat == type)
	{
		pWebPr->m_oHtmlFormat.Init();
		pWebPr->m_oHtmlFormat->SetValue((SimpleTypes::Spreadsheet::EHtmlFormat)m_oBufferedStream.GetLong());
	}
	else if(c_oSerWebPrTypes::Xml == type)
	{
		pWebPr->m_oXml = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::SourceData == type)
	{
		pWebPr->m_oSourceData = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::Consecutive == type)
	{
		pWebPr->m_oConsecutive = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::FirstRow == type)
	{
		pWebPr->m_oFirstRow = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::Xl97 == type)
	{
		pWebPr->m_oXl97 = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::TextDates == type)
	{
		pWebPr->m_oTextDates = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::Xl2000 == type)
	{
		pWebPr->m_oXl2000 = m_oBufferedStream.GetBool();
	}
	else if(c_oSerWebPrTypes::HtmlTables == type)
	{
		pWebPr->m_oHtmlTables = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadProtection(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if (c_oSerWorkbookProtection::AlgorithmName == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookAlgorithmName.Init();
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookAlgorithmName->SetValue((SimpleTypes::ECryptAlgoritmName)m_oBufferedStream.GetUChar());
	}
	else if (c_oSerWorkbookProtection::SpinCount == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookSpinCount.Init();
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookSpinCount->SetValue(m_oBufferedStream.GetULong());
	}
	else if (c_oSerWorkbookProtection::HashValue == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookHashValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorkbookProtection::SaltValue == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oWorkbookSaltValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorkbookProtection::Password == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oPassword = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorkbookProtection::LockStructure == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oLockStructure.Init();
		m_oWorkbook.m_oWorkbookProtection->m_oLockStructure->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorkbookProtection::LockWindows == type)
	{
		m_oWorkbook.m_oWorkbookProtection->m_oLockWindows.Init();
		m_oWorkbook.m_oWorkbookProtection->m_oLockWindows->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadWorkbookPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookPrTypes::Date1904 == type)
	{
		m_oWorkbook.m_oWorkbookPr->m_oDate1904.Init();
		m_oWorkbook.m_oWorkbookPr->m_oDate1904->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerWorkbookPrTypes::DateCompatibility == type)
	{
		m_oWorkbook.m_oWorkbookPr->m_oDateCompatibility.Init();
		m_oWorkbook.m_oWorkbookPr->m_oDateCompatibility->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerWorkbookPrTypes::HidePivotFieldList == type)
	{
		m_oWorkbook.m_oWorkbookPr->m_oHidePivotFieldList.Init();
		m_oWorkbook.m_oWorkbookPr->m_oHidePivotFieldList->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerWorkbookPrTypes::ShowPivotChartFilter == type)
	{
		m_oWorkbook.m_oWorkbookPr->m_oShowPivotChartFilter.Init();
		m_oWorkbook.m_oWorkbookPr->m_oShowPivotChartFilter->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadBookViews(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if (c_oSerWorkbookTypes::WorkbookView == type)
	{
		OOX::Spreadsheet::CWorkbookView* pWorkbookView = new OOX::Spreadsheet::CWorkbookView();
		READ2_DEF_SPREADSHEET(length, res, this->ReadWorkbookView, pWorkbookView);
		m_oWorkbook.m_oBookViews->m_arrItems.push_back(pWorkbookView);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadWorkbookView(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CWorkbookView* pWorkbookView = static_cast<OOX::Spreadsheet::CWorkbookView*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookViewTypes::ActiveTab == type)
	{
		pWorkbookView->m_oActiveTab.Init();
		pWorkbookView->m_oActiveTab->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalReference(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;

	OOX::Spreadsheet::CExternalLink *externalLink = static_cast<OOX::Spreadsheet::CExternalLink*>(poResult);
	
	if (c_oSerWorkbookTypes::ExternalBook == type)
	{
		externalLink->m_oExternalBook.Init();
		READ1_DEF(length, res, this->ReadExternalBook, externalLink);
	}
	else if (c_oSerWorkbookTypes::OleLink == type)
	{
		externalLink->m_oOleLink.Init();
		READ1_DEF(length, res, this->ReadOleLink, externalLink);
	}
	else if (c_oSerWorkbookTypes::DdeLink == type)
	{
		externalLink->m_oDdeLink.Init();
		READ1_DEF(length, res, this->ReadDdeLink, externalLink->m_oDdeLink.GetPointer());
	}
	else if (c_oSerWorkbookTypes::ExternalFileKey == type)
	{
		externalLink->m_oFileKey = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorkbookTypes::ExternalInstanceId == type)
	{
		externalLink->m_oInstanceId = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalReferences(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if (c_oSerWorkbookTypes::ExternalReference == type)
	{
		OOX::Spreadsheet::CExternalLink *extLink = new OOX::Spreadsheet::CExternalLink(NULL);
		READ1_DEF(length, res, this->ReadExternalReference, extLink);

		smart_ptr<OOX::File> oCurFile(extLink);
		const OOX::RId oRId = m_oWorkbook.Add(oCurFile);

		OOX::Spreadsheet::CExternalReference* pExternalReference = new OOX::Spreadsheet::CExternalReference();

		pExternalReference->m_oRid.Init();
		pExternalReference->m_oRid->SetValue(oRId.get());
		
		m_oWorkbook.m_oExternalReferences->m_arrItems.push_back(pExternalReference);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDefinedNames(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookTypes::DefinedName == type)
	{
		OOX::Spreadsheet::CDefinedName* pDefinedName = new OOX::Spreadsheet::CDefinedName();
		READ1_DEF(length, res, this->ReadDefinedName, pDefinedName);
		m_oWorkbook.m_oDefinedNames->m_arrItems.push_back(pDefinedName);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDefinedName(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDefinedName* pDefinedName = static_cast<OOX::Spreadsheet::CDefinedName*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerDefinedNameTypes::Name == type)
	{
		pDefinedName->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerDefinedNameTypes::Ref == type)
	{
		pDefinedName->m_oRef = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerDefinedNameTypes::LocalSheetId == type)
	{
		pDefinedName->m_oLocalSheetId.Init();
		pDefinedName->m_oLocalSheetId->SetValue(m_oBufferedStream.GetLong());
	}
    else if(c_oSerDefinedNameTypes::Hidden == type)
    {
         pDefinedName->m_oHidden.Init();
         pDefinedName->m_oHidden->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
    }
	else if(c_oSerDefinedNameTypes::Comment == type)
    {
		pDefinedName->m_oComment = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadCalcPr(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
    OOX::Spreadsheet::CCalcPr* calcPr = static_cast<OOX::Spreadsheet::CCalcPr*>(poResult);
	if(c_oSerCalcPrTypes::CalcId == type)
	{
		calcPr->m_oCalcId.Init();
		calcPr->m_oCalcId->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSerCalcPrTypes::CalcMode == type)
	{
		calcPr->m_oCalcMode = (SimpleTypes::Spreadsheet::ECalcMode)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerCalcPrTypes::FullCalcOnLoad == type)
	{
		calcPr->m_oFullCalcOnLoad = m_oBufferedStream.GetBool();
	}
	else if(c_oSerCalcPrTypes::RefMode == type)
	{
		calcPr->m_oRefMode = (SimpleTypes::Spreadsheet::ERefMode)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerCalcPrTypes::Iterate == type)
	{
		calcPr->m_oIterate.Init();
		calcPr->m_oIterate->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerCalcPrTypes::IterateCount == type)
	{
		calcPr->m_oIterateCount.Init();
		calcPr->m_oIterateCount->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSerCalcPrTypes::IterateDelta == type)
	{
		calcPr->m_oIterateDelta.Init();
		calcPr->m_oIterateDelta->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSerCalcPrTypes::FullPrecision == type)
	{
		calcPr->m_oFullPrecision.Init();
		calcPr->m_oFullPrecision->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerCalcPrTypes::CalcCompleted == type)
	{
		calcPr->m_oCalcCompleted.Init();
		calcPr->m_oCalcCompleted->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerCalcPrTypes::CalcOnSave == type)
	{
		calcPr->m_oCalcOnSave.Init();
		calcPr->m_oCalcOnSave->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerCalcPrTypes::ConcurrentCalc == type)
	{
		calcPr->m_oConcurrentCalc.Init();
		calcPr->m_oConcurrentCalc->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSerCalcPrTypes::ConcurrentManualCount == type)
	{
		calcPr->m_oConcurrentManualCount.Init();
		calcPr->m_oConcurrentManualCount->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSerCalcPrTypes::ForceFullCalc == type)
	{
		calcPr->m_oForceFullCalc.Init();
		calcPr->m_oForceFullCalc->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalBook(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalLink* extLink = static_cast<OOX::Spreadsheet::CExternalLink*>(poResult);
	OOX::Spreadsheet::CExternalBook* pExternalBook = extLink->m_oExternalBook.GetPointer();
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::Id == type)
	{
		std::wstring sName(m_oBufferedStream.GetString3(length));

		OOX::Spreadsheet::ExternalLinkPath *link = new OOX::Spreadsheet::ExternalLinkPath(NULL, OOX::CPath(sName, false));
		smart_ptr<OOX::File> oLinkFile(link);
		const OOX::RId oRIdLink = extLink->Add(oLinkFile);

		pExternalBook->m_oRid.Init();
		pExternalBook->m_oRid->SetValue(oRIdLink.get());
	}
	else if(c_oSer_ExternalLinkTypes::SheetNames == type)
	{
		pExternalBook->m_oSheetNames.Init();
		READ1_DEF(length, res, this->ReadExternalSheetNames, pExternalBook->m_oSheetNames.GetPointer());
	}
	else if(c_oSer_ExternalLinkTypes::DefinedNames == type)
	{
		pExternalBook->m_oDefinedNames.Init();
		READ1_DEF(length, res, this->ReadExternalDefinedNames, pExternalBook->m_oDefinedNames.GetPointer());
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataSet == type)
	{
		pExternalBook->m_oSheetDataSet.Init();
		READ1_DEF(length, res, this->ReadExternalSheetDataSet, pExternalBook->m_oSheetDataSet.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalSheetNames(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalSheetNames* pSheetNames = static_cast<OOX::Spreadsheet::CExternalSheetNames*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::SheetName == type)
	{
		ComplexTypes::Spreadsheet::String* pSheetName = new ComplexTypes::Spreadsheet::String();
		pSheetName->m_sVal = m_oBufferedStream.GetString4(length);
		pSheetNames->m_arrItems.push_back(pSheetName);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalDefinedNames(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalDefinedNames* pDefinedNames = static_cast<OOX::Spreadsheet::CExternalDefinedNames*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::DefinedName == type)
	{
		OOX::Spreadsheet::CExternalDefinedName* pDefinedName = new OOX::Spreadsheet::CExternalDefinedName();
		READ1_DEF(length, res, this->ReadExternalDefinedName, pDefinedName);
		pDefinedNames->m_arrItems.push_back(pDefinedName);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalDefinedName(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalDefinedName* pDefinedName = static_cast<OOX::Spreadsheet::CExternalDefinedName*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::DefinedNameName == type)
	{
		pDefinedName->m_oName.Init();
		pDefinedName->m_oName->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_ExternalLinkTypes::DefinedNameRefersTo == type)
	{
		pDefinedName->m_oRefersTo.Init();
		pDefinedName->m_oRefersTo->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_ExternalLinkTypes::DefinedNameSheetId == type)
	{
		pDefinedName->m_oSheetId.Init();
		pDefinedName->m_oSheetId->SetValue(m_oBufferedStream.GetULong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalSheetDataSet(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalSheetDataSet* pSheetDataSet = static_cast<OOX::Spreadsheet::CExternalSheetDataSet*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::SheetData == type)
	{
		OOX::Spreadsheet::CExternalSheetData* pSheetData = new OOX::Spreadsheet::CExternalSheetData();
		READ1_DEF(length, res, this->ReadExternalSheetData, pSheetData);
		pSheetDataSet->m_arrItems.push_back(pSheetData);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalSheetData(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalSheetData* pSheetData = static_cast<OOX::Spreadsheet::CExternalSheetData*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::SheetDataSheetId == type)
	{
		pSheetData->m_oSheetId.Init();
		pSheetData->m_oSheetId->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataRefreshError == type)
	{
		pSheetData->m_oRefreshError.Init();
		pSheetData->m_oRefreshError->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataRow == type)
	{
		OOX::Spreadsheet::CExternalRow* pRow = new OOX::Spreadsheet::CExternalRow();
		READ1_DEF(length, res, this->ReadExternalRow, pRow);
		pSheetData->m_arrItems.push_back(pRow);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalRow(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalRow* pRow = static_cast<OOX::Spreadsheet::CExternalRow*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::SheetDataRowR == type)
	{
		pRow->m_oR.Init();
		pRow->m_oR->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataRowCell == type)
	{
		OOX::Spreadsheet::CExternalCell* pCell = new OOX::Spreadsheet::CExternalCell();
		READ1_DEF(length, res, this->ReadExternalCell, pCell);
		pRow->m_arrItems.push_back(pCell);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadExternalCell(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalCell* pCell = static_cast<OOX::Spreadsheet::CExternalCell*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ExternalLinkTypes::SheetDataRowCellRef == type)
	{
		pCell->m_oRef.Init();
		pCell->m_oRef->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataRowCellType == type)
	{
		pCell->m_oType.Init();
		pCell->m_oType->SetValue((SimpleTypes::Spreadsheet::ECellTypeType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ExternalLinkTypes::SheetDataRowCellValue == type)
	{
		pCell->m_oValue.Init();
		pCell->m_oValue->m_sText.append(m_oBufferedStream.GetString3(length));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadOleLink(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExternalLink* extLink = static_cast<OOX::Spreadsheet::CExternalLink*>(poResult);
	OOX::Spreadsheet::COleLink* oleLink = extLink->m_oOleLink.GetPointer();
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_OleLinkTypes::Id == type)
	{
		std::wstring sName(m_oBufferedStream.GetString3(length));

		OOX::Spreadsheet::ExternalOleObject *link = new OOX::Spreadsheet::ExternalOleObject(NULL, sName);
		smart_ptr<OOX::File> oLinkFile(link);
		const OOX::RId oRIdLink = extLink->Add(oLinkFile);

		oleLink->m_oRid.Init();
		oleLink->m_oRid->SetValue(oRIdLink.get());
	}
	else if(c_oSer_OleLinkTypes::ProgId == type)
	{
		oleLink->m_oProgId.Init();
		oleLink->m_oProgId->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_OleLinkTypes::OleItem == type)
	{
		if (!oleLink->m_oOleItems.IsInit())
		{
			oleLink->m_oOleItems.Init();
		}
		OOX::Spreadsheet::COleItem* pOleItem = new OOX::Spreadsheet::COleItem();
		READ1_DEF(length, res, this->ReadOleItem, pOleItem);
		oleLink->m_oOleItems->m_arrItems.push_back(pOleItem);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadOleItem(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::COleItem* pOleItem = static_cast<OOX::Spreadsheet::COleItem*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_OleLinkTypes::Name == type)
	{
		pOleItem->m_oName.Init();
		pOleItem->m_oName->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_OleLinkTypes::Icon == type)
	{
		pOleItem->m_oIcon.Init();
		pOleItem->m_oIcon->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_OleLinkTypes::Advise == type)
	{
		pOleItem->m_oAdvise.Init();
		pOleItem->m_oAdvise->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_OleLinkTypes::PreferPic == type)
	{
		pOleItem->m_oPreferPic.Init();
		pOleItem->m_oPreferPic->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDdeLink(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDdeLink* ddeLink = static_cast<OOX::Spreadsheet::CDdeLink*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DdeLinkTypes::DdeService == type)
	{
		ddeLink->m_oDdeService.Init();
		ddeLink->m_oDdeService->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_DdeLinkTypes::DdeTopic == type)
	{
		ddeLink->m_oDdeTopic.Init();
		ddeLink->m_oDdeTopic->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_DdeLinkTypes::DdeItem == type)
	{
		if (!ddeLink->m_oDdeItems.IsInit())
		{
			ddeLink->m_oDdeItems.Init();
		}
		OOX::Spreadsheet::CDdeItem* pDdeItem = new OOX::Spreadsheet::CDdeItem();
		READ1_DEF(length, res, this->ReadDdeItem, pDdeItem);
		ddeLink->m_oDdeItems->m_arrItems.push_back(pDdeItem);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDdeItem(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDdeItem* pDdeItem = static_cast<OOX::Spreadsheet::CDdeItem*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DdeLinkTypes::Name == type)
	{
		pDdeItem->m_oName.Init();
		pDdeItem->m_oName->append(m_oBufferedStream.GetString3(length));
	}
	else if(c_oSer_DdeLinkTypes::Ole == type)
	{
		pDdeItem->m_oOle.Init();
		pDdeItem->m_oOle->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_DdeLinkTypes::Advise == type)
	{
		pDdeItem->m_oAdvise.Init();
		pDdeItem->m_oAdvise->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_DdeLinkTypes::PreferPic == type)
	{
		pDdeItem->m_oPreferPic.Init();
		pDdeItem->m_oPreferPic->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_DdeLinkTypes::DdeValues == type)
	{
		pDdeItem->m_oDdeValues.Init();
		READ1_DEF(length, res, this->ReadDdeValues, pDdeItem->m_oDdeValues.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDdeValues(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDdeValues* pDdeValues = static_cast<OOX::Spreadsheet::CDdeValues*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DdeLinkTypes::DdeValuesRows == type)
	{
		pDdeValues->m_oRows.Init();
		pDdeValues->m_oRows->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_DdeLinkTypes::DdeValuesCols == type)
	{
		pDdeValues->m_oCols.Init();
		pDdeValues->m_oCols->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_DdeLinkTypes::DdeValue == type)
	{
		OOX::Spreadsheet::CDdeValue* pDdeValue = new OOX::Spreadsheet::CDdeValue();
		READ1_DEF(length, res, this->ReadDdeValue, pDdeValue);
		pDdeValues->m_arrItems.push_back(pDdeValue);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadDdeValue(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDdeValue* pDdeValue = static_cast<OOX::Spreadsheet::CDdeValue*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DdeLinkTypes::DdeValueType == type)
	{
		pDdeValue->m_oType.Init();
		pDdeValue->m_oType->SetValue((SimpleTypes::Spreadsheet::EDdeValueType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_DdeLinkTypes::DdeValueVal == type)
	{
		OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
		pText->m_sText.append(m_oBufferedStream.GetString3(length));
		pDdeValue->m_arrItems.push_back(pText);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadPivotCaches(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookTypes::PivotCache == type)
	{
		PivotCachesTemp oPivotCachesTemp;
		READ1_DEF(length, res, this->ReadPivotCache, &oPivotCachesTemp);
		if(-1 != oPivotCachesTemp.nId && NULL != oPivotCachesTemp.pDefinitionData)
		{
			OOX::Spreadsheet::CPivotCacheDefinitionFile* pDefinitionFile = new OOX::Spreadsheet::CPivotCacheDefinitionFile(NULL);
			std::wstring srIdRecords;
			if(NULL != oPivotCachesTemp.pRecords)
			{
				NSCommon::smart_ptr<OOX::File> pFileRecords(oPivotCachesTemp.pRecords);
				srIdRecords = pDefinitionFile->Add(pFileRecords).ToString();
			}
			pDefinitionFile->setData(oPivotCachesTemp.pDefinitionData, oPivotCachesTemp.nDefinitionLength, srIdRecords);
			
			NSCommon::smart_ptr<OOX::File> pFile(pDefinitionFile);
			OOX::RId rIdDefinition = m_oWorkbook.Add(pFile);

			m_oWorkbook.m_oPivotCachesXml->append(L"<pivotCache cacheId=\"");
			m_oWorkbook.m_oPivotCachesXml->append(std::to_wstring(oPivotCachesTemp.nId));
			m_oWorkbook.m_oPivotCachesXml->append(L"\" r:id=\"");
			m_oWorkbook.m_oPivotCachesXml->append(rIdDefinition.ToString());
			m_oWorkbook.m_oPivotCachesXml->append(L"\"/>");

			m_mapPivotCacheDefinitions[oPivotCachesTemp.nId] = pFile;
		}
		else
		{
			RELEASEOBJECT(oPivotCachesTemp.pRecords);
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadPivotCache(BYTE type, long length, void* poResult)
{
	PivotCachesTemp* pPivotCachesTemp = static_cast<PivotCachesTemp*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_PivotTypes::id == type)
	{
		pPivotCachesTemp->nId = m_oBufferedStream.GetLong();
	}
	else if(c_oSer_PivotTypes::cache == type)
	{
		pPivotCachesTemp->pDefinitionData = m_oBufferedStream.GetPointer(length);
		pPivotCachesTemp->nDefinitionLength = length;
	}
	else if(c_oSer_PivotTypes::record == type)
	{
		pPivotCachesTemp->pRecords = new OOX::Spreadsheet::CPivotCacheRecordsFile(NULL);
		pPivotCachesTemp->pRecords->setData(m_oBufferedStream.GetPointer(length), length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorkbookTableReader::ReadSlicerCaches(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSlicerCaches* pSlicerCaches = static_cast<OOX::Spreadsheet::CSlicerCaches*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorkbookTypes::SlicerCache == type)
	{
		OOX::Spreadsheet::CSlicerCacheFile* pSlicerCache = new OOX::Spreadsheet::CSlicerCacheFile(NULL);
		pSlicerCache->m_oSlicerCacheDefinition.Init();

		m_oBufferedStream.GetUChar();//type
		pSlicerCache->m_oSlicerCacheDefinition->fromPPTY(&m_oBufferedStream);

		NSCommon::smart_ptr<OOX::File> pSlicerCacheFile(pSlicerCache);
		const OOX::RId oRId = m_oWorkbook.Add(pSlicerCacheFile);

		pSlicerCaches->m_oSlicerCache.emplace_back();
		pSlicerCaches->m_oSlicerCache.back().m_oRId.Init();
		pSlicerCaches->m_oSlicerCache.back().m_oRId->SetValue(oRId.get());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

BinaryCommentReader::BinaryCommentReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorksheet* pCurWorksheet) 
	: Binary_CommonReader(oBufferedStream), m_pCurWorksheet(pCurWorksheet)
{
}
int BinaryCommentReader::Read(long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadComments, poResult);
	return res;
}
int BinaryCommentReader::ReadExternal(long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	READ1_DEF(length, res, this->ReadCommentDatasExternal, poResult);
	return res;
}
int BinaryCommentReader::ReadCommentDatasExternal(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	std::vector<SerializeCommon::CommentData*>* pCommentDatas = static_cast<std::vector<SerializeCommon::CommentData*>*>(poResult);
	if ( c_oSer_Comments::CommentData == type )
	{
		SerializeCommon::CommentData* oCommentData = new SerializeCommon::CommentData();
		READ1_DEF(length, res, this->ReadCommentData, oCommentData);
		pCommentDatas->push_back(oCommentData);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadComments(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::Comment == type)
	{
		OOX::Spreadsheet::CCommentItem* pNewComment = new OOX::Spreadsheet::CCommentItem();
		READ2_DEF_SPREADSHEET(length, res, this->ReadComment, pNewComment);

		if(NULL != m_pCurWorksheet && pNewComment->IsValid())
		{
			std::wstring sId = std::to_wstring(pNewComment->m_nRow.get()) + L"-" + std::to_wstring(pNewComment->m_nCol.get());
			m_pCurWorksheet->m_mapComments [sId] = pNewComment;
		}
		else
			RELEASEOBJECT(pNewComment);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadComment(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCommentItem* pNewComment = static_cast<OOX::Spreadsheet::CCommentItem*>(poResult);
	if ( c_oSer_Comments::Row == type )
		pNewComment->m_nRow = m_oBufferedStream.GetLong();
	else if ( c_oSer_Comments::Col == type )
		pNewComment->m_nCol = m_oBufferedStream.GetLong();
	else if ( c_oSer_Comments::CommentDatas == type )
	{
		if(!pNewComment->m_sGfxdata.IsInit())
		{
			int nStartPos = m_oBufferedStream.GetPos();
			BYTE* pSourceBuffer = m_oBufferedStream.GetPointer(length);
			m_oBufferedStream.Seek(nStartPos);

			std::string sSignature("XLS2");
            int nSignatureSize      = (int)sSignature.length();
			int nDataLengthSize     = sizeof(_INT32);
            int nJunkSize           = 2;
            int nWriteBufferLength  = nSignatureSize + nDataLengthSize + length + nJunkSize;

            BYTE* pWriteBuffer = new BYTE[nWriteBufferLength];
            memcpy(pWriteBuffer, sSignature.c_str(), nSignatureSize);

			*((_INT32*)(pWriteBuffer + nSignatureSize)) = (_INT32)length;
			memcpy(pWriteBuffer + nSignatureSize + nDataLengthSize, pSourceBuffer, length);
			//пишем в конце 0, потому что при редактировании Excel меняет посление байты.
			memset(pWriteBuffer + nSignatureSize + nDataLengthSize + length, 0, nJunkSize);

			int nBase64BufferLen = Base64::Base64EncodeGetRequiredLength(nWriteBufferLength, Base64::B64_BASE64_FLAG_NONE);
            BYTE* pbBase64Buffer = new BYTE[nBase64BufferLen+64];
            std::wstring sGfxdata;
//					if(true == Base64::Base64Encode(pWriteBuffer, nWriteBufferLength, (LPSTR)pbBase64Buffer, &nBase64BufferLen, Base64::B64_BASE64_FLAG_NONE))
            if(true == Base64_1::Base64Encode(pWriteBuffer, nWriteBufferLength, pbBase64Buffer, &nBase64BufferLen))
            {
				std::wstring strGfxdata = NSFile::CUtf8Converter::GetUnicodeStringFromUTF8(pbBase64Buffer, nBase64BufferLen);
                sGfxdata = std::wstring(strGfxdata.c_str());
				//важно иначе при редактировании и сохранении в Excel перетирается
                sGfxdata += L"\r\n";
			}
			RELEASEARRAYOBJECTS(pbBase64Buffer);
			RELEASEARRAYOBJECTS(pWriteBuffer);

            if(!sGfxdata.empty())
			{
				pNewComment->m_sGfxdata = sGfxdata;
			}
		}
		READ1_DEF(length, res, this->ReadCommentDatas, pNewComment);
	}
	else if ( c_oSer_Comments::Left == type )
		pNewComment->m_nLeft = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::Top == type )
		pNewComment->m_nTop = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::Right == type )
		pNewComment->m_nRight = abs( m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::Bottom == type )
		pNewComment->m_nBottom = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::LeftOffset == type )
		pNewComment->m_nLeftOffset = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::TopOffset == type )
		pNewComment->m_nTopOffset = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::RightOffset == type )
		pNewComment->m_nRightOffset = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::BottomOffset == type )
		pNewComment->m_nBottomOffset = abs(m_oBufferedStream.GetLong());
	else if ( c_oSer_Comments::LeftMM == type )
		pNewComment->m_dLeftMM = m_oBufferedStream.GetDoubleReal();
	else if ( c_oSer_Comments::TopMM == type )
		pNewComment->m_dTopMM = m_oBufferedStream.GetDoubleReal();
	else if ( c_oSer_Comments::WidthMM == type )
		pNewComment->m_dWidthMM = m_oBufferedStream.GetDoubleReal();
	else if ( c_oSer_Comments::HeightMM == type )
		pNewComment->m_dHeightMM = m_oBufferedStream.GetDoubleReal();
	else if ( c_oSer_Comments::MoveWithCells == type )
		pNewComment->m_bMove = m_oBufferedStream.GetBool();
	else if ( c_oSer_Comments::SizeWithCells == type )
		pNewComment->m_bSize = m_oBufferedStream.GetBool();
	else if ( c_oSer_Comments::ThreadedComment == type )
	{
		pNewComment->m_pThreadedComment = new OOX::Spreadsheet::CThreadedComment();
		READ1_DEF(length, res, this->ReadThreadedComment, pNewComment->m_pThreadedComment);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadCommentDatas(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCommentItem* pNewComment = static_cast<OOX::Spreadsheet::CCommentItem*>(poResult);
	if ( c_oSer_Comments::CommentData == type )
	{
		if(!pNewComment->m_oText.IsInit())
		{
			SerializeCommon::CommentData oCommentData;
			READ1_DEF(length, res, this->ReadCommentData, &oCommentData);
			pNewComment->m_sAuthor = oCommentData.sUserName;
			pNewComment->m_oText.Init();
			parseCommentData(&oCommentData, pNewComment->m_oText.get2());
		}
		else
			res = c_oSerConstants::ReadUnknown;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadCommentData(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	SerializeCommon::CommentData* pComments = static_cast<SerializeCommon::CommentData*>(poResult);
	if ( c_oSer_CommentData::Text == type )
		pComments->sText = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::Time == type )
		pComments->sTime = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::OOTime == type )
		pComments->sOOTime = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::UserId == type )
		pComments->sUserId = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::UserName == type )
		pComments->sUserName = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::QuoteText == type )
		pComments->sQuoteText = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::Guid == type )
		pComments->sGuid = m_oBufferedStream.GetString4(length);
	else if ( c_oSer_CommentData::Solved == type )
	{
		pComments->bSolved = true;
		pComments->Solved = m_oBufferedStream.GetBool();
	}
	else if ( c_oSer_CommentData::Document == type )
	{
		pComments->bDocument = true;
		pComments->Document = m_oBufferedStream.GetBool();
	}
	else if ( c_oSer_CommentData::UserData == type )
	{
		pComments->sUserData = m_oBufferedStream.GetString4(length);
	}
	else if ( c_oSer_CommentData::Replies == type ) {
		READ1_DEF(length, res, this->ReadCommentReplies, &pComments->aReplies);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadCommentReplies(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	std::vector<SerializeCommon::CommentData*>* pComments = static_cast<std::vector<SerializeCommon::CommentData*>*>(poResult);
	if ( c_oSer_CommentData::Reply == type )
	{
		SerializeCommon::CommentData* pCommentData = new SerializeCommon::CommentData();
		READ1_DEF(length, res, this->ReadCommentData, pCommentData);
		pComments->push_back(pCommentData);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadThreadedComment(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CThreadedComment* pThreadedComment = static_cast<OOX::Spreadsheet::CThreadedComment*>(poResult);
	if ( c_oSer_ThreadedComment::dT == type )
	{
		pThreadedComment->dT = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::personId == type )
	{
		pThreadedComment->personId = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::id == type )
	{
		pThreadedComment->id = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::done == type )
	{
		pThreadedComment->done = m_oBufferedStream.GetBool();
	}
	else if ( c_oSer_ThreadedComment::text == type )
	{
		pThreadedComment->m_oText.Init();
		pThreadedComment->m_oText->m_sText = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::mention == type )
	{
		OOX::Spreadsheet::CThreadedCommentMention* pMention = new OOX::Spreadsheet::CThreadedCommentMention();
		READ1_DEF(length, res, this->ReadThreadedCommentMention, pMention);
		if(!pThreadedComment->m_oMentions.IsInit())
		{
			pThreadedComment->m_oMentions.Init();
		}
		pThreadedComment->m_oMentions->m_arrItems.push_back(pMention);
	}
	else if ( c_oSer_ThreadedComment::reply == type )
	{
		OOX::Spreadsheet::CThreadedComment* pReply = new OOX::Spreadsheet::CThreadedComment();
		READ1_DEF(length, res, this->ReadThreadedComment, pReply);
		pThreadedComment->m_arrReplies.push_back(pReply);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCommentReader::ReadThreadedCommentMention(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CThreadedCommentMention* pMention = static_cast<OOX::Spreadsheet::CThreadedCommentMention*>(poResult);
	if ( c_oSer_ThreadedComment::mentionpersonId == type )
	{
		pMention->mentionpersonId = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::mentionId == type )
	{
		pMention->mentionId = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_ThreadedComment::startIndex == type )
	{
		pMention->startIndex.Init();
		pMention->startIndex->SetValue(m_oBufferedStream.GetULong());
	}
	else if ( c_oSer_ThreadedComment::length == type )
	{
		pMention->length.Init();
		pMention->length->SetValue(m_oBufferedStream.GetULong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
void BinaryCommentReader::parseCommentData(SerializeCommon::CommentData* pCommentData, OOX::Spreadsheet::CSi& oSi)
{
	if(NULL != pCommentData && false == pCommentData->sText.empty())
	{
		int nLimit = OOX::Spreadsheet::SpreadsheetCommon::MAX_STRING_LEN;
		if (pCommentData->sUserName.empty())
		{
			nLimit = addCommentRun(oSi, pCommentData->sText, false, nLimit);
		}
		else
		{
			nLimit = addCommentRun(oSi, pCommentData->sUserName + _T(":"), true, nLimit);
			if (nLimit <= 0)
				return;
			nLimit = addCommentRun(oSi, _T("\n") + pCommentData->sText, false, nLimit);
		}
	}
}
int BinaryCommentReader::addCommentRun(OOX::Spreadsheet::CSi& oSi, const std::wstring& text, bool isBold, int nLimit)
{
	OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
	pRun->m_oRPr.Init();
	OOX::Spreadsheet::CRPr& pRPr = pRun->m_oRPr.get2();
	if(isBold)
	{
		pRPr.m_oBold.Init();
		pRPr.m_oBold->m_oVal.FromBool(true);
	}
	pRPr.m_oRFont.Init();
	pRPr.m_oRFont->m_sVal = L"Tahoma";
	pRPr.m_oSz.Init();
	pRPr.m_oSz->m_oVal.Init();
	pRPr.m_oSz->m_oVal->SetValue(9);

	OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
	//Fix Excel recovery error; Fix bug 42968
	pText->m_sText.append(text, 0, nLimit);
	nLimit -= text.length();

	pRun->m_arrItems.push_back(pText);
	oSi.m_arrItems.push_back(pRun);
	return nLimit;
}
void BinaryCommentReader::addThreadedComment(OOX::Spreadsheet::CSi& oSi, OOX::Spreadsheet::CThreadedComment* pThreadedComment, nullable<std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>>& mapPersonList)
{
	int nLimit = OOX::Spreadsheet::SpreadsheetCommon::MAX_STRING_LEN;
	if(pThreadedComment->m_oText.IsInit())
	{
		std::wstring displayName = getThreadedCommentAuthor(mapPersonList, pThreadedComment->personId, L"Comment");
		nLimit = addCommentRun(oSi, displayName + L":", true, nLimit);
		if (nLimit <= 0)
			return;
		nLimit = addCommentRun(oSi, L"\n" + pThreadedComment->m_oText->ToString() + L"\n", false, nLimit);
		if (nLimit <= 0)
			return;
	}
	for(size_t i = 0; i < pThreadedComment->m_arrReplies.size(); ++i)
	{
		if(pThreadedComment->m_arrReplies[i]->m_oText.IsInit())
		{
			std::wstring displayName = getThreadedCommentAuthor(mapPersonList, pThreadedComment->m_arrReplies[i]->personId, L"Reply");
			nLimit = addCommentRun(oSi, displayName + L":", true, nLimit);
			if (nLimit <= 0)
				return;
			nLimit = addCommentRun(oSi, L"\n" + pThreadedComment->m_arrReplies[i]->m_oText->ToString() + L"\n", false, nLimit);
			if (nLimit <= 0)
				return;
		}
	}
}
std::wstring BinaryCommentReader::getThreadedCommentAuthor(nullable<std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>>& mapPersonList, nullable<SimpleTypes::CGuid>& personId, const std::wstring& sDefault)
{
	if (mapPersonList.IsInit() && personId.IsInit())
	{
		std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>::iterator it = mapPersonList->find(personId->ToString());
		if (it != mapPersonList->end())
		{
			if (it->second->displayName.IsInit())
			{
				return it->second->displayName.get();
			}
		}
	}
	return sDefault;
}

BinaryWorksheetsTableReader::BinaryWorksheetsTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook,
	OOX::Spreadsheet::CSharedStrings* pSharedStrings, std::vector<OOX::Spreadsheet::CWorksheet*>& arWorksheets, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets,
    boost::unordered_map<long, ImageObject*>& mapMedia, const std::wstring& sDestinationDir, const std::wstring& sMediaDir, SaveParams& oSaveParams,
    NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>& mapPivotCacheDefinitions)

: Binary_CommonReader(oBufferedStream), m_oWorkbook(oWorkbook), m_oBcr2(oBufferedStream), m_sMediaDir(sMediaDir), m_oSaveParams(oSaveParams), 
m_mapMedia(mapMedia), m_sDestinationDir(sDestinationDir), m_arWorksheets(arWorksheets), m_mapWorksheets(mapWorksheets), m_pSharedStrings(pSharedStrings),m_mapPivotCacheDefinitions(mapPivotCacheDefinitions)
{
	m_pOfficeDrawingConverter = pOfficeDrawingConverter;
	m_nNextObjectId = 0xfffff; // в CDrawingConverter своя нумерация .. 
	m_lObjectIdVML = 1024;
} 
int BinaryWorksheetsTableReader::Read()
{
	m_oWorkbook.m_oSheets.Init();
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadWorksheetsTableContent, this);
	return res;
}
int BinaryWorksheetsTableReader::ReadWorksheetsTableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::Worksheet == type)
	{
		m_pCurWorksheet.reset( new OOX::Spreadsheet::CWorksheet(NULL) );
		m_pCurSheet.reset(new OOX::Spreadsheet::CSheet());
		m_pCurVmlDrawing.reset(new OOX::CVmlDrawing(NULL, false));
		m_pCurDrawing.reset(new OOX::Spreadsheet::CDrawing(NULL));
		m_pCurOleObjects.reset(new OOX::Spreadsheet::COleObjects());

		m_lObjectIdVML += 1024;
		m_pCurVmlDrawing->m_lObjectIdVML = m_lObjectIdVML;

		boost::unordered_map<BYTE, std::vector<unsigned int>> mapPos;
		READ1_DEF(length, res, this->ReadWorksheetSeekPositions, &mapPos);

		m_pCurWorksheet->m_bWriteDirectlyToFile = true;
		
		smart_ptr<OOX::File> oCurWorksheetFile = m_pCurWorksheet.smart_dynamic_cast<OOX::File>();
		m_oWorkbook.AssignOutputFilename(oCurWorksheetFile);

		std::wstring sWsPath = m_sDestinationDir + FILE_SEPARATOR_STR + _T("xl")  + FILE_SEPARATOR_STR + m_pCurWorksheet->DefaultDirectory().GetPath();
		NSDirectory::CreateDirectories(sWsPath);
		sWsPath += FILE_SEPARATOR_STR + m_pCurWorksheet->m_sOutputFilename;
		NSFile::CStreamWriter oStreamWriter;
		oStreamWriter.CreateFileW(sWsPath);
		
		m_pCurStreamWriter = &oStreamWriter;
		res = ReadWorksheet(mapPos, oStreamWriter, poResult);
		oStreamWriter.CloseFile();

		if(m_pCurSheet->m_oName.IsInit())
		{
			const OOX::RId oRId = m_oWorkbook.Add(oCurWorksheetFile);
			m_pCurSheet->m_oRid.Init();
			m_pCurSheet->m_oRid->SetValue(oRId.get());
			
			m_arWorksheets.push_back(m_pCurWorksheet.GetPointer()); m_pCurWorksheet.AddRef();
			m_mapWorksheets [m_pCurSheet->m_oName.get()] = m_pCurWorksheet.GetPointer(); //for csv
			
			m_oWorkbook.m_oSheets->m_arrItems.push_back(m_pCurSheet.GetPointer()); m_pCurSheet.AddRef();
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadWorksheetSeekPositions(BYTE type, long length, void* poResult)
{
	boost::unordered_map<BYTE, std::vector<unsigned int>>* mapPos = static_cast<boost::unordered_map<BYTE, std::vector<unsigned int>>*>(poResult);
	boost::unordered_map<BYTE, std::vector<unsigned int>>::iterator pFind = mapPos->find(type);
	
	if(pFind != mapPos->end())
	{
		pFind->second.push_back(m_oBufferedStream.GetPos());
		pFind->second.push_back(length);
	}
	else
	{
		std::vector<unsigned int> data;
		data.push_back(m_oBufferedStream.GetPos());
		data.push_back(length);
		(*mapPos)[type] = data;
	}
	return c_oSerConstants::ReadUnknown;
}
int BinaryWorksheetsTableReader::ReadWorksheet(boost::unordered_map<BYTE, std::vector<unsigned int>>& mapPos, NSFile::CStreamWriter& oStreamWriter, void* poResult)
{
	m_pCurWorksheet->toXMLStart(oStreamWriter);
	LONG nOldPos = m_oBufferedStream.GetPos();
//-------------------------------------------------------------------------------------------------------------
	int res = c_oSerConstants::ReadOk;
	boost::unordered_map<BYTE, std::vector<unsigned int>>::iterator pFind;
	LONG nPos;
	LONG length;
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SheetPr);
		OOX::Spreadsheet::CSheetPr oSheetPr;
		READ1_DEF(length, res, this->ReadSheetPr, &oSheetPr);
	SEEK_TO_POS_END(oSheetPr);
//-------------------------------------------------------------------------------------------------------------
	OOX::Spreadsheet::CSheetViews oSheetViews;
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SheetViews);
		READ1_DEF(length, res, this->ReadSheetViews, &oSheetViews);
	SEEK_TO_POS_END2();
	if(oSheetViews.m_arrItems.empty())
		oSheetViews.m_arrItems.push_back(new OOX::Spreadsheet::CSheetView());
	OOX::Spreadsheet::CSheetView* pSheetView = oSheetViews.m_arrItems.front();
	if(false == pSheetView->m_oWorkbookViewId.IsInit())
	{
		pSheetView->m_oWorkbookViewId.Init();
		pSheetView->m_oWorkbookViewId->SetValue(0);
	}
	oSheetViews.toXML(oStreamWriter);
//-------------------------------------------------------------------------------------------------------------
	OOX::Spreadsheet::CSheetFormatPr oSheetFormatPr;
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SheetFormatPr);
		READ2_DEF_SPREADSHEET(length, res, this->ReadSheetFormatPr, &oSheetFormatPr);
	SEEK_TO_POS_END2();
	if(!oSheetFormatPr.m_oDefaultRowHeight.IsInit())
	{
		oSheetFormatPr.m_oDefaultRowHeight = 15.;
	}
	oSheetFormatPr.toXML(oStreamWriter);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Cols);
		OOX::Spreadsheet::CCols oCols;
		READ1_DEF(length, res, this->ReadWorksheetCols, &oCols);
	SEEK_TO_POS_END(oCols);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SheetData)
		if (NULL == m_oSaveParams.pCSVWriter)
		{
			OOX::Spreadsheet::CSheetData oSheetData;
			oSheetData.toXMLStart(oStreamWriter);
			READ1_DEF(length, res, this->ReadSheetData, NULL);
			oSheetData.toXMLEnd(oStreamWriter);
		}
		else if(m_arWorksheets.size() == m_oWorkbook.GetActiveSheetIndex())
		{
			m_oSaveParams.pCSVWriter->WriteSheetStart(m_pCurWorksheet.GetPointer());
			READ1_DEF(length, res, this->ReadSheetData, NULL);
			m_oSaveParams.pCSVWriter->WriteSheetEnd(m_pCurWorksheet.GetPointer());
		}
	SEEK_TO_POS_END2()
	SEEK_TO_POS_ELSE()
		OOX::Spreadsheet::CSheetData oSheetData;
		oSheetData.toXMLStart(oStreamWriter);
		oSheetData.toXMLEnd(oStreamWriter);
	SEEK_TO_POS_ELSE_END()
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Protection);
	OOX::Spreadsheet::CSheetProtection oProtection;
	READ2_DEF_SPREADSHEET(length, res, this->ReadProtection, &oProtection);
	SEEK_TO_POS_END(oProtection);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::ProtectedRanges);
	OOX::Spreadsheet::CProtectedRanges oProtectedRanges;
	READ1_DEF(length, res, this->ReadProtectedRanges, &oProtectedRanges);
	SEEK_TO_POS_END(oProtectedRanges);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Autofilter);
		OOX::Spreadsheet::CAutofilter oAutofilter;
		BinaryTableReader oBinaryTableReader(m_oBufferedStream, m_pCurWorksheet.GetPointer());
		READ1_DEF(length, res, oBinaryTableReader.ReadAutoFilter, &oAutofilter);
	SEEK_TO_POS_END(oAutofilter);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SortState);
		OOX::Spreadsheet::CSortState oSortState;
		BinaryTableReader oBinaryTableReader(m_oBufferedStream, m_pCurWorksheet.GetPointer());
		READ1_DEF(length, res, oBinaryTableReader.ReadSortState, &oSortState);
	SEEK_TO_POS_END(oSortState);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::MergeCells);
		OOX::Spreadsheet::CMergeCells oMergeCells;
		READ1_DEF(length, res, this->ReadMergeCells, &oMergeCells);
		oMergeCells.m_oCount.Init();
		oMergeCells.m_oCount->SetValue((unsigned int)oMergeCells.m_arrItems.size());
	SEEK_TO_POS_END(oMergeCells);
//-------------------------------------------------------------------------------------------------------------
	OOX::Drawing::COfficeArtExtension* pOfficeArtExtensionCF = new OOX::Drawing::COfficeArtExtension();

	SEEK_TO_POS_START(c_oSerWorksheetsTypes::ConditionalFormatting);
		OOX::Spreadsheet::CConditionalFormatting *pConditionalFormatting = new OOX::Spreadsheet::CConditionalFormatting();
		READ1_DEF(length, res, this->ReadConditionalFormatting, pConditionalFormatting);

		if (pConditionalFormatting->IsExtended())
		{
			pOfficeArtExtensionCF->m_arrConditionalFormatting.push_back(pConditionalFormatting); 
		}
		else
		{
			pConditionalFormatting->toXML(oStreamWriter);
			delete pConditionalFormatting;
		}
	SEEK_TO_POS_END2();

	if (pOfficeArtExtensionCF->m_arrConditionalFormatting.empty())
	{
		delete pOfficeArtExtensionCF;
	}
	else
	{
		pOfficeArtExtensionCF->m_sUri.Init();
		pOfficeArtExtensionCF->m_sUri = L"{78C0D931-6437-407d-A8EE-F0AAD7539E65}";
		pOfficeArtExtensionCF->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_pCurWorksheet->m_oExtLst.IsInit() == false)
			m_pCurWorksheet->m_oExtLst.Init();
		m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtensionCF);
	}
//-------------------------------------------------------------------------------------------------------------
	OOX::Spreadsheet::CDataValidations oDataValidations;

	SEEK_TO_POS_START(c_oSerWorksheetsTypes::DataValidations);
		READ1_DEF(length, res, this->ReadDataValidations, &oDataValidations);
	SEEK_TO_POS_END2();

	OOX::Drawing::COfficeArtExtension* pOfficeArtExtensionDV = new OOX::Drawing::COfficeArtExtension();
	pOfficeArtExtensionDV->m_oDataValidations.Init();
	pOfficeArtExtensionDV->m_oDataValidations->m_oDisablePrompts = oDataValidations.m_oDisablePrompts;
	pOfficeArtExtensionDV->m_oDataValidations->m_oXWindow = oDataValidations.m_oXWindow;
	pOfficeArtExtensionDV->m_oDataValidations->m_oYWindow = oDataValidations.m_oYWindow;

	for (size_t i = 0; i < oDataValidations.m_arrItems.size(); ++i)
	{
		if ((oDataValidations.m_arrItems[i]) && (oDataValidations.m_arrItems[i]->IsExtended()))
		{
			pOfficeArtExtensionDV->m_oDataValidations->m_arrItems.push_back(oDataValidations.m_arrItems[i]); 
			oDataValidations.m_arrItems[i] = NULL;
		}
	}
	size_t i = 0;
	while(!oDataValidations.m_arrItems.empty() && i < oDataValidations.m_arrItems.size())
	{
		if (oDataValidations.m_arrItems[i] == NULL)
			oDataValidations.m_arrItems.erase(oDataValidations.m_arrItems.begin() + i, oDataValidations.m_arrItems.begin() + i + 1);
		else
			i++;
	}

	pOfficeArtExtensionDV->m_oDataValidations->m_oCount = (int)pOfficeArtExtensionDV->m_oDataValidations->m_arrItems.size();
	oDataValidations.m_oCount = (int)oDataValidations.m_arrItems.size();

	oDataValidations.toXML(oStreamWriter);

	if (pOfficeArtExtensionDV->m_oDataValidations->m_arrItems.empty())
	{
		delete pOfficeArtExtensionDV;
	}
	else
	{
		pOfficeArtExtensionDV->m_sUri.Init();
		pOfficeArtExtensionDV->m_sUri = L"{CCE6A557-97BC-4b89-ADB6-D9C93CAAB3DF}";
		pOfficeArtExtensionDV->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_pCurWorksheet->m_oExtLst.IsInit() == false)
			m_pCurWorksheet->m_oExtLst.Init();
		m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtensionDV);
	}
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Hyperlinks);
		OOX::Spreadsheet::CHyperlinks oHyperlinks;
		READ1_DEF(length, res, this->ReadHyperlinks, &oHyperlinks);
	SEEK_TO_POS_END(oHyperlinks);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::PrintOptions);
		OOX::Spreadsheet::CPrintOptions oPrintOptions;
		READ2_DEF_SPREADSHEET(length, res, this->ReadPrintOptions, &oPrintOptions);
	SEEK_TO_POS_END(oPrintOptions);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::PageMargins);
		OOX::Spreadsheet::CPageMargins oPageMargins;
		READ2_DEF_SPREADSHEET(length, res, this->ReadPageMargins, &oPageMargins);
	SEEK_TO_POS_END(oPageMargins);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::PageSetup);
		OOX::Spreadsheet::CPageSetup oPageSetup;
		READ2_DEF_SPREADSHEET(length, res, this->ReadPageSetup, &oPageSetup);
	SEEK_TO_POS_END(oPageSetup);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::HeaderFooter);
		OOX::Spreadsheet::CHeaderFooter oHeaderFooter;
		READ1_DEF(length, res, this->ReadHeaderFooter, &oHeaderFooter);
	SEEK_TO_POS_END(oHeaderFooter);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::RowBreaks);
		OOX::Spreadsheet::CRowColBreaks oRowBreaks;
		READ1_DEF(length, res, this->ReadRowColBreaks, &oRowBreaks);
		oRowBreaks.toXML2(oStreamWriter, L"rowBreaks");
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::ColBreaks);
		OOX::Spreadsheet::CRowColBreaks oColBreaks;
		READ1_DEF(length, res, this->ReadRowColBreaks, &oColBreaks);
		oColBreaks.toXML2(oStreamWriter, L"colBreaks");
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::CellWatches);
	OOX::Spreadsheet::CCellWatches oCellWatches;
	READ1_DEF(length, res, this->ReadCellWatches, &oCellWatches);
	oCellWatches.toXML(oStreamWriter);
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	//important before Drawings
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Comments);
		BinaryCommentReader oBinaryCommentReader(m_oBufferedStream, m_pCurWorksheet.GetPointer());
		oBinaryCommentReader.Read(length, poResult);
		WriteComments();
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------	
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Drawings);

		m_pOfficeDrawingConverter->SetDstContentRels();
		READ1_DEF(length, res, this->ReadDrawings, m_pCurDrawing.GetPointer());

	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	OOX::Spreadsheet::CControls oControls;
	
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Controls);
		READ1_DEF(length, res, this->ReadControls, &oControls);
	//SEEK_TO_POS_END(oControls); ниже ...
	SEEK_TO_POS_END2(); 
//-------------------------------------------------------------------------------------------------------------
	OOX::CPath pathDrawingsDir = m_sDestinationDir  + FILE_SEPARATOR_STR + _T("xl")  + FILE_SEPARATOR_STR + _T("drawings");
	OOX::CPath pathDrawingsRelsDir = pathDrawingsDir.GetPath()  + FILE_SEPARATOR_STR + _T("_rels");
	
	if (false == m_pCurDrawing->IsEmpty() || false == m_pCurVmlDrawing->IsEmpty())
	{
		OOX::CSystemUtility::CreateDirectories(pathDrawingsDir.GetPath());
		OOX::CSystemUtility::CreateDirectories(pathDrawingsRelsDir.GetPath());
	}

	if (false == m_pCurDrawing->IsEmpty())
	{
		NSCommon::smart_ptr<OOX::File> pFile = m_pCurDrawing.smart_dynamic_cast<OOX::File>();
		const OOX::RId oRId = m_pCurWorksheet->Add(pFile);
		
		OOX::Spreadsheet::CDrawingWorksheet oDrawingWorksheet;
		oDrawingWorksheet.m_oId.Init();
		oDrawingWorksheet.m_oId->SetValue(oRId.get());
		oDrawingWorksheet.toXML(oStreamWriter);

		OOX::CPath pathDrawingsRels = pathDrawingsRelsDir.GetPath()  + FILE_SEPARATOR_STR + m_pCurDrawing->m_sOutputFilename + _T(".rels");
		m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());
	}
//-------------------------------------------------------------------------------------------------------------
	if (false == m_pCurVmlDrawing->IsEmpty())
	{
		NSCommon::smart_ptr<OOX::File> pFile = m_pCurVmlDrawing.smart_dynamic_cast<OOX::File>();
		const OOX::RId oRId = m_pCurWorksheet->Add(pFile);
		OOX::Spreadsheet::CLegacyDrawingWorksheet oLegacyDrawing;
		oLegacyDrawing.m_oId.Init();
		oLegacyDrawing.m_oId->SetValue(oRId.get());
		oLegacyDrawing.toXML(oStreamWriter);
	}
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::LegacyDrawingHF);
		OOX::Spreadsheet::CLegacyDrawingHFWorksheet oLegacyDrawingHF;
		READ1_DEF(length, res, this->ReadLegacyDrawingHF, &oLegacyDrawingHF);
	SEEK_TO_POS_END(oLegacyDrawingHF);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Picture);
		std::wstring sPicture = m_pOfficeDrawingConverter->m_pReader->m_strFolder + FILE_SEPARATOR_STR + _T("media")  + FILE_SEPARATOR_STR + m_oBufferedStream.GetString4(length);
		smart_ptr<OOX::File> additionalFile;
		NSBinPptxRW::_relsGeneratorInfo oRelsGeneratorInfo = m_pOfficeDrawingConverter->m_pReader->m_pRels->WriteImage(sPicture, additionalFile, L"", L"");

		NSCommon::smart_ptr<OOX::Image> pImageFileWorksheet(new OOX::Image(NULL, false));
		pImageFileWorksheet->set_filename(oRelsGeneratorInfo.sFilepathImage, false);
		smart_ptr<OOX::File> pFileWorksheet = pImageFileWorksheet.smart_dynamic_cast<OOX::File>();
		OOX::RId oRId = m_pCurWorksheet->Add(pFileWorksheet);

		OOX::Spreadsheet::CPictureWorksheet oPicture;
		oPicture.m_oId.Init();
		oPicture.m_oId->SetValue(oRId.get());
		oPicture.toXML(oStreamWriter);
	SEEK_TO_POS_END2();
	
	if (false == m_pCurOleObjects->m_mapOleObjects.empty())
	{
		m_pCurOleObjects->toXML(oStreamWriter);
	}

	oControls.toXML(oStreamWriter);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::TableParts);
		BinaryTableReader oBinaryTableReader(m_oBufferedStream, m_pCurWorksheet.GetPointer());
		OOX::Spreadsheet::CTableParts oTableParts;
		oBinaryTableReader.Read(length, &oTableParts);
		oTableParts.m_oCount.Init();
		oTableParts.m_oCount->SetValue((unsigned int)oTableParts.m_arrItems.size());
	SEEK_TO_POS_END(oTableParts);
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::QueryTable);
		BinaryTableReader oBinaryTableReader(m_oBufferedStream, m_pCurWorksheet.GetPointer());
		smart_ptr<OOX::Spreadsheet::CQueryTableFile> pQueryTableFile(new OOX::Spreadsheet::CQueryTableFile(NULL));
		pQueryTableFile->m_oQueryTable.Init();

		oBinaryTableReader.ReadQueryTable(length, pQueryTableFile->m_oQueryTable.GetPointer());

		smart_ptr<OOX::File> oFile = pQueryTableFile.smart_dynamic_cast<OOX::File>();
		m_pCurWorksheet->Add(oFile);
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SparklineGroups);
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSparklineGroups.Init();

		READ1_DEF(length, res, this->ReadSparklineGroups, pOfficeArtExtension->m_oSparklineGroups.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{05C60535-1F16-4fd2-B633-F4F36F0B64E0}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_pCurWorksheet->m_oExtLst.IsInit() == false)
			m_pCurWorksheet->m_oExtLst.Init();
		m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);

	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::Slicers);
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSlicerList.Init();
		READ1_DEF(length, res, this->ReadSlicers, pOfficeArtExtension->m_oSlicerList.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{A8765BA9-456A-4dab-B4F3-ACF838C121DE}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"";

		if (m_pCurWorksheet->m_oExtLst.IsInit() == false)
			m_pCurWorksheet->m_oExtLst.Init();
		m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	SEEK_TO_POS_END2();

	SEEK_TO_POS_START(c_oSerWorksheetsTypes::SlicersExt);
		OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
		pOfficeArtExtension->m_oSlicerListExt.Init();
		READ1_DEF(length, res, this->ReadSlicers, pOfficeArtExtension->m_oSlicerListExt.GetPointer());

		pOfficeArtExtension->m_sUri.Init();
		pOfficeArtExtension->m_sUri = L"{3A4CF648-6AED-40f4-86FF-DC5316D8AED3}";
		pOfficeArtExtension->m_sAdditionalNamespace = L"xmlns:x15=\"http://schemas.microsoft.com/office/spreadsheetml/2010/11/main\"";

		if (m_pCurWorksheet->m_oExtLst.IsInit() == false)
			m_pCurWorksheet->m_oExtLst.Init();
		m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
	SEEK_TO_POS_END2();

	if (m_pCurWorksheet->m_oExtLst.IsInit())
	{
		oStreamWriter.WriteString(m_pCurWorksheet->m_oExtLst->toXMLWithNS(L""));
	}
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::WorksheetProp);
		READ2_DEF_SPREADSHEET(length, res, this->ReadWorksheetProp, poResult);
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::PivotTable);
		PivotCachesTemp oPivotCachesTemp;

		READ1_DEF(length, res, this->ReadPivotTable, &oPivotCachesTemp);
		boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>>::const_iterator pair = m_mapPivotCacheDefinitions.find(oPivotCachesTemp.nCacheId);

		if(m_mapPivotCacheDefinitions.end() != pair && NULL != oPivotCachesTemp.pTable)
		{
			NSCommon::smart_ptr<OOX::File> pFileTable(oPivotCachesTemp.pTable);
			oPivotCachesTemp.pTable->AddNoWrite(pair->second, L"../pivotCache");
			m_pCurWorksheet->Add(pFileTable);
		}
		else
		{
			RELEASEOBJECT(oPivotCachesTemp.pTable);
		}
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	SEEK_TO_POS_START(c_oSerWorksheetsTypes::NamedSheetView);
		smart_ptr<OOX::Spreadsheet::CNamedSheetViewFile> pNamedSheetViewFile(new OOX::Spreadsheet::CNamedSheetViewFile(NULL));
		pNamedSheetViewFile->m_oNamedSheetViews.Init();
		pNamedSheetViewFile->m_oNamedSheetViews->fromPPTY(&m_oBufferedStream);
		smart_ptr<OOX::File> oFile = pNamedSheetViewFile.smart_dynamic_cast<OOX::File>();
		m_pCurWorksheet->Add(oFile);
	SEEK_TO_POS_END2();
//-------------------------------------------------------------------------------------------------------------
	m_oBufferedStream.Seek(nOldPos);
	m_pCurWorksheet->toXMLEnd(oStreamWriter);
	return res;
}
void BinaryWorksheetsTableReader::WriteComments()
{
	if (m_pCurWorksheet->m_mapComments.empty()) return;

	m_pCurVmlDrawing->m_mapComments = &m_pCurWorksheet->m_mapComments;

	boost::unordered_map<std::wstring, unsigned int> mapByAuthors;
	OOX::Spreadsheet::CComments* pComments = new OOX::Spreadsheet::CComments(NULL);

	pComments->m_oCommentList.Init();
	std::vector<OOX::Spreadsheet::CComment*>& aComments = pComments->m_oCommentList->m_arrItems;

	pComments->m_oAuthors.Init();

	OOX::Spreadsheet::CThreadedComments* pThreadedComments = NULL;

	for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = m_pCurWorksheet->m_mapComments.begin(); it != m_pCurWorksheet->m_mapComments.end(); ++it)
	{
		OOX::Spreadsheet::CCommentItem* pCommentItem = it->second;
		if (pCommentItem->IsValid())
		{
			OOX::Spreadsheet::CComment* pNewComment = new OOX::Spreadsheet::CComment();
			if (pCommentItem->m_nRow.IsInit() && pCommentItem->m_nCol.IsInit())
			{
				pNewComment->m_oRef.Init();
				pNewComment->m_oRef->SetValue(OOX::Spreadsheet::CCell::combineRef(pCommentItem->m_nRow.get(), pCommentItem->m_nCol.get()));
			}
			if (NULL != pCommentItem->m_pThreadedComment)
			{
				if (NULL == pThreadedComments)
				{
					pThreadedComments = new OOX::Spreadsheet::CThreadedComments(NULL);
					NSCommon::smart_ptr<OOX::File> pThreadedCommentsFile(pThreadedComments);
					m_pCurWorksheet->Add(pThreadedCommentsFile);
				}
				OOX::Spreadsheet::CThreadedComment* pThreadedComment = pCommentItem->m_pThreadedComment;
				if (pNewComment->m_oRef.IsInit())
				{
					pThreadedComment->ref = pNewComment->m_oRef->ToString();
				}
				if (!pThreadedComment->id.IsInit())
				{
					pThreadedComment->id = L"{" + XmlUtils::GenerateGuid() + L"}";
				}
				pNewComment->m_oUid = pThreadedComment->id->ToString();
				pCommentItem->m_sAuthor = L"tc=" + pThreadedComment->id->ToString();

				pCommentItem->m_oText.Init();
				nullable<std::unordered_map<std::wstring, OOX::Spreadsheet::CPerson*>> mapPersonList;
				if (m_oWorkbook.m_pPersonList)
				{
					mapPersonList = m_oWorkbook.m_pPersonList->GetPersonList();
				}
				BinaryCommentReader::addThreadedComment(pCommentItem->m_oText.get2(), pThreadedComment, mapPersonList);

				pThreadedComments->m_arrItems.push_back(pThreadedComment);
				for (size_t i = 0; i < pThreadedComment->m_arrReplies.size(); ++i)
				{
					pThreadedComment->m_arrReplies[i]->parentId = pThreadedComment->id->ToString();
					pThreadedComment->m_arrReplies[i]->ref = pThreadedComment->ref.get();
					if (!pThreadedComment->m_arrReplies[i]->id.IsInit())
					{
						pThreadedComment->m_arrReplies[i]->id = L"{" + XmlUtils::GenerateGuid() + L"}";
					}
					pThreadedComments->m_arrItems.push_back(pThreadedComment->m_arrReplies[i]);
				}
			}

			if (pCommentItem->m_sAuthor.IsInit())
			{
				const std::wstring& sAuthor = pCommentItem->m_sAuthor.get();
				boost::unordered_map<std::wstring, unsigned int>::const_iterator pFind = mapByAuthors.find(sAuthor);

				int nAuthorId;
				if (pFind != mapByAuthors.end())
					nAuthorId = (int)pFind->second;
				else
				{
					nAuthorId = (int)mapByAuthors.size();

					mapByAuthors.insert(std::make_pair(sAuthor, nAuthorId));

					pComments->m_oAuthors->m_arrItems.push_back(sAuthor);
				}
				pNewComment->m_oAuthorId.Init();
				pNewComment->m_oAuthorId->SetValue(nAuthorId);
			}
			pNewComment->m_oText.reset(pCommentItem->m_oText.GetPointerEmptyNullable());

			aComments.push_back(pNewComment);
		}
		else if (NULL != pCommentItem->m_pThreadedComment)
		{
			RELEASEOBJECT(pCommentItem->m_pThreadedComment);
			for (size_t i = 0; i < pCommentItem->m_pThreadedComment->m_arrReplies.size(); ++i)
			{
				RELEASEOBJECT(pCommentItem->m_pThreadedComment->m_arrReplies[i]);
			}
		}
	}

	NSCommon::smart_ptr<OOX::File> pCommentsFile(pComments);
	m_pCurWorksheet->Add(pCommentsFile);
}
int BinaryWorksheetsTableReader::ReadPivotTable(BYTE type, long length, void* poResult)
{
	PivotCachesTemp* pPivotCachesTemp = static_cast<PivotCachesTemp*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_PivotTypes::cacheId == type)
	{
		pPivotCachesTemp->nCacheId =m_oBufferedStream.GetLong();
	}
	else if(c_oSer_PivotTypes::table == type)
	{
		OOX::Spreadsheet::CPivotTableFile* pPivotTable = new OOX::Spreadsheet::CPivotTableFile(NULL);
		pPivotTable->setData(m_oBufferedStream.GetPointer(length), length);
		pPivotCachesTemp->pTable = pPivotTable;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int BinaryWorksheetsTableReader::ReadWorksheetProp(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetPropTypes::Name == type)
	{
		m_pCurSheet->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerWorksheetPropTypes::SheetId == type)
	{
		m_pCurSheet->m_oSheetId = m_oBufferedStream.GetLong();
	}
	else if(c_oSerWorksheetPropTypes::State == type)
	{
		m_pCurSheet->m_oState = (SimpleTypes::Spreadsheet::EVisibleType)m_oBufferedStream.GetUChar();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadWorksheetCols(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCols* pCols = static_cast<OOX::Spreadsheet::CCols*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::Col == type)
	{
		OOX::Spreadsheet::CCol* pCol = new OOX::Spreadsheet::CCol();
		READ2_DEF_SPREADSHEET(length, res, this->ReadWorksheetCol, pCol);
		pCols->m_arrItems.push_back(pCol);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadWorksheetCol(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCol* pCol = static_cast<OOX::Spreadsheet::CCol*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetColTypes::BestFit == type)
	{
		pCol->m_oBestFit.Init();
		pCol->m_oBestFit->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerWorksheetColTypes::Hidden == type)
	{
		pCol->m_oHidden.Init();
		pCol->m_oHidden->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerWorksheetColTypes::Max == type)
	{
		pCol->m_oMax.Init();
		pCol->m_oMax->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerWorksheetColTypes::Min == type)
	{
		pCol->m_oMin.Init();
		pCol->m_oMin->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerWorksheetColTypes::Style == type)
	{
		pCol->m_oStyle.Init();
		pCol->m_oStyle->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerWorksheetColTypes::Width == type)
	{
		pCol->m_oWidth.Init();
		pCol->m_oWidth->SetValue(m_oBufferedStream.GetDoubleReal());
		if(g_nCurFormatVersion < 2)
		{
			pCol->m_oCustomWidth.Init();
			pCol->m_oCustomWidth->SetValue(SimpleTypes::onoffTrue);
		}
	}
	else if(c_oSerWorksheetColTypes::CustomWidth == type)
	{
		pCol->m_oCustomWidth.Init();
		pCol->m_oCustomWidth->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerWorksheetColTypes::OutLevel == type)
	{
		pCol->m_oOutlineLevel.Init();
		pCol->m_oOutlineLevel->SetValue( m_oBufferedStream.GetLong());
	}
	else if(c_oSerWorksheetColTypes::Collapsed == type)
	{
		pCol->m_oCollapsed.Init();
		pCol->m_oCollapsed->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadProtectedRanges(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CProtectedRanges* pProtectedRanges = static_cast<OOX::Spreadsheet::CProtectedRanges*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if (c_oSerWorksheetsTypes::ProtectedRange == type)
	{
		OOX::Spreadsheet::CProtectedRange* pProtectedRange = new OOX::Spreadsheet::CProtectedRange();
		READ2_DEF_SPREADSHEET(length, res, this->ReadProtectedRange, pProtectedRange);
		pProtectedRanges->m_arrItems.push_back(pProtectedRange);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadProtectedRange(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CProtectedRange* pProtectedRange = static_cast<OOX::Spreadsheet::CProtectedRange*>(poResult);
	int res = c_oSerConstants::ReadOk;

	if (c_oSerProtectedRangeTypes::AlgorithmName == type)
	{
		pProtectedRange->m_oAlgorithmName.Init();
		pProtectedRange->m_oAlgorithmName->SetValue((SimpleTypes::ECryptAlgoritmName)m_oBufferedStream.GetUChar());
	}
	else if (c_oSerProtectedRangeTypes::SpinCount == type)
	{
		pProtectedRange->m_oSpinCount.Init();
		pProtectedRange->m_oSpinCount->SetValue(m_oBufferedStream.GetULong());
	}
	else if (c_oSerProtectedRangeTypes::HashValue == type)
	{
		pProtectedRange->m_oHashValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerProtectedRangeTypes::SaltValue == type)
	{
		pProtectedRange->m_oSaltValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerProtectedRangeTypes::SqRef == type)
	{
		pProtectedRange->m_oSqref = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerProtectedRangeTypes::Name == type)
	{
		pProtectedRange->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerProtectedRangeTypes::SecurityDescriptor == type)
	{
		pProtectedRange->m_arSecurityDescriptors.push_back(m_oBufferedStream.GetString4(length));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}

int BinaryWorksheetsTableReader::ReadSheetViews(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSheetViews* pSheetViews = static_cast<OOX::Spreadsheet::CSheetViews*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::SheetView == type)
	{
		OOX::Spreadsheet::CSheetView* pSheetView = new OOX::Spreadsheet::CSheetView();
		READ1_DEF(length, res, this->ReadSheetView, pSheetView);
		pSheetViews->m_arrItems.push_back(pSheetView);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadSheetView(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSheetView* pSheetView = static_cast<OOX::Spreadsheet::CSheetView*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_SheetView::ColorId == type)
	{
		pSheetView->m_oColorId.Init();
		pSheetView->m_oColorId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_SheetView::DefaultGridColor == type)
	{
		pSheetView->m_oDefaultGridColor.Init();
		pSheetView->m_oDefaultGridColor->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::RightToLeft == type)
	{
		pSheetView->m_oRightToLeft.Init();
		pSheetView->m_oRightToLeft->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowFormulas == type)
	{
		pSheetView->m_oShowFormulas.Init();
		pSheetView->m_oShowFormulas->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowGridLines == type)
	{
		pSheetView->m_oShowGridLines.Init();
		pSheetView->m_oShowGridLines->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowOutlineSymbols == type)
	{
		pSheetView->m_oShowOutlineSymbols.Init();
		pSheetView->m_oShowOutlineSymbols->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowRowColHeaders == type)
	{
		pSheetView->m_oShowRowColHeaders.Init();
		pSheetView->m_oShowRowColHeaders->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowRuler == type)
	{
		pSheetView->m_oShowRuler.Init();
		pSheetView->m_oShowRuler->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowWhiteSpace == type)
	{
		pSheetView->m_oShowWhiteSpace.Init();
		pSheetView->m_oShowWhiteSpace->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::ShowZeros == type)
	{
		pSheetView->m_oShowZeros.Init();
		pSheetView->m_oShowZeros->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::TabSelected == type)
	{
		pSheetView->m_oTabSelected.Init();
		pSheetView->m_oTabSelected->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::TopLeftCell == type)
	{
		pSheetView->m_oTopLeftCell = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_SheetView::View == type)
	{
		pSheetView->m_oView.Init();
		pSheetView->m_oView->SetValue((SimpleTypes::Spreadsheet::ESheetViewType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_SheetView::WindowProtection == type)
	{
		pSheetView->m_oWindowProtection.Init();
		pSheetView->m_oWindowProtection->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetView::WorkbookViewId == type)
	{
		pSheetView->m_oWorkbookViewId.Init();
		pSheetView->m_oWorkbookViewId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_SheetView::ZoomScale == type)
	{
		pSheetView->m_oZoomScale.Init();
		pSheetView->m_oZoomScale->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_SheetView::ZoomScaleNormal == type)
	{
		pSheetView->m_oZoomScaleNormal.Init();
		pSheetView->m_oZoomScaleNormal->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_SheetView::ZoomScalePageLayoutView == type)
	{
		pSheetView->m_oZoomScalePageLayoutView.Init();
		pSheetView->m_oZoomScalePageLayoutView->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_SheetView::ZoomScaleSheetLayoutView == type)
	{
		pSheetView->m_oZoomScaleSheetLayoutView.Init();
		pSheetView->m_oZoomScaleSheetLayoutView->SetValue(m_oBufferedStream.GetLong());
	}
	else if (c_oSer_SheetView::Pane == type)
	{
		pSheetView->m_oPane.Init();
		READ1_DEF(length, res, this->ReadPane, pSheetView->m_oPane.GetPointer());
	}
	else if (c_oSer_SheetView::Selection == type)
	{
		OOX::Spreadsheet::CSelection* pSelection = new OOX::Spreadsheet::CSelection();
		READ1_DEF(length, res, this->ReadSelection, pSelection);
		pSheetView->m_arrItems.push_back(pSelection);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPane(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPane* pPane = static_cast<OOX::Spreadsheet::CPane*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_Pane::ActivePane == type)
	{
		pPane->m_oActivePane.Init();
		pPane->m_oActivePane->SetValue((SimpleTypes::Spreadsheet::EActivePane)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_Pane::State == type)
	{
		pPane->m_oState.Init();
        std::wstring sVal = m_oBufferedStream.GetString4(length);
        pPane->m_oState->FromString(sVal.c_str());
	}
	else if(c_oSer_Pane::TopLeftCell == type)
	{
		pPane->m_oTopLeftCell = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_Pane::XSplit == type)
	{
		pPane->m_oXSplit.Init();
		pPane->m_oXSplit->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_Pane::YSplit == type)
	{
		pPane->m_oYSplit.Init();
		pPane->m_oYSplit->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadSelection(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSelection* pSelection = static_cast<OOX::Spreadsheet::CSelection*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_Selection::ActiveCell == type)
	{
		pSelection->m_oActiveCell = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_Selection::ActiveCellId == type)
	{
		pSelection->m_oActiveCellId.Init();
		pSelection->m_oActiveCellId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_Selection::Sqref == type)
	{
		pSelection->m_oSqref = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_Selection::Pane == type)
	{
		pSelection->m_oPane.Init();
		pSelection->m_oPane->SetValue((SimpleTypes::Spreadsheet::EActivePane)m_oBufferedStream.GetUChar());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadSheetPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSheetPr* pSheetPr = static_cast<OOX::Spreadsheet::CSheetPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_SheetPr::CodeName == type)
	{
		pSheetPr->m_oCodeName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_SheetPr::EnableFormatConditionsCalculation == type)
	{
		pSheetPr->m_oEnableFormatConditionsCalculation.Init();
		pSheetPr->m_oEnableFormatConditionsCalculation->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::FilterMode == type)
	{
		pSheetPr->m_oFilterMode.Init();
		pSheetPr->m_oFilterMode->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::Published == type)
	{
		pSheetPr->m_oPublished.Init();
		pSheetPr->m_oPublished->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::SyncHorizontal == type)
	{
		pSheetPr->m_oSyncHorizontal.Init();
		pSheetPr->m_oSyncHorizontal->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::SyncRef == type)
	{
		pSheetPr->m_oSyncRef = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_SheetPr::SyncVertical == type)
	{
		pSheetPr->m_oSyncVertical.Init();
		pSheetPr->m_oSyncVertical->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::TransitionEntry == type)
	{
		pSheetPr->m_oTransitionEntry.Init();
		pSheetPr->m_oTransitionEntry->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::TransitionEvaluation == type)
	{
		pSheetPr->m_oTransitionEvaluation.Init();
		pSheetPr->m_oTransitionEvaluation->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSer_SheetPr::TabColor == type)
	{
		pSheetPr->m_oTabColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSheetPr->m_oTabColor.GetPointer());
	}
	else if(c_oSer_SheetPr::PageSetUpPr == type)
	{
		pSheetPr->m_oPageSetUpPr.Init();
		READ1_DEF(length, res, this->ReadPageSetUpPr, pSheetPr->m_oPageSetUpPr.GetPointer());
	}
	else if(c_oSer_SheetPr::OutlinePr == type)
	{
		pSheetPr->m_oOutlinePr.Init();
		READ1_DEF(length, res, this->ReadOutlinePr, pSheetPr->m_oOutlinePr.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadOutlinePr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::COutlinePr* pOutlinePr = static_cast<OOX::Spreadsheet::COutlinePr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_SheetPr::ApplyStyles == type)
	{
		pOutlinePr->m_oApplyStyles.Init();
		pOutlinePr->m_oApplyStyles->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetPr::ShowOutlineSymbols == type)
	{
		pOutlinePr->m_oShowOutlineSymbols.Init();
		pOutlinePr->m_oShowOutlineSymbols->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetPr::SummaryBelow == type)
	{
		pOutlinePr->m_oSummaryBelow.Init();
		pOutlinePr->m_oSummaryBelow->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetPr::SummaryRight == type)
	{
		pOutlinePr->m_oSummaryRight.Init();
		pOutlinePr->m_oSummaryRight->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPageSetUpPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPageSetUpPr* pPageSetUpPr = static_cast<OOX::Spreadsheet::CPageSetUpPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_SheetPr::AutoPageBreaks == type)
	{
		pPageSetUpPr->m_oAutoPageBreaks.Init();
		pPageSetUpPr->m_oAutoPageBreaks->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_SheetPr::FitToPage == type)
	{
		pPageSetUpPr->m_oFitToPage.Init();
		pPageSetUpPr->m_oFitToPage->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadColor(BYTE type, long length, void* poResult)
{
	return m_oBcr2.ReadColor(type, length, poResult);
}
int BinaryWorksheetsTableReader::ReadSheetFormatPr(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSheetFormatPr* pSheetFormatPr = static_cast<OOX::Spreadsheet::CSheetFormatPr*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerSheetFormatPrTypes::DefaultColWidth == type)
	{
		pSheetFormatPr->m_oDefaultColWidth = m_oBufferedStream.GetDoubleReal();
	}
	else if(c_oSerSheetFormatPrTypes::DefaultRowHeight == type)
	{
		pSheetFormatPr->m_oDefaultRowHeight = m_oBufferedStream.GetDoubleReal();
	}
	else if (c_oSerSheetFormatPrTypes::BaseColWidth == type)
	{
		pSheetFormatPr->m_oBaseColWidth = m_oBufferedStream.GetLong();
	}
	else if (c_oSerSheetFormatPrTypes::CustomHeight == type)
	{
		pSheetFormatPr->m_oCustomHeight = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSheetFormatPrTypes::ZeroHeight == type)
	{
		pSheetFormatPr->m_oZeroHeight = m_oBufferedStream.GetBool();
	}
	else if (c_oSerSheetFormatPrTypes::OutlineLevelCol == type)
	{
		pSheetFormatPr->m_oOutlineLevelCol = m_oBufferedStream.GetLong();
	}
	else if (c_oSerSheetFormatPrTypes::OutlineLevelRow == type)
	{
		pSheetFormatPr->m_oOutlineLevelRow = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPageMargins(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPageMargins* pPageMargins = static_cast<OOX::Spreadsheet::CPageMargins*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_PageMargins::Left == type)
	{
		pPageMargins->m_oLeft.Init();
		pPageMargins->m_oLeft->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageMargins::Top == type)
	{
		pPageMargins->m_oTop.Init();
		pPageMargins->m_oTop->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageMargins::Right == type)
	{
		pPageMargins->m_oRight.Init();
		pPageMargins->m_oRight->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageMargins::Bottom == type)
	{
		pPageMargins->m_oBottom.Init();
		pPageMargins->m_oBottom->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageMargins::Header == type)
	{
		pPageMargins->m_oHeader.Init();
		pPageMargins->m_oHeader->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageMargins::Footer == type)
	{
		pPageMargins->m_oFooter.Init();
		pPageMargins->m_oFooter->FromMm(m_oBufferedStream.GetDoubleReal());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPageSetup(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPageSetup* pPageSetup = static_cast<OOX::Spreadsheet::CPageSetup*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_PageSetup::BlackAndWhite == type)
	{
		pPageSetup->m_oBlackAndWhite.Init();
		pPageSetup->m_oBlackAndWhite->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_PageSetup::CellComments == type)
	{
		pPageSetup->m_oCellComments.Init();
		pPageSetup->m_oCellComments->SetValue((SimpleTypes::Spreadsheet::ECellComments)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::Copies == type)
	{
		pPageSetup->m_oCopies.Init();
		pPageSetup->m_oCopies->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_PageSetup::Draft == type)
	{
		pPageSetup->m_oDraft.Init();
		pPageSetup->m_oDraft->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_PageSetup::Errors == type)
	{
		pPageSetup->m_oErrors.Init();
		pPageSetup->m_oErrors->SetValue((SimpleTypes::Spreadsheet::EPrintError)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::FirstPageNumber == type)
	{
		pPageSetup->m_oFirstPageNumber.Init();
		pPageSetup->m_oFirstPageNumber->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_PageSetup::FitToHeight == type)
	{
		pPageSetup->m_oFitToHeight.Init();
		pPageSetup->m_oFitToHeight->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_PageSetup::FitToWidth == type)
	{
		pPageSetup->m_oFitToWidth.Init();
		pPageSetup->m_oFitToWidth->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_PageSetup::HorizontalDpi == type)
	{
		pPageSetup->m_oHorizontalDpi.Init();
		pPageSetup->m_oHorizontalDpi->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_PageSetup::Orientation == type)
	{
		pPageSetup->m_oOrientation.Init();
		pPageSetup->m_oOrientation->SetValue((SimpleTypes::EPageOrientation)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::PageOrder == type)
	{
		pPageSetup->m_oPageOrder.Init();
		pPageSetup->m_oPageOrder->SetValue((SimpleTypes::Spreadsheet::EPageOrder)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::PaperHeight == type)
	{
		pPageSetup->m_oPaperHeight.Init();
		pPageSetup->m_oPaperHeight->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageSetup::PaperSize == type)
	{
		pPageSetup->m_oPaperSize.Init();
		pPageSetup->m_oPaperSize->SetValue((SimpleTypes::Spreadsheet::EPageSize)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::PaperWidth == type)
	{
		pPageSetup->m_oPaperWidth.Init();
		pPageSetup->m_oPaperWidth->SetValue(m_oBufferedStream.GetDoubleReal());
	}
	else if(c_oSer_PageSetup::PaperUnits == type)
	{
		pPageSetup->m_oPaperUnits.Init();
		pPageSetup->m_oPaperUnits->SetValue((SimpleTypes::Spreadsheet::EPageUnits)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_PageSetup::Scale == type)
	{
		pPageSetup->m_oScale.Init();
		pPageSetup->m_oScale->SetValue(m_oBufferedStream.GetULong());
	}
	else if(c_oSer_PageSetup::UseFirstPageNumber == type)
	{
		pPageSetup->m_oUseFirstPageNumber.Init();
		pPageSetup->m_oUseFirstPageNumber->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_PageSetup::UsePrinterDefaults == type)
	{
		pPageSetup->m_oUsePrinterDefaults.Init();
		pPageSetup->m_oUsePrinterDefaults->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_PageSetup::VerticalDpi == type)
	{
		pPageSetup->m_oVerticalDpi.Init();
		pPageSetup->m_oVerticalDpi->SetValue(m_oBufferedStream.GetULong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadProtection(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CSheetProtection* pProtection = static_cast<OOX::Spreadsheet::CSheetProtection*>(poResult);
	int res = c_oSerConstants::ReadOk;
	
	if (c_oSerWorksheetProtection::AlgorithmName == type)
	{
		pProtection->m_oAlgorithmName.Init();
		pProtection->m_oAlgorithmName->SetValue((SimpleTypes::ECryptAlgoritmName)m_oBufferedStream.GetUChar());
	}
	else if (c_oSerWorksheetProtection::SpinCount == type)
	{
		pProtection->m_oSpinCount.Init();
		pProtection->m_oSpinCount->SetValue(m_oBufferedStream.GetULong());
	}
	else if (c_oSerWorksheetProtection::HashValue == type)
	{
		pProtection->m_oHashValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorksheetProtection::SaltValue == type)
	{
		pProtection->m_oSaltValue = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorksheetProtection::Password == type)
	{
		pProtection->m_oPassword = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerWorksheetProtection::AutoFilter == type)
	{
		pProtection->m_oAutoFilter.Init();
		pProtection->m_oAutoFilter->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::Content == type)
	{
		pProtection->m_oContent.Init();
		pProtection->m_oContent->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::DeleteColumns == type)
	{
		pProtection->m_oDeleteColumns.Init();
		pProtection->m_oDeleteColumns->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::DeleteRows == type)
	{
		pProtection->m_oDeleteRows.Init();
		pProtection->m_oDeleteRows->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::FormatCells == type)
	{
		pProtection->m_oFormatCells.Init();
		pProtection->m_oFormatCells->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::FormatColumns == type)
	{
		pProtection->m_oFormatColumns.Init();
		pProtection->m_oFormatColumns->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::FormatRows == type)
	{
		pProtection->m_oFormatRows.Init();
		pProtection->m_oFormatRows->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::InsertColumns == type)
	{
		pProtection->m_oInsertColumns.Init();
		pProtection->m_oInsertColumns->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::InsertHyperlinks == type)
	{
		pProtection->m_oInsertHyperlinks.Init();
		pProtection->m_oInsertHyperlinks->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::InsertRows == type)
	{
		pProtection->m_oInsertRows.Init();
		pProtection->m_oInsertRows->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::Objects == type)
	{
		pProtection->m_oObjects.Init();
		pProtection->m_oObjects->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::PivotTables == type)
	{
		pProtection->m_oPivotTables.Init();
		pProtection->m_oPivotTables->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::Scenarios == type)
	{
		pProtection->m_oScenarios.Init();
		pProtection->m_oScenarios->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::SelectLockedCells == type)
	{
		pProtection->m_oSelectLockedCells.Init();
		pProtection->m_oSelectLockedCells->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::SelectUnlockedCell == type)
	{
		pProtection->m_oSelectUnlockedCells.Init();
		pProtection->m_oSelectUnlockedCells->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::Sheet == type)
	{
		pProtection->m_oSheet.Init();
		pProtection->m_oSheet->FromBool(m_oBufferedStream.GetBool());
	}
	else if (c_oSerWorksheetProtection::Sort == type)
	{
		pProtection->m_oSort.Init();
		pProtection->m_oSort->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
		return res;
}
int BinaryWorksheetsTableReader::ReadHeaderFooter(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CHeaderFooter* pHeaderFooter = static_cast<OOX::Spreadsheet::CHeaderFooter*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_HeaderFooter::AlignWithMargins == type)
	{
		pHeaderFooter->m_oAlignWithMargins.Init();
		pHeaderFooter->m_oAlignWithMargins->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_HeaderFooter::DifferentFirst == type)
	{
		pHeaderFooter->m_oDifferentFirst.Init();
		pHeaderFooter->m_oDifferentFirst->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_HeaderFooter::DifferentOddEven == type)
	{
		pHeaderFooter->m_oDifferentOddEven.Init();
		pHeaderFooter->m_oDifferentOddEven->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_HeaderFooter::ScaleWithDoc == type)
	{
		pHeaderFooter->m_oScaleWithDoc.Init();
		pHeaderFooter->m_oScaleWithDoc->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_HeaderFooter::EvenFooter == type)
	{
		pHeaderFooter->m_oEvenFooter.Init();
		pHeaderFooter->m_oEvenFooter->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_HeaderFooter::EvenHeader == type)
	{
		pHeaderFooter->m_oEvenHeader.Init();
		pHeaderFooter->m_oEvenHeader->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_HeaderFooter::FirstFooter == type)
	{
		pHeaderFooter->m_oFirstFooter.Init();
		pHeaderFooter->m_oFirstFooter->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_HeaderFooter::FirstHeader == type)
	{
		pHeaderFooter->m_oFirstHeader.Init();
		pHeaderFooter->m_oFirstHeader->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_HeaderFooter::OddFooter == type)
	{
		pHeaderFooter->m_oOddFooter.Init();
		pHeaderFooter->m_oOddFooter->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_HeaderFooter::OddHeader == type)
	{
		pHeaderFooter->m_oOddHeader.Init();
		pHeaderFooter->m_oOddHeader->m_sText = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadCellWatches(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCellWatches* pCellWatches = static_cast<OOX::Spreadsheet::CCellWatches*>(poResult);
	if (c_oSerWorksheetsTypes::CellWatch == type)
	{
		pCellWatches->m_arrItems.push_back(new OOX::Spreadsheet::CCellWatch());
		READ2_DEF_SPREADSHEET(length, res, this->ReadCellWatch, pCellWatches->m_arrItems.back());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadCellWatch(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CCellWatch* pCellWatch = static_cast<OOX::Spreadsheet::CCellWatch*>(poResult);
	if (c_oSerWorksheetsTypes::CellWatchR == type)
	{
		pCellWatch->m_oR = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadRowColBreaks(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRowColBreaks* pRowColBreaks = static_cast<OOX::Spreadsheet::CRowColBreaks*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_RowColBreaks::Count == type)
	{
		pRowColBreaks->m_oCount.Init();
		pRowColBreaks->m_oCount->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_RowColBreaks::ManualBreakCount == type)
	{
		pRowColBreaks->m_oManualBreakCount.Init();
		pRowColBreaks->m_oManualBreakCount->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_RowColBreaks::Break == type)
	{
		OOX::Spreadsheet::CBreak* pColBreaks = new OOX::Spreadsheet::CBreak();
		READ1_DEF(length, res, this->ReadBreak, pColBreaks);
		pRowColBreaks->m_arrItems.push_back(pColBreaks);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadBreak(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CBreak* pBreak = static_cast<OOX::Spreadsheet::CBreak*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_RowColBreaks::Id == type)
	{
		pBreak->m_oId.Init();
		pBreak->m_oId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_RowColBreaks::Man == type)
	{
		pBreak->m_oMan.Init();
		pBreak->m_oMan->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_RowColBreaks::Max == type)
	{
		pBreak->m_oMax.Init();
		pBreak->m_oMax->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_RowColBreaks::Min == type)
	{
		pBreak->m_oMin.Init();
		pBreak->m_oMin->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_RowColBreaks::Pt == type)
	{
		pBreak->m_oPt.Init();
		pBreak->m_oPt->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPrintOptions(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPrintOptions* pPrintOptions = static_cast<OOX::Spreadsheet::CPrintOptions*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_PrintOptions::GridLines == type)
	{
		pPrintOptions->m_oGridLines.Init();
		pPrintOptions->m_oGridLines->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_PrintOptions::Headings == type)
	{
		pPrintOptions->m_oHeadings.Init();
		pPrintOptions->m_oHeadings->FromBool(m_oBufferedStream.GetBool());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadHyperlinks(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CHyperlinks* pHyperlinks = static_cast<OOX::Spreadsheet::CHyperlinks*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::Hyperlink == type)
	{
		OOX::Spreadsheet::CHyperlink* pHyperlink = new OOX::Spreadsheet::CHyperlink();
		READ1_DEF(length, res, this->ReadHyperlink, pHyperlink);
		pHyperlinks->m_arrItems.push_back(pHyperlink);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadHyperlink(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CHyperlink* pHyperlink = static_cast<OOX::Spreadsheet::CHyperlink*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerHyperlinkTypes::Ref == type)
	{
		pHyperlink->m_oRef = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerHyperlinkTypes::Display == type)
	{
		pHyperlink->m_oDisplay = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerHyperlinkTypes::Hyperlink == type)
	{
        std::wstring sHyperlink(m_oBufferedStream.GetString3(length));
		const OOX::RId& rId = m_pCurWorksheet->AddHyperlink(sHyperlink);
		pHyperlink->m_oRid.Init();
		pHyperlink->m_oRid->SetValue(rId.get());
	}
	else if(c_oSerHyperlinkTypes::Location == type)
	{
		pHyperlink->m_oLocation = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerHyperlinkTypes::Tooltip == type)
	{
		pHyperlink->m_oTooltip = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadMergeCells(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CMergeCells* pMergeCells = static_cast<OOX::Spreadsheet::CMergeCells*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::MergeCell == type)
	{
		OOX::Spreadsheet::CMergeCell* pMergeCell = new OOX::Spreadsheet::CMergeCell();
		pMergeCell->m_oRef = m_oBufferedStream.GetString4(length);
		pMergeCells->m_arrItems.push_back(pMergeCell);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadDrawings(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDrawing* pDrawing = static_cast<OOX::Spreadsheet::CDrawing*>(poResult);
	int res = c_oSerConstants::ReadOk;

    if(c_oSerWorksheetsTypes::Drawing == type)
	{
		OOX::Spreadsheet::CCellAnchor* pCellAnchor = new OOX::Spreadsheet::CCellAnchor(SimpleTypes::Spreadsheet::CCellAnchorType());
		READ1_DEF(length, res, this->ReadDrawing, pCellAnchor);
		
		pCellAnchor->m_bShapeOle = false;
		pCellAnchor->m_bShapeControl = false;

		bool bAddToDrawing = true;

		if (pCellAnchor->m_oElement.is_init() && pCellAnchor->m_oElement->is<PPTX::Logic::Pic>())
		{
			PPTX::Logic::Pic& oPic = pCellAnchor->m_oElement->as<PPTX::Logic::Pic>();
			if(oPic.oleObject.IsInit() && oPic.oleObject->m_OleObjectFile.IsInit())
			{
				pCellAnchor->m_bShapeOle = oPic.oleObject->isValid();
				if (pCellAnchor->m_bShapeOle)
				{			
					OOX::Spreadsheet::COleObject* pOleObject = new OOX::Spreadsheet::COleObject();
					
					if (oPic.oleObject->m_sProgId.IsInit())						pOleObject->m_oProgId = oPic.oleObject->m_sProgId.get();
					if (oPic.oleObject->m_oDrawAspect.IsInit())
					{
						std::wstring sDrawAspect;
						if(0 == oPic.oleObject->m_oDrawAspect->GetBYTECode())	pOleObject->m_oDvAspect = L"DVASPECT_CONTENT";
						else													pOleObject->m_oDvAspect = L"DVASPECT_ICON";
					}
					if (oPic.oleObject->m_oUpdateMode.IsInit())
					{
						if(0 == oPic.oleObject->m_oUpdateMode->GetBYTECode())	pOleObject->m_oOleUpdate = L"OLEUPDATE_ALWAYS";
						else													pOleObject->m_oOleUpdate = L"OLEUPDATE_ONCALL";
					}
					pOleObject->m_oObjectPr.Init();
					pOleObject->m_oObjectPr->m_oAnchor.Init();

					pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells = oPic.oleObject->m_oSizeWithCells;
					pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells = oPic.oleObject->m_oMoveWithCells;

					pOleObject->m_OleObjectFile	= oPic.oleObject->m_OleObjectFile;

					if(pOleObject->m_OleObjectFile.IsInit())
					{
				//generate ClientData						
						OOX::Vml::CClientData oClientData;
						oClientData.m_oObjectType.Init();
						oClientData.m_oObjectType->SetValue(SimpleTypes::Vml::vmlclientdataobjecttypePict);

						oClientData.m_oSizeWithCells = pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells;
						oClientData.m_oMoveWithCells = pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells;

						oClientData.m_oAnchor = pCellAnchor->toVmlXML();
						
						oPic.m_sClientDataXml = oClientData.toXML();
						
				//add VmlDrawing
						NSBinPptxRW::CXmlWriter oWriter(XMLWRITER_DOC_TYPE_XLSX);
						COOXToVMLGeometry oOOXToVMLRenderer;

						oWriter.m_pOOXToVMLRenderer = &oOOXToVMLRenderer;
						oWriter.m_lObjectIdVML = m_pCurVmlDrawing->m_lObjectIdVML;
						
						NSCommon::smart_ptr<PPTX::Logic::ClrMap> oClrMap;						
						oPic.toXmlWriterVML(&oWriter, m_oSaveParams.pTheme, oClrMap);
						
						std::wstring strXml = oWriter.GetXmlString();								
						
						m_pCurVmlDrawing->m_arObjectXml.push_back(strXml);
						m_lObjectIdVML = m_pCurVmlDrawing->m_lObjectIdVML = oWriter.m_lObjectIdVML;

						pOleObject->m_oShapeId = *oPic.oleObject->m_sShapeId;
                       
						OOX::CPath pathImageCache = pOleObject->m_OleObjectFile->filename_cache();
						smart_ptr<OOX::RId> oRIdImg;
						
						if (pathImageCache.GetPath().empty() == false)
						{
						//add image rels to VmlDrawing
							NSCommon::smart_ptr<OOX::Image> pImageFileVml(new OOX::Image(NULL, false));
							pImageFileVml->set_filename(pathImageCache, false);
							
							smart_ptr<OOX::File> pFileVml = pImageFileVml.smart_dynamic_cast<OOX::File>();
							m_pCurVmlDrawing->Add(*oPic.blipFill.blip->embed, pFileVml);
				
						//add image rels to Worksheet
							NSCommon::smart_ptr<OOX::Image> pImageFileWorksheet(new OOX::Image(NULL, false));

							pImageFileWorksheet->set_filename(pathImageCache, false);
							
							smart_ptr<OOX::File> pFileWorksheet = pImageFileWorksheet.smart_dynamic_cast<OOX::File>();
							
							oRIdImg = new OOX::RId(m_pCurWorksheet->Add(pFileWorksheet));
						}
				//add oleObject rels
                        smart_ptr<OOX::File> pFileObject = pOleObject->m_OleObjectFile.smart_dynamic_cast<OOX::File>();
                        const OOX::RId oRIdBin = m_pCurWorksheet->Add(pFileObject);

						if(!pOleObject->m_oRid.IsInit())
						{
							pOleObject->m_oRid.Init();
						}
						pOleObject->m_oRid->SetValue(oRIdBin.get());
				//ObjectPr
						pOleObject->m_oObjectPr->m_oDefaultSize.Init();
						pOleObject->m_oObjectPr->m_oDefaultSize->FromBool(false);
						pOleObject->m_oObjectPr->m_oRid.Init();
						
						if (oRIdImg.IsInit())
							pOleObject->m_oObjectPr->m_oRid->SetValue(oRIdImg->get());

						//SimpleTypes::Spreadsheet::ECellAnchorType eAnchorType = pCellAnchor->m_oAnchorType.GetValue();
						//if (SimpleTypes::Spreadsheet::cellanchorOneCell == eAnchorType)
						//{
						//	pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells = true;
						//}
						//else if (SimpleTypes::Spreadsheet::cellanchorTwoCell == eAnchorType)
						//{
						//	pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells = true;
						//}
						pOleObject->m_oObjectPr->m_oAnchor->m_oFrom = pCellAnchor->m_oFrom;
						pOleObject->m_oObjectPr->m_oAnchor->m_oTo	= pCellAnchor->m_oTo;

						m_pCurOleObjects->m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
					}
					else
					{
						pCellAnchor->m_bShapeOle = false;
						delete pOleObject;
					}
				}
			}
		}
		else if (pCellAnchor->m_oElement.is_init() && pCellAnchor->m_oElement->is<PPTX::Logic::Shape>())
		{
			PPTX::Logic::Shape& oShape = pCellAnchor->m_oElement->as<PPTX::Logic::Shape>();
			if (oShape.signatureLine.IsInit())
			{
				bAddToDrawing = false;
			//generate ClientData						
				OOX::Vml::CClientData oClientData;
				oClientData.m_oObjectType.Init();
				oClientData.m_oObjectType->SetValue(SimpleTypes::Vml::vmlclientdataobjecttypePict);
				oClientData.m_oSizeWithCells = true;
				oClientData.m_oAnchor = pCellAnchor->toVmlXML();

				oShape.m_sClientDataXml = oClientData.toXML();

			//add VmlDrawing
				NSBinPptxRW::CXmlWriter oWriter(XMLWRITER_DOC_TYPE_XLSX);
				COOXToVMLGeometry oOOXToVMLRenderer;

				oWriter.m_pOOXToVMLRenderer = &oOOXToVMLRenderer;
				oWriter.m_lObjectIdVML = m_pCurVmlDrawing->m_lObjectIdVML;

				NSCommon::smart_ptr<PPTX::Logic::ClrMap> oClrMap;
				oShape.toXmlWriterVML(&oWriter, m_oSaveParams.pTheme, oClrMap, false, true);

				std::wstring strXml = oWriter.GetXmlString();

				m_pCurVmlDrawing->m_arObjectXml.push_back(strXml);
				m_lObjectIdVML = m_pCurVmlDrawing->m_lObjectIdVML = oWriter.m_lObjectIdVML;

				if (oShape.spPr.Fill.m_type == PPTX::Logic::UniFill::blipFill)
				{
					OOX::CPath pathImageCache = oShape.spPr.Fill.as<PPTX::Logic::BlipFill>().blip->imageFilepath;
					
					smart_ptr<OOX::RId> oRIdImg;
					if (pathImageCache.GetPath().empty() == false)
					{
					//add image rels to VmlDrawing
						NSCommon::smart_ptr<OOX::Image> pImageFileVml(new OOX::Image(NULL, false));
						pImageFileVml->set_filename(pathImageCache, false);

						smart_ptr<OOX::File> pFileVml = pImageFileVml.smart_dynamic_cast<OOX::File>();
						m_pCurVmlDrawing->Add(*oShape.spPr.Fill.as<PPTX::Logic::BlipFill>().blip->embed, pFileVml);
					}
				}
			}
		}
		if (bAddToDrawing)
		{
			pDrawing->m_arrItems.push_back(pCellAnchor);
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadCellAnchor(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCellAnchor* pCellAnchor = static_cast<OOX::Spreadsheet::CCellAnchor*>(poResult);
	int res = c_oSerConstants::ReadOk;

	if(c_oSer_DrawingType::Type == type)
	{
		pCellAnchor->setAnchorType((SimpleTypes::Spreadsheet::ECellAnchorType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_DrawingType::EditAs == type)
	{
		pCellAnchor->m_oEditAs.Init();
		pCellAnchor->m_oEditAs->SetValue((SimpleTypes::Spreadsheet::ECellAnchorType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_DrawingType::From == type)
	{
		pCellAnchor->m_oFrom.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadFromTo, pCellAnchor->m_oFrom.GetPointer());
	}
	else if(c_oSer_DrawingType::To == type)
	{
		pCellAnchor->m_oTo.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadFromTo, pCellAnchor->m_oTo.GetPointer());
	}
	else if(c_oSer_DrawingType::Pos == type)
	{
		pCellAnchor->m_oPos.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadPos, pCellAnchor->m_oPos.GetPointer());
	}
	else if(c_oSer_DrawingType::Ext == type)
	{
		pCellAnchor->m_oExt.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadExt, pCellAnchor->m_oExt.GetPointer());
	}
	else if (c_oSer_DrawingType::ClientData == type)
	{
		pCellAnchor->m_oClientData.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadClientData, pCellAnchor->m_oClientData.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadDrawing(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCellAnchor* pCellAnchor = static_cast<OOX::Spreadsheet::CCellAnchor*>(poResult);
	
	int res = ReadCellAnchor(type, length, poResult);

	if (res != c_oSerConstants::ReadUnknown) return res;	
	res = c_oSerConstants::ReadOk;

	if(c_oSer_DrawingType::pptxDrawing == type)
	{
		LONG nOldPos = m_oBufferedStream.GetPos();
		pCellAnchor->m_oElement.Init();

		BYTE typeRec1   = m_oBufferedStream.GetUChar();    // must be 0;
		LONG _e         = m_oBufferedStream.GetPos()   + m_oBufferedStream.GetLong() + 4;

		m_oBufferedStream.Skip(5); // type record (must be 1) + 4 byte - len record

		pCellAnchor->m_oElement->fromPPTY(&m_oBufferedStream);

		if (!pCellAnchor->m_oElement->is_init())
		{
			m_oBufferedStream.Seek(nOldPos);
			res = c_oSerConstants::ReadUnknown;
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadLegacyDrawingHF(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CLegacyDrawingHFWorksheet* pLegacyDrawingHF = static_cast<OOX::Spreadsheet::CLegacyDrawingHFWorksheet*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_LegacyDrawingHF::Drawings == type)
	{
		m_pOfficeDrawingConverter->SetDstContentRels();
		OOX::CVmlDrawing* pVmlDrawing = new OOX::CVmlDrawing(NULL, false);
		pVmlDrawing->m_lObjectIdVML = m_lObjectIdVML;
		READ1_DEF(length, res, this->ReadLegacyDrawingHFDrawings, pVmlDrawing);
		m_lObjectIdVML = pVmlDrawing->m_lObjectIdVML;
		NSCommon::smart_ptr<OOX::File> pVmlDrawingFile(pVmlDrawing);
		const OOX::RId oRId = m_pCurWorksheet->Add(pVmlDrawingFile);
		pLegacyDrawingHF->m_oId.Init();
		pLegacyDrawingHF->m_oId->SetValue(oRId.get());

		OOX::CPath pathVmlsDir = m_sDestinationDir  + FILE_SEPARATOR_STR + _T("xl")  + FILE_SEPARATOR_STR + _T("drawings");
		OOX::CSystemUtility::CreateDirectories(pathVmlsDir.GetPath());
		OOX::CPath pathVmlsRelsDir = pathVmlsDir.GetPath()  + FILE_SEPARATOR_STR + _T("_rels");
		OOX::CSystemUtility::CreateDirectories(pathVmlsRelsDir.GetPath());
		OOX::CPath pathVmlRels = pathVmlsRelsDir.GetPath()  + FILE_SEPARATOR_STR + pVmlDrawingFile->m_sOutputFilename + _T(".rels");
		m_pOfficeDrawingConverter->SaveDstContentRels(pathVmlRels.GetPath());
	}
	else if(c_oSer_LegacyDrawingHF::Cfe == type)
	{
		pLegacyDrawingHF->m_oCfe.Init();
		pLegacyDrawingHF->m_oCfe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Cff == type)
	{
		pLegacyDrawingHF->m_oCff.Init();
		pLegacyDrawingHF->m_oCff->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Cfo == type)
	{
		pLegacyDrawingHF->m_oCfo.Init();
		pLegacyDrawingHF->m_oCfo->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Che == type)
	{
		pLegacyDrawingHF->m_oChe.Init();
		pLegacyDrawingHF->m_oChe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Chf == type)
	{
		pLegacyDrawingHF->m_oChf.Init();
		pLegacyDrawingHF->m_oChf->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Cho == type)
	{
		pLegacyDrawingHF->m_oCho.Init();
		pLegacyDrawingHF->m_oCho->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lfe == type)
	{
		pLegacyDrawingHF->m_oLfe.Init();
		pLegacyDrawingHF->m_oLfe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lff == type)
	{
		pLegacyDrawingHF->m_oLff.Init();
		pLegacyDrawingHF->m_oLff->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lfo == type)
	{
		pLegacyDrawingHF->m_oLfo.Init();
		pLegacyDrawingHF->m_oLfo->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lhe == type)
	{
		pLegacyDrawingHF->m_oLhe.Init();
		pLegacyDrawingHF->m_oLhe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lhf == type)
	{
		pLegacyDrawingHF->m_oLhf.Init();
		pLegacyDrawingHF->m_oLhf->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Lho == type)
	{
		pLegacyDrawingHF->m_oLho.Init();
		pLegacyDrawingHF->m_oLho->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rfe == type)
	{
		pLegacyDrawingHF->m_oRfe.Init();
		pLegacyDrawingHF->m_oRfe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rff == type)
	{
		pLegacyDrawingHF->m_oRff.Init();
		pLegacyDrawingHF->m_oRff->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rfo == type)
	{
		pLegacyDrawingHF->m_oRfo.Init();
		pLegacyDrawingHF->m_oRfo->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rhe == type)
	{
		pLegacyDrawingHF->m_oRhe.Init();
		pLegacyDrawingHF->m_oRhe->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rhf == type)
	{
		pLegacyDrawingHF->m_oRhf.Init();
		pLegacyDrawingHF->m_oRhf->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_LegacyDrawingHF::Rho == type)
	{
		pLegacyDrawingHF->m_oRho.Init();
		pLegacyDrawingHF->m_oRho->SetValue(m_oBufferedStream.GetLong());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadLegacyDrawingHFDrawings(BYTE type, long length, void* poResult)
{
	OOX::CVmlDrawing* pVmlDrawing = static_cast<OOX::CVmlDrawing*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_LegacyDrawingHF::Drawing == type)
	{
		OOX::CVmlDrawing::_vml_shape oVmlShape;
		READ1_DEF(length, res, this->ReadLegacyDrawingHFDrawing, &oVmlShape);
		
		if (NULL != oVmlShape.pElement && !oVmlShape.sXml.empty())
		{
			PPTX::Logic::SpTreeElem* pSpTree = static_cast<PPTX::Logic::SpTreeElem*>(oVmlShape.pElement);
			
			NSBinPptxRW::CXmlWriter						oWriter(XMLWRITER_DOC_TYPE_XLSX);
			COOXToVMLGeometry							oOOXToVMLRenderer;
			NSCommon::smart_ptr<PPTX::Logic::ClrMap>	oClrMap;

			oWriter.m_pOOXToVMLRenderer = &oOOXToVMLRenderer;
			oWriter.m_lObjectIdVML = pVmlDrawing->m_lObjectIdVML;

			//oWriter.m_strId = oVmlShape.sXml.c_str(); //??
			
			pSpTree->toXmlWriterVML(&oWriter, m_oSaveParams.pTheme, oClrMap);
			
			pVmlDrawing->m_lObjectIdVML = oWriter.m_lObjectIdVML;
			pVmlDrawing->m_arObjectXml.push_back(oWriter.GetXmlString());
		}
		RELEASEOBJECT(oVmlShape.pElement);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadLegacyDrawingHFDrawing(BYTE type, long length, void* poResult)
{
	OOX::CVmlDrawing::_vml_shape* poVmlShape = static_cast<OOX::CVmlDrawing::_vml_shape*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_LegacyDrawingHF::DrawingId == type)
	{
		poVmlShape->sXml = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_LegacyDrawingHF::DrawingShape == type)
	{
		PPTX::Logic::SpTreeElem* pSpTree = new PPTX::Logic::SpTreeElem();
		BYTE typeRec1   = m_oBufferedStream.GetUChar();    // must be 0;
		LONG _e         = m_oBufferedStream.GetPos()   + m_oBufferedStream.GetLong() + 4;
		m_oBufferedStream.Skip(5); // type record (must be 1) + 4 byte - len recor
		pSpTree->fromPPTY(&m_oBufferedStream);
		poVmlShape->pElement = pSpTree;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadFromTo(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CFromTo* pFromTo = static_cast<OOX::Spreadsheet::CFromTo*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DrawingFromToType::Col == type)
	{
		pFromTo->m_oCol.Init();
		pFromTo->m_oCol->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DrawingFromToType::ColOff == type)
	{
		double dColOffMm = m_oBufferedStream.GetDoubleReal();
		pFromTo->m_oColOff.Init();
		pFromTo->m_oColOff->FromMm(dColOffMm);
	}
	else if(c_oSer_DrawingFromToType::Row == type)
	{
		pFromTo->m_oRow.Init();
		pFromTo->m_oRow->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_DrawingFromToType::RowOff == type)
	{
		double dRowOffMm = m_oBufferedStream.GetDoubleReal();
		pFromTo->m_oRowOff.Init();
		pFromTo->m_oRowOff->FromMm(dRowOffMm);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadClientData(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CClientData *pClientData = static_cast<OOX::Spreadsheet::CClientData*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if (c_oSer_DrawingClientDataType::fLocksWithSheet == type)
	{
		pClientData->fLocksWithSheet = m_oBufferedStream.GetBool();
	}
	else if (c_oSer_DrawingClientDataType::fPrintsWithSheet == type)
	{
		pClientData->fPrintsWithSheet = m_oBufferedStream.GetBool();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadExt(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CExt* pExt = static_cast<OOX::Spreadsheet::CExt*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DrawingExtType::Cx == type)
	{
		double dCxMm = m_oBufferedStream.GetDoubleReal();
		pExt->m_oCx.Init();
		pExt->m_oCx->FromMm(dCxMm);
	}
	else if(c_oSer_DrawingExtType::Cy == type)
	{
		double dCyMm = m_oBufferedStream.GetDoubleReal();
		pExt->m_oCy.Init();
		pExt->m_oCy->FromMm(dCyMm);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadPos(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CPos* pPos = static_cast<OOX::Spreadsheet::CPos*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DrawingPosType::X == type)
	{
		double dXMm = m_oBufferedStream.GetDoubleReal();
		pPos->m_oX.Init();
		pPos->m_oX->FromMm(dXMm);
	}
	else if(c_oSer_DrawingPosType::Y == type)
	{
		double dYMm = m_oBufferedStream.GetDoubleReal();
		pPos->m_oY.Init();
		pPos->m_oY->FromMm(dYMm);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadSheetData(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSerWorksheetsTypes::XlsbPos == type)
	{
		int nOldPos = m_oBufferedStream.GetPos();
		m_oBufferedStream.Seek(m_oBufferedStream.GetULong());

		OOX::Spreadsheet::CSheetData oSheetData;
		oSheetData.fromXLSB(m_oBufferedStream, m_oBufferedStream.XlsbReadRecordType(), m_oSaveParams.pCSVWriter, *m_pCurStreamWriter);

		m_oBufferedStream.Seek(nOldPos);
		res = c_oSerConstants::ReadUnknown;
	}
	else if(c_oSerWorksheetsTypes::Row == type)
	{
		OOX::Spreadsheet::CRow oRow;
		READ2_DEF_SPREADSHEET(length, res, this->ReadRow, &oRow);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadRow(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRow* pRow = static_cast<OOX::Spreadsheet::CRow*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerRowTypes::Row == type)
	{
		pRow->m_oR.Init();
		pRow->m_oR->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerRowTypes::Style == type)
	{
		pRow->m_oS.Init();
		pRow->m_oS->SetValue(m_oBufferedStream.GetLong());
		pRow->m_oCustomFormat.Init();
		pRow->m_oCustomFormat->FromBool(true);
	}
	else if(c_oSerRowTypes::Height == type)
	{
		pRow->m_oHt.Init();
		pRow->m_oHt->SetValue(m_oBufferedStream.GetDoubleReal());
		if(g_nCurFormatVersion < 2)
		{
			pRow->m_oCustomHeight.Init();
			pRow->m_oCustomHeight->SetValue(SimpleTypes::onoffTrue);
		}
	}
	else if(c_oSerRowTypes::Hidden == type)
	{
		pRow->m_oHidden = m_oBufferedStream.GetBool();
	}
	else if(c_oSerRowTypes::CustomHeight == type)
	{
		pRow->m_oCustomHeight = m_oBufferedStream.GetBool();
	}
	else if(c_oSerRowTypes::OutLevel == type)
	{
		pRow->m_oOutlineLevel.Init();
		pRow->m_oOutlineLevel->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerRowTypes::Collapsed == type)
	{
		pRow->m_oCollapsed = m_oBufferedStream.GetBool();
	}
	else if(c_oSerRowTypes::Cells == type)
	{
		if (NULL == m_oSaveParams.pCSVWriter)
		{
			pRow->toXMLStart(*m_pCurStreamWriter);
			READ1_DEF(length, res, this->ReadCells, pRow);
			pRow->toXMLEnd(*m_pCurStreamWriter);
		}
		else
		{
			m_oSaveParams.pCSVWriter->WriteRowStart(pRow);
			READ1_DEF(length, res, this->ReadCells, pRow);
			m_oSaveParams.pCSVWriter->WriteRowEnd(pRow);
		}
	}

	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadCells(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CRow* pRow = static_cast<OOX::Spreadsheet::CRow*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerRowTypes::Cell == type)
	{
		OOX::Spreadsheet::CCell oCell;
		READ1_DEF(length, res, this->ReadCell, &oCell);

//текст error и формул пишем
		if(NULL != m_pSharedStrings && oCell.m_oType.IsInit() && oCell.m_oValue.IsInit())
		{
			SimpleTypes::Spreadsheet::ECellTypeType eCellType = oCell.m_oType->GetValue();
			bool bMoveText = false;
			if(SimpleTypes::Spreadsheet::celltypeError == eCellType)
				bMoveText = true;
			else if((SimpleTypes::Spreadsheet::celltypeSharedString == eCellType && oCell.m_oFormula.IsInit()))
			{
				bMoveText = true;
				oCell.m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeStr);
			}
			if(bMoveText)
			{
                int nValue = XmlUtils::GetInteger(oCell.m_oValue->ToString());

                if (nValue >= 0 && nValue < (int)m_pSharedStrings->m_arrItems.size())
				{
                    OOX::Spreadsheet::CSi *pSi = m_pSharedStrings->m_arrItems[nValue];
					if(NULL != pSi && !pSi->m_arrItems.empty())
					{
						OOX::Spreadsheet::WritingElement* pWe = pSi->m_arrItems.front();
						if(OOX::et_x_t == pWe->getType())
						{
							OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(pWe);
							oCell.m_oValue->m_sText = pText->m_sText;
							oCell.m_oValue->m_oSpace = pText->m_oSpace;
						}
					}
				}
			}
		}
		if (NULL == m_oSaveParams.pCSVWriter)
		{
			oCell.toXML(*m_pCurStreamWriter);
		}
		else
		{
			m_oSaveParams.pCSVWriter->WriteCell(&oCell);
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;


	return res;
}
int BinaryWorksheetsTableReader::ReadCell(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CCell* pCell = static_cast<OOX::Spreadsheet::CCell*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerCellTypes::Ref == type)
	{
		pCell->setRef(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSerCellTypes::RefRowCol == type)
	{
		int nRow = m_oBufferedStream.GetLong();
		int nCol = m_oBufferedStream.GetLong();
		pCell->setRowCol(nRow - 1, nCol);
	}
	else if(c_oSerCellTypes::Style == type)
	{
		pCell->m_oStyle = m_oBufferedStream.GetULong();
	}
	else if(c_oSerCellTypes::Type == type)
	{
		pCell->m_oType = (SimpleTypes::Spreadsheet::ECellTypeType)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerCellTypes::Formula == type)
	{
		pCell->m_oFormula.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadFormula, pCell->m_oFormula.GetPointer());
	}
	else if(c_oSerCellTypes::Value == type)
	{
		double dValue = m_oBufferedStream.GetDoubleReal();
		pCell->m_oValue.Init();
		pCell->m_oValue->m_sText = OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(dValue);
	}
	else if(c_oSerCellTypes::ValueText == type)
	{
		pCell->m_oValue.Init();
		pCell->m_oValue->m_sText = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSerCellTypes::ValueCache == type)
	{
		pCell->m_oCacheValue = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadControls(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CControls* pControls = static_cast<OOX::Spreadsheet::CControls*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
	if(c_oSerControlTypes::Control == type)
	{		
		nullable<OOX::Spreadsheet::CControl> pControl; pControl.Init();
		
		pControl->m_oFormControlPr.Init();
		
		pControl->m_oControlPr.Init();
		pControl->m_oControlPr->m_oAnchor.Init();
		
		pControl->m_oShapeId = 4096 + m_lObjectIdVML++;

		READ1_DEF(length, res, this->ReadControl, pControl.GetPointer());
		
		std::wstring strXml = GetControlVmlShape(pControl.GetPointer());
				
		m_pCurVmlDrawing->m_arControlXml.push_back(strXml);

		smart_ptr<OOX::Spreadsheet::CCtrlPropFile> pCtrlPropFile(new OOX::Spreadsheet::CCtrlPropFile(NULL));		
		pCtrlPropFile->m_oFormControlPr = pControl->m_oFormControlPr;

		smart_ptr<OOX::File> pFile = pCtrlPropFile.smart_dynamic_cast<OOX::File>();
		pControl->m_oRid = new SimpleTypes::CRelationshipId(m_pCurWorksheet->Add(pFile).get());
	
		pControls->m_mapControls.insert(std::make_pair(pControl->m_oShapeId->GetValue(), pControl));
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadControl(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CControl* pControl = static_cast<OOX::Spreadsheet::CControl*>(poResult);
	
	int res = c_oSerConstants::ReadOk;

	if (c_oSerControlTypes::ControlAnchor == type)
	{	
		OOX::Spreadsheet::CCellAnchor *pCellAnchor = new OOX::Spreadsheet::CCellAnchor(SimpleTypes::Spreadsheet::CCellAnchorType());
		READ1_DEF(length, res, this->ReadCellAnchor, pCellAnchor);		

		pControl->m_oControlPr->m_oAnchor.Init();
		
		SimpleTypes::Spreadsheet::ECellAnchorType eAnchorType = pCellAnchor->m_oAnchorType.GetValue();
		if(SimpleTypes::Spreadsheet::cellanchorOneCell == eAnchorType)
		{
			pControl->m_oControlPr->m_oAnchor->m_oMoveWithCells = true;
		}
		else if(SimpleTypes::Spreadsheet::cellanchorTwoCell == eAnchorType)
		{
			pControl->m_oControlPr->m_oAnchor->m_oSizeWithCells = true;
		}
		pControl->m_oControlPr->m_oAnchor->m_oFrom = pCellAnchor->m_oFrom;
		pControl->m_oControlPr->m_oAnchor->m_oTo = pCellAnchor->m_oTo;

		RELEASEOBJECT(pCellAnchor);
	}
	else if(c_oSerControlTypes::ObjectType == type)
	{	
		pControl->m_oFormControlPr->m_oObjectType = (SimpleTypes::Spreadsheet::EObjectType)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::Name == type)
	{
		pControl->m_oName = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::AltText == type)
	{
		pControl->m_oControlPr->m_oAltText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::AutoFill == type)
	{
		pControl->m_oControlPr->m_oAutoFill = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::AutoLine == type)
	{
		pControl->m_oControlPr->m_oAutoLine = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::AutoPict == type)
	{
		pControl->m_oControlPr->m_oAutoPict = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::DefaultSize == type)
	{
		pControl->m_oControlPr->m_oDefaultSize = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Disabled == type)
	{
		pControl->m_oControlPr->m_oDisabled = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Locked == type)
	{
		pControl->m_oControlPr->m_oLocked = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Macro == type)
	{
		pControl->m_oControlPr->m_oMacro = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::Print == type)
	{
		pControl->m_oControlPr->m_oPrint = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::RecalcAlways == type)
	{
		pControl->m_oControlPr->m_oRecalcAlways = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Checked == type)
	{
		pControl->m_oFormControlPr->m_oChecked = (SimpleTypes::Spreadsheet::EChecked)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::Colored == type)
	{
		pControl->m_oFormControlPr->m_oColored = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::DropLines == type)
	{
		pControl->m_oFormControlPr->m_oDropLines = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::DropStyle == type)
	{
		pControl->m_oFormControlPr->m_oDropStyle = (SimpleTypes::Spreadsheet::EDropStyle)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::Dx == type)
	{
		pControl->m_oFormControlPr->m_oDx = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::FirstButton == type)
	{
		pControl->m_oFormControlPr->m_oFirstButton = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::FmlaGroup == type)
	{
		pControl->m_oFormControlPr->m_oFmlaGroup = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::FmlaLink == type)
	{
		pControl->m_oFormControlPr->m_oFmlaLink = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::FmlaRange == type)
	{
		pControl->m_oFormControlPr->m_oFmlaRange = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::FmlaTxbx == type)
	{
		pControl->m_oFormControlPr->m_oFmlaTxbx = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::Horiz == type)
	{
		pControl->m_oFormControlPr->m_oHoriz = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Inc == type)
	{
		pControl->m_oFormControlPr->m_oInc = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::JustLastX == type)
	{
		pControl->m_oFormControlPr->m_oJustLastX = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::LockText == type)
	{
		pControl->m_oFormControlPr->m_oLockText = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Max == type)
	{
		pControl->m_oFormControlPr->m_oMax = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::Min == type)
	{
		pControl->m_oFormControlPr->m_oMin = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::MultiSel == type)
	{
		pControl->m_oFormControlPr->m_oMultiSel = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::NoThreeD == type)
	{
		pControl->m_oFormControlPr->m_oNoThreeD = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::NoThreeD2 == type)
	{
		pControl->m_oFormControlPr->m_oNoThreeD2 = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Page == type)
	{
		pControl->m_oFormControlPr->m_oPage = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::Sel == type)
	{
		pControl->m_oFormControlPr->m_oSel = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::SelType == type)
	{
		pControl->m_oFormControlPr->m_oSelType = (SimpleTypes::Spreadsheet::ESelType)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::TextHAlign == type)
	{
		pControl->m_oFormControlPr->m_oTextHAlign = (SimpleTypes::Spreadsheet::EHorizontalAlignment)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::TextVAlign == type)
	{
		pControl->m_oFormControlPr->m_oTextVAlign = (SimpleTypes::Spreadsheet::EVerticalAlignment)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::Val == type)
	{
		pControl->m_oFormControlPr->m_oVal = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::WidthMin == type)
	{
		pControl->m_oFormControlPr->m_oWidthMin = m_oBufferedStream.GetULong();
	}
	else if(c_oSerControlTypes::EditVal == type)
	{
		pControl->m_oFormControlPr->m_oEditVal = (SimpleTypes::Spreadsheet::EEditValidation)m_oBufferedStream.GetUChar();
	}
	else if(c_oSerControlTypes::MultiLine == type)
	{
		pControl->m_oFormControlPr->m_oMultiLine = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::VerticalBar == type)
	{
		pControl->m_oFormControlPr->m_oVerticalBar = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::PasswordEdit == type)
	{
		pControl->m_oFormControlPr->m_oPasswordEdit = m_oBufferedStream.GetBool();
	}
	else if(c_oSerControlTypes::Text == type)
	{
		pControl->m_oFormControlPr->m_oText = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSerControlTypes::ItemLst == type)
	{
		pControl->m_oFormControlPr->m_oItemLst.Init();
		READ1_DEF(length, res, this->ReadControlItems, pControl->m_oFormControlPr->m_oItemLst.GetPointer());
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadControlItems(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CListItems* pItems = static_cast<OOX::Spreadsheet::CListItems*>(poResult);

	int res = c_oSerConstants::ReadOk;
	if(c_oSerControlTypes::Item == type)
	{
		nullable<OOX::Spreadsheet::CListItem> pItem; pItem.Init();
		pItem->m_oVal = m_oBufferedStream.GetString4(length);

		pItems->m_arrItems.push_back(pItem);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
std::wstring BinaryWorksheetsTableReader::GetControlVmlShape(void* pC)
{
	OOX::Spreadsheet::CControl* pControl = static_cast<OOX::Spreadsheet::CControl*>(pC);
//generate ClientData
		
	std::wstring sAnchor;
		sAnchor += pControl->m_oControlPr->m_oAnchor->m_oFrom->m_oCol->ToString()					+ L",";
		sAnchor += std::to_wstring(pControl->m_oControlPr->m_oAnchor->m_oFrom->m_oColOff->ToPx())	+ L",";
		sAnchor += pControl->m_oControlPr->m_oAnchor->m_oFrom->m_oRow->ToString()					+ L",";
		sAnchor += std::to_wstring(pControl->m_oControlPr->m_oAnchor->m_oFrom->m_oRowOff->ToPx())	+ L",";
		sAnchor += pControl->m_oControlPr->m_oAnchor->m_oTo->m_oCol->ToString()						+ L",";
		sAnchor += std::to_wstring(pControl->m_oControlPr->m_oAnchor->m_oTo->m_oColOff->ToPx())		+ L",";
		sAnchor += pControl->m_oControlPr->m_oAnchor->m_oTo->m_oRow->ToString()						+ L",";
		sAnchor += std::to_wstring(pControl->m_oControlPr->m_oAnchor->m_oTo->m_oRowOff->ToPx());
		
	OOX::Vml::CClientData oClientData;
		
	oClientData.m_oSizeWithCells = true;
	oClientData.m_oAnchor = sAnchor;
	
	SimpleTypes::Vml::EVmlClientDataObjectType objectType = SimpleTypes::Vml::vmlclientdataobjecttypePict;

	if (pControl->m_oFormControlPr->m_oObjectType.IsInit())
	{
		switch(pControl->m_oFormControlPr->m_oObjectType->GetValue())
		{
			case SimpleTypes::Spreadsheet::objectButton:	objectType = SimpleTypes::Vml::vmlclientdataobjecttypeButton;	break;
			case SimpleTypes::Spreadsheet::objectCheckBox:	objectType = SimpleTypes::Vml::vmlclientdataobjecttypeCheckbox; break;
			case SimpleTypes::Spreadsheet::objectDrop:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeDrop;		break;
			case SimpleTypes::Spreadsheet::objectGBox:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeGBox;		break;
			case SimpleTypes::Spreadsheet::objectLabel:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeLabel;	break;
			case SimpleTypes::Spreadsheet::objectList:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeList;		break;
			case SimpleTypes::Spreadsheet::objectRadio:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeRadio;	break;
			case SimpleTypes::Spreadsheet::objectScroll:	objectType = SimpleTypes::Vml::vmlclientdataobjecttypeScroll;	break;
			case SimpleTypes::Spreadsheet::objectSpin:		objectType = SimpleTypes::Vml::vmlclientdataobjecttypeSpin;		break;
			case SimpleTypes::Spreadsheet::objectEditBox:	objectType = SimpleTypes::Vml::vmlclientdataobjecttypeEdit;		break;
			case SimpleTypes::Spreadsheet::objectDialog:	objectType = SimpleTypes::Vml::vmlclientdataobjecttypeDialog;	break;
		}
	}
	oClientData.m_oObjectType.Init();
	oClientData.m_oObjectType->SetValue(objectType);

	if (pControl->m_oFormControlPr->m_oChecked.IsInit())
		oClientData.m_oChecked = pControl->m_oFormControlPr->m_oChecked->ToString();
	
	if (pControl->m_oFormControlPr->m_oDropStyle.IsInit())
		oClientData.m_oDropStyle = pControl->m_oFormControlPr->m_oDropStyle->ToVmlString();

	oClientData.m_oDefaultSize = pControl->m_oControlPr->m_oDefaultSize;
	oClientData.m_oAutoLine = pControl->m_oControlPr->m_oAutoLine;
	oClientData.m_oAutoPict = pControl->m_oControlPr->m_oAutoPict;	
	oClientData.m_oCf = pControl->m_oControlPr->m_oCf;
	
	oClientData.m_oMin = pControl->m_oFormControlPr->m_oMin;
	oClientData.m_oMax = pControl->m_oFormControlPr->m_oMax;
	oClientData.m_oVal = pControl->m_oFormControlPr->m_oVal;
	oClientData.m_oInc = pControl->m_oFormControlPr->m_oInc;
	oClientData.m_oDx = pControl->m_oFormControlPr->m_oDx;
	oClientData.m_oPage = pControl->m_oFormControlPr->m_oPage;
	oClientData.m_oDropLines = pControl->m_oFormControlPr->m_oDropLines;
	oClientData.m_oChecked = pControl->m_oFormControlPr->m_oChecked;
	oClientData.m_oColored = pControl->m_oFormControlPr->m_oColored;
	oClientData.m_oDropStyle = pControl->m_oFormControlPr->m_oDropStyle;
	oClientData.m_oFirstButton = pControl->m_oFormControlPr->m_oFirstButton;
	oClientData.m_oHoriz = pControl->m_oFormControlPr->m_oHoriz;
	oClientData.m_oJustLastX = pControl->m_oFormControlPr->m_oJustLastX;
	oClientData.m_oLockText = pControl->m_oFormControlPr->m_oLockText;
	oClientData.m_oMultiLine = pControl->m_oFormControlPr->m_oMultiLine;
	oClientData.m_oSecretEdit = pControl->m_oFormControlPr->m_oPasswordEdit;
	oClientData.m_oTextHAlign = pControl->m_oFormControlPr->m_oTextHAlign;
	oClientData.m_oTextVAlign = pControl->m_oFormControlPr->m_oTextVAlign;
	oClientData.m_oVal = pControl->m_oFormControlPr->m_oVal;
	oClientData.m_oSel = pControl->m_oFormControlPr->m_oSel;
	oClientData.m_oSelType = pControl->m_oFormControlPr->m_oSelType;
	oClientData.m_oVScroll = pControl->m_oFormControlPr->m_oVerticalBar;
	oClientData.m_oWidthMin = pControl->m_oFormControlPr->m_oWidthMin;
	oClientData.m_oNoThreeD = pControl->m_oFormControlPr->m_oNoThreeD;
	oClientData.m_oNoThreeD2 = pControl->m_oFormControlPr->m_oNoThreeD2;
	oClientData.m_oFmlaLink = pControl->m_oFormControlPr->m_oFmlaLink;
	oClientData.m_oFmlaRange = pControl->m_oFormControlPr->m_oFmlaRange;
	oClientData.m_oFmlaTxbx = pControl->m_oFormControlPr->m_oFmlaTxbx;
	oClientData.m_oFmlaGroup = pControl->m_oFormControlPr->m_oFmlaGroup;

	std::wstring result = L"<v:shape type=\"#_x0000_t201\" id=\"_x0000_s" + std::to_wstring(pControl->m_oShapeId->GetValue()) + L"\"";
	result += L" style='position:absolute' stroked=\"f\" strokecolor=\"windowText [64]\" o:insetmode=\"auto\"";
	result += L" filled=\"f\" fillcolor=\"window [65]\"";
	
	if (objectType == SimpleTypes::Vml::vmlclientdataobjecttypeButton)
	{
		result += L" o:button=\"t\"";
	}
	result += L">";
	result += L"<o:lock v:ext=\"edit\" rotation=\"t\" text=\"t\"/>";

	result += oClientData.toXML();

	if (pControl->m_oFormControlPr->m_oText.IsInit())
	{
		OOX::Vml::CTextbox oTextbox;

		oTextbox.m_oText = pControl->m_oFormControlPr->m_oText;
		result += oTextbox.toXML();
	}
	result += L"</v:shape>";

	return result;
}
int BinaryWorksheetsTableReader::ReadFormula(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CFormula* pFormula = static_cast<OOX::Spreadsheet::CFormula*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSerFormulaTypes::Aca == type)
	{
		pFormula->m_oAca.Init();
		pFormula->m_oAca->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Bx == type)
	{
		pFormula->m_oBx.Init();
		pFormula->m_oBx->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Ca == type)
	{
		pFormula->m_oCa.Init();
		pFormula->m_oCa->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Del1 == type)
	{
		pFormula->m_oDel1.Init();
		pFormula->m_oDel1->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Del2 == type)
	{
		pFormula->m_oDel2.Init();
		pFormula->m_oDel2->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Dt2D == type)
	{
		pFormula->m_oDt2D.Init();
		pFormula->m_oDt2D->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::Dtr == type)
	{
		pFormula->m_oDtr.Init();
		pFormula->m_oDtr->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
	}
	else if(c_oSerFormulaTypes::R1 == type)
	{
		pFormula->m_oR1.Init();
		pFormula->m_oR1->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSerFormulaTypes::R2 == type)
	{
		pFormula->m_oR2.Init();
		pFormula->m_oR2->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSerFormulaTypes::Ref == type)
	{
		pFormula->m_oRef.Init();
		pFormula->m_oRef->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSerFormulaTypes::Si == type)
	{
		pFormula->m_oSi.Init();
		pFormula->m_oSi->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSerFormulaTypes::T == type)
	{
		pFormula->m_oT.Init();
		pFormula->m_oT->SetValue((SimpleTypes::Spreadsheet::ECellFormulaType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSerFormulaTypes::Text == type)
	{
		pFormula->m_sText = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadConditionalFormatting(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConditionalFormatting* pConditionalFormatting = static_cast<OOX::Spreadsheet::CConditionalFormatting*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormatting::Pivot == type)
	{
		pConditionalFormatting->m_oPivot.Init();
		pConditionalFormatting->m_oPivot->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormatting::SqRef == type)
	{
		pConditionalFormatting->m_oSqRef.Init();
		pConditionalFormatting->m_oSqRef->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_ConditionalFormatting::ConditionalFormattingRule == type)
	{
		OOX::Spreadsheet::CConditionalFormattingRule* pConditionalFormattingRule = new OOX::Spreadsheet::CConditionalFormattingRule();
		READ1_DEF(length, res, this->ReadConditionalFormattingRule, pConditionalFormattingRule);
		pConditionalFormatting->m_arrItems.push_back(pConditionalFormattingRule);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadConditionalFormattingRule(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConditionalFormattingRule* pConditionalFormattingRule = static_cast<OOX::Spreadsheet::CConditionalFormattingRule*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingRule::AboveAverage == type)
	{
		pConditionalFormattingRule->m_oAboveAverage.Init();
		pConditionalFormattingRule->m_oAboveAverage->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingRule::Bottom == type)
	{
		pConditionalFormattingRule->m_oBottom.Init();
		pConditionalFormattingRule->m_oBottom->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingRule::DxfId == type)
	{
		pConditionalFormattingRule->m_oDxfId.Init();
		pConditionalFormattingRule->m_oDxfId->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingRule::EqualAverage == type)
	{
		pConditionalFormattingRule->m_oEqualAverage.Init();
		pConditionalFormattingRule->m_oEqualAverage->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingRule::Operator == type)
	{
		pConditionalFormattingRule->m_oOperator.Init();
		pConditionalFormattingRule->m_oOperator->SetValue((SimpleTypes::Spreadsheet::ECfOperator)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ConditionalFormattingRule::Percent == type)
	{
		pConditionalFormattingRule->m_oPercent.Init();
		pConditionalFormattingRule->m_oPercent->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingRule::Priority == type)
	{
		pConditionalFormattingRule->m_oPriority.Init();
		pConditionalFormattingRule->m_oPriority->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingRule::Rank == type)
	{
		pConditionalFormattingRule->m_oRank.Init();
		pConditionalFormattingRule->m_oRank->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingRule::StdDev == type)
	{
		pConditionalFormattingRule->m_oStdDev.Init();
		pConditionalFormattingRule->m_oStdDev->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingRule::StopIfTrue == type)
	{
		pConditionalFormattingRule->m_oStopIfTrue.Init();
		pConditionalFormattingRule->m_oStopIfTrue->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingRule::Text == type)
	{
		pConditionalFormattingRule->m_oText.Init();
		pConditionalFormattingRule->m_oText->append(m_oBufferedStream.GetString4(length));
	}
	else if(c_oSer_ConditionalFormattingRule::strTimePeriod == type)
	{
		pConditionalFormattingRule->m_oTimePeriod = m_oBufferedStream.GetString4(length);
	}
	else if (c_oSer_ConditionalFormattingRule::TimePeriod == type)
	{
		pConditionalFormattingRule->m_oTimePeriod.Init();
		pConditionalFormattingRule->m_oTimePeriod->SetValue((SimpleTypes::Spreadsheet::ETimePeriod)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ConditionalFormattingRule::Type == type)
	{
		pConditionalFormattingRule->m_oType.Init();
		pConditionalFormattingRule->m_oType->SetValue((SimpleTypes::Spreadsheet::ECfType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ConditionalFormattingRule::Dxf == type)
	{
		OOX::Spreadsheet::CStyles oStyles(NULL);
		BinaryStyleTableReader style_reader(m_oBufferedStream, oStyles);
		
		OOX::Spreadsheet::CDxf* pDxf = new OOX::Spreadsheet::CDxf();
		READ1_DEF(length, res, style_reader.ReadDxf, pDxf);

		pConditionalFormattingRule->m_oDxf = pDxf;
	}
	else if(c_oSer_ConditionalFormattingRule::ColorScale == type)
	{
		OOX::Spreadsheet::CColorScale* pColorScale = new OOX::Spreadsheet::CColorScale();
		READ1_DEF(length, res, this->ReadColorScale, pColorScale);

		pConditionalFormattingRule->m_oColorScale = pColorScale;
	}
	else if(c_oSer_ConditionalFormattingRule::DataBar == type)
	{
		OOX::Spreadsheet::CDataBar* pDataBar = new OOX::Spreadsheet::CDataBar();
		READ1_DEF(length, res, this->ReadDataBar, pDataBar);
		
		pConditionalFormattingRule->m_oDataBar = pDataBar;
	}
	else if(c_oSer_ConditionalFormattingRule::FormulaCF == type)
	{
		nullable<OOX::Spreadsheet::CFormulaCF> pFormulaCF; pFormulaCF.Init();
		pFormulaCF->m_sText = m_oBufferedStream.GetString4(length);
		
		pConditionalFormattingRule->m_arrFormula.push_back( pFormulaCF );
	}
	else if(c_oSer_ConditionalFormattingRule::IconSet == type)
	{
		OOX::Spreadsheet::CIconSet* pIconSet = new OOX::Spreadsheet::CIconSet();
		READ1_DEF(length, res, this->ReadIconSet, pIconSet);
		
		pConditionalFormattingRule->m_oIconSet = pIconSet;
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadColorScale(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CColorScale* pColorScale = static_cast<OOX::Spreadsheet::CColorScale*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingRuleColorScale::CFVO == type)
	{
		nullable<OOX::Spreadsheet::CConditionalFormatValueObject> pCFVO; pCFVO.Init();
		READ1_DEF(length, res, this->ReadCFVO, pCFVO.GetPointer());
		pColorScale->m_arrValues.push_back(pCFVO);
	}
	else if(c_oSer_ConditionalFormattingRuleColorScale::Color == type)
	{
		nullable<OOX::Spreadsheet::CColor> pColor; pColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pColor.GetPointer());
		pColorScale->m_arrColors.push_back(pColor);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadDataBar(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CDataBar* pDataBar = static_cast<OOX::Spreadsheet::CDataBar*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingDataBar::MaxLength == type)
	{
		pDataBar->m_oMaxLength.Init();
		pDataBar->m_oMaxLength->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingDataBar::MinLength == type)
	{
		pDataBar->m_oMinLength.Init();
		pDataBar->m_oMinLength->SetValue(m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingDataBar::ShowValue == type)
	{
		pDataBar->m_oShowValue.Init();
		pDataBar->m_oShowValue->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingDataBar::GradientEnabled == type)
	{
		pDataBar->m_oGradient.Init();
		pDataBar->m_oGradient->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingDataBar::NegativeBarColorSameAsPositive == type)
	{
		pDataBar->m_oNegativeBarColorSameAsPositive.Init();
		pDataBar->m_oNegativeBarColorSameAsPositive->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingDataBar::NegativeBarBorderColorSameAsPositive == type)
	{
		pDataBar->m_oNegativeBarBorderColorSameAsPositive.Init();
		pDataBar->m_oNegativeBarBorderColorSameAsPositive->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingDataBar::AxisPosition == type)
	{
		pDataBar->m_oAxisPosition.Init();
		pDataBar->m_oAxisPosition->SetValue((SimpleTypes::Spreadsheet::EDataBarAxisPosition)m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingDataBar::Direction == type)
	{
		pDataBar->m_oDirection.Init();
		pDataBar->m_oDirection->SetValue((SimpleTypes::Spreadsheet::EDataBarDirection)m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingDataBar::Color == type)
	{
		pDataBar->m_oColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pDataBar->m_oColor.GetPointer());
	}
	else if(c_oSer_ConditionalFormattingDataBar::AxisColor == type)
	{
		pDataBar->m_oAxisColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pDataBar->m_oAxisColor.GetPointer());
	}
	else if(c_oSer_ConditionalFormattingDataBar::BorderColor == type)
	{
		pDataBar->m_oBorderColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pDataBar->m_oBorderColor.GetPointer());
	}
	else if(c_oSer_ConditionalFormattingDataBar::NegativeColor == type)
	{
		pDataBar->m_oNegativeFillColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pDataBar->m_oNegativeFillColor.GetPointer());
	}	
	else if(c_oSer_ConditionalFormattingDataBar::NegativeBorderColor == type)
	{
		pDataBar->m_oNegativeBorderColor.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pDataBar->m_oNegativeBorderColor.GetPointer());
	}
	else if(c_oSer_ConditionalFormattingDataBar::CFVO == type)
	{
		nullable<OOX::Spreadsheet::CConditionalFormatValueObject> pCFVO; pCFVO.Init();
		READ1_DEF(length, res, this->ReadCFVO, pCFVO.GetPointer());
		pDataBar->m_arrValues.push_back(pCFVO);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadIconSet(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CIconSet* pIconSet = static_cast<OOX::Spreadsheet::CIconSet*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingIconSet::IconSet == type)
	{
		pIconSet->m_oIconSet.Init();
		pIconSet->m_oIconSet->SetValue((SimpleTypes::Spreadsheet::EIconSetType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ConditionalFormattingIconSet::Percent == type)
	{
		pIconSet->m_oPercent.Init();
		pIconSet->m_oPercent->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingIconSet::Reverse == type)
	{
		pIconSet->m_oReverse.Init();
		pIconSet->m_oReverse->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingIconSet::ShowValue == type)
	{
		pIconSet->m_oShowValue.Init();
		pIconSet->m_oShowValue->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingIconSet::CFVO == type)
	{
		nullable<OOX::Spreadsheet::CConditionalFormatValueObject> pCFVO; pCFVO.Init();
		READ1_DEF(length, res, this->ReadCFVO, pCFVO.GetPointer());
		pIconSet->m_arrValues.push_back(pCFVO);
	}
	else if(c_oSer_ConditionalFormattingIconSet::CFIcon == type)
	{
		nullable<OOX::Spreadsheet::CConditionalFormatIconSet> pCFIcon; pCFIcon.Init();
		READ1_DEF(length, res, this->ReadCFIcon, pCFIcon.GetPointer());
		pIconSet->m_arrIconSets.push_back(pCFIcon);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadCFIcon(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConditionalFormatIconSet* pCFIcon = static_cast<OOX::Spreadsheet::CConditionalFormatIconSet*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingIcon::iconSet == type)
	{
		pCFIcon->m_oIconSet.Init();
		pCFIcon->m_oIconSet->SetValue((SimpleTypes::Spreadsheet::EIconSetType)m_oBufferedStream.GetLong());
	}
	else if(c_oSer_ConditionalFormattingIcon::iconId == type)
	{
		pCFIcon->m_oIconId.Init();
		pCFIcon->m_oIconId->SetValue(m_oBufferedStream.GetLong());
	}
	return res;
}
int BinaryWorksheetsTableReader::ReadCFVO(BYTE type, long length, void* poResult)
{
	OOX::Spreadsheet::CConditionalFormatValueObject* pCFVO = static_cast<OOX::Spreadsheet::CConditionalFormatValueObject*>(poResult);
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_ConditionalFormattingValueObject::Gte == type)
	{
		pCFVO->m_oGte.Init();
		pCFVO->m_oGte->FromBool(m_oBufferedStream.GetBool());
	}
	else if(c_oSer_ConditionalFormattingValueObject::Type == type)
	{
		pCFVO->m_oType.Init();
		pCFVO->m_oType->SetValue((SimpleTypes::Spreadsheet::ECfvoType)m_oBufferedStream.GetUChar());
	}
	else if(c_oSer_ConditionalFormattingValueObject::Val == type)
	{
		pCFVO->m_oVal.Init();
		pCFVO->m_oVal = m_oBufferedStream.GetString4(length);
	}
	else if(c_oSer_ConditionalFormattingValueObject::Formula == type)
	{
		pCFVO->m_oFormula.Init();
		pCFVO->m_oFormula->m_sText = m_oBufferedStream.GetString4(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryWorksheetsTableReader::ReadSparklineGroups(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CSparklineGroups* pSparklineGroups = static_cast<OOX::Spreadsheet::CSparklineGroups*>(poResult);
    
	int res = c_oSerConstants::ReadOk;
    if(c_oSer_Sparkline::SparklineGroup == type)
    {
        OOX::Spreadsheet::CSparklineGroup* pSparklineGroup = new OOX::Spreadsheet::CSparklineGroup();
		READ1_DEF(length, res, this->ReadSparklineGroup, pSparklineGroup);
        pSparklineGroups->m_arrItems.push_back(pSparklineGroup);
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadSparklineGroup(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CSparklineGroup* pSparklineGroup = static_cast<OOX::Spreadsheet::CSparklineGroup*>(poResult);
    int res = c_oSerConstants::ReadOk;
    if(c_oSer_Sparkline::ManualMax == type)
    {
        pSparklineGroup->m_oManualMax.Init();
        pSparklineGroup->m_oManualMax->SetValue(m_oBufferedStream.GetDoubleReal());
    }
    else if(c_oSer_Sparkline::ManualMin == type)
    {
        pSparklineGroup->m_oManualMin.Init();
        pSparklineGroup->m_oManualMin->SetValue(m_oBufferedStream.GetDoubleReal());
    }
    else if(c_oSer_Sparkline::LineWeight == type)
    {
        pSparklineGroup->m_oLineWeight.Init();
        pSparklineGroup->m_oLineWeight->SetValue(m_oBufferedStream.GetDoubleReal());
    }
    else if(c_oSer_Sparkline::Type == type)
    {
        pSparklineGroup->m_oType.Init();
        pSparklineGroup->m_oType->SetValue((SimpleTypes::Spreadsheet::ESparklineType)m_oBufferedStream.GetChar());
    }
    else if(c_oSer_Sparkline::DateAxis == type)
    {
        pSparklineGroup->m_oDateAxis.Init();
        pSparklineGroup->m_oDateAxis->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::DisplayEmptyCellsAs == type)
    {
        pSparklineGroup->m_oDisplayEmptyCellsAs.Init();
		pSparklineGroup->m_oDisplayEmptyCellsAs.get2() = (OOX::Spreadsheet::ST_DispBlanksAs)m_oBufferedStream.GetChar();
    }
    else if(c_oSer_Sparkline::Markers == type)
    {
        pSparklineGroup->m_oMarkers.Init();
        pSparklineGroup->m_oMarkers->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::High == type)
    {
        pSparklineGroup->m_oHigh.Init();
        pSparklineGroup->m_oHigh->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::Low == type)
    {
        pSparklineGroup->m_oLow.Init();
        pSparklineGroup->m_oLow->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::First == type)
    {
        pSparklineGroup->m_oFirst.Init();
        pSparklineGroup->m_oFirst->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::Last == type)
    {
        pSparklineGroup->m_oLast.Init();
        pSparklineGroup->m_oLast->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::Negative == type)
    {
        pSparklineGroup->m_oNegative.Init();
        pSparklineGroup->m_oNegative->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::DisplayXAxis == type)
    {
        pSparklineGroup->m_oDisplayXAxis.Init();
        pSparklineGroup->m_oDisplayXAxis->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::DisplayHidden == type)
    {
        pSparklineGroup->m_oDisplayHidden.Init();
        pSparklineGroup->m_oDisplayHidden->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::MinAxisType == type)
    {
        pSparklineGroup->m_oMinAxisType.Init();
        pSparklineGroup->m_oMinAxisType->SetValue((SimpleTypes::Spreadsheet::ESparklineAxisMinMax)m_oBufferedStream.GetChar());
    }
    else if(c_oSer_Sparkline::MaxAxisType == type)
    {
        pSparklineGroup->m_oMaxAxisType.Init();
        pSparklineGroup->m_oMaxAxisType->SetValue((SimpleTypes::Spreadsheet::ESparklineAxisMinMax)m_oBufferedStream.GetChar());
    }
    else if(c_oSer_Sparkline::RightToLeft == type)
    {
        pSparklineGroup->m_oRightToLeft.Init();
        pSparklineGroup->m_oRightToLeft->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_Sparkline::ColorSeries == type)
    {
        pSparklineGroup->m_oColorSeries.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorSeries.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorNegative == type)
    {
        pSparklineGroup->m_oColorNegative.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorNegative.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorAxis == type)
    {
        pSparklineGroup->m_oColorAxis.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorAxis.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorMarkers == type)
    {
        pSparklineGroup->m_oColorMarkers.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorMarkers.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorFirst == type)
    {
        pSparklineGroup->m_oColorFirst.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorFirst.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorLast == type)
    {
        pSparklineGroup->m_oColorLast.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorLast.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorHigh == type)
    {
        pSparklineGroup->m_oColorHigh.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorHigh.GetPointer());
    }
    else if(c_oSer_Sparkline::ColorLow == type)
    {
        pSparklineGroup->m_oColorLow.Init();
		READ2_DEF_SPREADSHEET(length, res, this->ReadColor, pSparklineGroup->m_oColorLow.GetPointer());
    }
    else if(c_oSer_Sparkline::Ref == type)
    {
        pSparklineGroup->m_oRef.Init();
        pSparklineGroup->m_oRef->append(m_oBufferedStream.GetString4(length));
    }
    else if(c_oSer_Sparkline::Sparklines == type)
    {
        pSparklineGroup->m_oSparklines.Init();
		READ1_DEF(length, res, this->ReadSparklines, pSparklineGroup->m_oSparklines.GetPointer());
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadDataValidationsContent(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CDataValidations* pDataValidations = static_cast<OOX::Spreadsheet::CDataValidations*>(poResult);
	
	int res = c_oSerConstants::ReadOk;
    if(c_oSer_DataValidation::DataValidation == type)
    {
        OOX::Spreadsheet::CDataValidation* pDataValidation = new OOX::Spreadsheet::CDataValidation();
		READ2_DEF_SPREADSHEET(length, res, this->ReadDataValidation, pDataValidation);
        pDataValidations->m_arrItems.push_back(pDataValidation);
	}
	else 
		res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadDataValidations(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CDataValidations* pDataValidations = static_cast<OOX::Spreadsheet::CDataValidations*>(poResult);
    
	int res = c_oSerConstants::ReadOk;
    if(c_oSer_DataValidation::DataValidations == type)
    {
		READ1_DEF(length, res, this->ReadDataValidationsContent, pDataValidations);
    }
    else if(c_oSer_DataValidation::DisablePrompts == type)
    {
        pDataValidations->m_oDisablePrompts.Init();
        pDataValidations->m_oDisablePrompts->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_DataValidation::XWindow == type)
    {
		if (!pDataValidations->m_oXWindow.IsInit())
			pDataValidations->m_oXWindow.Init();
        pDataValidations->m_oXWindow->SetValue(m_oBufferedStream.GetLong());
    }
    else if(c_oSer_DataValidation::YWindow == type)
    {
		if (!pDataValidations->m_oYWindow.IsInit())
			pDataValidations->m_oYWindow.Init();
        pDataValidations->m_oYWindow->SetValue(m_oBufferedStream.GetLong());
    }
	else 
		res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadDataValidation(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CDataValidation* pDataValidation = static_cast<OOX::Spreadsheet::CDataValidation*>(poResult);
    
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_DataValidation::AllowBlank == type)
    {
        pDataValidation->m_oAllowBlank.Init();
        pDataValidation->m_oAllowBlank->FromBool(m_oBufferedStream.GetBool());
    }
    else if(c_oSer_DataValidation::Type == type)
    {
        pDataValidation->m_oType.Init();
        pDataValidation->m_oType->SetValue((SimpleTypes::Spreadsheet::EDataValidationType)m_oBufferedStream.GetChar());
    }
	else if(c_oSer_DataValidation::Error == type)
    {
        pDataValidation->m_oError = m_oBufferedStream.GetString4(length);
    }
	else if(c_oSer_DataValidation::ErrorTitle == type)
    {
        pDataValidation->m_oErrorTitle = m_oBufferedStream.GetString4(length);
    }
    else if(c_oSer_DataValidation::ErrorStyle == type)
    {
        pDataValidation->m_oErrorStyle.Init();
        pDataValidation->m_oErrorStyle->SetValue((SimpleTypes::Spreadsheet::EDataValidationErrorStyle)m_oBufferedStream.GetChar());
    }
    else if(c_oSer_DataValidation::ImeMode == type)
    {
        pDataValidation->m_oImeMode.Init();
        pDataValidation->m_oImeMode->SetValue((SimpleTypes::Spreadsheet::EDataValidationImeMode)m_oBufferedStream.GetChar());
    }
    else if(c_oSer_DataValidation::Operator == type)
    {
        pDataValidation->m_oOperator.Init();
        pDataValidation->m_oOperator->SetValue((SimpleTypes::Spreadsheet::EDataValidationOperator)m_oBufferedStream.GetChar());
    }
	else if(c_oSer_DataValidation::Promt == type)
    {
        pDataValidation->m_oPrompt = m_oBufferedStream.GetString4(length);
    }
	else if(c_oSer_DataValidation::PromptTitle == type)
    {
        pDataValidation->m_oPromptTitle = m_oBufferedStream.GetString4(length);
    }
	else if(c_oSer_DataValidation::ShowDropDown == type)
    {
        pDataValidation->m_oShowDropDown.Init();
        pDataValidation->m_oShowDropDown->FromBool(m_oBufferedStream.GetBool());
    }
	else if(c_oSer_DataValidation::ShowErrorMessage == type)
    {
        pDataValidation->m_oShowErrorMessage.Init();
        pDataValidation->m_oShowErrorMessage->FromBool(m_oBufferedStream.GetBool());
    }
	else if(c_oSer_DataValidation::ShowInputMessage == type)
    {
        pDataValidation->m_oShowInputMessage.Init();
        pDataValidation->m_oShowInputMessage->FromBool(m_oBufferedStream.GetBool());
    }
	else if(c_oSer_DataValidation::SqRef == type)
    {
        pDataValidation->m_oSqRef = m_oBufferedStream.GetString4(length);
    }
	else if(c_oSer_DataValidation::Formula1 == type)
    {
        pDataValidation->m_oFormula1.Init();
        pDataValidation->m_oFormula1->m_sText = m_oBufferedStream.GetString4(length);
    }
	else if(c_oSer_DataValidation::Formula2 == type)
    {
        pDataValidation->m_oFormula2.Init();
        pDataValidation->m_oFormula2->m_sText = m_oBufferedStream.GetString4(length);
    }
	else
        res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadSparklines(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CSparklines* pSparklines = static_cast<OOX::Spreadsheet::CSparklines*>(poResult);
    int res = c_oSerConstants::ReadOk;
    if(c_oSer_Sparkline::Sparkline == type)
    {
        OOX::Spreadsheet::CSparkline* pSparkline = new OOX::Spreadsheet::CSparkline();
		READ1_DEF(length, res, this->ReadSparkline, pSparkline);
        pSparklines->m_arrItems.push_back(pSparkline);
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadSparkline(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CSparkline* pSparkline = static_cast<OOX::Spreadsheet::CSparkline*>(poResult);
    int res = c_oSerConstants::ReadOk;
    if(c_oSer_Sparkline::SparklineRef == type)
    {
        pSparkline->m_oRef.Init();
        pSparkline->m_oRef->append(m_oBufferedStream.GetString4(length));
    }
    else if(c_oSer_Sparkline::SparklineSqRef == type)
    {
        pSparkline->m_oSqRef.Init();
        pSparkline->m_oSqRef->append(m_oBufferedStream.GetString4(length));
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}
int BinaryWorksheetsTableReader::ReadSlicers(BYTE type, long length, void* poResult)
{
    OOX::Spreadsheet::CSlicerRefs* pSlicerRefs = static_cast<OOX::Spreadsheet::CSlicerRefs*>(poResult);
    int res = c_oSerConstants::ReadOk;
    if(c_oSerWorksheetsTypes::Slicer == type)
    {
		OOX::Spreadsheet::CSlicerFile* pSlicer = new OOX::Spreadsheet::CSlicerFile(NULL);
		pSlicer->m_oSlicers.Init();
		
		m_oBufferedStream.GetUChar();//type
		pSlicer->m_oSlicers->fromPPTY(&m_oBufferedStream);
		
		NSCommon::smart_ptr<OOX::File> pSlicerFile(pSlicer);
		const OOX::RId oRId = m_pCurWorksheet->Add(pSlicerFile);
        
		pSlicerRefs->m_oSlicer.emplace_back();
		pSlicerRefs->m_oSlicer.back().m_oRId.Init();
		pSlicerRefs->m_oSlicer.back().m_oRId->SetValue(oRId.get());	
    }
    else
        res = c_oSerConstants::ReadUnknown;
    return res;
}

void BinaryWorksheetsTableReader::AddLineBreak(OOX::Spreadsheet::CSi& oSi)
{
	OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
	pRun->m_oRPr.Init();
	OOX::Spreadsheet::CRPr& pRPr = pRun->m_oRPr.get2();
	pRPr.m_oRFont.Init();
	pRPr.m_oRFont->m_sVal = L"Tahoma";
	pRPr.m_oSz.Init();
	pRPr.m_oSz->m_oVal.Init();
	pRPr.m_oSz->m_oVal->SetValue(9);
	pRPr.m_oBold.Init();
	pRPr.m_oBold->FromBool(true);

	OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
	pText->m_sText.append(_T("\n"));

	pRun->m_arrItems.push_back(pText);
	oSi.m_arrItems.push_back(pRun);
}


BinaryOtherTableReader::BinaryOtherTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, boost::unordered_map<long, ImageObject*>& mapMedia,
                       const std::wstring& sFileInDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter,
                       const std::wstring& sMediaDir) :
    Binary_CommonReader(oBufferedStream),   m_mapMedia(mapMedia), m_sFileInDir(sFileInDir), m_oSaveParams(oSaveParams),
                                            m_pOfficeDrawingConverter(pOfficeDrawingConverter),m_sMediaDir(sMediaDir)
{
    m_nCurId    = 0;
	m_nCurIndex = 1;
}
int BinaryOtherTableReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadOtherTableContent, this);
	return res;
}
int BinaryOtherTableReader::ReadOtherTableContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_OtherType::Media == type)
	{
		READ1_DEF(length, res, this->ReadMediaContent, poResult);
	}
	else if(c_oSer_OtherType::Theme == type)
	{
		m_oSaveParams.pTheme = new PPTX::Theme(NULL);
		m_oSaveParams.pTheme->fromPPTY(&m_oBufferedStream);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryOtherTableReader::ReadMediaContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_OtherType::MediaItem == type)
	{
		m_nCurId = -1;
        m_sCurSrc.clear();
		READ1_DEF(length, res, this->ReadMediaItem, poResult);
        if(-1 != m_nCurId && false == m_sCurSrc.empty())
		{
			m_mapMedia [m_nCurId] = new ImageObject(m_sCurSrc, m_nCurIndex);
			m_nCurIndex++;
		}
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryOtherTableReader::ReadMediaItem(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	if(c_oSer_OtherType::MediaSrc == type)
	{
        std::wstring sImage (m_oBufferedStream.GetString3(length));
        std::wstring sImageSrc;
		bool bAddToDelete = false;
		NSFile::CFileBinary oFile;

        if(0 == sImage.find(L"data:"))
		{
			if(oFile.CreateTempFile())
				SerializeCommon::convertBase64ToImage(oFile, sImage);
		}
        else if(0 == sImage.find(_T("http:")) || 0 == sImage.find(_T("https:")) || 0 == sImage.find(_T("ftp:")) || 0 == sImage.find(_T("www")))
		{
			//url
			sImageSrc = SerializeCommon::DownloadImage(sImage);
            std::wstring sNewTempFile = SerializeCommon::changeExtention(sImageSrc, L"jpg");
			NSFile::CFileBinary::Move(sImageSrc, sNewTempFile);
			sImageSrc = sNewTempFile;
			bAddToDelete = true;
		}
		else
		{
            if (0 == sImage.find(_T("file:///")))
			{
				sImageSrc = sImage;
                XmlUtils::replace_all( sImageSrc, L"file:///", L"");
			}
			else
			{
				//local
                sImageSrc = m_sFileInDir + _T("media/") + sImage;
			}
		}
		//Проверяем что файл существует
		FILE* pFileNative = oFile.GetFileNative();
		if(NULL != pFileNative)
		{
			ReadMediaItemSaveFileFILE(pFileNative);
		}
		else if(NSFile::CFileBinary::Exists(sImageSrc))
		{
			ReadMediaItemSaveFilePath(sImageSrc);
			if(bAddToDelete)
				NSFile::CFileBinary::Remove(sImageSrc);
		}
	}
	else if(c_oSer_OtherType::MediaId == type)
	{
		m_nCurId = m_oBufferedStream.GetLong();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
std::wstring BinaryOtherTableReader::ReadMediaItemSaveFileGetNewPath(const std::wstring& sTempPath)
{
	if( !NSDirectory::Exists(m_sMediaDir) )
		OOX::CSystemUtility::CreateDirectories(m_sMediaDir);
    std::wstring sNewImageName = L"image" + std::to_wstring(m_nCurIndex);
    sNewImageName += OOX::CPath(sTempPath).GetExtention(true);
	m_nCurIndex++;
    std::wstring sNewImagePath = m_sMediaDir + FILE_SEPARATOR_STR + sNewImageName;
	return sNewImagePath;
}
void BinaryOtherTableReader::ReadMediaItemSaveFileFILE(FILE* pFile)
{
	long size = ftell(pFile);
	if(size > 0)
	{
		rewind(pFile);
		BYTE* pData = new BYTE[size];
		DWORD dwSizeRead = (DWORD)fread((void*)pData, 1, size, pFile);
		if(dwSizeRead > 0)
		{
            std::wstring sNewImagePath = ReadMediaItemSaveFileGetNewPath(L"1.jpg");
			NSFile::CFileBinary oFile;
			oFile.CreateFileW(sNewImagePath);
			oFile.WriteFile(pData, dwSizeRead);
			oFile.CloseFile();
			m_sCurSrc = sNewImagePath;
		}
		RELEASEARRAYOBJECTS(pData);
	}
}
void BinaryOtherTableReader::ReadMediaItemSaveFilePath(const std::wstring& sTempPath)
{
    std::wstring sNewImagePath = ReadMediaItemSaveFileGetNewPath(sTempPath);

	NSFile::CFileBinary::Copy(sTempPath, sNewImagePath);
	m_sCurSrc = sNewImagePath;
}

//------------------------------------------------------------------------------------------------------------------------------------
BinaryPersonReader::BinaryPersonReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook):Binary_CommonReader(oBufferedStream),m_oWorkbook(oWorkbook)
{
}
int BinaryPersonReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CPersonList* pPersonList = new OOX::Spreadsheet::CPersonList(NULL);
	READ_TABLE_DEF(res, this->ReadPersonList, pPersonList);
	smart_ptr<OOX::File> oFilePersonListFile(pPersonList);
	m_oWorkbook.Add(oFilePersonListFile);
	m_oWorkbook.m_pPersonList = pPersonList;
	return res;
}
int BinaryPersonReader::ReadPersonList(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CPersonList* pPersonList = static_cast<OOX::Spreadsheet::CPersonList*>(poResult);
	if ( c_oSer_Person::person == type )
	{
		OOX::Spreadsheet::CPerson* pPerson = new OOX::Spreadsheet::CPerson();
		READ1_DEF(length, res, this->ReadPerson, pPerson);
		pPersonList->m_arrItems.push_back(pPerson);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryPersonReader::ReadPerson(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::Spreadsheet::CPerson* pPerson = static_cast<OOX::Spreadsheet::CPerson*>(poResult);
	if ( c_oSer_Person::id == type )
	{
		pPerson->id = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_Person::providerId == type )
	{
		pPerson->providerId = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_Person::userId == type )
	{
		pPerson->userId = m_oBufferedStream.GetString3(length);
	}
	else if ( c_oSer_Person::displayName == type )
	{
		pPerson->displayName = m_oBufferedStream.GetString3(length);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
//------------------------------------------------------------------------------------------------------------------------------------
BinaryCustomsReader::BinaryCustomsReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook* pWorkbook) : Binary_CommonReader(oBufferedStream), m_pWorkbook(pWorkbook)
{
}
int BinaryCustomsReader::Read()
{
	int res = c_oSerConstants::ReadOk;
	READ_TABLE_DEF(res, this->ReadCustom, NULL);
	return res;
}
int BinaryCustomsReader::ReadCustom(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;

	if (c_oSerCustoms::Custom == type)
	{
		OOX::CCustomXMLProps *pCustomXmlProps = new OOX::CCustomXMLProps(NULL);

		int res = c_oSerConstants::ReadOk;
		READ1_DEF(length, res, this->ReadCustomContent, pCustomXmlProps);

		OOX::CCustomXML *pCustomXml = new OOX::CCustomXML(NULL, false);
		pCustomXml->m_sXmlA = pCustomXmlProps->m_oCustomXmlContentA;

		smart_ptr<OOX::File> oCustomXmlPropsFile(pCustomXmlProps);
		smart_ptr<OOX::File> oCustomXmlFile(pCustomXml);

		pCustomXml->Add(oCustomXmlPropsFile);
		m_pWorkbook->Add(oCustomXmlFile);
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
int BinaryCustomsReader::ReadCustomContent(BYTE type, long length, void* poResult)
{
	int res = c_oSerConstants::ReadOk;
	OOX::CCustomXMLProps* pCustomXMLProps = static_cast<OOX::CCustomXMLProps*>(poResult);

	if (c_oSerCustoms::Uri == type)
	{
		if (false == pCustomXMLProps->m_oShemaRefs.IsInit())
			pCustomXMLProps->m_oShemaRefs.Init();

		pCustomXMLProps->m_oShemaRefs->m_arrItems.push_back(new OOX::CCustomXMLProps::CShemaRef());
		pCustomXMLProps->m_oShemaRefs->m_arrItems.back()->m_sUri = m_oBufferedStream.GetString3(length);
	}
	else if (c_oSerCustoms::ItemId == type)
	{
		pCustomXMLProps->m_oItemID.FromString(m_oBufferedStream.GetString3(length));
	}
	else if (c_oSerCustoms::Content == type)
	{
		pCustomXMLProps->m_oCustomXmlContentA = m_oBufferedStream.GetString2A();
	}
	else
		res = c_oSerConstants::ReadUnknown;
	return res;
}
//------------------------------------------------------------------------------------------------------------------------------------
BinaryFileReader::BinaryFileReader()
{
}
int BinaryFileReader::Xml2Xlsx(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bMacro)
{
	OOX::Spreadsheet::CXlsxFlat *pXlsxFlat = new OOX::Spreadsheet::CXlsxFlat();
	if (!pXlsxFlat) return AVS_FILEUTILS_ERROR_CONVERT;
	
	pXlsxFlat->m_strFontDirectory = pOfficeDrawingConverter->m_strFontDirectory;
	
	pXlsxFlat->read(OOX::CPath(sSrcFileName));
	if (pXlsxFlat->m_arWorksheets.empty())
	{
		delete pXlsxFlat;
		return AVS_FILEUTILS_ERROR_CONVERT;
	}
	OOX::Spreadsheet::CXlsx oXlsx;

	oXlsx.m_pStyles = pXlsxFlat->m_pStyles.GetPointerEmptyNullable(); oXlsx.bDeleteStyles = true;
	oXlsx.m_pSharedStrings = pXlsxFlat->m_pSharedStrings.GetPointerEmptyNullable(); oXlsx.bDeleteSharedStrings = true;
	oXlsx.m_pWorkbook = pXlsxFlat->m_pWorkbook.GetPointerEmptyNullable(); oXlsx.bDeleteWorkbook = true;
	
	for (size_t i = 0; i < pXlsxFlat->m_arWorksheets.size(); ++i)
	{
		OOX::Spreadsheet::CWorksheet *sheet = pXlsxFlat->m_arWorksheets[i];
		oXlsx.m_arWorksheets.push_back(sheet);

		if (false == oXlsx.m_pWorkbook->m_oSheets.IsInit()) oXlsx.m_pWorkbook->m_oSheets.Init();
		
		while (oXlsx.m_pWorkbook->m_oSheets->m_arrItems.size() <= i)
			oXlsx.m_pWorkbook->m_oSheets->m_arrItems.push_back(new OOX::Spreadsheet::CSheet());
		{
			std::wstring rId = L"sId" + std::to_wstring(i + 1);
			oXlsx.m_pWorkbook->m_oSheets->m_arrItems[i]->m_oRid = new SimpleTypes::CRelationshipId(rId);

			smart_ptr<OOX::File> pFile(sheet);
			oXlsx.m_pWorkbook->Add(rId, pFile);
		}

		if (false == sheet->m_mapComments.empty())
		{
			OOX::CVmlDrawing *vmlDrawing = new OOX::CVmlDrawing(NULL, false);

			NSCommon::smart_ptr<OOX::File> pVmlDrawingFile(vmlDrawing);
			const OOX::RId oRId = sheet->Add(pVmlDrawingFile);

			sheet->m_oLegacyDrawing.Init(); sheet->m_oLegacyDrawing->m_oId.Init();
			sheet->m_oLegacyDrawing->m_oId->SetValue(oRId.get());

			vmlDrawing->m_mapComments = &sheet->m_mapComments;

			std::map<std::wstring, unsigned int> mapByAuthors;
			OOX::Spreadsheet::CComments* pComments = new OOX::Spreadsheet::CComments(NULL);

			pComments->m_oCommentList.Init();
			std::vector<OOX::Spreadsheet::CComment*>& aComments = pComments->m_oCommentList->m_arrItems;

			pComments->m_oAuthors.Init();

			for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = sheet->m_mapComments.begin(); it != sheet->m_mapComments.end(); ++it)
			{
				OOX::Spreadsheet::CCommentItem* pCommentItem = it->second;
				if (pCommentItem->IsValid())
				{
					OOX::Spreadsheet::CComment* pNewComment = new OOX::Spreadsheet::CComment();
					if (pCommentItem->m_nRow.IsInit() && pCommentItem->m_nCol.IsInit())
					{
						pNewComment->m_oRef.Init();
						pNewComment->m_oRef->SetValue(OOX::Spreadsheet::CCell::combineRef(pCommentItem->m_nRow.get(), pCommentItem->m_nCol.get()));
					}
					if (pCommentItem->m_sAuthor.IsInit())
					{
						const std::wstring& sAuthor = pCommentItem->m_sAuthor.get();
						std::map<std::wstring, unsigned int>::const_iterator pFind = mapByAuthors.find(sAuthor);

						int nAuthorId;
						if (pFind != mapByAuthors.end())
							nAuthorId = (int)pFind->second;
						else
						{
							nAuthorId = (int)mapByAuthors.size();

							mapByAuthors.insert(std::make_pair(sAuthor, nAuthorId));

							pComments->m_oAuthors->m_arrItems.push_back(sAuthor);
						}
						pNewComment->m_oAuthorId.Init();
						pNewComment->m_oAuthorId->SetValue(nAuthorId);
					}
					pNewComment->m_oText.reset(pCommentItem->m_oText.GetPointerEmptyNullable());

					aComments.push_back(pNewComment);
				}
			}
			NSCommon::smart_ptr<OOX::File> pCommentsFile(pComments);
			sheet->Add(pCommentsFile);
		}
	}
	oXlsx.bDeleteWorksheets = false;
	pXlsxFlat->m_arWorksheets.clear();

	oXlsx.PrepareToWrite();

	OOX::CContentTypes oContentTypes;
	oXlsx.Write(sDstPath, oContentTypes);
//---------------------
	delete pXlsxFlat;
	return 0;
}
int BinaryFileReader::ReadFile(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bMacro)
{
	bool bResultOk = false;
	
	NSFile::CFileBinary oFile;
	
	if (false == oFile.OpenFile(sSrcFileName)) return AVS_FILEUTILS_ERROR_CONVERT;

	DWORD nBase64DataSize = 0;
	BYTE* pBase64Data = new BYTE[oFile.GetFileSize()];
	oFile.ReadFile(pBase64Data, oFile.GetFileSize(), nBase64DataSize);
	oFile.CloseFile();

	//проверяем формат
	bool bValidFormat = false;
	std::wstring sSignature(g_sFormatSignature);
	size_t nSigLength = sSignature.length();
	if(nBase64DataSize > nSigLength)
	{
		std::string sCurSig((char*)pBase64Data, nSigLength);
		std::wstring wsCurSig(sCurSig.begin(), sCurSig.end());

		if(sSignature == wsCurSig)
		{
			bValidFormat = true;
		}
	}
	if (bValidFormat)
	{
		//Читаем из файла версию и длину base64
		int nIndex = (int)nSigLength;
		int nType = 0;
		std::string version = "";
		std::string dst_len = "";
		
		while (nIndex < nBase64DataSize)
		{
			nIndex++;
			BYTE _c = pBase64Data[nIndex];
			if (_c == ';')
			{

				if(0 == nType)
				{
					nType = 1;
					continue;
				}
				else
				{
					nIndex++;
					break;
				}
			}
			if(0 == nType)
				version += _c;
			else
				dst_len += _c;
		}
		int nVersion = g_nFormatVersion;
		if(!version.empty())
		{
			version = version.substr(1);
			g_nCurFormatVersion = nVersion = std::stoi(version.c_str());
		}
		bool bIsNoBase64 = nVersion == g_nFormatVersionNoBase64;

		NSBinPptxRW::CBinaryFileReader& oBufferedStream = *pOfficeDrawingConverter->m_pReader;

		int nDataSize = 0;
		BYTE* pData = NULL;
		if (!bIsNoBase64)
		{
			nDataSize = atoi(dst_len.c_str());
			pData = new BYTE[nDataSize];
			if(Base64::Base64Decode((const char*)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
			{
				oBufferedStream.Init(pData, 0, nDataSize);
			}
			else
			{
				RELEASEARRAYOBJECTS(pData);
			}
		}
		else
		{
			nDataSize = nBase64DataSize;
			pData = pBase64Data;
			oBufferedStream.Init(pData, 0, nDataSize);
			oBufferedStream.Seek(nIndex);
		}

		if(NULL != pData)
		{
	// File Type
			std::wstring sDstPathCSV = sDstPath;
			BYTE fileType;
			UINT nCodePage;
			std::wstring sDelimiter;
			BYTE saveFileType;

			SerializeCommon::ReadFileType(sXMLOptions, fileType, nCodePage, sDelimiter, saveFileType);
	// Делаем для CSV перебивку пути, иначе создается папка с одинаковым имеем (для rels) и файл не создается.
			
			if (BinXlsxRW::c_oFileTypes::CSV == fileType)
			{
				sDstPath = pOfficeDrawingConverter->GetTempPath();
				if (sDstPath.empty())
					sDstPath = NSDirectory::GetTempPath();

				sDstPath = NSDirectory::CreateDirectoryWithUniqueName(sDstPath);
			}
			
			OOX::Spreadsheet::CXlsx oXlsx;
			
			std::wstring themePath = sDstPath + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Workbook.DefaultDirectory().GetPath() + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();
			std::wstring drawingsPath = sDstPath + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Workbook.DefaultDirectory().GetPath() + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Drawings.DefaultDirectory().GetPath();
			std::wstring embeddingsPath = sDstPath + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Workbook.DefaultDirectory().GetPath() + FILE_SEPARATOR_STR + OOX::FileTypes::MicrosoftOfficeUnknown.DefaultDirectory().GetPath();
			std::wstring chartsPath = sDstPath + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Workbook.DefaultDirectory().GetPath() + FILE_SEPARATOR_STR + OOX::FileTypes::Chart.DefaultDirectory().GetPath();

			oBufferedStream.m_pRels->m_pManager->SetDstCharts(chartsPath);
			
			bResultOk = true;
			
			if(BinXlsxRW::c_oFileTypes::XLSX == fileType)
			{
				SaveParams oSaveParams(drawingsPath, embeddingsPath, themePath, pOfficeDrawingConverter->GetContentTypes(), NULL, bMacro);
				
				try
				{
					ReadMainTable(oXlsx, oBufferedStream, OOX::CPath(sSrcFileName).GetDirectory(), sDstPath, oSaveParams, pOfficeDrawingConverter);
				}
				catch(...)
				{
					bResultOk = false;
				}
				
				oXlsx.PrepareToWrite();
				oXlsx.Write(sDstPath, *oSaveParams.pContentTypes);
			}
			else
			{
				CSVWriter oCSVWriter;
				
				oCSVWriter.Init(oXlsx, nCodePage, sDelimiter, false);
				oCSVWriter.Start(sDstPathCSV);
				SaveParams oSaveParams(drawingsPath, embeddingsPath, themePath, pOfficeDrawingConverter->GetContentTypes(), &oCSVWriter);
				
				try
				{
					ReadMainTable(oXlsx, oBufferedStream, OOX::CPath(sSrcFileName).GetDirectory(), sDstPath, oSaveParams, pOfficeDrawingConverter);
				}
				catch(...)
				{
					bResultOk = false;
				}				
				oCSVWriter.End();
			}
		}
		if (!bIsNoBase64)
		{
			RELEASEARRAYOBJECTS(pData);
		}

	}
	RELEASEARRAYOBJECTS(pBase64Data);

	if (bResultOk)	return 0;
	else			return AVS_FILEUTILS_ERROR_CONVERT;
}
int BinaryFileReader::ReadMainTable(OOX::Spreadsheet::CXlsx& oXlsx, NSBinPptxRW::CBinaryFileReader& oBufferedStream, const std::wstring& sFileInDir, const std::wstring& sOutDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
{
	oBufferedStream.m_nDocumentType = XMLWRITER_DOC_TYPE_XLSX;

	long res = c_oSerConstants::ReadOk;
	//mtLen
	res = oBufferedStream.Peek(1) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
	if(c_oSerConstants::ReadOk != res)
		return res;
	
	long nOtherOffset = -1;
	std::vector<BYTE> aTypes;
	std::vector<long> aOffBits;
	long nOtherOffBits = -1;
	long nSharedStringsOffBits = -1;
	long nWorkbookOffBits = -1;
	long nPersonListOffBits = -1;
	BYTE mtLen = oBufferedStream.GetUChar();
	
	for(int i = 0; i < mtLen; ++i)
	{
		//mtItem
		res = oBufferedStream.Peek(5) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
		if(c_oSerConstants::ReadOk != res)
			return res;

		BYTE mtiType = 0;
		if (false == oBufferedStream.GetUCharWithResult(&mtiType))
			break;

		long mtiOffBits = oBufferedStream.GetLong();
		if(c_oSerTableTypes::Other == mtiType)
			nOtherOffBits = mtiOffBits;
		else if(c_oSerTableTypes::SharedStrings == mtiType)
			nSharedStringsOffBits = mtiOffBits;
		else if(c_oSerTableTypes::Workbook == mtiType)
			nWorkbookOffBits = mtiOffBits;
		else if(c_oSerTableTypes::PersonList == mtiType)
			nPersonListOffBits = mtiOffBits;
		else
		{
			aTypes.push_back(mtiType);
			aOffBits.push_back(mtiOffBits);
		}
	}
	OOX::CPath pathMedia = sOutDir + FILE_SEPARATOR_STR + _T("xl")   + FILE_SEPARATOR_STR + _T("media");
	std::wstring sMediaDir = pathMedia.GetPath();

	boost::unordered_map<long, ImageObject*> mapMedia;
	if(-1 != nOtherOffBits)
	{
		oBufferedStream.Seek(nOtherOffBits);
		res = BinaryOtherTableReader(oBufferedStream, mapMedia, sFileInDir, oSaveParams, pOfficeDrawingConverter, sMediaDir).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;

		oXlsx.m_pTheme = oSaveParams.pTheme;
	}
	if(-1 != nSharedStringsOffBits)
	{
		oBufferedStream.Seek(nSharedStringsOffBits);
		oXlsx.CreateSharedStrings();
		res = BinarySharedStringTableReader(oBufferedStream, *oXlsx.m_pSharedStrings).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}
	oXlsx.CreateWorkbook();

	boost::unordered_map<long, NSCommon::smart_ptr<OOX::File>> m_mapPivotCacheDefinitions;
	if(-1 != nWorkbookOffBits)
	{
		oXlsx.m_pWorkbook->m_bMacroEnabled = oSaveParams.bMacroEnabled;

		oBufferedStream.Seek(nWorkbookOffBits);
		res = BinaryWorkbookTableReader(oBufferedStream, *oXlsx.m_pWorkbook, m_mapPivotCacheDefinitions, sOutDir, pOfficeDrawingConverter).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
		oSaveParams.bMacroEnabled = oXlsx.m_pWorkbook->m_bMacroEnabled;
	}
	if(-1 != nPersonListOffBits)
	{
		oBufferedStream.Seek(nPersonListOffBits);
		res = BinaryPersonReader(oBufferedStream, *oXlsx.m_pWorkbook).Read();
		if(c_oSerConstants::ReadOk != res)
			return res;
	}

	for (size_t i = 0, length = aTypes.size(); i < length; ++i)
	{
		BYTE mtiType = aTypes[i];
		long mtiOffBits = aOffBits[i];

		oBufferedStream.Seek(mtiOffBits);
		switch(mtiType)
		{
			case c_oSerTableTypes::App:
			{
				PPTX::App oApp(NULL);
				oApp.fromPPTY(&oBufferedStream);
				
				OOX::CApp* pApp = new OOX::CApp(NULL);
				pApp->FromPptxApp(&oApp);
				pApp->SetRequiredDefaults();
				oXlsx.m_pApp = pApp;
				smart_ptr<OOX::File> oCurFile(pApp);
				oXlsx.Add(oCurFile);
			}break;			
			case c_oSerTableTypes::Core:
			{
				PPTX::Core oCore(NULL);
				oCore.fromPPTY(&oBufferedStream);
				
				OOX::CCore* pCore = new OOX::CCore(NULL);
				pCore->FromPptxCore(&oCore);
				pCore->SetRequiredDefaults();
				oXlsx.m_pCore = pCore;
				smart_ptr<OOX::File> oCurFile(pCore);
				oXlsx.Add(oCurFile);
			}break;			
			case c_oSerTableTypes::CustomProperties:
			{
				PPTX::CustomProperties* oCustomProperties = new PPTX::CustomProperties(NULL);
				oCustomProperties->fromPPTY(&oBufferedStream);
				smart_ptr<OOX::File> oCurFile(oCustomProperties);
				oXlsx.Add(oCurFile);
			}break;			
			case c_oSerTableTypes::Styles:
			{
				oXlsx.CreateStyles();
				res = BinaryStyleTableReader(oBufferedStream, *oXlsx.m_pStyles).Read();
			}break;			
			case c_oSerTableTypes::Worksheets:
			{
				res = BinaryWorksheetsTableReader(oBufferedStream, *oXlsx.m_pWorkbook, oXlsx.m_pSharedStrings, oXlsx.m_arWorksheets, oXlsx.m_mapWorksheets, mapMedia, sOutDir, sMediaDir, oSaveParams, pOfficeDrawingConverter, m_mapPivotCacheDefinitions).Read();
			}break;
			case c_oSerTableTypes::Customs:
			{
				res = BinaryCustomsReader(oBufferedStream, oXlsx.m_pWorkbook).Read();
			}break;		
		}	
		if (c_oSerConstants::ReadOk != res)
			return res;
	}
	for (boost::unordered_map<long, ImageObject*>::const_iterator pPair = mapMedia.begin(); pPair != mapMedia.end(); ++pPair)
	{
		delete pPair->second;
	}
	mapMedia.clear();
	return res;
}
void BinaryFileReader::initWorkbook(OOX::Spreadsheet::CWorkbook* pWorkbook)
{

}

}

