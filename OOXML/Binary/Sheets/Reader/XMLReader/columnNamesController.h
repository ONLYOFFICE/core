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

#include  <utility>
#include <string>
#include <map>

/// @brief class that controls uniqueness of column names when opening xml documents
class ColumnNameController
{

public:
    /// @brief Create unique column name and set its number
    /// @param column string with column name
    /// @return number that this column will have
    _UINT32 CreateColumnName(std::wstring &column);

    /// @brief Search for column number with specified name
    /// @param columnName string with column name
    /// @return column number if exists, -1 if no such column
    _INT64 GetColumnNumber(const std::wstring &columnName);

    /// @brief Try to find and get original xml node name by unique one
    /// @param columnName unique node name created by controller
    /// @return non-unique xml node name from which unique was created
    std::wstring GetXmlName(const std::wstring &columnName);

    /// @brief Get all contained names and their column numbers
    /// @return map with unique names as keys and column numbers as values
    std::map<std::wstring, _UINT32> GetColumnNames();

private:

/// @brief stores unique column names as keys and pair of non-unique name and column number as value
std::map<std::wstring, std::pair<std::wstring, _UINT32>> colNames_;

/// @brief number added to repeating column names to make them unique
_UINT32 colNamePostfix_ = 2;

/// @brief maximum column number assigned when getting new name
_UINT32 colNumber_ = 0;
};
