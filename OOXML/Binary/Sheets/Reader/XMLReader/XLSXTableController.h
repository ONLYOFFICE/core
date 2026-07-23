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
#pragma once

#include "../CellFormatController/CellFormatController.h"

#include "../../../../Base/Base.h"
#include "../../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../XlsxFormat/Xlsx.h"

#include <string>
#include <vector>

/// @brief wrapper class that allows adding cells to a table and creating a document from created cells
class XLSXTableController
{

public:
    /// @brief object fields initialization
    /// @param book object that will be filled with data using FormBook method
    /// @param lcid locale identifier
    XLSXTableController(OOX::Spreadsheet::CXlsx &book, _INT32 lcid);

    /// @brief add cell
    /// @param sText text to insert
    /// @param nRow row number
    /// @param nCol column number
    /// @param bIsWrap wrap flag
    void AddCell(const std::wstring &sText, INT nRow, INT nCol);

    /// @brief get xlsx document
    void FormBook();

private:
    /// @brief add row
    /// @param pRow pointer to row
    /// @param pWorkSheet pointer to worksheet
    /// @param nRow row number
    _UINT32 addRow(OOX::Spreadsheet::CRow *pRow, OOX::Spreadsheet::CWorksheet *pWorkSheet,  INT nRow);

    /// @brief add page
    /// @param page pointer to worksheet
    /// @param pageNumber page number
    void addPage(OOX::Spreadsheet::CWorksheet *page, INT pageNumber);

    /// @brief xlsx document
    OOX::Spreadsheet::CXlsx *book_;

    /// @brief vector with table rows
    std::vector<OOX::Spreadsheet::CRow*> tableRows_;

    /// @brief format controller
    std::shared_ptr<CellFormatController> formates_;
};
