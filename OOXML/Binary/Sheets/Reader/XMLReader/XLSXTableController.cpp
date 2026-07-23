/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */
#include "XLSXTableController.h"

#include "../../../../XlsxFormat/Workbook/Sheets.h"
#include "../../../../XlsxFormat/Workbook/Workbook.h"
#include "../../../../../Common/OfficeFileErrorDescription.h"

#include "../../../../XlsxFormat/Styles/Styles.h"
#include "../../../../XlsxFormat/Styles/NumFmts.h"
#include "../../../../XlsxFormat/Styles/Xfs.h"

constexpr auto SheetName = L"Sheet";

XLSXTableController::XLSXTableController(OOX::Spreadsheet::CXlsx &book, _INT32 lcid)
: book_{&book}
{
    book_->CreateWorkbook();
    book_->m_pWorkbook->m_oSheets.Init();
    tableRows_.push_back(nullptr);

    // Create styles
	book_->CreateStyles();

    formates_ = std::make_shared<CellFormatController>(book_->m_pStyles, lcid);
}


void XLSXTableController::FormBook()
{
    /// @todo make this block execute only once
    if(1)
    {
        auto pWorksheet = new OOX::Spreadsheet::CWorksheet(NULL);
        pWorksheet->m_oSheetData.Init();
        for(int i = 1; i< tableRows_.size(); i++)
        {
            addRow(tableRows_.at(i), pWorksheet, i);
        }

        addPage(pWorksheet, 1);
    }
}

void XLSXTableController::AddCell(const std::wstring &sText, INT nRow, INT nCol)
{

    size_t length = sText.length();
    // Don't write empty
    if (sText.empty() || (sText[0] == L'\0'))
    {
        return;
    }

    auto pCell = new OOX::Spreadsheet::CCell();
    pCell->m_oType.Init();

    formates_->ProcessCellType(pCell, sText);

    pCell->setRowCol(nRow - 1, nCol);

    while(tableRows_.size() - 1 < nRow)
    {
        tableRows_.push_back(new OOX::Spreadsheet::CRow);
    }

    tableRows_.at(nRow)->m_arrItems.push_back(pCell);

}

void XLSXTableController::addPage(OOX::Spreadsheet::CWorksheet *page, INT pageNumber)
{
    book_->m_arWorksheets.push_back(page);

    smart_ptr<OOX::File> oWorksheetFile = page;
    const OOX::RId oRid = book_->m_pWorkbook->Add(oWorksheetFile);

    book_->m_mapWorksheets.insert(std::make_pair(oRid.ToString(), page)); // for bin

    OOX::Spreadsheet::CSheet *pSheet = new OOX::Spreadsheet::CSheet();

    std::wstring sheetName = SheetName + std::to_wstring(pageNumber);
    pSheet->m_oName = sheetName;
    pSheet->m_oSheetId.Init();
    pSheet->m_oSheetId->SetValue(pageNumber);
    pSheet->m_oRid.Init();
    pSheet->m_oRid->SetValue(oRid.ToString());

    book_->m_pWorkbook->m_oSheets.Init();
    book_->m_pWorkbook->m_oSheets->m_arrItems.push_back(pSheet);
}

bool CompareCells(const OOX::Spreadsheet::CCell *a, const OOX::Spreadsheet::CCell *b)
{
    return *a->m_oCol < *b->m_oCol;
}

bool CompareColumns(const OOX::Spreadsheet::CCell *a, const OOX::Spreadsheet::CCell *b)
{
    return *a->m_oCol == *b->m_oCol;
}

_UINT32 XLSXTableController::addRow(OOX::Spreadsheet::CRow *pRow, OOX::Spreadsheet::CWorksheet *pWorkSheet,  INT nRow)
{
    if (pWorkSheet->m_oSheetData->m_arrItems.size() > 1048576)
            {
                return AVS_FILEUTILS_ERROR_CONVERT_ROWLIMITS; // MS limitation
            }
    pRow->m_oR.Init();
    pRow->m_oR->SetValue(nRow);

    /// sort cells in the row
    if(pRow->m_arrItems.size() > 1)
    {
        std::sort(pRow->m_arrItems.begin(), pRow->m_arrItems.end(), CompareCells);
        // Remove duplicates using unique algorithm
        auto it = std::unique(pRow->m_arrItems.begin(), pRow->m_arrItems.end(), CompareColumns);
        pRow->m_arrItems.erase(it, pRow->m_arrItems.end());
    }
    pWorkSheet->m_oSheetData->m_arrItems.push_back(pRow);

    return 0;
}
