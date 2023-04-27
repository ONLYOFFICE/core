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

#include "XMLMap.h"

#include "../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>
#include <set>
#include <map>

/// @brief класс -обертка над xmlLiteReader для превращения xml нод в табличные строки
class XMLConverter
{
public:

    /// @brief конструктор загружающий в обънет reader с прочитанным xml
    /// @param reader xmlLiteReader с загруженным в него xml документом
    /// @param xmlStruct указатель на корневую ноду структуры xml документа
    /// @param nameController контроллер имен, заполненный именами столбцов
    XMLConverter(XmlUtils::CXmlLiteReader &reader, std::shared_ptr<XmlNode> xmlStruct, ColumnNameController &nameController);

    /// @brief метод, конвертирующий xml в табличный вид
    /// @param table таблица с данными xml в строковом виде
    void ConvertXml(std::vector<std::vector<std::wstring>> &table);

private:

    /// @brief считывает аттрибуты текущей ноды
    void readAttributes();

    /// @brief обрабатывает открытие текущей ноды
    void openNode();

    /// @brief обрабатывает закрытие текущей ноды
    void closeNode();

    /// @brief проверка ноды на возможность вставить её данные в таблицу с последующей их  вставкой в случае успеха
    /// @param type тип обрабатываемой ноды
    void storeData(const XmlUtils::XmlNodeType &type);

    /// @brief заполняет данными переданный map
    /// @param row map в который будут помещены данные и соответствующие им номера столбцов
    /// @return номер вставляемой строки
    void insertRow(std::map<_UINT32, std::wstring> &row);

    /// @brief вставляет значение во временную внутреннюю структуру
    /// @param key ключ, по которому будет вставлено значение
    /// @param value значение которое нужно вставить
    void insertValue(const std::wstring &key, const std::wstring &value);

    /// @brief Получение уникального имени ноды
    /// @param name имя ноды, прочитанное из xml
    /// @return найденное или сгенерированное уникальное имя ноды
    std::wstring getNodeName(const std::wstring &name);

    /// @brief указатель на считавший xml данные reader
    XmlUtils::CXmlLiteReader *reader_;

    /// @brief вектор с родительскими нодами
    std::vector<std::shared_ptr<XmlNode>> parents_;

    /// @brief map с набором ключей в виде уникальных имен и их значений для вставки в таблицу
    std::map<std::wstring, std::wstring> keyvalues_;

    /// @brief контроллер имен столбцов таблицы
    ColumnNameController *colNames_;

    /// @brief map в который выводятся данные при прочтении ноды
    std::map<_UINT32, std::wstring> stringBuffer_;

    /// @brief дерево нод xml документа
    std::shared_ptr<XmlNode> nodeTree_;

    /// @brief указатель на текущую ноду в структуре
    std::shared_ptr<XmlNode> nodePointer_;

    /// @brief тип предыдущей ноды(для поиска нод вида <node></node>)
    XmlUtils::XmlNodeType prevType_ = XmlUtils::XmlNodeType::XmlNodeType_None;

};
