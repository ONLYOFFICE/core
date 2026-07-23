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

#include <string>
#include <vector>

namespace lcInfo
{

/// @brief class containing locale information and standards used in it
class LocalInfo
{
public:

    /// @brief build short date format from template
    /// @return template with numbers defining date element order where 0-1 days 2-3 months 4-5 years
    std::wstring GetShortDateFormat();

    /// @brief get month names in this locale
    /// @param index number from month list
    /// @param shortName whether abbreviated name is requested
    /// @return vector of month names starting from January
    std::vector<std::wstring> GetMonthNames(const _INT16 &index, const bool isShortName = false);

    /// @brief build short date format from template
    /// @return month number starting from zero on success, negative number on failure
    _INT16 GetMonthNumber(const std::wstring &monthName,  const bool isShortName = false);

    /// @brief get string month name in this locale
    /// @param index requested month number
    /// @param shortName whether abbreviated name is requested
    /// @return month name
    std::wstring GetLocMonthName(const _INT16 &index, bool shortName = false);

    /// @brief locale id
    _INT32 lcid;

    /// @brief locale name
    std::wstring Name;

    /// @brief date separator
    std::wstring DateSeparator;

    /// @brief short date
    std::wstring ShortDatePattern;

    /// @brief local month names
    _INT16 MonthNamesIndex;

    /// @brief maximum characters in abbreviated month length
    _INT16 MonthAbrvLen;
};

/// @brief get locale information by its id
LocalInfo getLocalInfo(const _INT32 lcid);

}
