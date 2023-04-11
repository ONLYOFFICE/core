/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>

/// @brief Первое измерение вектора хранит данные раздельно по глубине xml второе измерение хранит данные по ключу, третье - сами значения данных
using XmlData = std::vector<std::vector<std::vector<std::wstring>>>;

/// @brief класс -обертка над xmlLiteReader для превращения xml нод в табличные строки
class XML2TableConverter
{
public:

    /// @brief метод, преобразующий xml документ в формат, удобный для табличного размещения
    /// @param reader xmlLiteReader с загруженным в него xml документом
    /// @param data трехмерная структура из векторов строк, нужна для хранения табличных данных вместе с ключами в разрезе уровней глубины
    /// @return true в случае успеха, иначе false
    bool GetTableData(XmlUtils::CXmlLiteReader &reader, XmlData &data);

private:

    /// @brief считывает аттрибуты текущей ноды
    /// @param reader xmlLiteReader аттрибуты ноды которого нажуно считать
    void readAttributes(XmlUtils::CXmlLiteReader &reader);

    /// @brief обрабатывает текущую ноду - считывает её атрибуты и текст
    /// @param reader xmlLiteReader аттрибуты ноды которого нажуно считать
    /// @param type тип обрабатываемой ноды
    void processNode(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type);

    /// @brief вставляет значение во временную внутреннюю структуру
    /// @param key ключ, по которому будет вставлено значение
    /// @param value значение которое нужно вставить
    void insertValue(const std::wstring &key, const std::wstring &value);

    /// @return текущая глубина
    _UINT32 depth_;

    /// @return вектор с именами нод-предков
    std::vector<std::wstring> parents_;

    /// @return данные собранные с xml
    XmlData data_;

};