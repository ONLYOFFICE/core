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

#include "columnNamesController.h"

_UINT32 ColumnNameController::CreateColumnName(std::wstring &column)
{
    std::wstring nodeName = column;

    if(colNames_.find(column) != colNames_.end())
    {
        column += std::to_wstring(colNamePostfix_);
        colNamePostfix_++;
    }

    auto colNumber = colNumber_;
    colNumber_ ++;

    colNames_.emplace(column, std::make_pair(nodeName, colNumber));
    return colNumber;
}

_INT64 ColumnNameController::GetColumnNumber(const std::wstring &columnName)
{
    if(colNames_.find(columnName) != colNames_.end())
    {
        return colNames_.at(columnName).second;
    }

    return -1;
}

std::wstring ColumnNameController::GetXmlName(const std::wstring &columnName)
{
    auto nodeName = colNames_.find(columnName);
    if(nodeName == colNames_.end())
    {
        return L"";
    }

    return nodeName->second.first;
}

std::map<std::wstring, _UINT32> ColumnNameController::GetColumnNames()
{
    std::map<std::wstring, _UINT32> columns = {};
    for(auto i = colNames_.begin(); i != colNames_.end(); i++)
    {
        columns.emplace(i->first, i->second.second);
    }
    return columns;
}