/*
 * (c) Copyright Ascensio System SIA 2010-2024
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
#include <vector>

namespace lcInfo
{

/// @brief класс содержащий в себе информацию о локали и используемых в ней стандартах
class LocalInfo
{
public:

    /// @brief собрать короткий формат даты из шаблона
    /// @return шаблон из номеров определяющих порядок элементов даты где 0-1 дни 2-3 месяцы 4-5 годы
    std::wstring GetShortDateFormat();

    /// @brief получить имена месяцев в этой локали
    /// @param index номер из списка месяцев
    /// @param shortName запрашивается ли сокращенное имя
    /// @return вектор имен месяцев начинающихся с января
    std::vector<std::wstring> GetMonthNames(const _INT16 &index, const bool isShortName = false);

    /// @brief собрать короткий формат даты из шаблона
    /// @return номер месяца начиная от нуля, в случае успеха, отрицательное число в случае неудачи
    _INT16 GetMonthNumber(const std::wstring &monthName,  const bool isShortName = false);

    /// @brief получить строковое имя месяца в этой локали
    /// @param index номер запрашиваемого месяца
    /// @param shortName запрашивается ли сокращенное имя
    /// @return имя месяца
    std::wstring GetLocMonthName(const _INT16 &index, bool shortName = false);

    /// @brief id локали
    _INT32 lcid;

    /// @brief название локали
    std::wstring Name;

    /// @brief разделитель даты
    std::wstring DateSeparator;

    /// @brief сокращенная дата
    std::wstring ShortDatePattern;

    /// @brief локальные имена месяцев
    _INT16 MonthNamesIndex;

    /// @brief максимум символов в сокращенной длине месяца
    _INT16 MonthAbrvLen;
};

/// @brief получение информации о локали по её id
LocalInfo getLocalInfo(const _INT32 lcid);

}
