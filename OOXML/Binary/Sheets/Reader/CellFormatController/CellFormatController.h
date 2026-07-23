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

#include "../../../../Base/Base.h"
#include "../../../../XlsxFormat/Worksheets/Worksheet.h"
#include "../../../../XlsxFormat/Styles/Styles.h"
#include "DateReader.h"
#include "DigitReader.h"


/// @brief class that determines and corrects data type for table cell values
class CellFormatController
{

public:
    /// @brief constructor
    /// @param styles styles from the table
    CellFormatController(OOX::Spreadsheet::CStyles *styles, _INT32 lcid);

    /// @brief processes data inserted into table cell, converting to required type, and fills the cell
    /// @param pCell pointer to cell
    /// @param value data to insert in string format
    int ProcessCellType(OOX::Spreadsheet::CCell *pCell, const std::wstring &value, bool bIsWrap = false);


	/// @brief pointer to document worksheet
	OOX::Spreadsheet::CWorksheet *m_pWorksheet = nullptr;

private:
    bool isFormula(const std::wstring& formula);
    std::wstring ConvertFormulaArguments(const std::wstring& formula);
    /// @brief create style for specified format
    /// @param format value format
    void createFormatStyle(const std::wstring &format);

	/// @brief add custom column width for cell
	/// @param pCell cell for which width will be added
	/// @param width width value to set for column if it's greater than current
	void addCustomColWidth(OOX::Spreadsheet::CCell *pCell, double width);

    /// @brief pointer to cell being worked with
    OOX::Spreadsheet::CCell *pCell_;

    /// @brief map with data formats
    std::map<std::wstring, unsigned int> mapDataNumber_;

    /// @brief pointer to document styles
	OOX::Spreadsheet::CStyles *m_pStyles;

    /// @brief pointer to received string value
    const std::wstring *value_;

    /// @brief locale identifier
    _INT32 lcid_;

    DigitReader digitReader_;
    DateReader dateReader_;
};
