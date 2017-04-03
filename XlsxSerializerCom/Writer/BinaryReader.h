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
#ifndef BINARY_READER
#define BINARY_READER

#include "../../Common/Base64.h"
#include "../../Common/ATLDefine.h"

#include "../../Common/DocxFormat/Source/XlsxFormat/Worksheets/Sparkline.h"
#include "../../DesktopEditor/common/Path.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../Common/BinReaderWriterDefines.h"
#include "../Common/Common.h"
#include "../Writer/CSVWriter.h"
#include "BinaryCommonReader.h"
#include "../Reader/ChartFromToBinary.h"

namespace BinXlsxRW {

	class ImageObject
	{
	public:
        std::wstring sPath;
		int nIndex;
        std::map<OOX::Spreadsheet::CDrawing*, std::wstring> mapDrawings;
	public:
		ImageObject()
		{
		}
        ImageObject(std::wstring& _sPath, int _nIndex)
		{
			sPath = _sPath;
			nIndex = _nIndex;
		}
	};
	class CellAnchorTransport
	{
	public:
		OOX::Spreadsheet::CCellAnchor* m_pCellAnchor;
		int m_nPos;
		int m_nLength;
	public:
		CellAnchorTransport(OOX::Spreadsheet::CCellAnchor* pCellAnchor): m_pCellAnchor(pCellAnchor)
		{
			m_nPos = 0;
			m_nLength = 0;
		}
	};
	class Binary_CommonReader2
	{
	protected:
		NSBinPptxRW::CBinaryFileReader& m_poBufferedStream;
	public:
		Binary_CommonReader2(NSBinPptxRW::CBinaryFileReader& poBufferedStream):m_poBufferedStream(poBufferedStream)
		{
		}
		int ReadColor(BYTE type, long length, void* poResult)
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
	};
	class BinaryTableReader : public Binary_CommonReader<BinaryTableReader>
	{
		public:
		BinaryTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream):Binary_CommonReader(oBufferedStream)
		{
		}
		int Read(long length, OOX::Spreadsheet::CWorksheet* pWorksheet)
		{
			return Read1(length, &BinaryTableReader::ReadTablePart, this, pWorksheet);
		}
		int ReadTablePart(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CWorksheet* pWorksheet = static_cast<OOX::Spreadsheet::CWorksheet*>(poResult);
			if(c_oSer_TablePart::Table == type)
			{
				OOX::Spreadsheet::CTableFile* pTable = new OOX::Spreadsheet::CTableFile();
				pTable->m_oTable.Init();
				res = Read1(length, &BinaryTableReader::ReadTable, this, pTable->m_oTable.GetPointer());

				OOX::Spreadsheet::CTablePart* pTablePart = new OOX::Spreadsheet::CTablePart();
				NSCommon::smart_ptr<OOX::File> pTableFile(pTable);
				const OOX::RId oRId = pWorksheet->Add(pTableFile);
				pTablePart->m_oRId.Init();
				pTablePart->m_oRId->SetValue(oRId.get());
				pWorksheet->m_oTableParts->m_arrItems.push_back(pTablePart);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadTable(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CTable* pTable = static_cast<OOX::Spreadsheet::CTable*>(poResult);
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
				pTable->m_oDisplayName.Init();
				pTable->m_oDisplayName->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_TablePart::AutoFilter == type)
			{
				pTable->m_oAutoFilter.Init();
				res = Read1(length, &BinaryTableReader::ReadAutoFilter, this, pTable->m_oAutoFilter.GetPointer());
			}
			else if(c_oSer_TablePart::SortState == type)
			{
				pTable->m_oSortState.Init();
				res = Read1(length, &BinaryTableReader::ReadSortState, this, pTable->m_oSortState.GetPointer());
			}
			else if(c_oSer_TablePart::TableColumns == type)
			{
				pTable->m_oTableColumns.Init();
				res = Read1(length, &BinaryTableReader::ReadTableColumns, this, pTable->m_oTableColumns.GetPointer());
			}
			else if(c_oSer_TablePart::TableStyleInfo == type)
			{
				pTable->m_oTableStyleInfo.Init();
				res = Read2(length, &BinaryTableReader::ReadTableStyleInfo, this, pTable->m_oTableStyleInfo.GetPointer());
			}
			else if(c_oSer_TablePart::AltTextTable == type)
			{
				OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
				pOfficeArtExtension->m_oAltTextTable.Init();

				res = Read1(length, &BinaryTableReader::ReadAltTextTable, this, pOfficeArtExtension->m_oAltTextTable.GetPointer());

				pOfficeArtExtension->m_sUri.Init();
				pOfficeArtExtension->m_sUri->append(_T("{504A1905-F514-4f6f-8877-14C23A59335A}"));
				pOfficeArtExtension->m_sAdditionalNamespace = _T(" xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"");
				pTable->m_oExtLst.Init();
				pTable->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadAltTextTable(BYTE type, long length, void* poResult)
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
		int ReadAutoFilter(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryTableReader::ReadFilterColumns, this, poResult);
			}
			else if(c_oSer_AutoFilter::SortState == type)
			{
				pAutofilter->m_oSortState.Init();
				res = Read1(length, &BinaryTableReader::ReadSortState, this, pAutofilter->m_oSortState.GetPointer());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadFilterColumns(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CAutofilter* pAutofilter = static_cast<OOX::Spreadsheet::CAutofilter*>(poResult);
			if(c_oSer_AutoFilter::FilterColumn == type)
			{
				OOX::Spreadsheet::CFilterColumn* pFilterColumn = new OOX::Spreadsheet::CFilterColumn();
				res = Read1(length, &BinaryTableReader::ReadFilterColumn, this, pFilterColumn);
				pAutofilter->m_arrItems.push_back(pFilterColumn);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadFilterColumn(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryTableReader::ReadFilterFilters, this, pFilterColumn->m_oFilters.GetPointer());
			}
			else if(c_oSer_FilterColumn::CustomFilters == type)
			{
				pFilterColumn->m_oCustomFilters.Init();
				res = Read1(length, &BinaryTableReader::ReadCustomFilters, this, pFilterColumn->m_oCustomFilters.GetPointer());
			}
			else if(c_oSer_FilterColumn::DynamicFilter == type)
			{
				pFilterColumn->m_oDynamicFilter.Init();
				res = Read2(length, &BinaryTableReader::ReadDynamicFilter, this, pFilterColumn->m_oDynamicFilter.GetPointer());
			}
			else if(c_oSer_FilterColumn::ColorFilter == type)
			{
				pFilterColumn->m_oColorFilter.Init();
				res = Read2(length, &BinaryTableReader::ReadColorFilter, this, pFilterColumn->m_oColorFilter.GetPointer());
			}
			else if(c_oSer_FilterColumn::Top10 == type)
			{
				pFilterColumn->m_oTop10.Init();
				res = Read2(length, &BinaryTableReader::ReadTop10, this, pFilterColumn->m_oTop10.GetPointer());
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
		int ReadFilterFilters(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CFilters* pFilters = static_cast<OOX::Spreadsheet::CFilters*>(poResult);
			if(c_oSer_FilterColumn::Filter == type)
			{
				OOX::Spreadsheet::CFilter* pFilter = new OOX::Spreadsheet::CFilter();
				res = Read1(length, &BinaryTableReader::ReadFilterFilter, this, pFilter);
				pFilters->m_arrItems.push_back(pFilter);
			}
			else if(c_oSer_FilterColumn::DateGroupItem == type)
			{
				OOX::Spreadsheet::CDateGroupItem* pDateGroupItem = new OOX::Spreadsheet::CDateGroupItem();
				res = Read2(length, &BinaryTableReader::ReadDateGroupItem, this, pDateGroupItem);
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
		int ReadFilterFilter(BYTE type, long length, void* poResult)
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
		int ReadDateGroupItem(BYTE type, long length, void* poResult)
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
		int ReadCustomFilters(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryTableReader::ReadCustomFilter, this, poResult);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCustomFilter(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CCustomFilters* pCustomFilters = static_cast<OOX::Spreadsheet::CCustomFilters*>(poResult);
			if(c_oSer_CustomFilters::CustomFilter == type)
			{
				OOX::Spreadsheet::CCustomFilter* pCustomFilter = new OOX::Spreadsheet::CCustomFilter();
				res = Read2(length, &BinaryTableReader::ReadCustomFiltersItem, this, pCustomFilter);
				pCustomFilters->m_arrItems.push_back(pCustomFilter);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCustomFiltersItem(BYTE type, long length, void* poResult)
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
		int ReadDynamicFilter(BYTE type, long length, void* poResult)
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
		int ReadColorFilter(BYTE type, long length, void* poResult)
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
		int ReadTop10(BYTE type, long length, void* poResult)
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
		int ReadSortState(BYTE type, long length, void* poResult)
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
			else if(c_oSer_SortState::SortConditions == type)
			{
				res = Read1(length, &BinaryTableReader::ReadSortConditions, this, pSortState);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadSortConditions(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CSortState* pSortState = static_cast<OOX::Spreadsheet::CSortState*>(poResult);
			if(c_oSer_SortState::SortCondition == type)
			{
				OOX::Spreadsheet::CSortCondition* pSortCondition = new OOX::Spreadsheet::CSortCondition();
				res = Read2(length, &BinaryTableReader::ReadSortCondition, this, pSortCondition);
				pSortState->m_arrItems.push_back(pSortCondition);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadSortCondition(BYTE type, long length, void* poResult)
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
		int ReadTableColumns(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CTableColumns* pTableColumns = static_cast<OOX::Spreadsheet::CTableColumns*>(poResult);
			if(c_oSer_TableColumns::TableColumn == type)
			{
				OOX::Spreadsheet::CTableColumn* pTableColumn = new OOX::Spreadsheet::CTableColumn();
				res = Read1(length, &BinaryTableReader::ReadTableColumn, this, pTableColumn);
				
				pTableColumn->m_oId.Init();
				pTableColumn->m_oId->SetValue((unsigned int)pTableColumns->m_arrItems.size() + 1);
				pTableColumns->m_arrItems.push_back(pTableColumn);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableColumn(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CTableColumn* pTableColumn = static_cast<OOX::Spreadsheet::CTableColumn*>(poResult);
			if(c_oSer_TableColumns::Name == type)
			{
				pTableColumn->m_oName.Init();
				pTableColumn->m_oName->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_TableColumns::TotalsRowLabel == type)
			{
				pTableColumn->m_oTotalsRowLabel.Init();
				pTableColumn->m_oTotalsRowLabel->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_TableColumns::TotalsRowFunction == type)
			{
				pTableColumn->m_oTotalsRowFunction.Init();
				pTableColumn->m_oTotalsRowFunction->SetValue((SimpleTypes::Spreadsheet::ETotalsRowFunction)m_oBufferedStream.GetUChar());
			}
			else if(c_oSer_TableColumns::TotalsRowFormula == type)
			{
				pTableColumn->m_oTotalsRowFormula.Init();
				pTableColumn->m_oTotalsRowFormula->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_TableColumns::DataDxfId == type)
			{
				pTableColumn->m_oDataDxfId.Init();
				pTableColumn->m_oDataDxfId->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSer_TableColumns::CalculatedColumnFormula == type)
			{
				pTableColumn->m_oCalculatedColumnFormula.Init();
				pTableColumn->m_oCalculatedColumnFormula->append(m_oBufferedStream.GetString4(length));
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableStyleInfo(BYTE type, long length, void* poResult)
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
	};
	class BinarySharedStringTableReader : public Binary_CommonReader<BinarySharedStringTableReader>
	{
		OOX::Spreadsheet::CSharedStrings& m_oSharedStrings;
		Binary_CommonReader2 m_oBcr;
	public:
		BinarySharedStringTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CSharedStrings& oSharedStrings):Binary_CommonReader(oBufferedStream), m_oSharedStrings(oSharedStrings), m_oBcr(oBufferedStream)
		{
		}
		int Read()
		{
			int res = ReadTable(&BinarySharedStringTableReader::ReadSharedStringTableContent, this);
			
			m_oSharedStrings.m_oCount.Init();
			m_oSharedStrings.m_oCount->SetValue((unsigned int)m_oSharedStrings.m_arrItems.size());
			
			m_oSharedStrings.m_oUniqueCount.Init();
			m_oSharedStrings.m_oUniqueCount->SetValue((unsigned int)m_oSharedStrings.m_arrItems.size());
			return res;
		};
		int ReadSharedStringTableContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerSharedStringTypes::Si == type)
			{
				OOX::Spreadsheet::CSi* pSi = new OOX::Spreadsheet::CSi();
				res = Read1(length, &BinarySharedStringTableReader::ReadSi, this, pSi);
				m_oSharedStrings.m_arrItems.push_back(pSi);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadSi(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CSi* pSi = static_cast<OOX::Spreadsheet::CSi*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerSharedStringTypes::Run == type)
			{
				OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
				res = Read1(length, &BinarySharedStringTableReader::ReadRun, this, pRun);
				pSi->m_arrItems.push_back(pRun);
			}
			else if(c_oSerSharedStringTypes::Text == type)
			{
				std::wstring sText(m_oBufferedStream.GetString4(length));
				OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
				pText->m_sText = sText;
				if(std::wstring::npos == sText.find(_T(" ")))
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
		int ReadRun(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CRun* pRun = static_cast<OOX::Spreadsheet::CRun*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerSharedStringTypes::RPr == type)
			{
				pRun->m_oRPr.Init();
				res = Read2(length, &BinarySharedStringTableReader::ReadRPr, this, pRun->m_oRPr.GetPointer());
			}
			else if(c_oSerSharedStringTypes::Text == type)
			{
				std::wstring sText(m_oBufferedStream.GetString4(length));
				OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
				pText->m_sText = sText;
				if(std::wstring::npos == sText.find(_T(" ")))
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
		int ReadRPr(BYTE type, long length, void* poResult)
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
				res = Read2(length, &BinarySharedStringTableReader::ReadColor, this, pFont->m_oColor.GetPointer());
			}
			else if(c_oSerFontTypes::Italic == type)
			{
				pFont->m_oItalic.Init();
				pFont->m_oItalic->m_oVal.SetValue((false != m_oBufferedStream.GetBool()) ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
			else if(c_oSerFontTypes::RFont == type)
			{
				pFont->m_oRFont.Init();
				pFont->m_oRFont->m_sVal.Init();
				pFont->m_oRFont->m_sVal->append(m_oBufferedStream.GetString4(length));
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
		int ReadColor(BYTE type, long length, void* poResult)
		{
			return m_oBcr.ReadColor(type, length, poResult);
		}
	};
	class BinaryStyleTableReader : public Binary_CommonReader<BinaryStyleTableReader>
	{
		OOX::Spreadsheet::CStyles& m_oStyles;
		Binary_CommonReader2 m_oBcr;
	public:
		BinaryStyleTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CStyles& oStyles):Binary_CommonReader(oBufferedStream), m_oStyles(oStyles), m_oBcr(oBufferedStream)
		{
		}
		int Read()
		{
			return ReadTable(&BinaryStyleTableReader::ReadStyleTableContent, this);
		};
		int ReadStyleTableContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Borders == type)
			{
				m_oStyles.m_oBorders.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadBorders, this, poResult);
				
				m_oStyles.m_oBorders->m_oCount.Init();
				m_oStyles.m_oBorders->m_oCount->SetValue((unsigned int)m_oStyles.m_oBorders->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::Fills == type)
			{
				m_oStyles.m_oFills.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadFills, this, poResult);
				
				m_oStyles.m_oFills->m_oCount.Init();
				m_oStyles.m_oFills->m_oCount->SetValue((unsigned int)m_oStyles.m_oFills->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::Fonts == type)
			{
				m_oStyles.m_oFonts.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadFonts, this, poResult);
				
				m_oStyles.m_oFonts->m_oCount.Init();
				m_oStyles.m_oFonts->m_oCount->SetValue((unsigned int)m_oStyles.m_oFonts->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::NumFmts == type)
			{
				m_oStyles.m_oNumFmts.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadNumFmts, this, poResult);
				
				m_oStyles.m_oNumFmts->m_oCount.Init();
				m_oStyles.m_oNumFmts->m_oCount->SetValue((unsigned int)m_oStyles.m_oNumFmts->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::CellStyleXfs == type)
			{
				m_oStyles.m_oCellStyleXfs.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadCellStyleXfs, this, poResult);
				
				m_oStyles.m_oCellStyleXfs->m_oCount.Init();
				m_oStyles.m_oCellStyleXfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellStyleXfs->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::CellXfs == type)
			{
				m_oStyles.m_oCellXfs.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadCellXfs, this, poResult);
				
				m_oStyles.m_oCellXfs->m_oCount.Init();
				m_oStyles.m_oCellXfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellXfs->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::CellStyles == type)
			{
				m_oStyles.m_oCellStyles.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadCellStyles, this, poResult);
				
				m_oStyles.m_oCellStyles->m_oCount.Init();
				m_oStyles.m_oCellStyles->m_oCount->SetValue((unsigned int)m_oStyles.m_oCellStyles->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::Dxfs == type)
			{
				m_oStyles.m_oDxfs.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadDxfs, this, m_oStyles.m_oDxfs.GetPointer());
				
				m_oStyles.m_oDxfs->m_oCount.Init();
				m_oStyles.m_oDxfs->m_oCount->SetValue((unsigned int)m_oStyles.m_oDxfs->m_arrItems.size());
			}
			else if(c_oSerStylesTypes::TableStyles == type)
			{
				m_oStyles.m_oTableStyles.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadTableStyles, this, m_oStyles.m_oTableStyles.GetPointer());
				if(false == m_oStyles.m_oTableStyles->m_oCount.IsInit())
				{
					m_oStyles.m_oTableStyles->m_oCount.Init();
					m_oStyles.m_oTableStyles->m_oCount->SetValue(0);
				}
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadBorders(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Border == type)
			{
				OOX::Spreadsheet::CBorder* pBorder = new OOX::Spreadsheet::CBorder();
				res = Read1(length, &BinaryStyleTableReader::ReadBorder, this, pBorder);
				m_oStyles.m_oBorders->m_arrItems.push_back(pBorder);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadBorder(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CBorder* pBorder = static_cast<OOX::Spreadsheet::CBorder*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerBorderTypes::Bottom == type)
			{
				pBorder->m_oBottom.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oBottom.GetPointer());
			}
			else if(c_oSerBorderTypes::Diagonal == type)
			{
				pBorder->m_oDiagonal.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oDiagonal.GetPointer());
			}
			else if(c_oSerBorderTypes::End == type)
			{
				pBorder->m_oEnd.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oEnd.GetPointer());
			}
			else if(c_oSerBorderTypes::Horizontal == type)
			{
				pBorder->m_oHorizontal.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oHorizontal.GetPointer());
			}
			else if(c_oSerBorderTypes::Start == type)
			{
				pBorder->m_oStart.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oStart.GetPointer());
			}
			else if(c_oSerBorderTypes::Top == type)
			{
				pBorder->m_oTop.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oTop.GetPointer());
			}
			else if(c_oSerBorderTypes::Vertical == type)
			{
				pBorder->m_oVertical.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadBorderProp, this, pBorder->m_oVertical.GetPointer());
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
		int ReadBorderProp(BYTE type, long length, void* poResult)
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
				res = Read2(length, &BinaryStyleTableReader::ReadColor, this, pBorderProp->m_oColor.GetPointer());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadColor(BYTE type, long length, void* poResult)
		{
			return m_oBcr.ReadColor(type, length, poResult);
		}
		int ReadFills(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Fill == type)
			{
				OOX::Spreadsheet::CFill* pFill = new OOX::Spreadsheet::CFill();
				pFill->m_oPatternFill.Init();
				pFill->m_oPatternFill->m_oPatternType.Init();
				pFill->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);
				res = Read1(length, &BinaryStyleTableReader::ReadFill, this, pFill);
				m_oStyles.m_oFills->m_arrItems.push_back(pFill);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadFill(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CFill* pFill = static_cast<OOX::Spreadsheet::CFill* >(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerFillTypes::PatternFill == type)
			{
				if(false == pFill->m_oPatternFill.IsInit())
				{
					pFill->m_oPatternFill.Init();
					pFill->m_oPatternFill->m_oPatternType.Init();
					pFill->m_oPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeNone);
				}
				res = Read1(length, &BinaryStyleTableReader::ReadPatternFill, this, pFill->m_oPatternFill.GetPointer());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadPatternFill(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CPatternFill* pPatternFill = static_cast<OOX::Spreadsheet::CPatternFill* >(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerFillTypes::PatternFillBgColor == type)
			{
				pPatternFill->m_oFgColor.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadColor, this, pPatternFill->m_oFgColor.GetPointer());
				pPatternFill->m_oPatternType->SetValue(SimpleTypes::Spreadsheet::patterntypeSolid);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadFonts(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Font == type)
			{
				OOX::Spreadsheet::CFont* pFont = new OOX::Spreadsheet::CFont();
				res = Read2(length, &BinaryStyleTableReader::ReadFont, this, pFont);
				m_oStyles.m_oFonts->m_arrItems.push_back(pFont);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadFont(BYTE type, long length, void* poResult)
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
				res = Read2(length, &BinaryStyleTableReader::ReadColor, this, pFont->m_oColor.GetPointer());
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
				pFont->m_oRFont->m_sVal.Init();
				pFont->m_oRFont->m_sVal->append(sFontName);
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
		int ReadNumFmts(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::NumFmt == type)
			{
				OOX::Spreadsheet::CNumFmt* pNumFmt = new OOX::Spreadsheet::CNumFmt();
				res = Read2(length, &BinaryStyleTableReader::ReadNumFmt, this, pNumFmt);
				m_oStyles.m_oNumFmts->m_arrItems.push_back(pNumFmt);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadNumFmt(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CNumFmt* pNumFmt = static_cast<OOX::Spreadsheet::CNumFmt*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerNumFmtTypes::FormatCode == type)
			{
				std::wstring sFormatCode(m_oBufferedStream.GetString4(length));
				pNumFmt->m_oFormatCode.Init();
				pNumFmt->m_oFormatCode->append(sFormatCode);
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
		int ReadCellStyleXfs(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Xfs == type)
			{
				OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
				res = Read2(length, &BinaryStyleTableReader::ReadXfs, this, pXfs);
				m_oStyles.m_oCellStyleXfs->m_arrItems.push_back(pXfs);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCellXfs(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::Xfs == type)
			{
				OOX::Spreadsheet::CXfs* pXfs = new OOX::Spreadsheet::CXfs();
				res = Read2(length, &BinaryStyleTableReader::ReadXfs, this, pXfs);
				m_oStyles.m_oCellXfs->m_arrItems.push_back(pXfs);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadXfs(BYTE type, long length, void* poResult)
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
			else if(c_oSerXfsTypes::Aligment == type)
			{
				pXfs->m_oAligment.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadAligment, this, pXfs->m_oAligment.GetPointer());
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
		int ReadAligment(BYTE type, long length, void* poResult)
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
				pAligment->m_oIndent.Init();
				pAligment->m_oIndent->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSerAligmentTypes::RelativeIndent == type)
			{
				pAligment->m_oRelativeIndent.Init();
				pAligment->m_oRelativeIndent->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSerAligmentTypes::ShrinkToFit == type)
			{
				pAligment->m_oShrinkToFit.Init();
				pAligment->m_oShrinkToFit->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
			else if(c_oSerAligmentTypes::TextRotation == type)
			{
				pAligment->m_oTextRotation.Init();
				pAligment->m_oTextRotation->SetValue(m_oBufferedStream.GetLong());
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
		int ReadDxfs(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CDxfs* pDxfs = static_cast<OOX::Spreadsheet::CDxfs*>(poResult);
			if(c_oSerStylesTypes::Dxf == type)
			{
				OOX::Spreadsheet::CDxf* pDxf = new OOX::Spreadsheet::CDxf();
				res = Read1(length, &BinaryStyleTableReader::ReadDxf, this, pDxf);
				pDxfs->m_arrItems.push_back(pDxf);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadDxf(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CDxf* pDxf = static_cast<OOX::Spreadsheet::CDxf*>(poResult);
			if(c_oSer_Dxf::Alignment == type)
			{
				pDxf->m_oAlignment.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadAligment, this, pDxf->m_oAlignment.GetPointer());
			}
			else if(c_oSer_Dxf::Border == type)
			{
				pDxf->m_oBorder.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadBorder, this, pDxf->m_oBorder.GetPointer());
			}
			else if(c_oSer_Dxf::Fill == type)
			{
				pDxf->m_oFill.Init();
				res = Read1(length, &BinaryStyleTableReader::ReadFill, this, pDxf->m_oFill.GetPointer());
			}
			else if(c_oSer_Dxf::Font == type)
			{
				pDxf->m_oFont.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadFont, this, pDxf->m_oFont.GetPointer());
			}
			else if(c_oSer_Dxf::NumFmt == type)
			{
				pDxf->m_oNumFmt.Init();
				res = Read2(length, &BinaryStyleTableReader::ReadNumFmt, this, pDxf->m_oNumFmt.GetPointer());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCellStyles(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerStylesTypes::CellStyle == type)
			{
				OOX::Spreadsheet::CCellStyle* pCellStyle = new OOX::Spreadsheet::CCellStyle();
				res = Read1(length, &BinaryStyleTableReader::ReadCellStyle, this, pCellStyle);
				m_oStyles.m_oCellStyles->m_arrItems.push_back(pCellStyle);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCellStyle(BYTE type, long length, void* poResult)
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
				pCellStyle->m_oName.Init();
				pCellStyle->m_oName->append(m_oBufferedStream.GetString4(length));
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
		int ReadTableStyles(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryStyleTableReader::ReadTableCustomStyles, this, pTableStyles);
				
				pTableStyles->m_oCount.Init();
				pTableStyles->m_oCount->SetValue((unsigned int)pTableStyles->m_arrItems.size());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableCustomStyles(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CTableStyles* pTableStyles = static_cast<OOX::Spreadsheet::CTableStyles*>(poResult);
			if(c_oSer_TableStyles::TableStyle == type)
			{
				OOX::Spreadsheet::CTableStyle* pTableStyle = new OOX::Spreadsheet::CTableStyle();
				res = Read1(length, &BinaryStyleTableReader::ReadTableCustomStyle, this, pTableStyle);
				pTableStyles->m_arrItems.push_back(pTableStyle);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableCustomStyle(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryStyleTableReader::ReadTableCustomStyleElements, this, pTableStyle);
				
				pTableStyle->m_oCount.Init();
				pTableStyle->m_oCount->SetValue((unsigned int)pTableStyle->m_arrItems.size());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableCustomStyleElements(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CTableStyle* pTableStyle = static_cast<OOX::Spreadsheet::CTableStyle*>(poResult);
			if(c_oSer_TableStyle::Element == type)
			{
				OOX::Spreadsheet::CTableStyleElement* pTableStyleElement = new OOX::Spreadsheet::CTableStyleElement();
				res = Read2(length, &BinaryStyleTableReader::ReadTableCustomStyleElement, this, pTableStyleElement);
				pTableStyle->m_arrItems.push_back(pTableStyleElement);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadTableCustomStyleElement(BYTE type, long length, void* poResult)
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
	};
	class BinaryWorkbookTableReader : public Binary_CommonReader<BinaryWorkbookTableReader>
	{
		OOX::Spreadsheet::CWorkbook& m_oWorkbook;
	public:
		BinaryWorkbookTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook):Binary_CommonReader(oBufferedStream), m_oWorkbook(oWorkbook)
		{
		}
		int Read()
		{
			return ReadTable(&BinaryWorkbookTableReader::ReadWorkbookTableContent, this);
		};
		int ReadWorkbookTableContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorkbookTypes::WorkbookPr == type)
			{
				m_oWorkbook.m_oWorkbookPr.Init();
				res = Read2(length, &BinaryWorkbookTableReader::ReadWorkbookPr, this, poResult);
			}
			else if(c_oSerWorkbookTypes::BookViews == type)
			{
				m_oWorkbook.m_oBookViews.Init();
				res = Read1(length, &BinaryWorkbookTableReader::ReadBookViews, this, poResult);
			}
			else if(c_oSerWorkbookTypes::DefinedNames == type)
			{
				m_oWorkbook.m_oDefinedNames.Init();
				res = Read1(length, &BinaryWorkbookTableReader::ReadDefinedNames, this, poResult);
			}
			else if(c_oSerWorkbookTypes::ExternalReferences == type)
			{
				m_oWorkbook.m_oExternalReferences.Init();
				res = Read1(length, &BinaryWorkbookTableReader::ReadExternalReferences, this, poResult);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadWorkbookPr(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorkbookPrTypes::Date1904 == type)
			{
				m_oWorkbook.m_oWorkbookPr->m_oDate1904.Init();
				m_oWorkbook.m_oWorkbookPr->m_oDate1904->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
			else if(c_oSerWorkbookPrTypes::DateCompatibility == type)
			{
				m_oWorkbook.m_oWorkbookPr->m_oDateCompatibility.Init();
				m_oWorkbook.m_oWorkbookPr->m_oDateCompatibility->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadBookViews(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorkbookTypes::WorkbookView == type)
			{
				OOX::Spreadsheet::CWorkbookView* pWorkbookView = new OOX::Spreadsheet::CWorkbookView();
				res = Read2(length, &BinaryWorkbookTableReader::ReadWorkbookView, this, pWorkbookView);
				m_oWorkbook.m_oBookViews->m_arrItems.push_back(pWorkbookView);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadWorkbookView(BYTE type, long length, void* poResult)
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
		int ReadExternalReferences(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorkbookTypes::ExternalReference == type)
			{
                std::wstring sName(m_oBufferedStream.GetString3(length));

                if (sName.empty() == false)
				{
					OOX::Spreadsheet::CExternalReference* pExternalReference = new OOX::Spreadsheet::CExternalReference();
					
						OOX::Spreadsheet::CExternalLink *extLink= new OOX::Spreadsheet::CExternalLink();
						smart_ptr<OOX::File> oCurFile(extLink);
						
						const OOX::RId oRId = m_oWorkbook.Add(oCurFile);
						
						pExternalReference->m_oRid.Init();
						pExternalReference->m_oRid->SetValue(oRId.get());

						extLink->m_oExternalBook.Init();

						OOX::Spreadsheet::ExternalLinkPath *link = new OOX::Spreadsheet::ExternalLinkPath(sName);
						smart_ptr<OOX::File> oLinkFile(link);
						const OOX::RId oRIdLink = extLink->Add(oLinkFile);

						extLink->m_oExternalBook->m_oRid.Init();
						extLink->m_oExternalBook->m_oRid->SetValue(oRIdLink.get());


					m_oWorkbook.m_oExternalReferences->m_arrItems.push_back(pExternalReference);

				}
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadDefinedNames(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorkbookTypes::DefinedName == type)
			{
				OOX::Spreadsheet::CDefinedName* pDefinedName = new OOX::Spreadsheet::CDefinedName();
				res = Read1(length, &BinaryWorkbookTableReader::ReadDefinedName, this, pDefinedName);
				m_oWorkbook.m_oDefinedNames->m_arrItems.push_back(pDefinedName);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadDefinedName(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CDefinedName* pDefinedName = static_cast<OOX::Spreadsheet::CDefinedName*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerDefinedNameTypes::Name == type)
			{
				pDefinedName->m_oName.Init();
				pDefinedName->m_oName->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSerDefinedNameTypes::Ref == type)
			{
				pDefinedName->m_oRef.Init();
				pDefinedName->m_oRef->append(m_oBufferedStream.GetString4(length));
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
				pDefinedName->m_oComment.Init();
				pDefinedName->m_oComment->append(m_oBufferedStream.GetString4(length));
            }
			return res;
		};
	};
	class BinaryCommentReader : public Binary_CommonReader<BinaryCommentReader>
	{
		OOX::Spreadsheet::CWorksheet* m_pCurWorksheet;
	public:
		BinaryCommentReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorksheet* pCurWorksheet):Binary_CommonReader(oBufferedStream),m_pCurWorksheet(pCurWorksheet)
		{
		}
		int Read(long length, void* poResult)
		{
			return Read1(length, &BinaryCommentReader::ReadComments, this, poResult);
		}
		int ReadExternal(long length, void* poResult)
		{
			return Read1(length, &BinaryCommentReader::ReadCommentDatasExternal, this, poResult);
		}
		int ReadCommentDatasExternal(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			std::vector<SerializeCommon::CommentData*>* pCommentDatas = static_cast<std::vector<SerializeCommon::CommentData*>*>(poResult);
			if ( c_oSer_Comments::CommentData == type )
			{
				SerializeCommon::CommentData* oCommentData = new SerializeCommon::CommentData();
				res = Read1(length, &BinaryCommentReader::ReadCommentData, this, oCommentData);
				pCommentDatas->push_back(oCommentData);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadComments(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::Comment == type)
			{
				OOX::Spreadsheet::CCommentItem* pNewComment = new OOX::Spreadsheet::CCommentItem();
				res = Read2(length, &BinaryCommentReader::ReadComment, this, pNewComment);

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
		};
		int ReadComment(BYTE type, long length, void* poResult)
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

                    std::string sSignature("XLST");
                    int nSignatureSize      = (int)sSignature.length();
                    int nDataLengthSize     = sizeof(long);
                    int nJunkSize           = 2;
                    int nWriteBufferLength  = nSignatureSize + nDataLengthSize + length + nJunkSize;

                    BYTE* pWriteBuffer = new BYTE[nWriteBufferLength];
                    memcpy(pWriteBuffer, sSignature.c_str(), nSignatureSize);

					memcpy(pWriteBuffer + nSignatureSize, &length, nDataLengthSize);
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
						pNewComment->m_sGfxdata.Init();
                        pNewComment->m_sGfxdata->append(sGfxdata);
					}
				}
				res = Read1(length, &BinaryCommentReader::ReadCommentDatas, this, pNewComment);
			}
			else if ( c_oSer_Comments::Left == type )
				pNewComment->m_nLeft = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::Top == type )
				pNewComment->m_nTop = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::Right == type )
				pNewComment->m_nRight = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::Bottom == type )
				pNewComment->m_nBottom = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::LeftOffset == type )
				pNewComment->m_nLeftOffset = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::TopOffset == type )
				pNewComment->m_nTopOffset = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::RightOffset == type )
				pNewComment->m_nRightOffset = m_oBufferedStream.GetLong();
			else if ( c_oSer_Comments::BottomOffset == type )
				pNewComment->m_nBottomOffset = m_oBufferedStream.GetLong();
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
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadCommentDatas(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			OOX::Spreadsheet::CCommentItem* pNewComment = static_cast<OOX::Spreadsheet::CCommentItem*>(poResult);
			if ( c_oSer_Comments::CommentData == type )
			{
				if(!pNewComment->m_oText.IsInit())
				{
					SerializeCommon::CommentData oCommentData;
					res = Read1(length, &BinaryCommentReader::ReadCommentData, this, &oCommentData);
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
		int ReadCommentData(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			SerializeCommon::CommentData* pComments = static_cast<SerializeCommon::CommentData*>(poResult);
			if ( c_oSer_CommentData::Text == type )
				pComments->sText = m_oBufferedStream.GetString4(length);
			else if ( c_oSer_CommentData::Time == type )
				pComments->sTime = m_oBufferedStream.GetString4(length);
			else if ( c_oSer_CommentData::UserId == type )
				pComments->sUserId = m_oBufferedStream.GetString4(length);
			else if ( c_oSer_CommentData::UserName == type )
				pComments->sUserName = m_oBufferedStream.GetString4(length);
			else if ( c_oSer_CommentData::QuoteText == type )
				pComments->sQuoteText = m_oBufferedStream.GetString4(length);
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
			else if ( c_oSer_CommentData::Replies == type )
				res = Read1(length, &BinaryCommentReader::ReadCommentReplies, this, &pComments->aReplies);
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadCommentReplies(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			std::vector<SerializeCommon::CommentData*>* pComments = static_cast<std::vector<SerializeCommon::CommentData*>*>(poResult);
			if ( c_oSer_CommentData::Reply == type )
			{
				SerializeCommon::CommentData* pCommentData = new SerializeCommon::CommentData();
				res = Read1(length, &BinaryCommentReader::ReadCommentData, this, pCommentData);
				pComments->push_back(pCommentData);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		void parseCommentData(SerializeCommon::CommentData* pCommentData, OOX::Spreadsheet::CSi& oSi)
		{
			if(NULL != pCommentData && false == pCommentData->sText.empty())
			{
				OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
				pRun->m_oRPr.Init();
				OOX::Spreadsheet::CRPr& pRPr = pRun->m_oRPr.get2();
				pRPr.m_oRFont.Init();
				pRPr.m_oRFont->m_sVal.Init();
				pRPr.m_oRFont->m_sVal->append(_T("Tahoma"));
				pRPr.m_oSz.Init();
				pRPr.m_oSz->m_oVal.Init();
				pRPr.m_oSz->m_oVal->SetValue(8);

				OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
				pText->m_sText.append(pCommentData->sText);

				pRun->m_arrItems.push_back(pText);
				oSi.m_arrItems.push_back(pRun);
			}
		}
	};
	class BinaryWorksheetsTableReader : public Binary_CommonReader<BinaryWorksheetsTableReader>
	{
		Binary_CommonReader2 m_oBcr2;
		OOX::Spreadsheet::CWorkbook& m_oWorkbook;
		OOX::Spreadsheet::CSharedStrings* m_pSharedStrings;
		std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>&  m_mapWorksheets;
		std::map<long, ImageObject*>& m_mapMedia;
		OOX::Spreadsheet::CSheet* m_pCurSheet;
		OOX::Spreadsheet::CWorksheet* m_pCurWorksheet;
		OOX::Spreadsheet::CDrawing* m_pCurDrawing;
		OOX::CVmlDrawing* m_pCurVmlDrawing;

        const std::wstring& m_sDestinationDir;
        const std::wstring& m_sMediaDir;
		SaveParams& m_oSaveParams;
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
		
		int m_nNextObjectId;
	public:
		BinaryWorksheetsTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, OOX::Spreadsheet::CWorkbook& oWorkbook,
			OOX::Spreadsheet::CSharedStrings* pSharedStrings, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets,
            std::map<long, ImageObject*>& mapMedia, const std::wstring& sDestinationDir, const std::wstring& sMediaDir, SaveParams& oSaveParams,
			NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter) : Binary_CommonReader(oBufferedStream), m_oWorkbook(oWorkbook),
			m_oBcr2(oBufferedStream), m_mapWorksheets(mapWorksheets), m_mapMedia(mapMedia), m_sDestinationDir(sDestinationDir), m_sMediaDir(sMediaDir), m_oSaveParams(oSaveParams), m_pSharedStrings(pSharedStrings)
		{
			m_pCurSheet = NULL;
			m_pCurWorksheet = NULL;
			m_pCurDrawing = NULL;
			m_pCurVmlDrawing = NULL;
			m_pOfficeDrawingConverter = pOfficeDrawingConverter;
			m_nNextObjectId = 0xfffff; // в CDrawingConverter своя нумерация .. 
		} 
		int Read()
		{
			m_oWorkbook.m_oSheets.Init();
			return ReadTable(&BinaryWorksheetsTableReader::ReadWorksheetsTableContent, this);
		};
		int ReadWorksheetsTableContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::Worksheet == type)
			{
				m_pCurSheet			= new OOX::Spreadsheet::CSheet();
				m_pCurWorksheet		= new OOX::Spreadsheet::CWorksheet();
				m_pCurVmlDrawing	= new OOX::CVmlDrawing(true);

				m_pCurVmlDrawing->m_lObjectIdVML = (long)(1024 * (m_oWorkbook.m_oSheets->m_arrItems.size() + 1) + 1);

                m_pOfficeDrawingConverter->SetObjectIdVML(m_pCurVmlDrawing->m_lObjectIdVML);

				res = Read1(length, &BinaryWorksheetsTableReader::ReadWorksheet, this, poResult);
				if(m_pCurSheet->m_oName.IsInit())
				{
					//ole & comment
					if(m_pCurVmlDrawing->m_aXml.size() > 0 || (NULL != m_pCurVmlDrawing->m_mapComments && m_pCurVmlDrawing->m_mapComments->size() > 0))
					{
						NSCommon::smart_ptr<OOX::File> pVmlDrawingFile(m_pCurVmlDrawing);
						m_pCurVmlDrawing = NULL;
						const OOX::RId oRId = m_pCurWorksheet->Add(pVmlDrawingFile);
                        m_pCurWorksheet->m_oLegacyDrawing.Init();
                        m_pCurWorksheet->m_oLegacyDrawing->m_oId.Init();
                        m_pCurWorksheet->m_oLegacyDrawing->m_oId->SetValue(oRId.get());
					}

					smart_ptr<OOX::File> oCurFile(m_pCurWorksheet);
					const OOX::RId oRId = m_oWorkbook.Add(oCurFile);
					m_pCurSheet->m_oRid.Init();
					m_pCurSheet->m_oRid->SetValue(oRId.get());
					m_mapWorksheets [m_pCurSheet->m_oName.get()] = m_pCurWorksheet;
					m_oWorkbook.m_oSheets->m_arrItems.push_back(m_pCurSheet);
				}
				RELEASEOBJECT(m_pCurVmlDrawing);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadWorksheet(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::WorksheetProp == type)
			{
				res = Read2(length, &BinaryWorksheetsTableReader::ReadWorksheetProp, this, poResult);
			}
			else if(c_oSerWorksheetsTypes::Cols == type)
			{
				m_pCurWorksheet->m_oCols.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadWorksheetCols, this, poResult);
			}
			else if (c_oSerWorksheetsTypes::SheetViews == type)
			{
				m_pCurWorksheet->m_oSheetViews.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadSheetViews, this, poResult);
			}
			else if (c_oSerWorksheetsTypes::SheetPr == type)
			{
				m_pCurWorksheet->m_oSheetPr.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadSheetPr, this, m_pCurWorksheet->m_oSheetPr.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::SheetFormatPr == type)
			{
				m_pCurWorksheet->m_oSheetFormatPr.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadSheetFormatPr, this, m_pCurWorksheet->m_oSheetFormatPr.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::PageMargins == type)
			{
				m_pCurWorksheet->m_oPageMargins.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadPageMargins, this, m_pCurWorksheet->m_oPageMargins.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::PageSetup == type)
			{
				m_pCurWorksheet->m_oPageSetup.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadPageSetup, this, m_pCurWorksheet->m_oPageSetup.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::PrintOptions == type)
			{
				m_pCurWorksheet->m_oPrintOptions.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadPrintOptions, this, m_pCurWorksheet->m_oPrintOptions.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::Hyperlinks == type)
			{
				m_pCurWorksheet->m_oHyperlinks.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadHyperlinks, this, poResult);
			}
			else if(c_oSerWorksheetsTypes::MergeCells == type)
			{
				m_pCurWorksheet->m_oMergeCells.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadMergeCells, this, poResult);
				
				m_pCurWorksheet->m_oMergeCells->m_oCount.Init();
				m_pCurWorksheet->m_oMergeCells->m_oCount->SetValue((unsigned int)m_pCurWorksheet->m_oMergeCells->m_arrItems.size());
			}
			else if(c_oSerWorksheetsTypes::Drawings == type)
			{
                OOX::CPath pathDrawingsDir = m_sDestinationDir  + FILE_SEPARATOR_STR + _T("xl")  + FILE_SEPARATOR_STR + _T("drawings");
				OOX::CSystemUtility::CreateDirectories(pathDrawingsDir.GetPath());
			
                OOX::CPath pathDrawingsRelsDir = pathDrawingsDir.GetPath()  + FILE_SEPARATOR_STR + _T("_rels");
				OOX::CSystemUtility::CreateDirectories(pathDrawingsRelsDir.GetPath());

				m_pOfficeDrawingConverter->SetDstContentRels();
				m_pCurDrawing = new OOX::Spreadsheet::CDrawing();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadDrawings, this, m_pCurDrawing);
				NSCommon::smart_ptr<OOX::File> pDrawingFile(m_pCurDrawing);
				const OOX::RId oRId = m_pCurWorksheet->Add(pDrawingFile);
				m_pCurWorksheet->m_oDrawing.Init();
				m_pCurWorksheet->m_oDrawing->m_oId.Init();
				m_pCurWorksheet->m_oDrawing->m_oId->SetValue(oRId.get());

				OOX::CPath pathDrawingsRels = pathDrawingsRelsDir.GetPath()  + FILE_SEPARATOR_STR + m_pCurDrawing->m_sOutputFilename + _T(".rels");
				m_pOfficeDrawingConverter->SaveDstContentRels(pathDrawingsRels.GetPath());
			}
			else if(c_oSerWorksheetsTypes::SheetData == type)
			{
				m_pCurWorksheet->m_oSheetData.Init();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadSheetData, this, poResult);
			}
			else if (c_oSerWorksheetsTypes::ConditionalFormatting == type)
			{
				OOX::Spreadsheet::CConditionalFormatting* pConditionalFormatting = new OOX::Spreadsheet::CConditionalFormatting();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadConditionalFormatting, this, pConditionalFormatting);
				m_pCurWorksheet->m_arrConditionalFormatting.push_back(pConditionalFormatting);
			}
			else if(c_oSerWorksheetsTypes::Comments == type)
			{
				BinaryCommentReader oBinaryCommentReader(m_oBufferedStream, m_pCurWorksheet);
				oBinaryCommentReader.Read(length, poResult);
				if(m_pCurWorksheet->m_mapComments.size() > 0)
				{
					m_pCurVmlDrawing->m_mapComments = &m_pCurWorksheet->m_mapComments;

					std::map<std::wstring, unsigned int> mapAuthors;
					OOX::Spreadsheet::CComments* pComments = new OOX::Spreadsheet::CComments();
					pComments->m_oCommentList.Init();
					std::vector<OOX::Spreadsheet::CComment*>& aComments = pComments->m_oCommentList->m_arrItems;
					pComments->m_oAuthors.Init();
					std::vector<std::wstring*>& aAuthors = pComments->m_oAuthors->m_arrItems;

					for (std::map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = m_pCurWorksheet->m_mapComments.begin(); it != m_pCurWorksheet->m_mapComments.end(); ++it)
					{
						if(it->second->IsValid())
						{
							OOX::Spreadsheet::CCommentItem* pCommentItem = it->second;
							OOX::Spreadsheet::CComment* pNewComment = new OOX::Spreadsheet::CComment();
							if(pCommentItem->m_nRow.IsInit() && pCommentItem->m_nCol.IsInit())
							{
								pNewComment->m_oRef.Init();
								pNewComment->m_oRef->SetValue(OOX::Spreadsheet::CCell::combineRef(pCommentItem->m_nRow.get(), pCommentItem->m_nCol.get()));
							}

							if(pCommentItem->m_sAuthor.IsInit())
							{
								const std::wstring& sAuthor = pCommentItem->m_sAuthor.get();
								std::map<std::wstring, unsigned int>::const_iterator pair = mapAuthors.find(sAuthor);
								int nAuthorId;
								if(mapAuthors.end() != pair)
									nAuthorId = (int)pair->second;
								else
								{
									nAuthorId = (int)mapAuthors.size();
									mapAuthors[sAuthor] = nAuthorId;
									aAuthors.push_back(new std::wstring(sAuthor));
								}
								pNewComment->m_oAuthorId.Init();
								pNewComment->m_oAuthorId->SetValue(nAuthorId);
							}

							OOX::Spreadsheet::CSi* pSi = pCommentItem->m_oText.GetPointerEmptyNullable();
							if(NULL != pSi)
								pNewComment->m_oText.reset(pSi);
							aComments.push_back(pNewComment);
						}
					}

					NSCommon::smart_ptr<OOX::File> pCommentsFile(pComments);
					m_pCurWorksheet->Add(pCommentsFile);
				}
			}
			else if(c_oSerWorksheetsTypes::Autofilter == type)
			{
				BinaryTableReader oBinaryTableReader(m_oBufferedStream);
				m_pCurWorksheet->m_oAutofilter.Init();
				res = oBinaryTableReader.Read1(length, &BinaryTableReader::ReadAutoFilter, &oBinaryTableReader, m_pCurWorksheet->m_oAutofilter.GetPointer());
			}
			else if(c_oSerWorksheetsTypes::TableParts == type)
			{
				BinaryTableReader oBinaryTableReader(m_oBufferedStream);
				m_pCurWorksheet->m_oTableParts.Init();
				oBinaryTableReader.Read(length, m_pCurWorksheet);
				
				m_pCurWorksheet->m_oTableParts->m_oCount.Init();
				m_pCurWorksheet->m_oTableParts->m_oCount->SetValue((unsigned int)m_pCurWorksheet->m_oTableParts->m_arrItems.size());
			}
            else if(c_oSerWorksheetsTypes::SparklineGroups == type)
            {
                OOX::Drawing::COfficeArtExtension* pOfficeArtExtension = new OOX::Drawing::COfficeArtExtension();
                pOfficeArtExtension->m_oSparklineGroups.Init();

                res = Read1(length, &BinaryWorksheetsTableReader::ReadSparklineGroups, this, pOfficeArtExtension->m_oSparklineGroups.GetPointer());

                pOfficeArtExtension->m_sUri.Init();
                pOfficeArtExtension->m_sUri->append(_T("{05C60535-1F16-4fd2-B633-F4F36F0B64E0}"));
                pOfficeArtExtension->m_sAdditionalNamespace = _T(" xmlns:x14=\"http://schemas.microsoft.com/office/spreadsheetml/2009/9/main\"");
                m_pCurWorksheet->m_oExtLst.Init();
                m_pCurWorksheet->m_oExtLst->m_arrExt.push_back(pOfficeArtExtension);
            }
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadWorksheetProp(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetPropTypes::Name == type)
			{
				m_pCurSheet->m_oName.Init();
				m_pCurSheet->m_oName->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSerWorksheetPropTypes::SheetId == type)
			{
				m_pCurSheet->m_oSheetId.Init();
				m_pCurSheet->m_oSheetId->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSerWorksheetPropTypes::State == type)
			{
				m_pCurSheet->m_oState.Init();
				m_pCurSheet->m_oState->SetValue((SimpleTypes::Spreadsheet::EVisibleType)m_oBufferedStream.GetUChar());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadWorksheetCols(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::Col == type)
			{
				OOX::Spreadsheet::CCol* pCol = new OOX::Spreadsheet::CCol();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadWorksheetCol, this, pCol);
				m_pCurWorksheet->m_oCols->m_arrItems.push_back(pCol);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadWorksheetCol(BYTE type, long length, void* poResult)
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
                pCol->m_oOutlineLevel->SetValue(m_oBufferedStream.GetLong());
            }
            else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadSheetViews(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::SheetView == type)
			{
				OOX::Spreadsheet::CSheetView* pSheetView = new OOX::Spreadsheet::CSheetView();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadSheetView, this, pSheetView);
				m_pCurWorksheet->m_oSheetViews->m_arrItems.push_back(pSheetView);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadSheetView(BYTE type, long length, void* poResult)
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
				pSheetView->m_oTopLeftCell.Init();
				pSheetView->m_oTopLeftCell->append(m_oBufferedStream.GetString4(length));
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
				res = Read1(length, &BinaryWorksheetsTableReader::ReadPane, this, pSheetView->m_oPane.GetPointer());
			}
			else if (c_oSer_SheetView::Selection == type)
			{
				OOX::Spreadsheet::CSelection* pSelection = new OOX::Spreadsheet::CSelection();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadSelection, this, pSelection);
				pSheetView->m_arrItems.push_back(pSelection);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadPane(BYTE type, long length, void* poResult)
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
				pPane->m_oTopLeftCell.Init();
				pPane->m_oTopLeftCell->append(m_oBufferedStream.GetString4(length));
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
		int ReadSelection(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CSelection* pSelection = static_cast<OOX::Spreadsheet::CSelection*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_Selection::ActiveCell == type)
			{
				pSelection->m_oActiveCell.Init();
				pSelection->m_oActiveCell->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_Selection::ActiveCellId == type)
			{
				pSelection->m_oActiveCellId.Init();
				pSelection->m_oActiveCellId->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSer_Selection::Sqref == type)
			{
				pSelection->m_oSqref.Init();
				pSelection->m_oSqref->append(m_oBufferedStream.GetString4(length));
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
		int ReadSheetPr(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CSheetPr* pSheetPr = static_cast<OOX::Spreadsheet::CSheetPr*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_SheetPr::CodeName == type)
			{
				pSheetPr->m_oCodeName.Init();
				pSheetPr->m_oCodeName->append(m_oBufferedStream.GetString4(length));
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
				pSheetPr->m_oSyncRef.Init();
				pSheetPr->m_oSyncRef->append(m_oBufferedStream.GetString4(length));
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
				res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSheetPr->m_oTabColor.GetPointer());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		}
		int ReadColor(BYTE type, long length, void* poResult)
		{
			return m_oBcr2.ReadColor(type, length, poResult);
		}
		int ReadSheetFormatPr(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CSheetFormatPr* pSheetFormatPr = static_cast<OOX::Spreadsheet::CSheetFormatPr*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerSheetFormatPrTypes::DefaultColWidth == type)
			{
				pSheetFormatPr->m_oDefaultColWidth.Init();
				pSheetFormatPr->m_oDefaultColWidth->SetValue(m_oBufferedStream.GetDoubleReal());
			}
			else if(c_oSerSheetFormatPrTypes::DefaultRowHeight == type)
			{
				pSheetFormatPr->m_oDefaultRowHeight.Init();
				pSheetFormatPr->m_oDefaultRowHeight->SetValue(m_oBufferedStream.GetDoubleReal());
			}
			else if (c_oSerSheetFormatPrTypes::BaseColWidth == type)
			{
				pSheetFormatPr->m_oBaseColWidth.Init();
				pSheetFormatPr->m_oBaseColWidth->SetValue(m_oBufferedStream.GetLong());
			}
			else if (c_oSerSheetFormatPrTypes::CustomHeight == type)
			{
				pSheetFormatPr->m_oCustomHeight.Init();
				pSheetFormatPr->m_oCustomHeight->FromBool(m_oBufferedStream.GetBool());
			}
			else if (c_oSerSheetFormatPrTypes::ZeroHeight == type)
			{
				pSheetFormatPr->m_oZeroHeight.Init();
				pSheetFormatPr->m_oZeroHeight->FromBool(m_oBufferedStream.GetBool());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadPageMargins(BYTE type, long length, void* poResult)
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
		};
		int ReadPageSetup(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CPageSetup* pPageSetup = static_cast<OOX::Spreadsheet::CPageSetup*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_PageSetup::Orientation == type)
			{
				pPageSetup->m_oOrientation.Init();
				pPageSetup->m_oOrientation->SetValue((SimpleTypes::EPageOrientation)m_oBufferedStream.GetUChar());
			}
			else if(c_oSer_PageSetup::PaperSize == type)
			{
				pPageSetup->m_oPaperSize.Init();
				pPageSetup->m_oPaperSize->SetValue((SimpleTypes::Spreadsheet::EPageSize)m_oBufferedStream.GetUChar());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadPrintOptions(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CPrintOptions* pPrintOptions = static_cast<OOX::Spreadsheet::CPrintOptions*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_PrintOptions::GridLines == type)
			{
				bool bGridLines = m_oBufferedStream.GetBool();
				pPrintOptions->m_oGridLines.Init();
				pPrintOptions->m_oGridLines->FromBool(bGridLines);
				pPrintOptions->m_oGridLinesSet.Init();
				pPrintOptions->m_oGridLinesSet->FromBool(bGridLines);
			}
			else if(c_oSer_PrintOptions::Headings == type)
			{
				pPrintOptions->m_oHeadings.Init();
				pPrintOptions->m_oHeadings->FromBool(m_oBufferedStream.GetBool());
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadHyperlinks(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::Hyperlink == type)
			{
				OOX::Spreadsheet::CHyperlink* pHyperlink = new OOX::Spreadsheet::CHyperlink();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadHyperlink, this, pHyperlink);
				m_pCurWorksheet->m_oHyperlinks->m_arrItems.push_back(pHyperlink);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadHyperlink(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CHyperlink* pHyperlink = static_cast<OOX::Spreadsheet::CHyperlink*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerHyperlinkTypes::Ref == type)
			{
				pHyperlink->m_oRef.Init();
				pHyperlink->m_oRef->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSerHyperlinkTypes::Display == type)
			{
				pHyperlink->m_oDisplay.Init();
				pHyperlink->m_oDisplay->append(m_oBufferedStream.GetString4(length));
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
				pHyperlink->m_oLocation.Init();
				pHyperlink->m_oLocation->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSerHyperlinkTypes::Tooltip == type)
			{
				pHyperlink->m_oTooltip.Init();
				pHyperlink->m_oTooltip->append(m_oBufferedStream.GetString4(length));
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadMergeCells(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::MergeCell == type)
			{
				OOX::Spreadsheet::CMergeCell* pMergeCell = new OOX::Spreadsheet::CMergeCell();
				pMergeCell->m_oRef.Init();
				pMergeCell->m_oRef->append(m_oBufferedStream.GetString4(length));
				m_pCurWorksheet->m_oMergeCells->m_arrItems.push_back(pMergeCell);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
        }
		int ReadDrawings(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CDrawing* pDrawing = static_cast<OOX::Spreadsheet::CDrawing*>(poResult);
			int res = c_oSerConstants::ReadOk;

            if(c_oSerWorksheetsTypes::Drawing == type)
			{
				OOX::Spreadsheet::CCellAnchor* pCellAnchor = new OOX::Spreadsheet::CCellAnchor(SimpleTypes::Spreadsheet::CCellAnchorType<>());
				CellAnchorTransport oTransport(pCellAnchor);
				res = Read1(length, &BinaryWorksheetsTableReader::ReadDrawing, this, &oTransport);
				if(oTransport.m_nPos > 0 && oTransport.m_nLength > 0)
				{
					std::wstring sOleXlsx;
					std::wstring sOleDrawing;
					if(NULL != m_pCurDrawing)
					{
                        m_pOfficeDrawingConverter->SetDocumentChartsCount(m_pCurDrawing->GetGlobalNumberByType(OOX::Spreadsheet::FileTypes::Charts.OverrideType()));
                    }

					long nCurPos = oTransport.m_nPos;
                    std::wstring sVmlXml;
                    HRESULT hRes = m_pOfficeDrawingConverter->SaveObjectEx(nCurPos, oTransport.m_nLength, std::wstring(), XMLWRITER_DOC_TYPE_XLSX, sVmlXml);

                    //m_oBufferedStream.Seek(nCurPos + oTransport.m_nLength);
					if(NULL != m_pCurDrawing)
					{
                        m_pCurDrawing->SetGlobalNumberByType(OOX::Spreadsheet::FileTypes::Charts.OverrideType(), m_pOfficeDrawingConverter->GetDocumentChartsCount());
                        m_pCurVmlDrawing->m_lObjectIdVML = m_pOfficeDrawingConverter->GetObjectIdVML();

                        sOleXlsx = m_pOfficeDrawingConverter->GetOleXlsx();
						sOleDrawing = m_pOfficeDrawingConverter->GetOleDrawing();
					}
                    if(S_OK == hRes && !sVmlXml.empty())
					{
						if(!sOleXlsx.empty() &&  pCellAnchor->m_oFrom.IsInit() && pCellAnchor->m_oTo.IsInit())
						{
							XmlUtils::CXmlLiteReader oXmlReader;
							oXmlReader.FromString(sOleXlsx);
							oXmlReader.ReadNextNode();
							OOX::Spreadsheet::COleObject* pOleObject = new OOX::Spreadsheet::COleObject(oXmlReader);
							if(pOleObject->m_oShapeId.IsInit() && pOleObject->m_oFilepathBin.IsInit() && pOleObject->m_oFilepathImg.IsInit() && pOleObject->m_oRidImg.IsInit())
							{
								//generate ClientData
                                std::wstring sAnchor;
								sAnchor += pCellAnchor->m_oFrom->m_oCol->ToString();
								sAnchor += L",";
								sAnchor += std::to_wstring(pCellAnchor->m_oFrom->m_oColOff->ToPx()).c_str();
								sAnchor += L",";
								sAnchor += pCellAnchor->m_oFrom->m_oRow->ToString();
								sAnchor += L",";
								sAnchor += std::to_wstring(pCellAnchor->m_oFrom->m_oRowOff->ToPx()).c_str();
								sAnchor += L",";
								sAnchor += pCellAnchor->m_oTo->m_oCol->ToString();
								sAnchor += L",";
								sAnchor += std::to_wstring(pCellAnchor->m_oTo->m_oColOff->ToPx()).c_str();
								sAnchor += L",";
								sAnchor += pCellAnchor->m_oTo->m_oRow->ToString();
								sAnchor += L",";
								sAnchor += std::to_wstring(pCellAnchor->m_oTo->m_oRowOff->ToPx()).c_str();
								OOX::Vml::CClientData oClientData;
								oClientData.m_oObjectType.Init();
								oClientData.m_oObjectType->SetValue(SimpleTypes::Vml::vmlclientdataobjecttypePict);
								oClientData.m_oSizeWithCells.Init();
								oClientData.m_oAnchor.Init();
                                oClientData.m_oAnchor->append(sAnchor);

								//add VmlDrawing
                                int nIndex = (int)sVmlXml.rfind('<');
                                std::wstring strXml = oClientData.toXML();
                                sVmlXml.insert(sVmlXml.begin() + nIndex, strXml.begin(), strXml.end());
                                m_pCurVmlDrawing->m_aXml.push_back(sVmlXml);

								//add image rels to VmlDrawing
								NSCommon::smart_ptr<OOX::File> pImageFileVml(new OOX::Spreadsheet::Image());
								m_pCurVmlDrawing->Add(OOX::RId(pOleObject->m_oRidImg->GetValue()), pImageFileVml);
								//меняем имя на полученное из pptx
								pImageFileVml->m_sOutputFilename = OOX::CPath(pOleObject->m_oFilepathImg->c_str()).GetFilename();

								//add image rels to Worksheet
								NSCommon::smart_ptr<OOX::File> pImageFileWorksheet(new OOX::Spreadsheet::Image());
								const OOX::RId oRIdImg = m_pCurWorksheet->Add(pImageFileWorksheet);
								//меняем имя на полученное из pptx
								pImageFileWorksheet->m_sOutputFilename = OOX::CPath(pOleObject->m_oFilepathImg->c_str()).GetFilename();

								//add oleObject rels
								if(!m_pCurWorksheet->m_oOleObjects.IsInit())
								{
									m_pCurWorksheet->m_oOleObjects.Init();
								}
								NSCommon::smart_ptr<OOX::File> pOleObjectFile(new OOX::OleObject(true));
								const OOX::RId oRIdBin = m_pCurWorksheet->Add(pOleObjectFile);
								//меняем имя на полученное из pptx
								pOleObjectFile->m_sOutputFilename = OOX::CPath(pOleObject->m_oFilepathBin->c_str()).GetFilename();
								if(!pOleObject->m_oRid.IsInit())
								{
									pOleObject->m_oRid.Init();
								}
								pOleObject->m_oRid->SetValue(oRIdBin.get());

								//ObjectPr
								pOleObject->m_oObjectPr.Init();
								pOleObject->m_oObjectPr->m_oDefaultSize.Init();
								pOleObject->m_oObjectPr->m_oDefaultSize->FromBool(false);
								pOleObject->m_oObjectPr->m_oRid.Init();
								pOleObject->m_oObjectPr->m_oRid->SetValue(oRIdImg.get());
								pOleObject->m_oObjectPr->m_oAnchor.Init();
								SimpleTypes::Spreadsheet::ECellAnchorType eAnchorType = pCellAnchor->m_oAnchorType.GetValue();
								if(SimpleTypes::Spreadsheet::cellanchorOneCell == eAnchorType)
								{
									pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells.Init();
									pOleObject->m_oObjectPr->m_oAnchor->m_oMoveWithCells->FromBool(true);
								}
								else if(SimpleTypes::Spreadsheet::cellanchorTwoCell == eAnchorType)
								{
									pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells.Init();
									pOleObject->m_oObjectPr->m_oAnchor->m_oSizeWithCells->FromBool(true);
								}
								pOleObject->m_oObjectPr->m_oAnchor->m_oFrom = pCellAnchor->m_oFrom;
								pOleObject->m_oObjectPr->m_oAnchor->m_oTo = pCellAnchor->m_oTo;

								//AlternateContent Drawing
								if (!sOleDrawing.empty())
								{
									pCellAnchor->m_oXml.Init();
									pCellAnchor->m_oXml->append(sOleDrawing);
									pCellAnchor->m_oXml->append(_T("<xdr:clientData/>"));
									pCellAnchor->m_oAlternateContent.Init();
									pCellAnchor->m_oAlternateContent->FromBool(true);
									pDrawing->m_arrItems.push_back(pCellAnchor);
								}

								m_pCurWorksheet->m_oOleObjects->m_mapOleObjects[pOleObject->m_oShapeId->GetValue()] = pOleObject;
							}
							else
							{
								delete pOleObject;
							}
						}
						else if(sOleXlsx.empty())//если sOleXlsx не пустой, то в bstrXml старый shape
						{
							pCellAnchor->m_oXml.Init();
                            pCellAnchor->m_oXml->append(sVmlXml);
							pCellAnchor->m_oXml->append(_T("<xdr:clientData/>"));
							pDrawing->m_arrItems.push_back(pCellAnchor);
						}
					}
				}
				else
				{
					pDrawing->m_arrItems.push_back(pCellAnchor);
				}
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadDrawing(BYTE type, long length, void* poResult)
		{
			CellAnchorTransport* pTransport = static_cast<CellAnchorTransport*>(poResult);
			OOX::Spreadsheet::CCellAnchor* pCellAnchor = pTransport->m_pCellAnchor;
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_DrawingType::Type == type)
			{
				pCellAnchor->setAnchorType((SimpleTypes::Spreadsheet::ECellAnchorType)m_oBufferedStream.GetUChar());
			}
			else if(c_oSer_DrawingType::From == type)
			{
				pCellAnchor->m_oFrom.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadFromTo, this, pCellAnchor->m_oFrom.GetPointer());
			}
			else if(c_oSer_DrawingType::To == type)
			{
				pCellAnchor->m_oTo.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadFromTo, this, pCellAnchor->m_oTo.GetPointer());
			}
			else if(c_oSer_DrawingType::Pos == type)
			{
				pCellAnchor->m_oPos.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadPos, this, pCellAnchor->m_oPos.GetPointer());
			}
			else if(c_oSer_DrawingType::Ext == type)
			{
				pCellAnchor->m_oExt.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadExt, this, pCellAnchor->m_oExt.GetPointer());
			}
			else if(c_oSer_DrawingType::Pic == type)
			{
				res = Read1(length, &BinaryWorksheetsTableReader::ReadPic, this, poResult);
			}
			else if(c_oSer_DrawingType::pptxDrawing == type)
			{
				pTransport->m_nPos = m_oBufferedStream.GetPos();
				pTransport->m_nLength = length;
				res = c_oSerConstants::ReadUnknown;
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadFromTo(BYTE type, long length, void* poResult)
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
		};
		int ReadExt(BYTE type, long length, void* poResult)
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
		};
		int ReadPos(BYTE type, long length, void* poResult)
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
		};
		int ReadPic(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CCellAnchor* pCellAnchor = static_cast<OOX::Spreadsheet::CCellAnchor*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_DrawingType::PicSrc == type)
			{
				long nId = m_oBufferedStream.GetLong();
				std::map<long, ImageObject*>::const_iterator pair = m_mapMedia.find(nId);
				if(m_mapMedia.end() != pair)
				{
                    std::wstring sRId;
                    std::map<OOX::Spreadsheet::CDrawing*, std::wstring>::const_iterator pPair = pair->second->mapDrawings.find(m_pCurDrawing);
					if(pair->second->mapDrawings.end() == pPair)
					{
						std::wstring sNewImageName = NSSystemPath::GetFileName(pair->second->sPath);

                        std::wstring sNewImgRel = L"../media/" + sNewImageName;
                        long rId;
                        m_pOfficeDrawingConverter->WriteRels(L"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image", sNewImgRel, L"", &rId);

                        sRId = L"rId" + std::to_wstring(rId);
						pair->second->mapDrawings[m_pCurDrawing] = sRId;
					}
					else
						sRId = pPair->second;
					pCellAnchor->m_oXml.Init();
					pCellAnchor->m_oXml->append(_T("<xdr:pic><xdr:nvPicPr><xdr:cNvPr id=\"1\" name=\"Image 1\"></xdr:cNvPr><xdr:cNvPicPr><a:picLocks noChangeAspect=\"1\"></a:picLocks></xdr:cNvPicPr></xdr:nvPicPr><xdr:blipFill><a:blip r:embed=\""));
					pCellAnchor->m_oXml->append(sRId);
					pCellAnchor->m_oXml->append(_T("\"></a:blip><a:stretch></a:stretch></xdr:blipFill><xdr:spPr><a:prstGeom prst=\"rect\"><a:avLst/></a:prstGeom></xdr:spPr></xdr:pic><xdr:clientData/>"));
				}
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadSheetData(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSerWorksheetsTypes::Row == type)
			{
				OOX::Spreadsheet::CRow* pRow = new OOX::Spreadsheet::CRow();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadRow, this, pRow);
				m_pCurWorksheet->m_oSheetData->m_arrItems.push_back(pRow);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadRow(BYTE type, long length, void* poResult)
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
				pRow->m_oHidden.Init();
				pRow->m_oHidden->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
			else if(c_oSerRowTypes::CustomHeight == type)
			{
				pRow->m_oCustomHeight.Init();
				pRow->m_oCustomHeight->SetValue(false != m_oBufferedStream.GetBool() ? SimpleTypes::onoffTrue : SimpleTypes::onoffFalse);
			}
            else if(c_oSerRowTypes::OutLevel == type)
            {
                pRow->m_oOutlineLevel.Init();
                pRow->m_oOutlineLevel->SetValue(m_oBufferedStream.GetLong());
            }
            else if(c_oSerRowTypes::Cells == type)
			{
				res = Read1(length, &BinaryWorksheetsTableReader::ReadCells, this, pRow);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadCells(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CRow* pRow = static_cast<OOX::Spreadsheet::CRow*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSerRowTypes::Cell == type)
			{
				OOX::Spreadsheet::CCell* pCell = new OOX::Spreadsheet::CCell();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadCell, this, pCell);
				//текст error и формул пишем
				if(NULL != m_pSharedStrings && pCell->m_oType.IsInit() && pCell->m_oValue.IsInit())
				{
					SimpleTypes::Spreadsheet::ECellTypeType eCellType = pCell->m_oType->GetValue();
					bool bMoveText = false;
					if(SimpleTypes::Spreadsheet::celltypeError == eCellType)
						bMoveText = true;
					else if((SimpleTypes::Spreadsheet::celltypeSharedString == eCellType && pCell->m_oFormula.IsInit()))
					{
						bMoveText = true;
						pCell->m_oType->SetValue(SimpleTypes::Spreadsheet::celltypeStr);
					}
					if(bMoveText)
					{
						int nValue = _wtoi(pCell->m_oValue->ToString().c_str());
						if(nValue >=0 && nValue < (int)m_pSharedStrings->m_arrItems.size())
						{
							OOX::Spreadsheet::CSi *pSi = static_cast<OOX::Spreadsheet::CSi *>(m_pSharedStrings->m_arrItems[nValue]);
							if(NULL != pSi && pSi->m_arrItems.size() > 0)
							{
								OOX::Spreadsheet::WritingElement* pWe = pSi->m_arrItems[0];
								if(OOX::Spreadsheet::et_t == pWe->getType())
								{
									OOX::Spreadsheet::CText* pText = static_cast<OOX::Spreadsheet::CText*>(pWe);
									pCell->m_oValue->m_sText = pText->m_sText;
									pCell->m_oValue->m_oSpace = pText->m_oSpace;
								}
							}
						}
					}
				}
				pRow->m_arrItems.push_back(pCell);
			}
			else
				res = c_oSerConstants::ReadUnknown;


			return res;
		};
		int ReadCell(BYTE type, long length, void* poResult)
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
				pCell->setRowCol(nRow, nCol);
			}
			else if(c_oSerCellTypes::Style == type)
			{
				pCell->m_oStyle.Init();
				pCell->m_oStyle->SetValue(m_oBufferedStream.GetLong());
			}
			else if(c_oSerCellTypes::Type == type)
			{
				pCell->m_oType.Init();
				pCell->m_oType->SetValue((SimpleTypes::Spreadsheet::ECellTypeType)m_oBufferedStream.GetUChar());
			}
			else if(c_oSerCellTypes::Formula == type)
			{
				pCell->m_oFormula.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadFormula, this, pCell->m_oFormula.GetPointer());
			}
			else if(c_oSerCellTypes::Value == type)
			{
				double dValue = m_oBufferedStream.GetDoubleReal();
				pCell->m_oValue.Init();
				pCell->m_oValue->m_sText.append(OOX::Spreadsheet::SpreadsheetCommon::WriteDouble(dValue));
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadFormula(BYTE type, long length, void* poResult)
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
		};
		int ReadConditionalFormatting(BYTE type, long length, void* poResult)
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
				res = Read1(length, &BinaryWorksheetsTableReader::ReadConditionalFormattingRule, this, pConditionalFormattingRule);
				pConditionalFormatting->m_arrItems.push_back(pConditionalFormattingRule);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadConditionalFormattingRule(BYTE type, long length, void* poResult)
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
			else if(c_oSer_ConditionalFormattingRule::TimePeriod == type)
			{
				pConditionalFormattingRule->m_oTimePeriod.Init();
				pConditionalFormattingRule->m_oTimePeriod->append(m_oBufferedStream.GetString4(length));
			}
			else if(c_oSer_ConditionalFormattingRule::Type == type)
			{
				pConditionalFormattingRule->m_oType.Init();
				pConditionalFormattingRule->m_oType->SetValue((SimpleTypes::Spreadsheet::ECfType)m_oBufferedStream.GetUChar());
			}
			else if(c_oSer_ConditionalFormattingRule::ColorScale == type)
			{
				OOX::Spreadsheet::CColorScale* pColorScale = new OOX::Spreadsheet::CColorScale();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadColorScale, this, pColorScale);
				pConditionalFormattingRule->m_arrItems.push_back(pColorScale);
			}
			else if(c_oSer_ConditionalFormattingRule::DataBar == type)
			{
				OOX::Spreadsheet::CDataBar* pDataBar = new OOX::Spreadsheet::CDataBar();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadDataBar, this, pDataBar);
				pConditionalFormattingRule->m_arrItems.push_back(pDataBar);
			}
			else if(c_oSer_ConditionalFormattingRule::FormulaCF == type)
			{
				OOX::Spreadsheet::CFormulaCF* pFormulaCF = new OOX::Spreadsheet::CFormulaCF();
				pFormulaCF->m_sText.append(m_oBufferedStream.GetString4(length));
				pConditionalFormattingRule->m_arrItems.push_back(pFormulaCF);
			}
			else if(c_oSer_ConditionalFormattingRule::IconSet == type)
			{
				OOX::Spreadsheet::CIconSet* pIconSet = new OOX::Spreadsheet::CIconSet();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadIconSet, this, pIconSet);
				pConditionalFormattingRule->m_arrItems.push_back(pIconSet);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadColorScale(BYTE type, long length, void* poResult)
		{
			OOX::Spreadsheet::CColorScale* pColorScale = static_cast<OOX::Spreadsheet::CColorScale*>(poResult);
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_ConditionalFormattingRuleColorScale::CFVO == type)
			{
				OOX::Spreadsheet::CConditionalFormatValueObject* pCFVO = new OOX::Spreadsheet::CConditionalFormatValueObject();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadCFVO, this, pCFVO);
				pColorScale->m_arrItems.push_back(pCFVO);
			}
			else if(c_oSer_ConditionalFormattingRuleColorScale::Color == type)
			{
				OOX::Spreadsheet::CColor* pColor = new OOX::Spreadsheet::CColor();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pColor);
				pColorScale->m_arrItems.push_back(pColor);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadDataBar(BYTE type, long length, void* poResult)
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
			else if(c_oSer_ConditionalFormattingDataBar::Color == type)
			{
				pDataBar->m_oColor.Init();
				res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pDataBar->m_oColor.GetPointer());
			}
			else if(c_oSer_ConditionalFormattingDataBar::CFVO == type)
			{
				OOX::Spreadsheet::CConditionalFormatValueObject* pCFVO = new OOX::Spreadsheet::CConditionalFormatValueObject();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadCFVO, this, pCFVO);
				pDataBar->m_arrItems.push_back(pCFVO);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadIconSet(BYTE type, long length, void* poResult)
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
				OOX::Spreadsheet::CConditionalFormatValueObject* pCFVO = new OOX::Spreadsheet::CConditionalFormatValueObject();
				res = Read1(length, &BinaryWorksheetsTableReader::ReadCFVO, this, pCFVO);
				pIconSet->m_arrItems.push_back(pCFVO);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadCFVO(BYTE type, long length, void* poResult)
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
				pCFVO->m_oVal->append(m_oBufferedStream.GetString4(length));
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadSparklineGroups(BYTE type, long length, void* poResult)
        {
            OOX::Spreadsheet::CSparklineGroups* pSparklineGroups = static_cast<OOX::Spreadsheet::CSparklineGroups*>(poResult);
            int res = c_oSerConstants::ReadOk;
            if(c_oSer_Sparkline::SparklineGroup == type)
            {
                OOX::Spreadsheet::CSparklineGroup* pSparklineGroup = new OOX::Spreadsheet::CSparklineGroup();
                res = Read1(length, &BinaryWorksheetsTableReader::ReadSparklineGroup, this, pSparklineGroup);
                pSparklineGroups->m_arrItems.push_back(pSparklineGroup);
            }
            else
                res = c_oSerConstants::ReadUnknown;
            return res;
        };
        int ReadSparklineGroup(BYTE type, long length, void* poResult)
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
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorSeries.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorNegative == type)
            {
                pSparklineGroup->m_oColorNegative.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorNegative.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorAxis == type)
            {
                pSparklineGroup->m_oColorAxis.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorAxis.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorMarkers == type)
            {
                pSparklineGroup->m_oColorMarkers.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorMarkers.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorFirst == type)
            {
                pSparklineGroup->m_oColorFirst.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorFirst.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorLast == type)
            {
                pSparklineGroup->m_oColorLast.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorLast.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorHigh == type)
            {
                pSparklineGroup->m_oColorHigh.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorHigh.GetPointer());
            }
            else if(c_oSer_Sparkline::ColorLow == type)
            {
                pSparklineGroup->m_oColorLow.Init();
                res = Read2(length, &BinaryWorksheetsTableReader::ReadColor, this, pSparklineGroup->m_oColorLow.GetPointer());
            }
            else if(c_oSer_Sparkline::Ref == type)
            {
                pSparklineGroup->m_oRef.Init();
                pSparklineGroup->m_oRef->append(m_oBufferedStream.GetString4(length));
            }
            else if(c_oSer_Sparkline::Sparklines == type)
            {
                pSparklineGroup->m_oSparklines.Init();
                res = Read1(length, &BinaryWorksheetsTableReader::ReadSparklines, this, pSparklineGroup->m_oSparklines.GetPointer());
            }
            else
                res = c_oSerConstants::ReadUnknown;
            return res;
        };
        int ReadSparklines(BYTE type, long length, void* poResult)
        {
            OOX::Spreadsheet::CSparklines* pSparklines = static_cast<OOX::Spreadsheet::CSparklines*>(poResult);
            int res = c_oSerConstants::ReadOk;
            if(c_oSer_Sparkline::Sparkline == type)
            {
                OOX::Spreadsheet::CSparkline* pSparkline = new OOX::Spreadsheet::CSparkline();
                res = Read1(length, &BinaryWorksheetsTableReader::ReadSparkline, this, pSparkline);
                pSparklines->m_arrItems.push_back(pSparkline);
            }
            else
                res = c_oSerConstants::ReadUnknown;
            return res;
        };
        int ReadSparkline(BYTE type, long length, void* poResult)
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
        };

		void AddLineBreak(OOX::Spreadsheet::CSi& oSi)
		{
			OOX::Spreadsheet::CRun* pRun = new OOX::Spreadsheet::CRun();
			pRun->m_oRPr.Init();
			OOX::Spreadsheet::CRPr& pRPr = pRun->m_oRPr.get2();
			pRPr.m_oRFont.Init();
			pRPr.m_oRFont->m_sVal.Init();
			pRPr.m_oRFont->m_sVal->append(_T("Tahoma"));
			pRPr.m_oSz.Init();
			pRPr.m_oSz->m_oVal.Init();
			pRPr.m_oSz->m_oVal->SetValue(8);
			pRPr.m_oBold.Init();
			pRPr.m_oBold->FromBool(true);

			OOX::Spreadsheet::CText* pText = new OOX::Spreadsheet::CText();
			pText->m_sText.append(_T("\n"));

			pRun->m_arrItems.push_back(pText);
			oSi.m_arrItems.push_back(pRun);
		}
	};
	class BinaryOtherTableReader : public Binary_CommonReader<BinaryOtherTableReader>
	{
        std::map<long, ImageObject*>&   m_mapMedia;
        const std::wstring&             m_sFileInDir;
        long                            m_nCurId;
        std::wstring                    m_sCurSrc;
        long                            m_nCurIndex;
        SaveParams&                     m_oSaveParams;
		NSBinPptxRW::CDrawingConverter* m_pOfficeDrawingConverter;
        const std::wstring&             m_sMediaDir;
	public:
        BinaryOtherTableReader(NSBinPptxRW::CBinaryFileReader& oBufferedStream, std::map<long, ImageObject*>& mapMedia,
                               const std::wstring& sFileInDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter,
                               const std::wstring& sMediaDir) :
            Binary_CommonReader(oBufferedStream),   m_mapMedia(mapMedia), m_sFileInDir(sFileInDir), m_oSaveParams(oSaveParams),
                                                    m_pOfficeDrawingConverter(pOfficeDrawingConverter),m_sMediaDir(sMediaDir)
		{
            m_nCurId    = 0;
			m_nCurIndex = 1;
		}
		int Read()
		{
			return ReadTable(&BinaryOtherTableReader::ReadOtherTableContent, this);
		};
		int ReadOtherTableContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_OtherType::Media == type)
				res = Read1(length, &BinaryOtherTableReader::ReadMediaContent, this, poResult);
			else if(c_oSer_OtherType::Theme == type)
			{
                std::wstring sThemePath =  m_oSaveParams.sThemePath + FILE_SEPARATOR_STR;
						sThemePath += OOX::FileTypes::Theme.DefaultFileName().GetPath();

				long nCurPos = m_oBufferedStream.GetPos();
				m_pOfficeDrawingConverter->SaveThemeXml(nCurPos, length, sThemePath);
				m_oBufferedStream.Seek(nCurPos + length);
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadMediaContent(BYTE type, long length, void* poResult)
		{
			int res = c_oSerConstants::ReadOk;
			if(c_oSer_OtherType::MediaItem == type)
			{
				m_nCurId = -1;
                m_sCurSrc.clear();
				res = Read1(length, &BinaryOtherTableReader::ReadMediaItem, this, poResult);
                if(-1 != m_nCurId && false == m_sCurSrc.empty())
				{
					m_mapMedia [m_nCurId] = new ImageObject(m_sCurSrc, m_nCurIndex);
					m_nCurIndex++;
				}
			}
			else
				res = c_oSerConstants::ReadUnknown;
			return res;
		};
		int ReadMediaItem(BYTE type, long length, void* poResult)
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
		};
        std::wstring ReadMediaItemSaveFileGetNewPath(const std::wstring& sTempPath)
		{
			if( !NSDirectory::Exists(m_sMediaDir) )
				OOX::CSystemUtility::CreateDirectories(m_sMediaDir);
            std::wstring sNewImageName = L"image" + std::to_wstring(m_nCurIndex);
            sNewImageName += OOX::CPath(sTempPath).GetExtention(true);
			m_nCurIndex++;
            std::wstring sNewImagePath = m_sMediaDir + FILE_SEPARATOR_STR + sNewImageName;
			return sNewImagePath;
		}
		void ReadMediaItemSaveFileFILE(FILE* pFile)
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
        void ReadMediaItemSaveFilePath(const std::wstring& sTempPath)
		{
            std::wstring sNewImagePath = ReadMediaItemSaveFileGetNewPath(sTempPath);

			NSFile::CFileBinary::Copy(sTempPath, sNewImagePath);
			m_sCurSrc = sNewImagePath;
		}
	};
	class BinaryFileReader
	{
	public: BinaryFileReader()
			{
			}
            int ReadFile(const std::wstring& sSrcFileName, std::wstring sDstPath, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions)
			{
				bool bResultOk = false;
				NSFile::CFileBinary oFile;
				
				if(oFile.OpenFile(sSrcFileName))
				{
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
					if(bValidFormat)
					{
						//Читаем из файла версию и длину base64
						int nIndex = (int)nSigLength;
						int nType = 0;
                        std::string version = "";
                        std::string dst_len = "";
						while (true)
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
                        int nDataSize = atoi(dst_len.c_str());
						BYTE* pData = new BYTE[nDataSize];

                        if(false != Base64::Base64Decode((const char*)(pBase64Data + nIndex), nBase64DataSize - nIndex, pData, &nDataSize))
						{
							NSBinPptxRW::CBinaryFileReader& oBufferedStream = *pOfficeDrawingConverter->m_pReader;
							oBufferedStream.Init(pData, 0, nDataSize);

							int nVersion = g_nFormatVersion;
                            if(version.length() > 0)
							{
                                version = version.substr(1);

                                int nTempVersion = atoi(version.c_str());
								if(0 != nTempVersion)
								{
									g_nCurFormatVersion = nVersion = nTempVersion;
								}
							}
                    // File Type
                            std::wstring sDstPathCSV = sDstPath;
							BYTE fileType;
							UINT nCodePage;
							WCHAR wcDelimiter;
							BYTE saveFileType;

							SerializeCommon::ReadFileType(sXMLOptions, fileType, nCodePage, wcDelimiter, saveFileType);
                    // Делаем для CSV перебивку пути, иначе создается папка с одинаковым имеем (для rels) и файл не создается.
							
							if (BinXlsxRW::c_oFileTypes::CSV == fileType)
                                sDstPath = NSSystemPath::GetDirectoryName(sDstPath);

							OOX::Spreadsheet::CXlsx oXlsx;
							std::wstring params_path = sDstPath + FILE_SEPARATOR_STR + OOX::Spreadsheet::FileTypes::Workbook.DefaultDirectory().GetPath() + FILE_SEPARATOR_STR + OOX::FileTypes::Theme.DefaultDirectory().GetPath();
                            
							SaveParams oSaveParams(params_path.c_str());
							
							ReadMainTable(oXlsx, oBufferedStream, OOX::CPath(sSrcFileName).GetDirectory(), sDstPath, oSaveParams, pOfficeDrawingConverter);

                            std::wstring sAdditionalContentTypes = oSaveParams.sAdditionalContentTypes;

                            if(NULL != pOfficeDrawingConverter)
							{
                                sAdditionalContentTypes += pOfficeDrawingConverter->GetContentTypes();
							}
							oXlsx.PrepareToWrite();

							switch(fileType)
							{
							case BinXlsxRW::c_oFileTypes::CSV:
                                CSVWriter::WriteFromXlsxToCsv(sDstPathCSV, oXlsx, nCodePage, wcDelimiter, false);
								break;
							case BinXlsxRW::c_oFileTypes::XLSX:
							default:
								oXlsx.Write(sDstPath, sAdditionalContentTypes);
								break;
							}
							bResultOk = true;
						}
					}
				}
				return S_OK;
			}
            int ReadMainTable(OOX::Spreadsheet::CXlsx& oXlsx, NSBinPptxRW::CBinaryFileReader& oBufferedStream, const std::wstring& sFileInDir, const std::wstring& sOutDir, SaveParams& oSaveParams, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
			{
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
				BYTE mtLen = oBufferedStream.GetUChar();
				for(int i = 0; i < mtLen; ++i)
				{
					//mtItem
					res = oBufferedStream.Peek(5) == false ? c_oSerConstants::ErrorStream : c_oSerConstants::ReadOk;
					if(c_oSerConstants::ReadOk != res)
						return res;
					BYTE mtiType = oBufferedStream.GetUChar();
					long mtiOffBits = oBufferedStream.GetLong();
					if(c_oSerTableTypes::Other == mtiType)
						nOtherOffBits = mtiOffBits;
					else if(c_oSerTableTypes::SharedStrings == mtiType)
						nSharedStringsOffBits = mtiOffBits;
					else
					{
						aTypes.push_back(mtiType);
						aOffBits.push_back(mtiOffBits);
					}
				}
                OOX::CPath pathMedia = sOutDir + FILE_SEPARATOR_STR + _T("xl")   + FILE_SEPARATOR_STR + _T("media");
                std::wstring sMediaDir = pathMedia.GetPath();

                std::map<long, ImageObject*> mapMedia;
				if(-1 != nOtherOffBits)
				{
					oBufferedStream.Seek(nOtherOffBits);
					res = BinaryOtherTableReader(oBufferedStream, mapMedia, sFileInDir, oSaveParams, pOfficeDrawingConverter, sMediaDir).Read();
					if(c_oSerConstants::ReadOk != res)
						return res;
				}
				OOX::Spreadsheet::CSharedStrings* pSharedStrings = NULL;
				if(-1 != nSharedStringsOffBits)
				{
					oBufferedStream.Seek(nSharedStringsOffBits);
					pSharedStrings = oXlsx.CreateSharedStrings();
					res = BinarySharedStringTableReader(oBufferedStream, *pSharedStrings).Read();
					if(c_oSerConstants::ReadOk != res)
						return res;
				}

				OOX::Spreadsheet::CWorkbook* pWorkbook = oXlsx.CreateWorkbook();
				for(size_t i = 0, length = aTypes.size(); i < length; ++i)
				{
					BYTE mtiType = aTypes[i];
					long mtiOffBits = aOffBits[i];

					oBufferedStream.Seek(mtiOffBits);
					switch(mtiType)
					{
					case c_oSerTableTypes::Styles:
						{
							OOX::Spreadsheet::CStyles* pStyles = oXlsx.CreateStyles();
							res = BinaryStyleTableReader(oBufferedStream, *pStyles).Read();
						}
						break;
					case c_oSerTableTypes::Workbook:
						{
							res = BinaryWorkbookTableReader(oBufferedStream, *pWorkbook).Read();
						}
						break;
					case c_oSerTableTypes::Worksheets:
						{
							res = BinaryWorksheetsTableReader(oBufferedStream, *pWorkbook, pSharedStrings, oXlsx.GetWorksheets(), mapMedia, sOutDir, sMediaDir, oSaveParams, pOfficeDrawingConverter).Read();
						}
						break;
					}
					if(c_oSerConstants::ReadOk != res)
						return res;
				}
				for (std::map<long, ImageObject*>::const_iterator pPair = mapMedia.begin(); pPair != mapMedia.end(); ++pPair)
				{
					delete pPair->second;
				}
				mapMedia.clear();
				return res;
			}
			void initWorkbook(OOX::Spreadsheet::CWorkbook* pWorkbook)
			{

			}
	};
}
#endif	// #ifndef BINARY_READER
