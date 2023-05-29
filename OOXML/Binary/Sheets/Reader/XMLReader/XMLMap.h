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

#include "columnNamesController.h"

#include "../../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <memory>

/// @brief узел xml дерева
struct XmlNode
{
    /// @brief имя ноды
    std::wstring name;
    /// @brief предок ноды
    std::shared_ptr<XmlNode> parent;
    /// @brief атрибуты ноды
    std::set<std::wstring> attributes;
    /// @brief имя столбца с данными ноды, пустое если нода не имеет данных
    std::wstring ValueColumnName;
    /// @brief наследуемые столбцы ноды
    std::set<std::wstring> childColumns;
    /// @brief потомки ноды
    std::set<std::shared_ptr<XmlNode>> childs;
    /// @brief счетчик повторений ноды, чтобы расширять одну ноду, вместо использования многих с одинаковым именем
    _UINT32 counter;
    /// @brief все ноды предки
    std::set<std::shared_ptr<XmlNode>> parents;
};


/// @brief класс осуществляющий считывание xml файла и построение его табличной структуры для дальнейшей конвертации
class XMLMap
{
public:
    /// @brief метод, считывающий структуру xml файла
    /// @param reader xmlLiteReader с загруженным в него xml документом
    /// @param nameController контроллер имен в который будут загружаться имена столбцов
    /// @param nodeTree указатель на корневой элемент дерева нод, которое будет заполнено этим методом
    /// @return true в случае успеха, иначе false
    bool ReadXmlStructure(XmlUtils::CXmlLiteReader &reader, ColumnNameController &nameController, std::shared_ptr<XmlNode> nodeTree,
    std::set<std::wstring> &repeatebleValues);

private:

    /// @brief считывает аттрибуты текущей ноды
    void readAttributes();

    /// @brief обрабатывает ноду типа element
    /// @param type тип обрабатываемой ноды
    void openNode();

    /// @brief обрабатывает ноду типа endelement
    /// @param type тип обрабатываемой ноды
    void closeNode();

    /// @brief вставляет значение во временную внутреннюю структуру
    void insertValue();

    /// @brief вставляет атрибут во временную внутреннюю структуру ноды
    /// @param key ключ, по которому будет вставлено значение
    void insertAttribute(const std::wstring &key);

    /// @brief Получение уникального имени ноды, либо его поиск в переданном множестве
    /// @param name имя ноды, прочитанное из xml
    /// @param names set содержащий уникальные имена, среди которых будет осуществляться поиск
    /// @return найденное или сгенерированное уникальное имя ноды
    std::wstring getNodeName(const std::wstring &name, std::set<std::wstring> &names);

    /// @brief ищет на верхнем уровне ноду с переданным именем, используется для подсчета строк таблицы
    /// @param name имя ноды
   std::shared_ptr<XmlNode> searchSameNode(const std::wstring &name);

    /// @brief указатель на считавший xml данные reader
    XmlUtils::CXmlLiteReader *reader_;

    /// @brief указатель на контроллер имен столбцов таблицы
    ColumnNameController *colNames_;

    /// @brief вектор с родительскими нодами и используемыми на их уровнях именами
    std::vector<std::shared_ptr<XmlNode>> parents_;

    /// @brief тип предыдущей ноды(для поиска нод вида <node></node>)
    XmlUtils::XmlNodeType prevType_ = XmlUtils::XmlNodeType::XmlNodeType_None;

    /// @brief столбцы значений, ноды которых повторялись более 1 раза
    std::set<std::wstring> *repeatebleValues_;

};