﻿/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#ifndef BINARY_WRITER
#define BINARY_WRITER

#include "../Writer/BinaryReader.h"
#include "../Reader/CSVReader.h"
#include "CommonWriter.h"
#include "ChartFromToBinary.h"

#include "../Common/Common.h"
#include "../Common/BinReaderWriterDefines.h"
#include "../../ASCOfficePPTXFile/Editor/BinReaderWriterDefines.h"
#include "../../DesktopEditor/common/Directory.h"

#include "../../Common/OfficeFileFormats.h"
#include "../../Common/Base64.h"
#include "../../Common/OfficeFileErrorDescription.h"

#include "../../ASCOfficeDocxFile2/DocWrapper/FontProcessor.h"
#include "../../ASCOfficePPTXFile/Editor/FontCutter.h"

#include "../../Common/DocxFormat/Source/SystemUtility/SystemUtility.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/Xlsx.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/OleObject.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/ActiveX.h"
#include "../../Common/DocxFormat/Source/DocxFormat/Media/VbaProject.h"
#include "../../Common/DocxFormat/Source/XlsxFormat/WorkbookComments.h"

namespace BinXlsxRW 
{
	class BinaryTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream):m_oBcw(oCBufferedStream)
		{
		}
		void Write(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTableParts& oTableParts)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = oTableParts.m_arrItems.size(); i < length; ++i)
				WriteTablePart(oWorksheet, *oTableParts.m_arrItems[i]);
        }
		void WriteTablePart(const OOX::Spreadsheet::CWorksheet& oWorksheet, const OOX::Spreadsheet::CTablePart& oTablePart)
		{
			int nCurPos = 0;
			if(oTablePart.m_oRId.IsInit())
			{
				smart_ptr<OOX::File> pFile = oWorksheet.Find(OOX::RId(oTablePart.m_oRId->GetValue()));
				if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::Table == pFile->type())
				{
					OOX::Spreadsheet::CTableFile* pTable = static_cast<OOX::Spreadsheet::CTableFile*>(pFile.operator->());
					if(pTable->m_oTable.IsInit())
					{
						nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::Table);
						WriteTable(*pTable->m_oTable.GetPointer());
						m_oBcw.WriteItemEnd(nCurPos);
					}
				}
			}
		}
		void WriteTable(OOX::Spreadsheet::CTable& oTable)
		{
			int nCurPos = 0;
			if(oTable.m_oRef.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::Ref);
				m_oBcw.m_oStream.WriteStringW(oTable.m_oRef->ToString());
			}
			if(oTable.m_oHeaderRowCount.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::HeaderRowCount);
				m_oBcw.m_oStream.WriteLONG(oTable.m_oHeaderRowCount->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oTotalsRowCount.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TotalsRowCount);
				m_oBcw.m_oStream.WriteLONG(oTable.m_oTotalsRowCount->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oDisplayName.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_TablePart::DisplayName);
				m_oBcw.m_oStream.WriteStringW(oTable.m_oDisplayName.get2());
			}
			if(oTable.m_oAutoFilter.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::AutoFilter);
				WriteAutoFilter(oTable.m_oAutoFilter.get());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oSortState.IsInit())
			{
				OOX::Spreadsheet::CSortState* pSortState = NULL;
				if(oTable.m_oSortState.IsInit())
					pSortState = oTable.m_oSortState.GetPointer();
				else
					pSortState = oTable.m_oAutoFilter->m_oSortState.GetPointer();

				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::SortState);
				WriteSortState(*pSortState);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oTableColumns.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableColumns);
				WriteTableColumns(oTable.m_oTableColumns.get());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oTableStyleInfo.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::TableStyleInfo);
				WriteTableStyleInfo(oTable.m_oTableStyleInfo.get());
				m_oBcw.WriteItemEnd(nCurPos);
			}
			if(oTable.m_oExtLst.IsInit())
			{
				for(size_t i = 0; i < oTable.m_oExtLst->m_arrExt.size(); ++i)
				{
					OOX::Drawing::COfficeArtExtension* pExt = oTable.m_oExtLst->m_arrExt[i];
					if(pExt->m_oAltTextTable.IsInit())
					{
						nCurPos = m_oBcw.WriteItemStart(c_oSer_TablePart::AltTextTable);
						WriteAltTextTable(pExt->m_oAltTextTable.get());
						m_oBcw.WriteItemWithLengthEnd(nCurPos);
					}
				}
			}
		}
		void WriteAltTextTable(const OOX::Spreadsheet::CAltTextTable& oAltTextTable)
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
		void WriteAutoFilter(const OOX::Spreadsheet::CAutofilter& oAutofilter)
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
		void WriteFilterColumns(const std::vector<OOX::Spreadsheet::CFilterColumn *>& aFilterColumn)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = aFilterColumn.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_AutoFilter::FilterColumn);
				WriteFilterColumn(*aFilterColumn[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteFilterColumn(OOX::Spreadsheet::CFilterColumn& oFilterColumn)
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
		void WriteFilters(const OOX::Spreadsheet::CFilters& oFilters)
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
		void WriteFilter(OOX::Spreadsheet::CFilter& oFilter)
		{
			int nCurPos = 0;
			if(oFilter.m_oVal.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_Filter::Val);
				m_oBcw.m_oStream.WriteStringW(oFilter.m_oVal.get2());
			}
		}
		void WriteDateGroupItem(OOX::Spreadsheet::CDateGroupItem& oDateGroupItem)
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
		void WriteCustomFilters(const OOX::Spreadsheet::CCustomFilters& oCustomFilters)
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
		void WriteCustomFiltersItems(const std::vector<OOX::Spreadsheet::CCustomFilter *>& aCustomFilters)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = aCustomFilters.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_CustomFilters::CustomFilter);
				WriteCustomFiltersItem(*aCustomFilters[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteCustomFiltersItem(OOX::Spreadsheet::CCustomFilter& oCustomFilter)
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
		void WriteDynamicFilter(const OOX::Spreadsheet::CDynamicFilter& oDynamicFilter)
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
		void WriteColorFilter(const OOX::Spreadsheet::CColorFilter& oColorFilter)
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
		void WriteTop10(const OOX::Spreadsheet::CTop10& oTop10)
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
		void WriteSortCondition(const OOX::Spreadsheet::CSortCondition& oSortCondition)
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
		void WriteSortState(const OOX::Spreadsheet::CSortState& oSortState)
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
			int nCurPos2 = m_oBcw.WriteItemStart(c_oSer_SortState::SortConditions);
			for(size_t i = 0, length = oSortState.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_SortState::SortCondition);
				WriteSortCondition(*oSortState.m_arrItems[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			m_oBcw.WriteItemEnd(nCurPos2);
		}
		void WriteTableColumn(const OOX::Spreadsheet::CTableColumn& oTableColumn)
		{
			int nCurPos = 0;
			if(oTableColumn.m_oName.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::Name);
				m_oBcw.m_oStream.WriteStringW(oTableColumn.m_oName.get2());
			}
			if(oTableColumn.m_oTotalsRowLabel.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_TableColumns::TotalsRowLabel);
				m_oBcw.m_oStream.WriteStringW(oTableColumn.m_oTotalsRowLabel.get2());
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
				m_oBcw.m_oStream.WriteStringW(oTableColumn.m_oTotalsRowFormula.get2());
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
				m_oBcw.m_oStream.WriteStringW(oTableColumn.m_oCalculatedColumnFormula.get2());
			}
		}
		void WriteTableColumns(const OOX::Spreadsheet::CTableColumns& oTableColumns)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = oTableColumns.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TableColumns::TableColumn);
				WriteTableColumn(*oTableColumns.m_arrItems[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteTableStyleInfo(const OOX::Spreadsheet::CTableStyleInfo& oTableStyleInfo)
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
	};
	class BinaryStyleTableWriter
	{
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
	public:
		BinaryStyleTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager):m_oBcw(oCBufferedStream),m_pEmbeddedFontsManager(pEmbeddedFontsManager)
		{
        }
		void Write(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteStylesContent(styles, pTheme, oFontProcessor);
			m_oBcw.WriteItemWithLengthEnd(nStart);
        }
		void WriteStylesContent(OOX::Spreadsheet::CStyles& styles, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
        }
		void WriteBorders(const OOX::Spreadsheet::CBorders& borders, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
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
		void WriteBorder(const OOX::Spreadsheet::CBorder& border, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
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
		void WriteBorderProp(const OOX::Spreadsheet::CBorderProp& borderProp, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
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
		void WriteCellStyleXfs(const OOX::Spreadsheet::CCellStyleXfs& cellStyleXfs)
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
		void WriteCellXfs(const OOX::Spreadsheet::CCellXfs& cellXfs)
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
		void WriteXfs(const OOX::Spreadsheet::CXfs& xfs)
		{
			int nCurPos = 0;
			//ApplyAlignment
			if(false != xfs.m_oApplyAlignment.IsInit())
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
			if(false != xfs.m_oApplyFill.IsInit())
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
			if(false != xfs.m_oApplyNumberFormat.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::ApplyNumberFormat);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(xfs.m_oApplyNumberFormat->ToBool());
			}
			//BorderId
			if(false != xfs.m_oBorderId.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::BorderId);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(xfs.m_oBorderId->GetValue());
			}
			//FillId
			if(false != xfs.m_oFillId.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::FillId);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(xfs.m_oFillId->GetValue());
			}
			//FontId
			if(false != xfs.m_oFontId.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::FontId);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(xfs.m_oFontId->GetValue());
			}
			//NumFmtId
			if(false != xfs.m_oNumFmtId.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::NumFmtId);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(xfs.m_oNumFmtId->GetValue());
			}
			//QuotePrefix
			if(false != xfs.m_oQuotePrefix.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::QuotePrefix);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(xfs.m_oQuotePrefix->ToBool());
			}
			//PivotButton
			if(false != xfs.m_oPivotButton.IsInit())
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
			if(false != xfs.m_oAligment.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerXfsTypes::Aligment);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteAligment(xfs.m_oAligment.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
        }
		void WriteAligment(const OOX::Spreadsheet::CAligment& aligment)
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
				m_oBcw.m_oStream.WriteLONG(aligment.m_oIndent->GetValue());
			}
			//RelativeIndent
			if(false != aligment.m_oRelativeIndent.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerAligmentTypes::RelativeIndent);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(aligment.m_oRelativeIndent->GetValue());
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
				m_oBcw.m_oStream.WriteLONG(aligment.m_oTextRotation->GetValue());
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
		void WriteFills(const OOX::Spreadsheet::CFills& fills, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = fills.m_arrItems.size(); i < length; ++i)
			{
					OOX::Spreadsheet::CFill* pFill = fills.m_arrItems[i];
					nCurPos = m_oBcw.WriteItemStart(c_oSerStylesTypes::Fill);
					WriteFill(*pFill, pIndexedColors, pTheme, false);
					m_oBcw.WriteItemEnd(nCurPos);
			}
        }
		void WriteFill(const OOX::Spreadsheet::CFill& fill, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, bool bPriorityBg = false)
		{
			int nCurPos = 0;
			//Bottom
			OOX::Spreadsheet::CColor* pColor = NULL;
			if(fill.m_oPatternFill.IsInit())
			{
				if((false == fill.m_oPatternFill->m_oPatternType.IsInit() || SimpleTypes::Spreadsheet::patterntypeNone != fill.m_oPatternFill->m_oPatternType->GetValue()))
				{
					if(bPriorityBg)
					{
						if(fill.m_oPatternFill->m_oBgColor.IsInit())
							pColor = fill.m_oPatternFill->m_oBgColor.operator ->();
						else if(fill.m_oPatternFill->m_oFgColor.IsInit())
							pColor = fill.m_oPatternFill->m_oFgColor.operator ->();
					}
					else
					{
						if(fill.m_oPatternFill->m_oFgColor.IsInit())
							pColor = fill.m_oPatternFill->m_oFgColor.operator ->();
						else if(fill.m_oPatternFill->m_oBgColor.IsInit())
							pColor = fill.m_oPatternFill->m_oBgColor.operator ->();
					}
				}
			}
			else
			{
				//выбираем один цвет
				if(fill.m_oGradientFill.IsInit())
				{
					const OOX::Spreadsheet::CGradientFill& gradient = fill.m_oGradientFill.get();
					if(gradient.m_arrItems.size() > 0)
					{
						OOX::Spreadsheet::CGradientStop* pStop = gradient.m_arrItems[0];
						if(pStop->m_oColor.IsInit())
						{
							pColor = pStop->m_oColor.operator ->();
						}
					}
				}
			}
			if(NULL != pColor)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerFillTypes::PatternFill);
				int nCurPos2 = m_oBcw.WriteItemStart(c_oSerFillTypes::PatternFillBgColor);
				m_oBcw.WriteColor(*pColor, pIndexedColors);
				m_oBcw.WriteItemEnd(nCurPos2);
				m_oBcw.WriteItemEnd(nCurPos);
			}
        }
		void WriteFonts(const OOX::Spreadsheet::CFonts& fonts, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteFont(const OOX::Spreadsheet::CFont& font, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* theme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteNumFmts(const OOX::Spreadsheet::CNumFmts& numFmts)
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
		void WriteNumFmt(const OOX::Spreadsheet::CNumFmt& numFmt)
		{
			int nCurPos = 0;
			//FormatCode
			if(numFmt.m_oFormatCode.IsInit())
			{
				std::wstring& sFormatCode = numFmt.m_oFormatCode.get2();
				m_oBcw.m_oStream.WriteBYTE(c_oSerNumFmtTypes::FormatCode);
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
		void WriteCellStyles(const OOX::Spreadsheet::CCellStyles& oCellStyles)
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
		void WriteCellStyle(const OOX::Spreadsheet::CCellStyle& oCellStyle)
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
				m_oBcw.m_oStream.WriteStringW(oCellStyle.m_oName.get2());
			}
			if (oCellStyle.m_oXfId.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_CellStyle::XfId);
				m_oBcw.m_oStream.WriteLONG(oCellStyle.m_oXfId->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteDxfs(const OOX::Spreadsheet::CDxfs& oDxfs, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteDxf(const OOX::Spreadsheet::CDxf& oDxf, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
				WriteFill(oDxf.m_oFill.get(), pIndexedColors, pTheme, true);
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
		void WriteTableStyles(const OOX::Spreadsheet::CTableStyles& oTableStyles)
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
		void WriteTableCustomStyles(const std::vector<OOX::Spreadsheet::CTableStyle *>& aTableStyles)
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
		void WriteTableStyle(const OOX::Spreadsheet::CTableStyle& oTableStyle)
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
		void WriteTableStyleElements(const std::vector<OOX::Spreadsheet::CTableStyleElement *>& aTableStyles)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = aTableStyles.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_TableStyle::Element);
				WriteTableStyleElement(*aTableStyles[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteTableStyleElement(const OOX::Spreadsheet::CTableStyleElement& oTableStyleElement)
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
	};
	class BinarySharedStringTableWriter
	{
		BinaryCommonWriter m_oBcw;
		NSFontCutter::CEmbeddedFontsManager* m_pEmbeddedFontsManager;
	public:
		BinarySharedStringTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager):m_oBcw(oCBufferedStream),m_pEmbeddedFontsManager(pEmbeddedFontsManager)
		{
        }
		void Write(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteSharedStrings(sharedString, pIndexedColors, pTheme, oFontProcessor);
			m_oBcw.WriteItemWithLengthEnd(nStart);
        }
		void WriteSharedStrings(OOX::Spreadsheet::CSharedStrings& sharedString, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteSharedString(OOX::Spreadsheet::CSi& si, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteRun(OOX::Spreadsheet::CRun& run, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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
		void WriteRPr(const OOX::Spreadsheet::CRPr& rPr, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor)
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

	};
	class BinaryWorkbookTableWriter
	{
		BinaryCommonWriter		m_oBcw;
		OOX::Spreadsheet::CXlsx &m_oXlsx;
	public:
		BinaryWorkbookTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, OOX::Spreadsheet::CXlsx &oXlsx) : m_oBcw(oCBufferedStream), m_oXlsx(oXlsx)
		{
		}
		void Write(OOX::Spreadsheet::CWorkbook& workbook)
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteWorkbook(workbook);
			m_oBcw.WriteItemWithLengthEnd(nStart);
		}
		void WriteWorkbook(OOX::Spreadsheet::CWorkbook& workbook)
		{
			int nCurPos;
	//WorkbookPr
			if(workbook.m_oWorkbookPr.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::WorkbookPr);
				WriteWorkbookPr(workbook.m_oWorkbookPr.get());
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
			
	//Write VbaProject
			if (NULL != m_oXlsx.m_pVbaProject)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::VbaProject);

				m_oBcw.m_oStream.StartRecord(0);
                m_oXlsx.m_pVbaProject->toPPTY(&m_oBcw.m_oStream);
                m_oBcw.m_oStream.EndRecord();

                m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
	//Write JsaProject
			if (NULL != m_oXlsx.m_pJsaProject)
			{
				BYTE* pData = NULL;
				DWORD nBytesCount;
				if(NSFile::CFileBinary::ReadAllBytes(m_oXlsx.m_pJsaProject->filename().GetPath(), &pData, nBytesCount))
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::JsaProject);
					m_oBcw.m_oStream.WriteBYTEArray(pData, nBytesCount);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
	//Workbook Comments
			if (NULL != m_oXlsx.m_pWorkbookComments)
			{
				BYTE* pData = NULL;
				DWORD nBytesCount;
				if(NSFile::CFileBinary::ReadAllBytes(m_oXlsx.m_pWorkbookComments->m_oReadPath.GetPath(), &pData, nBytesCount))
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::Comments);
					m_oBcw.m_oStream.WriteBYTEArray(pData, nBytesCount);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);
				}
			}
        }
		void WriteWorkbookPr(const OOX::Spreadsheet::CWorkbookPr& workbookPr)
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
		void WriteBookViews(const OOX::Spreadsheet::CBookViews& bookViews)
		{
			int nCurPos;
			if(bookViews.m_arrItems.size() > 0)
			{
	//WorkbookView
				OOX::Spreadsheet::CWorkbookView* pWorkbookView = static_cast<OOX::Spreadsheet::CWorkbookView*>(bookViews.m_arrItems[0]);
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::WorkbookView);
				WriteWorkbookView(*pWorkbookView);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
        }
		void WriteWorkbookView(const OOX::Spreadsheet::CWorkbookView& workbookView)
		{
	//ActiveTab
			if(workbookView.m_oActiveTab.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerWorkbookViewTypes::ActiveTab);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(workbookView.m_oActiveTab->GetValue());
			}
        }
		void WriteDefinedNames(const OOX::Spreadsheet::CDefinedNames& definedNames)
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
        void WriteCalcPr(const OOX::Spreadsheet::CCalcPr& CCalcPr)
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
		void WriteExternalReferences(const OOX::Spreadsheet::CExternalReferences& externalReferences, OOX::Spreadsheet::CWorkbook& workbook)
		{
			int nCurPos;
			for(size_t i = 0, length = externalReferences.m_arrItems.size(); i < length; ++i)
			{
				OOX::Spreadsheet::CExternalReference* pExternalReference = externalReferences.m_arrItems[i];
	//ExternalReference

				if(pExternalReference->m_oRid.IsInit())
				{
					smart_ptr<OOX::File> pFile = workbook.Find( OOX::RId(pExternalReference->m_oRid->GetValue()));

					if (pFile.IsInit() && OOX::Spreadsheet::FileTypes::ExternalLinks == pFile->type())
					{
						OOX::Spreadsheet::CExternalLink* pExternalLink = static_cast<OOX::Spreadsheet::CExternalLink*>(pFile.operator ->());
						if (pExternalLink)
						{
							if(pExternalLink->m_oExternalBook.IsInit())
							{
								std::wstring sLink;
								if (pExternalLink->m_oExternalBook->m_oRid.IsInit())
								{
									smart_ptr<OOX::File> pFile = pExternalLink->Find( OOX::RId(pExternalLink->m_oExternalBook->m_oRid.get().GetValue()));
									if (pFile.IsInit() && OOX::FileTypes::ExternalLinkPath == pFile->type())
									{
										OOX::Spreadsheet::ExternalLinkPath* pLinkFile = static_cast<OOX::Spreadsheet::ExternalLinkPath*>(pFile.operator ->());
										sLink = pLinkFile->Uri().GetPath();
									}
								}
								if(!sLink.empty())
								{
									nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::ExternalBook);
									WriteExternalBook(pExternalLink->m_oExternalBook.get(), sLink);
									m_oBcw.WriteItemWithLengthEnd(nCurPos);
								}
							}
							else if(pExternalLink->m_oOleLink.IsInit())
							{
								std::wstring sLink;
								if (pExternalLink->m_oOleLink->m_oRid.IsInit())
								{
									smart_ptr<OOX::File> pFile = pExternalLink->Find( OOX::RId(pExternalLink->m_oOleLink->m_oRid.get().GetValue()));
									if (pFile.IsInit() && OOX::FileTypes::OleObject == pFile->type())
									{
										OOX::OleObject* pLinkFile = static_cast<OOX::OleObject*>(pFile.operator ->());
										sLink = pLinkFile->filename().GetPath();
									}
								}
								if(!sLink.empty())
								{
									nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::OleLink);
									WriteOleLink(pExternalLink->m_oOleLink.get(), sLink);
									m_oBcw.WriteItemWithLengthEnd(nCurPos);
								}
							}
							else if(pExternalLink->m_oDdeLink.IsInit())
							{
								nCurPos = m_oBcw.WriteItemStart(c_oSerWorkbookTypes::DdeLink);
								WriteDdeLink(pExternalLink->m_oDdeLink.get());
								m_oBcw.WriteItemWithLengthEnd(nCurPos);
							}
						}
					}
				}
			}
        }
		void WriteExternalBook(const OOX::Spreadsheet::CExternalBook& externalBook, const std::wstring& sLink)
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
		void WriteExternalSheetNames(const OOX::Spreadsheet::CExternalSheetNames& sheetNames)
		{
			int nCurPos = 0;
			for (size_t i = 0, length = sheetNames.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetName);
				m_oBcw.m_oStream.WriteStringW3(sheetNames.m_arrItems[i]->ToString2());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		void WriteExternalDefinedNames(const OOX::Spreadsheet::CExternalDefinedNames& definedNames)
		{
			int nCurPos = 0;
			for (size_t i = 0, length = definedNames.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::DefinedName);
				WriteExternalDefinedName(*definedNames.m_arrItems[i]);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		void WriteExternalDefinedName(const OOX::Spreadsheet::CExternalDefinedName& definedName)
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
		void WriteExternalSheetDataSet(const OOX::Spreadsheet::CExternalSheetDataSet& sheetDataSet)
		{
			int nCurPos = 0;
			for (size_t i = 0, length = sheetDataSet.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ExternalLinkTypes::SheetData);
				WriteExternalSheetData(*sheetDataSet.m_arrItems[i]);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		void WriteExternalSheetData(const OOX::Spreadsheet::CExternalSheetData& sheetData)
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
		void WriteExternalRow(const OOX::Spreadsheet::CExternalRow& row)
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
		void WriteExternalCell(const OOX::Spreadsheet::CExternalCell& cell)
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
		void WriteOleLink(const OOX::Spreadsheet::COleLink& oleLink, const std::wstring& sLink)
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
		void WriteOleItem(const OOX::Spreadsheet::COleItem& oleItem)
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
		void WriteDdeLink(const OOX::Spreadsheet::CDdeLink& ddeLink)
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
		void WriteDdeItem(const OOX::Spreadsheet::CDdeItem& ddeItem)
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
		void WriteDdeValues(const OOX::Spreadsheet::CDdeValues& ddeValues)
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
		void WriteDdeValue(const OOX::Spreadsheet::CDdeValue& ddeValue)
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
		void WriteDefinedName(const OOX::Spreadsheet::CDefinedName& definedName)
		{
			int nCurPos = 0;
			//Name
			if(definedName.m_oName.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerDefinedNameTypes::Name);
				m_oBcw.m_oStream.WriteStringW(definedName.m_oName.get2());
			}
			//Ref
			if(definedName.m_oRef.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerDefinedNameTypes::Ref);
				m_oBcw.m_oStream.WriteStringW(definedName.m_oRef.get2());
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
				m_oBcw.m_oStream.WriteStringW(definedName.m_oComment.get2());
			}
		}
	};
	class BinaryWorksheetTableWriter
	{
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
		OOX::Spreadsheet::CIndexedColors*		m_pIndexedColors;
		PPTX::Theme*							m_pTheme;
		DocWrapper::FontProcessor&				m_oFontProcessor;
		NSBinPptxRW::CDrawingConverter*			m_pOfficeDrawingConverter;
	public:
		BinaryWorksheetTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, OOX::Spreadsheet::CIndexedColors* pIndexedColors, PPTX::Theme* pTheme, DocWrapper::FontProcessor& oFontProcessor, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter):
		  m_oBcw(oCBufferedStream),m_pEmbeddedFontsManager(pEmbeddedFontsManager),m_pIndexedColors(pIndexedColors),m_pTheme(pTheme),m_oFontProcessor(oFontProcessor),m_pOfficeDrawingConverter(pOfficeDrawingConverter)
		{
        }
		void Write(OOX::Spreadsheet::CWorkbook& workbook,  std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets)
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteWorksheets(workbook, mapWorksheets);
			m_oBcw.WriteItemWithLengthEnd(nStart);
        }
		void WriteWorksheets(OOX::Spreadsheet::CWorkbook& workbook, std::map<std::wstring, OOX::Spreadsheet::CWorksheet*>& mapWorksheets)
		{
			int nCurPos;
			//определяем порядок следования .. излинише с vector
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
							WriteWorksheet(*pSheet, *pFind->second);
							m_oBcw.WriteItemWithLengthEnd(nCurPos);
						}
					}
				}
			}
        }
		void WriteWorksheet(OOX::Spreadsheet::CSheet& oSheet, OOX::Spreadsheet::CWorksheet& oWorksheet)
		{
			int nCurPos;
			//WorksheetProp
			nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::WorksheetProp);
			WriteWorksheetProp(oSheet);
			m_oBcw.WriteItemWithLengthEnd(nCurPos);

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
				m_oBcw.m_oStream.WriteStringW(oWorksheet.m_oDimension->m_oRef.get2());
			}
			//SheetViews
			if(oWorksheet.m_oSheetViews.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetViews);
				WriteSheetViews(oWorksheet.m_oSheetViews.get());
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
				WriteConditionalFormattings(oWorksheet.m_arrConditionalFormatting);
			}
            if (oWorksheet.m_oExtLst.IsInit())
            {
                for(size_t i = 0; i < oWorksheet.m_oExtLst->m_arrExt.size(); ++i)
                {
                    OOX::Drawing::COfficeArtExtension* pExt = oWorksheet.m_oExtLst->m_arrExt[i];
					if ( !pExt->m_arrConditionalFormatting.empty() )
					{
						WriteConditionalFormattings(pExt->m_arrConditionalFormatting);
					}
                }
            }
			//Drawing
			if(oWorksheet.m_oDrawing.IsInit() && oWorksheet.m_oDrawing->m_oId.IsInit())
			{
				//из Drawing могут быть ссылки на объекты в VmlDrawing
				OOX::CVmlDrawing *currentVmlDrawing = NULL;

                if (oWorksheet.m_oLegacyDrawing.IsInit() &&
                                oWorksheet.m_oLegacyDrawing->m_oId.IsInit())
				{
                    smart_ptr<OOX::File> oFileV = oWorksheet.Find(oWorksheet.m_oLegacyDrawing->m_oId->GetValue());
					if (oFileV.IsInit() && OOX::FileTypes::VmlDrawing == oFileV->type())
					{
						currentVmlDrawing = (OOX::CVmlDrawing*)oFileV.operator->();
					}
				}
				
				smart_ptr<OOX::File> oFile = oWorksheet.Find(oWorksheet.m_oDrawing->m_oId->GetValue());
				
				if (oFile.IsInit() && OOX::Spreadsheet::FileTypes::Drawings == oFile->type())
				{
					OOX::Spreadsheet::CDrawing* pDrawing = (OOX::Spreadsheet::CDrawing*)oFile.operator->();
					
					smart_ptr<OOX::IFileContainer> oldRels = m_pOfficeDrawingConverter->GetRels();
					m_pOfficeDrawingConverter->SetRels(pDrawing);
					
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawings);
					WriteDrawings(oWorksheet, pDrawing, currentVmlDrawing);
					m_oBcw.WriteItemWithLengthEnd(nCurPos);

					m_pOfficeDrawingConverter->SetRels(oldRels);
				}
			}
			if(oWorksheet.m_oLegacyDrawingHF.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::LegacyDrawingHF);
				WriteLegacyDrawingHF(oWorksheet);
				m_oBcw.WriteItemEnd(nCurPos);
			}
			//Autofilter
			if(oWorksheet.m_oAutofilter.IsInit())
			{
				BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Autofilter);
				oBinaryTableWriter.WriteAutoFilter(oWorksheet.m_oAutofilter.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			//TableParts
			if(oWorksheet.m_oTableParts.IsInit())
			{
				BinaryTableWriter oBinaryTableWriter(m_oBcw.m_oStream);
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::TableParts);
				oBinaryTableWriter.Write(oWorksheet, oWorksheet.m_oTableParts.get());
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
			//Comments
			if(oWorksheet.m_mapComments.size() > 0)
			{
				bool bIsEmpty = true;
				int nCurPos = 0;

                for (boost::unordered_map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = oWorksheet.m_mapComments.begin(); it != oWorksheet.m_mapComments.end(); ++it)
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
			if (oWorksheet.m_oPicture.IsInit() && oWorksheet.m_oPicture->m_oId.IsInit())
			{
				smart_ptr<OOX::File> pFile = oWorksheet.Find(oWorksheet.m_oPicture->m_oId->GetValue());
				if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
				{
					OOX::Image* pImageFileCache = static_cast<OOX::Image*>(pFile.operator->());
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
        }
		void WriteWorksheetProp(OOX::Spreadsheet::CSheet& oSheet)
		{
			//Name
			if(oSheet.m_oName.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetPropTypes::Name);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				m_oBcw.m_oStream.WriteStringW(oSheet.m_oName.get2());
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
		void WriteCols(const OOX::Spreadsheet::CCols& oCols)
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
		void WriteCol(const OOX::Spreadsheet::CCol& oCol)
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
        }

		void WriteSheetViews(const OOX::Spreadsheet::CSheetViews& oSheetViews)
		{
			int nCurPos = 0;
			for (size_t nIndex = 0, nLength = oSheetViews.m_arrItems.size(); nIndex < nLength; ++nIndex)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::SheetView);
				WriteSheetView(*oSheetViews.m_arrItems[nIndex]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}

		void WriteSheetView(const OOX::Spreadsheet::CSheetView& oSheetView)
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
				m_oBcw.m_oStream.WriteStringW4(oSheetView.m_oTopLeftCell.get2());
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

		void WritePane(const OOX::Spreadsheet::CPane& oPane)
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
				m_oBcw.m_oStream.WriteStringW(oPane.m_oTopLeftCell.get2());
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
		void WriteSelection(const OOX::Spreadsheet::CSelection& oSelection)
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

		void WriteSheetFormatPr(const OOX::Spreadsheet::CSheetFormatPr& oSheetFormatPr)
		{
			//DefaultColWidth
			if(oSheetFormatPr.m_oDefaultColWidth.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::DefaultColWidth);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
				m_oBcw.m_oStream.WriteDoubleReal(oSheetFormatPr.m_oDefaultColWidth->GetValue());
			}
			//DefaultRowHeight
			if(oSheetFormatPr.m_oDefaultRowHeight.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::DefaultRowHeight);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Double);
				m_oBcw.m_oStream.WriteDoubleReal(oSheetFormatPr.m_oDefaultRowHeight->GetValue());
			}
			//BaseColWidth
			if(oSheetFormatPr.m_oBaseColWidth.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::BaseColWidth);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Long);
				m_oBcw.m_oStream.WriteLONG(oSheetFormatPr.m_oBaseColWidth->GetValue());
			}
			//CustomHeight
			if(oSheetFormatPr.m_oCustomHeight.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::CustomHeight);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBYTE(oSheetFormatPr.m_oCustomHeight->ToBool());
			}
			//ZeroHeight
			if(oSheetFormatPr.m_oZeroHeight.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerSheetFormatPrTypes::ZeroHeight);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBYTE(oSheetFormatPr.m_oZeroHeight->ToBool());
			}
		}
		void WritePageMargins(const OOX::Spreadsheet::CPageMargins& oPageMargins)
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
		void WritePageSetup(const OOX::Spreadsheet::CPageSetup& oPageSetup)
		{
			//Orientation
			if(oPageSetup.m_oOrientation.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::Orientation);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oOrientation->GetValue());
			}
			//PageSize
			if(oPageSetup.m_oPaperSize.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_PageSetup::PaperSize);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBYTE(oPageSetup.m_oPaperSize->GetValue());
			}
		}
		void WritePrintOptions(const OOX::Spreadsheet::CPrintOptions& oPrintOptions)
		{
			//GridLines
			bool bGridLines = false;
			if((oPrintOptions.m_oGridLines.IsInit() && oPrintOptions.m_oGridLines->ToBool()) || (oPrintOptions.m_oGridLinesSet.IsInit() && oPrintOptions.m_oGridLinesSet->ToBool()))
				bGridLines = true;
			m_oBcw.m_oStream.WriteBYTE(c_oSer_PrintOptions::GridLines);
			m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
			m_oBcw.m_oStream.WriteBOOL(bGridLines);
			//Headings
			if(oPrintOptions.m_oHeadings.IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_PrintOptions::Headings);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Byte);
				m_oBcw.m_oStream.WriteBOOL(oPrintOptions.m_oHeadings->ToBool());
			}
		}
		void WriteHyperlinks(const OOX::Spreadsheet::CHyperlinks& oHyperlinks, OOX::Spreadsheet::CWorksheet& oWorksheet)
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
		void WriteHyperlink(const OOX::Spreadsheet::CHyperlink& oHyperlink, OOX::Spreadsheet::CWorksheet& oWorksheet)
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
			if(oHyperlink.m_oLocation.IsInit())
				sLocation = oHyperlink.m_oLocation.get();
			if (oHyperlink.m_oDisplay.IsInit())
				sDisplay = oHyperlink.m_oDisplay.get();

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
					m_oBcw.m_oStream.WriteStringW(oHyperlink.m_oTooltip.get2());
				}
				//Display
				if(oHyperlink.m_oDisplay.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerHyperlinkTypes::Display);
					m_oBcw.m_oStream.WriteStringW(oHyperlink.m_oDisplay.get2());
				}
			}
        }
		void WriteMergeCells(const OOX::Spreadsheet::CMergeCells& oMergeCells)
		{
			for(size_t i = 0, length = oMergeCells.m_arrItems.size(); i < length; ++i)
			{
				OOX::Spreadsheet::CMergeCell* pMergeCell = oMergeCells.m_arrItems[i];
				if(pMergeCell->m_oRef.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSerWorksheetsTypes::MergeCell);
					m_oBcw.m_oStream.WriteStringW(pMergeCell->m_oRef.get2());
				}
			}
        }
		void WriteSheetData(const OOX::Spreadsheet::CSheetData& oSheetData)
		{
			int nCurPos;
			for(size_t i = 0, length = oSheetData.m_arrItems.size(); i < length; ++i)
			{
				OOX::Spreadsheet::CRow* pRow = oSheetData.m_arrItems[i];
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Row);
				WriteRow(*pRow);
				m_oBcw.WriteItemEnd(nCurPos);
			}
        }
		void WriteRow(const OOX::Spreadsheet::CRow& oRows)
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
            if(oRows.m_arrItems.size() > 0)
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSerRowTypes::Cells);
				m_oBcw.m_oStream.WriteBYTE(c_oSerPropLenType::Variable);
				nCurPos = m_oBcw.WriteItemWithLengthStart();
				WriteCells(oRows);
				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
        }
		void WriteCells(const OOX::Spreadsheet::CRow& oRows)
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
		void WriteCell(const OOX::Spreadsheet::CCell& oCell)
		{
			int nCurPos;
			//Ref
			if (oCell.isInitRef())
			{
				int nRow = 0;
				int nCol = 0;
				oCell.getRowCol(nRow, nCol);

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
				m_oBcw.m_oStream.WriteLONG(oCell.m_oStyle->GetValue());
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
			if(oCell.m_oValue.IsInit())
			{

                double dValue = 0;
                try
                {
                    dValue = _wtof(oCell.m_oValue->ToString().c_str());
                }
                catch(...)
                {   //1.3912059045063478e-310
                    //Lighting Load Calculation.xls
                }

				nCurPos = m_oBcw.WriteItemStart(c_oSerCellTypes::Value);
				m_oBcw.m_oStream.WriteDoubleReal(dValue);
				m_oBcw.WriteItemEnd(nCurPos);
			}
        }
		void WriteFormula(OOX::Spreadsheet::CFormula& oFormula)
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

        void WriteDrawings(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::CVmlDrawing *pVmlDrawing = NULL)
		{
			for (size_t i = 0, length = pDrawing->m_arrItems.size(); i  < length ; ++i)
			{
				OOX::Spreadsheet::CCellAnchor* pCellAnchor = pDrawing->m_arrItems[i];
		
		//we use legacyDrawing or objectPr in OleObject so skip shape in drawing
			
				if (!pCellAnchor->m_bShapeOle && pCellAnchor->isValid())
				{
					if(oWorksheet.m_oOleObjects.IsInit() && pCellAnchor->m_nId.IsInit())
					{
                        boost::unordered_map<int, OOX::Spreadsheet::COleObject*>::const_iterator pFind = oWorksheet.m_oOleObjects->m_mapOleObjects.find(pCellAnchor->m_nId.get());
						if (pFind != oWorksheet.m_oOleObjects->m_mapOleObjects.end())
						{
							pCellAnchor->m_bShapeOle = true;
							continue;
						}
					}
					int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
					WriteDrawing(oWorksheet, pDrawing, pCellAnchor, pVmlDrawing);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
	//OleObjects
			if(oWorksheet.m_oOleObjects.IsInit())
			{
				for (boost::unordered_map<int, OOX::Spreadsheet::COleObject*>::const_iterator it = oWorksheet.m_oOleObjects->m_mapOleObjects.begin(); it != oWorksheet.m_oOleObjects->m_mapOleObjects.end(); ++it)
				{
					OOX::Spreadsheet::COleObject*	pOleObject	= it->second;
					OOX::WritingElement*			pShapeElem	= NULL;
					OOX::Spreadsheet::CCellAnchor*	pCellAnchor = NULL;
                    
					if (!pOleObject) continue;

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
					bool bSetAnchor = false;
					if (pOleObject->m_oObjectPr.IsInit() && pOleObject->m_oObjectPr->m_oAnchor.IsInit() && pOleObject->m_oObjectPr->m_oRid.IsInit())
					{						
						const OOX::Spreadsheet::CExtAnchor& oAnchor = pOleObject->m_oObjectPr->m_oAnchor.get();

						SimpleTypes::Spreadsheet::CCellAnchorType<> eAnchorType;
						eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
						
						if (oAnchor.m_oFrom.IsInit() && oAnchor.m_oTo.IsInit())
						{
							if(oAnchor.m_oMoveWithCells.IsInit() && oAnchor.m_oMoveWithCells->ToBool())
								eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorOneCell);
							else if(oAnchor.m_oSizeWithCells.IsInit() && oAnchor.m_oSizeWithCells->ToBool())
								eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorTwoCell);
							else
								eAnchorType.SetValue(SimpleTypes::Spreadsheet::cellanchorAbsolute);
							
							pCellAnchor = new OOX::Spreadsheet::CCellAnchor(eAnchorType);
							
							pCellAnchor->m_oFrom	= oAnchor.m_oFrom.get();
							pCellAnchor->m_oTo		= oAnchor.m_oTo.get();
							
							bSetAnchor = true;
						}
						else
							pCellAnchor = new OOX::Spreadsheet::CCellAnchor(eAnchorType);

						pCellAnchor->m_bShapeOle= true;
						
						PPTX::Logic::Pic *olePic = new PPTX::Logic::Pic;
						
						olePic->oleObject.Init();
						olePic->blipFill.blip.Init();	

						if (pOleObject->m_oProgId.IsInit())
							olePic->oleObject->m_sProgId = pOleObject->m_oProgId.get();
						
						olePic->oleObject->m_oId = pRId;
						olePic->oleObject->m_OleObjectFile = pFileOleObject;

						if (olePic->oleObject->m_OleObjectFile.IsInit())
						{
							olePic->blipFill.blip->oleFilepathBin = olePic->oleObject->m_OleObjectFile->filename().GetPath();
						}

						OOX::Image*		pImageFileCache = NULL;
						std::wstring	sIdImageFileCache;
						if ((NULL != pShapeElem) && (OOX::et_v_shapetype != pShapeElem->getType()))
						{
							OOX::Vml::CShape* pShape = static_cast<OOX::Vml::CShape*>(pShapeElem);
							for(size_t j = 0; (pShape) && (j < pShape->m_arrItems.size()); ++j)
							{
								OOX::WritingElement* pChildElemShape = pShape->m_arrItems[j];
								if(!bSetAnchor && OOX::et_v_ClientData == pChildElemShape->getType())
								{
									OOX::Vml::CClientData* pClientData = static_cast<OOX::Vml::CClientData*>(pChildElemShape);
									pClientData->toCellAnchor(pCellAnchor);
								}
								if(OOX::et_v_imagedata == pChildElemShape->getType())
								{
									OOX::Vml::CImageData* pImageData = static_cast<OOX::Vml::CImageData*>(pChildElemShape);									
														
									if (pImageData->m_oRelId.IsInit())		sIdImageFileCache = pImageData->m_oRelId->GetValue();
									else if (pImageData->m_rId.IsInit())	sIdImageFileCache = pImageData->m_rId->GetValue();
									else if (pImageData->m_rPict.IsInit())	sIdImageFileCache = pImageData->m_rPict->GetValue();
																		
									if (!sIdImageFileCache.empty())
									{
										//ищем физический файл ( rId относительно vml_drawing)									
										smart_ptr<OOX::File> pFile = pVmlDrawing->Find(sIdImageFileCache);
										
										if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
										{
											pImageFileCache = static_cast<OOX::Image*>(pFile.operator->());
										}
 									}
								}
							}	
						}
						
						if (pImageFileCache == NULL && pOleObject->m_oObjectPr.IsInit() && pOleObject->m_oObjectPr->m_oRid.IsInit())
						{
							sIdImageFileCache = pOleObject->m_oObjectPr->m_oRid->GetValue();
							
							smart_ptr<OOX::File> pFile = oWorksheet.Find(sIdImageFileCache);
							if (pFile.IsInit() && (	OOX::FileTypes::Image == pFile->type()))
							{
								pImageFileCache = static_cast<OOX::Image*>(pFile.operator->());
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

						pCellAnchor->m_oElement = new PPTX::Logic::SpTreeElem();
						pCellAnchor->m_oElement->InitElem(olePic);
					}

					if (pCellAnchor)
					{
						int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
						WriteDrawing(oWorksheet, pDrawing, pCellAnchor, pVmlDrawing, pOleObject);
						m_oBcw.WriteItemEnd(nCurPos);
						
						delete pCellAnchor;
					}
				}
			}

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
									
									OOX::Spreadsheet::CCellAnchor *pCellAnchor = NULL;
									pClientData->toCellAnchor(pCellAnchor);

									if (pCellAnchor)
									{
										pCellAnchor->m_sVmlSpId.Init();
										pCellAnchor->m_sVmlSpId->append(it->first);

										int nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Drawing);
										WriteDrawing(oWorksheet, pDrawing, pCellAnchor, pVmlDrawing, NULL);
										m_oBcw.WriteItemEnd(nCurPos);
										
										delete pCellAnchor;
									}
								}
							}
						}
						it->second.bUsed = true;
					}
				}
			}
		}
        void WriteDrawing(const OOX::Spreadsheet::CWorksheet& oWorksheet, OOX::Spreadsheet::CDrawing* pDrawing, OOX::Spreadsheet::CCellAnchor* pCellAnchor, OOX::CVmlDrawing *pVmlDrawing = NULL, OOX::Spreadsheet::COleObject* pOleObject = NULL)
		{
			if (!pCellAnchor) return;

			if (pCellAnchor->m_oElement.IsInit() == false && 
				pCellAnchor->m_sVmlSpId.IsInit() == false) return;
	//Type
			int nCurPos;
			nCurPos = m_oBcw.WriteItemStart(c_oSer_DrawingType::Type);
			m_oBcw.m_oStream.WriteBYTE(pCellAnchor->m_oAnchorType.GetValue());
			m_oBcw.WriteItemEnd(nCurPos);
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
				m_oBcw.m_oStream.WriteBYTE(c_oSer_DrawingType::pptxDrawing);
				int nCurPos = m_oBcw.WriteItemWithLengthStart();

				smart_ptr<OOX::IFileContainer> oldRels = *m_oBcw.m_oStream.m_pCurrentContainer;
				*m_oBcw.m_oStream.m_pCurrentContainer = pDrawing;
				m_oBcw.m_oStream.m_pCurrentContainer->AddRef();

				m_oBcw.m_oStream.StartRecord(0);
				m_oBcw.m_oStream.WriteRecord2(1, pCellAnchor->m_oElement->GetElem());
				m_oBcw.m_oStream.EndRecord();

				*m_oBcw.m_oStream.m_pCurrentContainer = oldRels;

				m_oBcw.WriteItemWithLengthEnd(nCurPos);
			}
		}
		void WriteLegacyDrawingHF(const OOX::Spreadsheet::CWorksheet& oWorksheet)
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
					OOX::CVmlDrawing* pVmlDrawing = (OOX::CVmlDrawing*)oFileV.operator->();
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
		void WriteLegacyDrawingHFDrawings(OOX::CVmlDrawing* pVmlDrawing)
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
		void WriteLegacyDrawingHFDrawing(const OOX::CVmlDrawing::_vml_shape& oVmlShape)
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

		void WriteFromTo(const OOX::Spreadsheet::CFromTo& oFromTo)
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
		void WritePos(const OOX::Spreadsheet::CPos& oPos)
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
		void WriteExt(const OOX::Spreadsheet::CExt& oExt)
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
        void WriteComments(boost::unordered_map<std::wstring, OOX::Spreadsheet::CCommentItem*>& mapComments)
		{
			int nCurPos = 0;

            for (boost::unordered_map<std::wstring, OOX::Spreadsheet::CCommentItem*>::const_iterator it = mapComments.begin(); it != mapComments.end(); ++it)
			{
				if(it->second->IsValid())
				{
					OOX::Spreadsheet::CCommentItem& oComment = *it->second;
					std::vector<SerializeCommon::CommentData*> aCommentDatas;
					getSavedComment(oComment, aCommentDatas);
					nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::Comment);
					//записываем тот обьект, который был в бинарнике, подменяем только текст, который мог быть отредактирован в Excel
					WriteComment(oComment, aCommentDatas, oComment.m_oText);
					m_oBcw.WriteItemEnd(nCurPos);

					for(size_t i = 0, length = aCommentDatas.size(); i < length; ++i)
					{
						RELEASEOBJECT(aCommentDatas[i]);
					}
					aCommentDatas.clear();
				}
			}
		}
		void getSavedComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aDatas)
		{
			if(oComment.m_sGfxdata.IsInit())
			{
                const std::wstring& sGfxData = oComment.m_sGfxdata.get2();
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
		void WriteComment(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas, nullable<OOX::Spreadsheet::CSi>& oCommentText)
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
			WriteCommentData(oComment, aCommentDatas, oCommentText);
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
		}
		void WriteCommentData(OOX::Spreadsheet::CCommentItem& oComment, std::vector<SerializeCommon::CommentData*>& aCommentDatas, nullable<OOX::Spreadsheet::CSi>& oCommentText)
		{
			int nCurPos = 0;
			if(aCommentDatas.size() > 0)
			{
				for(size_t i = 0, length = aCommentDatas.size(); i < length; ++i)
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSer_Comments::CommentData);
					if(0 == i)
						WriteCommentDataContent(&oComment, aCommentDatas[i], &oCommentText);
					else
						WriteCommentDataContent(NULL, aCommentDatas[i], NULL);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			else
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_Comments::CommentData);
				WriteCommentDataContent(&oComment, NULL, &oCommentText);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteCommentDataContent(OOX::Spreadsheet::CCommentItem* pComment, SerializeCommon::CommentData* pCommentData, nullable<OOX::Spreadsheet::CSi>* pCommentText)
		{
			int nCurPos = 0;
			if(NULL != pCommentData && !pCommentData->sText.empty())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Text);
				m_oBcw.m_oStream.WriteStringW(pCommentData->sText);
			}
			else if(NULL != pCommentText && pCommentText->IsInit())
			{
				m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::Text);
				m_oBcw.m_oStream.WriteStringW((*pCommentText)->ToString());
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
				if (!pCommentData->sUserName.empty())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserName);
					m_oBcw.m_oStream.WriteStringW(pCommentData->sUserName);
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
					WriteCommentReplies(pCommentData->aReplies);
					m_oBcw.WriteItemEnd(nCurPos);
				}
			}
			else if(NULL != pComment)
			{
				if(pComment->m_sAuthor.IsInit())
				{
					m_oBcw.m_oStream.WriteBYTE(c_oSer_CommentData::UserName);
					m_oBcw.m_oStream.WriteStringW(pComment->m_sAuthor.get2());
				}
			}
		}
		void WriteCommentReplies(std::vector<SerializeCommon::CommentData*>& aReplies)
		{
			int nCurPos = 0;
			for(size_t i = 0, length = aReplies.size(); i < length; i++)
			{
				SerializeCommon::CommentData* pReply = aReplies[i];
				nCurPos = m_oBcw.WriteItemStart(c_oSer_CommentData::Reply);
				WriteCommentDataContent(NULL, pReply, NULL);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteSheetPr(const OOX::Spreadsheet::CSheetPr& oSheetPr)
		{
			int nCurPos = 0;
			if (oSheetPr.m_oCodeName.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_SheetPr::CodeName);
				m_oBcw.m_oStream.WriteStringW4(oSheetPr.m_oCodeName.get2());
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
				m_oBcw.m_oStream.WriteStringW4(oSheetPr.m_oSyncRef.get2());
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
		}
		void WritemHeaderFooter(const OOX::Spreadsheet::CHeaderFooter& oHeaderFooter)
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
		void WritemRowColBreaks(const OOX::Spreadsheet::CRowColBreaks& oRowColBreaks)
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
			for (int i = 0; i < oRowColBreaks.m_arrItems.size(); ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_RowColBreaks::Break);
				WritemBreak(*oRowColBreaks.m_arrItems[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WritemBreak(const OOX::Spreadsheet::CBreak& oBreak)
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
		void WriteConditionalFormattings(std::vector<OOX::Spreadsheet::CConditionalFormatting*>& arrConditionalFormatting)
		{
			int nCurPos = 0;
			for (size_t nIndex = 0, nLength = arrConditionalFormatting.size(); nIndex < nLength; ++nIndex)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSerWorksheetsTypes::ConditionalFormatting);
				WriteConditionalFormatting(*arrConditionalFormatting[nIndex]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteConditionalFormatting(const OOX::Spreadsheet::CConditionalFormatting& oConditionalFormatting)
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

			if (0 < oConditionalFormatting.m_arrItems.size())
			{
				WriteConditionalFormattingRules(oConditionalFormatting.m_arrItems);
			}
		}
		void WriteConditionalFormattingRules(const std::vector<OOX::Spreadsheet::CConditionalFormattingRule *>& aConditionalFormattingRules)
		{
			int nCurPos = 0;
			for (size_t i = 0, length = aConditionalFormattingRules.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormatting::ConditionalFormattingRule);
				WriteConditionalFormattingRule(*aConditionalFormattingRules[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteConditionalFormattingRule(const OOX::Spreadsheet::CConditionalFormattingRule& oConditionalFormattingRule)
		{
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
				m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::TimePeriod);
				m_oBcw.m_oStream.WriteStringW(oConditionalFormattingRule.m_oTimePeriod.get2());
			}
			if (oConditionalFormattingRule.m_oType.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::Type);
				m_oBcw.m_oStream.WriteBYTE(oConditionalFormattingRule.m_oType->GetValue());
				m_oBcw.WriteItemEnd(nCurPos);
			}

			if (0 < oConditionalFormattingRule.m_arrItems.size())
			{
				WriteConditionalFormattingRuleElements(oConditionalFormattingRule.m_arrItems);
			}
		}
		void WriteConditionalFormattingRuleElements(const std::vector<OOX::Spreadsheet::WritingElement *>& aConditionalFormattingRuleElements)
		{
			OOX::Spreadsheet::CColorScale* pColorScale = NULL;
			OOX::Spreadsheet::CDataBar* pDataBar = NULL;
			OOX::Spreadsheet::CFormulaCF* pFormulaCF = NULL;
			OOX::Spreadsheet::CIconSet* pIconSet = NULL;

			int nCurPos = 0;
			for (size_t i = 0, length = aConditionalFormattingRuleElements.size(); i < length; ++i)
			{
				switch (aConditionalFormattingRuleElements[i]->getType())
				{
				case OOX::et_x_ColorScale:
					pColorScale = static_cast<OOX::Spreadsheet::CColorScale*>(aConditionalFormattingRuleElements[i]);
					nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::ColorScale);
					WriteColorScale(*pColorScale);
					m_oBcw.WriteItemEnd(nCurPos);
					break;
				case OOX::et_x_DataBar:
					pDataBar = static_cast<OOX::Spreadsheet::CDataBar*>(aConditionalFormattingRuleElements[i]);
					nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::DataBar);
					WriteDataBar(*pDataBar);
					m_oBcw.WriteItemEnd(nCurPos);
					break;
				case OOX::et_x_FormulaCF:
					pFormulaCF = static_cast<OOX::Spreadsheet::CFormulaCF*>(aConditionalFormattingRuleElements[i]);
					m_oBcw.m_oStream.WriteBYTE(c_oSer_ConditionalFormattingRule::FormulaCF);
					m_oBcw.m_oStream.WriteStringW(pFormulaCF->m_sText);
					break;
				case OOX::et_x_IconSet:
					pIconSet = static_cast<OOX::Spreadsheet::CIconSet*>(aConditionalFormattingRuleElements[i]);
					nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRule::IconSet);
					WriteIconSet(*pIconSet);
					m_oBcw.WriteItemEnd(nCurPos);
					break;
				default:
					break;
				}
			}
		}
		void WriteColorScale(const OOX::Spreadsheet::CColorScale& oColorScale)
		{
			OOX::Spreadsheet::CConditionalFormatValueObject* pCFVO = NULL;
			OOX::Spreadsheet::CColor* pColor = NULL;

			// ToDo более правильно заделать виртуальную функцию, которая будет писать без привидения типов
			int nCurPos = 0;

			for (size_t i = 0, length = oColorScale.m_arrItems.size(); i < length; ++i)
			{
				pCFVO = dynamic_cast<OOX::Spreadsheet::CConditionalFormatValueObject*>(oColorScale.m_arrItems[i]);
				if (NULL != pCFVO)
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRuleColorScale::CFVO);
					WriteCFVO(*pCFVO);
					m_oBcw.WriteItemEnd(nCurPos);
					continue;
				}
				pColor = dynamic_cast<OOX::Spreadsheet::CColor*>(oColorScale.m_arrItems[i]);
				if (NULL != pColor)
				{
					nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingRuleColorScale::Color);
					m_oBcw.WriteColor(*pColor, m_pIndexedColors);
					m_oBcw.WriteItemEnd(nCurPos);
					continue;
				}
			}
		}
		void WriteDataBar(const OOX::Spreadsheet::CDataBar& oDataBar)
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
			if (oDataBar.m_oColor.IsInit())
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::Color);
				m_oBcw.WriteColor(oDataBar.m_oColor.get(), m_pIndexedColors);
				m_oBcw.WriteItemEnd(nCurPos);
			}

			for (size_t i = 0, length = oDataBar.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingDataBar::CFVO);
				WriteCFVO(*oDataBar.m_arrItems[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteIconSet(const OOX::Spreadsheet::CIconSet& oIconSet)
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

			for (size_t i = 0, length = oIconSet.m_arrItems.size(); i < length; ++i)
			{
				nCurPos = m_oBcw.WriteItemStart(c_oSer_ConditionalFormattingIconSet::CFVO);
				WriteCFVO(*oIconSet.m_arrItems[i]);
				m_oBcw.WriteItemEnd(nCurPos);
			}
		}
		void WriteCFVO(const OOX::Spreadsheet::CConditionalFormatValueObject& oCFVO)
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
		}
        void WriteSparklineGroups(const OOX::Spreadsheet::CSparklineGroups& oSparklineGroups)
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
        void WriteSparklineGroup(const OOX::Spreadsheet::CSparklineGroup& oSparklineGroup)
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
        void WriteSparklines(const OOX::Spreadsheet::CSparklines& oSparklines)
        {
            int nCurPos = 0;
            for(size_t i = 0; i < oSparklines.m_arrItems.size(); ++i)
            {
                nCurPos = m_oBcw.WriteItemStart(c_oSer_Sparkline::Sparkline);
                WriteSparkline(*oSparklines.m_arrItems[i]);
                m_oBcw.WriteItemEnd(nCurPos);
            }
        }
        void WriteSparkline(const OOX::Spreadsheet::CSparkline& oSparkline)
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
	};
	class BinaryCalcChainTableWriter
	{
		BinaryCommonWriter m_oBcw;
	public:
		BinaryCalcChainTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream) : m_oBcw(oCBufferedStream)
		{
        }
		void Write(OOX::Spreadsheet::CCalcChain& pCalcChain)
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteCalcChainTableContent(pCalcChain);
			m_oBcw.WriteItemWithLengthEnd(nStart);
        }
		void WriteCalcChainTableContent(OOX::Spreadsheet::CCalcChain& pCalcChain)
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
		void WriteCalcChain(OOX::Spreadsheet::CCalcCell& oCalcCell)
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
	};
	class BinaryOtherTableWriter
	{
		class EmbeddedBinaryWriter
		{
		private: NSBinPptxRW::CBinaryFileWriter &m_oStream;
		public: EmbeddedBinaryWriter(NSBinPptxRW::CBinaryFileWriter &oStream):m_oStream(oStream)
			{
			}
		public: void WriteBYTE(BYTE btVal)
			{
				m_oStream.WriteBYTE(btVal);
			}
        public: void WriteString(std::wstring& sVal)
			{
				m_oStream.WriteStringW2(sVal);
			}
		public: void WriteULONG(long nVal)
			{
				m_oStream.WriteLONG(nVal);
			}
		};
		BinaryCommonWriter						m_oBcw;
		NSFontCutter::CEmbeddedFontsManager*	m_pEmbeddedFontsManager;
		PPTX::Theme*							m_pTheme;
		NSBinPptxRW::CDrawingConverter*			m_pOfficeDrawingConverter;
	public:
		BinaryOtherTableWriter(NSBinPptxRW::CBinaryFileWriter &oCBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, PPTX::Theme* pTheme, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
			:	m_oBcw					(oCBufferedStream), 
				m_pEmbeddedFontsManager	(pEmbeddedFontsManager),
				m_pTheme				(pTheme),
				m_pOfficeDrawingConverter(pOfficeDrawingConverter)
		{
        }
		void Write()
		{
			int nStart = m_oBcw.WriteItemWithLengthStart();
			WriteOtherTableContent();
			m_oBcw.WriteItemWithLengthEnd(nStart);
        }
		void WriteOtherTableContent()
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
	};
	class BinaryFileWriter {
	private:
		BinaryCommonWriter* m_oBcw;
		int m_nLastFilePos;
		int m_nRealTableCount;
		int m_nMainTableStart;
		DocWrapper::FontProcessor& m_oFontProcessor;
	public:
		BinaryFileWriter(DocWrapper::FontProcessor& oFontProcessor) : m_oBcw(NULL), m_oFontProcessor(oFontProcessor)
		{
			m_nLastFilePos = 0;
			m_nRealTableCount = 0;
		}
		~BinaryFileWriter()
		{
			RELEASEOBJECT(m_oBcw);
		}
        _UINT32 Open(const std::wstring& sInputDir, const std::wstring& sFileDst, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager,
            NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter, const std::wstring& sXMLOptions, bool bIsNoBase64)
		{
			_UINT32 result = 0;

			OOX::CPath path(sFileDst);
	//создаем папку для media
            std::wstring mediaDir = path.GetDirectory() + L"media";
			NSDirectory::CreateDirectory(mediaDir);

            pOfficeDrawingConverter->SetDstPath(path.GetDirectory() + FILE_SEPARATOR_STR + L"word");
            pOfficeDrawingConverter->SetMediaDstPath(mediaDir);

			long nGrowSize = 1 * 1024 * 1024;//1мб
			NSBinPptxRW::CBinaryFileWriter& oBufferedStream = *pOfficeDrawingConverter->m_pBinaryWriter;

			m_oBcw = new BinaryCommonWriter(oBufferedStream);

			// File Type
			BYTE fileType;
			UINT nCodePage;
			std::wstring sDelimiter;
			BYTE saveFileType;
			SerializeCommon::ReadFileType(sXMLOptions, fileType, nCodePage, sDelimiter, saveFileType);

			OOX::Spreadsheet::CXlsx *pXlsx = NULL;
			switch(fileType)
			{
				case BinXlsxRW::c_oFileTypes::CSV:
				{
					pXlsx = new OOX::Spreadsheet::CXlsx();
					result = CSVReader::ReadFromCsvToXlsx(sInputDir, *pXlsx, nCodePage, sDelimiter);
				}break;
				case BinXlsxRW::c_oFileTypes::XLSX:
				default:
				{
					pXlsx = new OOX::Spreadsheet::CXlsx(OOX::CPath(sInputDir));
				}break;
			}		
			if (0 != result)
			{
				RELEASEOBJECT(pXlsx);
				return result;
			}	

			pXlsx->PrepareWorkbook();

			if (NULL == pXlsx->m_pWorkbook)
			{
				RELEASEOBJECT(pXlsx);
				return AVS_FILEUTILS_ERROR_CONVERT;
			}			

			if (BinXlsxRW::c_oFileTypes::JSON == saveFileType)
			{
				//todo 46 временно CP_UTF8
				CSVWriter::WriteFromXlsxToCsv(sFileDst, *pXlsx, 46, std::wstring(L","), true);
			}
			else
			{
				if (bIsNoBase64)
				{
					oBufferedStream.WriteStringUtf8(WriteFileHeader(0, g_nFormatVersionNoBase64));
				}
				intoBindoc(*pXlsx, oBufferedStream, pEmbeddedFontsManager, pOfficeDrawingConverter);

				BYTE* pbBinBuffer = oBufferedStream.GetBuffer();
				int nBinBufferLen = oBufferedStream.GetPosition();
				if (bIsNoBase64)
				{
					NSFile::CFileBinary oFile;
					oFile.CreateFileW(sFileDst);
					oFile.WriteFile(pbBinBuffer, nBinBufferLen);
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

			return result;
		}
		void intoBindoc(OOX::Spreadsheet::CXlsx &oXlsx, NSBinPptxRW::CBinaryFileWriter &oBufferedStream, NSFontCutter::CEmbeddedFontsManager* pEmbeddedFontsManager, NSBinPptxRW::CDrawingConverter* pOfficeDrawingConverter)
		{
			if (!m_oBcw) 
				m_oBcw = new BinaryCommonWriter(oBufferedStream);
			int nCurPos;
			WriteMainTableStart();
	//SharedString
			OOX::Spreadsheet::CIndexedColors* pIndexedColors = NULL;
			if( (oXlsx.m_pStyles) && oXlsx.m_pStyles->m_oColors.IsInit() && oXlsx.m_pStyles->m_oColors->m_oIndexedColors.IsInit())
				pIndexedColors = oXlsx.m_pStyles->m_oColors->m_oIndexedColors.operator ->();

			if(oXlsx.m_pSharedStrings)
			{
				nCurPos = WriteTableStart(c_oSerTableTypes::SharedStrings);
				BinarySharedStringTableWriter oBinarySharedStringTableWriter(oBufferedStream, pEmbeddedFontsManager);
				oBinarySharedStringTableWriter.Write(*oXlsx.m_pSharedStrings, pIndexedColors, oXlsx.GetTheme(), m_oFontProcessor);
				WriteTableEnd(nCurPos);
			}
	//Styles
			if(oXlsx.m_pStyles)
			{
				nCurPos = WriteTableStart(c_oSerTableTypes::Styles);
				BinaryStyleTableWriter oBinaryStyleTableWriter(oBufferedStream, pEmbeddedFontsManager);
				oBinaryStyleTableWriter.Write(*oXlsx.m_pStyles, oXlsx.GetTheme(), m_oFontProcessor);
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
			if(oXlsx.m_pWorkbook)
			{
				nCurPos = WriteTableStart(c_oSerTableTypes::Workbook);
				BinaryWorkbookTableWriter oBinaryWorkbookTableWriter(oBufferedStream, oXlsx);
				oBinaryWorkbookTableWriter.Write(*oXlsx.m_pWorkbook);
				WriteTableEnd(nCurPos);

	//Worksheets
				nCurPos = WriteTableStart(c_oSerTableTypes::Worksheets);
				BinaryWorksheetTableWriter oBinaryWorksheetTableWriter(oBufferedStream, pEmbeddedFontsManager, pIndexedColors, oXlsx.GetTheme(), m_oFontProcessor, pOfficeDrawingConverter);
				oBinaryWorksheetTableWriter.Write(*oXlsx.m_pWorkbook, oXlsx.m_mapWorksheets); 
				WriteTableEnd(nCurPos);
			}

	//OtherTable
			nCurPos = WriteTableStart(c_oSerTableTypes::Other);
			BinaryOtherTableWriter oBinaryOtherTableWriter(oBufferedStream, pEmbeddedFontsManager, oXlsx.GetTheme(), pOfficeDrawingConverter);
			oBinaryOtherTableWriter.Write();
			WriteTableEnd(nCurPos);
			
			WriteMainTableEnd();
		}
 	private:
       std::wstring WriteFileHeader(int nDataSize, int version)
		{
            std::wstring sHeader = std::wstring(g_sFormatSignature) + L";v" + std::to_wstring(version)+ L";" + std::to_wstring(nDataSize) + L";";
			return sHeader;
		}
		void WriteMainTableStart()
		{
			int nTableCount = 128;//Специально ставим большое число, чтобы не увеличивать его при добавлении очередной таблицы.
			m_nRealTableCount = 0;
			m_nMainTableStart = m_oBcw->m_oStream.GetPosition();
			//вычисляем с какой позиции можно писать таблицы
			int nmtItemSize = 5;//5 byte
			m_nLastFilePos = m_nMainTableStart + nTableCount * nmtItemSize;
			//Write mtLen 
			m_oBcw->m_oStream.WriteBYTE(0);
		}
		void WriteMainTableEnd()
		{
	//Количество таблиц
			m_oBcw->m_oStream.SetPosition(m_nMainTableStart);
			m_oBcw->m_oStream.WriteBYTE(m_nRealTableCount);

			m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
		}
		int WriteTableStart(BYTE type, int nStartPos = -1)
		{
			if(-1 != nStartPos)
				m_oBcw->m_oStream.SetPosition(nStartPos);
			//Write mtItem
			//Write mtiType
			m_oBcw->m_oStream.WriteBYTE(type);
			//Write mtiOffBits
			m_oBcw->m_oStream.WriteLONG(m_nLastFilePos);

			//Write table
			//Запоминаем позицию в MainTable
			int nCurPos = m_oBcw->m_oStream.GetPosition();
			//Seek в свободную область
			m_oBcw->m_oStream.SetPosition(m_nLastFilePos);
			return nCurPos;
		}
		void WriteTableEnd(int nCurPos)
		{
			//сдвигаем позицию куда можно следующую таблицу
			m_nLastFilePos = m_oBcw->m_oStream.GetPosition();
			m_nRealTableCount++;
			//Seek вобратно в MainTable
			m_oBcw->m_oStream.SetPosition(nCurPos);
		}
	};
}

#endif	// #ifndef BINARY_WRITER
