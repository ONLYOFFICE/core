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

#include  <utility>
#include <string>
#include <map>

/// @brief класс контролирующий уникальность имен столбцов при открытии  xml документов
class ColumnNameController
{

public:
    /// @brief Создание уникального имени столбца и установка его номера
    /// @param column строка с именем столбца
    /// @return номер, который будет у столбца с этим именем
    _UINT32 CreateColumnName(std::wstring &column);

    /// @brief Поиск номера столбца с указанным именем
    /// @param columnName строка с именем столбца
    /// @return номер столбца, если такой есть и -1, если такого столбца нет
    _INT64 GetColumnNumber(const std::wstring &columnName);

    /// @brief попытка поиска и получения изначального имени xml узла по уникальному
    /// @param columnName уникальное имя узла, созданное контроллером
    /// @return неуникальное имя xml ноды из которого было создано уникальное
    std::wstring GetXmlName(const std::wstring &columnName);

    /// @brief Получение всех содержащихся имен и номеров их столбцов
    /// @return map с уникальными именами как ключами и номерами столбцов как значениями
    std::map<std::wstring, _UINT32> GetColumnNames();

private:

/// @brief хранит уникальные имена столбцов как ключи и пару неуникальное имя и номер столбца как значение
std::map<std::wstring, std::pair<std::wstring, _UINT32>> colNames_;

/// @brief цифра добавляющаяся к повторяющимся именам столбцов для их уникальности
_UINT32 colNamePostfix_ = 2;

/// @brief максимальный номер столбца, который присваивается при получении нового имени
_UINT32 colNumber_ = 0;
};
