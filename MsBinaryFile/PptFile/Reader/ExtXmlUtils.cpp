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
#include "ExtXmlUtils.h"


std::vector<std::wstring> XmlUtils::FindAttrValues(const std::wstring &xml, std::wstring attrName)
{
    attrName += L"=\"";
    std::vector<std::wstring> values;
    size_t posBeg = xml.find(attrName);
    auto endTagPos = posBeg + attrName.size();
    size_t posEnd = xml.find(L"\"", endTagPos);

    // Repeat till end is reached
    while( posBeg != std::string::npos)
    {
        auto strValue = xml.substr(endTagPos, posEnd - (endTagPos));
        values.push_back(strValue);

        // Get the next occurrence from the current position
        posBeg = xml.find(attrName, endTagPos);
        endTagPos = posBeg + attrName.size();
        posEnd = xml.find(L"\"", endTagPos);
    }
    return values;
}

std::vector<int> XmlUtils::FindAttrValuesInt(const std::wstring &xml, std::wstring attrName)
{
    auto vecStrValues = FindAttrValues(xml, attrName);
    std::vector<int> vecIntValues;
    for (const auto& strValue : vecStrValues)
    {
        try {
            vecIntValues.push_back(std::stoi(strValue));
        } catch (...) {
        }
    }
    return vecIntValues;
}
