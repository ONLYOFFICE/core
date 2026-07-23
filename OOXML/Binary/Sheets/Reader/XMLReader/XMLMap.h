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

#include "columnNamesController.h"

#include "../../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>
#include <set>
#include <map>
#include <utility>
#include <memory>

/// @brief XML tree node
struct XmlNode
{
    /// @brief node name
    std::wstring name;
    /// @brief parent node
    std::shared_ptr<XmlNode> parent;
    /// @brief node attributes
    std::set<std::wstring> attributes;
    /// @brief column name with node data, empty if node has no data
    std::wstring ValueColumnName;
    /// @brief inherited columns of the node
    std::set<std::wstring> childColumns;
    /// @brief child nodes
    std::set<std::shared_ptr<XmlNode>> childs;
    /// @brief node repetition counter, to expand one node instead of using many with the same name
    _UINT32 counter;
    /// @brief all ancestor nodes
    std::set<std::shared_ptr<XmlNode>> parents;
};


/// @brief class that reads XML file and builds its table structure for further conversion
class XMLMap
{
public:
    /// @brief method that reads XML file structure
    /// @param reader xmlLiteReader with loaded XML document
    /// @param nameController name controller where column names will be loaded
    /// @param nodeTree pointer to root element of node tree that will be filled by this method
    /// @return true on success, false otherwise
    bool ReadXmlStructure(XmlUtils::CXmlLiteReader &reader, ColumnNameController &nameController, std::shared_ptr<XmlNode> nodeTree,
    std::set<std::wstring> &repeatebleValues);

private:

    /// @brief reads attributes of current node
    void readAttributes();

    /// @brief processes element type node
    /// @param type type of node being processed
    void openNode();

    /// @brief processes endelement type node
    /// @param type type of node being processed
    void closeNode();

    /// @brief inserts value into temporary internal structure
    void insertValue();

    /// @brief inserts attribute into temporary internal node structure
    /// @param key key by which value will be inserted
    void insertAttribute(const std::wstring &key);

    /// @brief gets unique node name or searches for it in provided set
    /// @param name node name read from XML
    /// @param names set containing unique names to search among
    /// @return found or generated unique node name
    std::wstring getNodeName(const std::wstring &name, std::set<std::wstring> &names);

    /// @brief searches for a node with the given name at the top level, used for counting table rows
    /// @param name node name
   std::shared_ptr<XmlNode> searchSameNode(const std::wstring &name);

    /// @brief pointer to the XML data reader
    XmlUtils::CXmlLiteReader *reader_;

    /// @brief pointer to the table column name controller
    ColumnNameController *colNames_;

    /// @brief vector with parent nodes and names used at their levels
    std::vector<std::shared_ptr<XmlNode>> parents_;

    /// @brief type of previous node (for searching nodes like <node></node>)
    XmlUtils::XmlNodeType prevType_ = XmlUtils::XmlNodeType::XmlNodeType_None;

    /// @brief value columns whose nodes were repeated more than once
    std::set<std::wstring> *repeatebleValues_;

};