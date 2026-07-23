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
#include <chrono>
#include <vector>

class DateReader
{
public:
    /// @brief create reader with specified locale id
    /// @param lcid locale identifier that determines how date will be read
	DateReader(_INT32 lcid = 9);

    /// @brief get date as number in excel format
    /// @param date date in string format
    /// @param result result in excel format
    /// @param return true if conversion successful, otherwise false
    bool GetDigitalDate(const std::wstring &date, double &result, bool &Hasdate, bool &Hastime);


    /// @brief parse standardized date
    /// @param date date in string format
    /// @param result in tm format
    /// @param return true if conversion successful, otherwise false
    bool parseIsoDate(const std::wstring &date, tm &result);

    /// @brief parse string date with known locale
    /// @param date date in string format
    /// @param result in tm format
    /// @param return true if conversion successful, otherwise false
    bool parseLocalDate(const std::wstring &date, tm &result, bool &Hasdate, bool &Hastime);

private:
    /// @brief get date as number in excel format from dates after 1900
    /// @param datetime structure with date
    /// @return date in excel format
    _INT32 getStandartDate(tm date);

    /// @brief get time as decimal part of double
    /// @param datetime structure with date and time
    /// @return time as decimal part of double number
    double getStandartTime(tm date);

    /// @brief get date as number in excel format from dates between 1900 and 1970
    /// @param datetime structure with date
    /// @return date in excel format
    _INT32 getNonUnixDate(tm date);

    /// @brief normalize year for excel standard
    /// @param year either in yyyy - 2021 or yy - 21 format
    /// @return number of years since 1900
    _INT32 normalizeYear(_INT32 year);

    /// @brief parse am and pm parts of time
    /// @param stringBuf buffer with characters
    /// @param date structure with date
    /// @return true if string is am or pm part
    bool parseAmPm(std::vector<wchar_t> &stringBuf, tm &date);

    /// @brief parse month name and add it to date
    /// @param stringBuf buffer with characters
    /// @param date structure with date
    /// @return true if string is month name
    bool parseMonthName(std::vector<wchar_t> &stringBuf, tm &date);

    _INT32 lcid_ = 9;

    /// @brief cell counter
    _UINT16 cellCounter_ = 0;

    bool dateFound_ = false;
};

