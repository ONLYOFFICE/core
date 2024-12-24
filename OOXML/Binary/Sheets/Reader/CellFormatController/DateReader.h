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

#pragma once

#include "../../../../Base/Base.h"

#include <string>
#include <chrono>
#include <vector>

class DateReader
{
public:
    /// @brief создание считывателя с указанным id локали
    /// @param lcid идентификатор локали в зависимости от которой будет читаться дата
    DateReader(_INT32 lcid);

    /// @brief получение даты в виде числа в формате excel
    /// @param date дата в строковом формате
    /// @param result результат в формате excel
    /// @param return true в случае успешной конвертации, иначе false
    bool GetDigitalDate(const std::wstring &date, double &result, bool &Hasdate, bool &Hastime);

private:

    /// @brief парсинг стандартизированной даты
    /// @param date дата в строковом формате
    /// @param result в формате tm
    /// @param return true в случае успешной конвертации, иначе false
    bool parseIsoDate(const std::wstring &date, tm &result);

    /// @brief парсинг строковой даты с известной локалью
    /// @param date дата в строковом формате
    /// @param result в формате tm
    /// @param return true в случае успешной конвертации, иначе false
    bool parseLocalDate(const std::wstring &date, tm &result, bool &Hasdate, bool &Hastime);


    /// @brief получение даты в виде числа в формате excel из дат позднее 1900 года
    /// @param datetime структура с датой
    /// @return дата в формате excel
    _INT32 getStandartDate(tm date);

    /// @brief получение времени в виде десятичной части double
    /// @param datetime структура с датой и временем
    /// @return время в виде десятичной части double числа
    double getStandartTime(tm date);

    /// @brief получение даты в виде числа в формате excel из дат от 1900 года и до 1970
    /// @param datetime структура с датой
    /// @return дата в формате excel
    _INT32 getNonUnixDate(tm date);

    /// @brief нормализация года под стандарт excel
    /// @param year год либо в формате yyyy - 2021 либо в формате yy - 21
    /// @return количество лет прошедших с 1900 года
    _INT32 normalizeYear(_INT32 year);

    /// @brief парсинг am и pm частей времени
    /// @param буфер с символами
    /// @param date структура с датой
    /// @return true если строка является частью am или pm
    bool parseAmPm(std::vector<wchar_t> &stringBuf, tm &date);

    /// @brief парсинг имени месяца и внесение его в дату
    /// @param буфер с символами
    /// @param date структура с датой
    /// @return true если строка является именем месяца
    bool parseMonthName(std::vector<wchar_t> &stringBuf, tm &date);

    _INT32 lcid_;
};

