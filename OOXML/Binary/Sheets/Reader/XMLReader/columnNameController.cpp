/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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