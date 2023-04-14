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

#include "columnNamesController.h"
#include "XlSXTableController.h"

#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>
#include <deque>

using keyValueArray = std::deque<std::pair<std::wstring, std::wstring>>;

/// @brief класс -обертка над xmlLiteReader для превращения xml нод в табличные строки
class XML2TableConverter
{
public:

    /// @brief метод, преобразующий xml документ в формат, удобный для табличного размещения
    /// @param reader xmlLiteReader с загруженным в него xml документом
    /// @param data трехмерная структура из векторов строк, нужна для хранения табличных данных вместе с ключами в разрезе уровней глубины
    /// @return true в случае успеха, иначе false
    bool GetTableData(XmlUtils::CXmlLiteReader &reader, XLSXTableController &table);

private:

    /// @brief считывает аттрибуты текущей ноды
    /// @param reader xmlLiteReader аттрибуты ноды которого нажуно считать
    void readAttributes(XmlUtils::CXmlLiteReader &reader);

    /// @brief обрабатывает текущую ноду
    /// @param reader xmlLiteReader аттрибуты ноды которого нажуно обработать
    /// @param type тип обрабатываемой ноды
    void processNode(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type);

    /// @brief проверка ноды на возможность вставить её данные в таблицу с последующей их  вставкой в случае успеха
    /// @param reader xmlLiteReader аттрибуты ноды которого нажуно вставить
    /// @param type тип обрабатываемой ноды
    void tryInsertData(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type);

    /// @brief вставляет строку xml данных в таблицу
    /// @param dataRow строка с данными
    /// @param parentsRow строка с данными родительских нод
    void insertRow(const keyValueArray &dataRow, const keyValueArray &parentsRow);

    /// @brief вставляет значение во временную внутреннюю структуру
    /// @param key ключ, по которому будет вставлено значение
    /// @param value значение которое нужно вставить
    void insertValue(const std::wstring &key, const std::wstring &value);

    /// @brief текущая глубина
    _UINT32 depth_;

    /// @brief глубина предыдущего элемента
    _UINT32 tempDepth_;

    /// @brief номер текущей строки с данными
    _UINT32 rowIndex_ = 2;

    /// @brief вектор с родительскими нодами
    std::vector<std::wstring> parents_;

    /// @brief дека с парами ключ значение для их вставки в таблицу
    keyValueArray keyvalues_;

    /// @brief дека с парами ключ значение родительских нод, вставляемых в строках дочерних нод
    keyValueArray parentValues_;

    /// @brief контроллер таблицы заполняемой во время обработки xml документа
    XLSXTableController *table_;

    /// @brief контроллер имен столбцов таблицы
    ColumnNameController colNames_;

};