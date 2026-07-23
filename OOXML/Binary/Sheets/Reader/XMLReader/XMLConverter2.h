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

#include "XMLMap.h"
#include "XLSXTableController.h"

#include "../../../../../DesktopEditor/xml/include/xmlutils.h"
#include "../../../../Base/Base.h"

#include <string>
#include <vector>
#include <set>
#include <map>

/// @brief Wrapper class over xmlLiteReader for converting xml nodes to table rows
class XMLConverter
{
public:

    /// @brief Constructor that loads the reader with parsed xml
    /// @param reader xmlLiteReader with the loaded xml document
    /// @param xmlStruct Pointer to the root node of the xml document structure
    /// @param nameController Name controller, filled with column names
    /// @param repeatebleValues Set with repeatable columns, intended for forming table rows
    XMLConverter(XmlUtils::CXmlLiteReader &reader, std::shared_ptr<XmlNode> xmlStruct, ColumnNameController &nameController,
    std::set<std::wstring> &repeatebleValues);

    /// @brief method that converts xml to table view
    /// @param table xlsx table controller
    void ConvertXml(XLSXTableController &table);

private:

    /// @brief reads attributes of the current node
    void readAttributes();

    /// @brief handles opening of the current node
    void openNode();

    /// @brief handles closing of the current node
    void closeNode();

    /// @brief checks if node data can be inserted into the table and inserts it if successful
    /// @param type type of node being processed
    void storeData(const XmlUtils::XmlNodeType &type);

    /// @brief fills the passed map with data
    /// @param row map where data and corresponding column numbers will be placed
    /// @return inserted row number
    void insertRow(std::map<_UINT32, std::wstring> &row);

    /// @brief inserts value into temporary internal structure
    /// @param key key by which the value will be inserted
    /// @param value value to be inserted
    void insertValue(const std::wstring &key, const std::wstring &value);

    /// @brief gets unique node name
    /// @param name node name read from xml
    /// @return found or generated unique node name
    std::wstring getNodeName(const std::wstring &name);

    /// @brief fills table row with attributes
    /// @param table xlsx table controller
    /// @param attribNode node from which attributes will be taken
    /// @param filledValues set containing names of filled columns
    /// @param arowNumbe xlsx table controller
    void fillAttribures(XLSXTableController &table, std::shared_ptr<XmlNode> attribNode, std::set<std::wstring> &filledValues,
        const _UINT32 &rowNumber);

    /// @brief fills child nodes' attributes with parent's when parent node doesn't go as a separate row
    /// @param childs child nodes whose attributes need to be filled with parent's
    /// @param inheritersCount number of nodes that will inherit parent's attributes
    void moveParentAttributes(const std::shared_ptr<XmlNode> parent, const std::set<std::shared_ptr<XmlNode>>childs, const _UINT32 inheritersCount);

    /// @brief pointer to the reader that read xml data
    XmlUtils::CXmlLiteReader *reader_;

    /// @brief vector with parent nodes
    std::vector<std::shared_ptr<XmlNode>> parents_;

    /// @brief map with unique name keys and their values for insertion into the table
    std::map<std::wstring, std::vector<std::wstring>> data_;

    /// @brief table column names controller
    ColumnNameController *colNames_;

    /// @brief set containing list of columns whose nodes appear multiple times
    std::set<std::wstring> *listableColumns_;

    /// @brief xml document node tree
    std::shared_ptr<XmlNode> nodeTree_;

    /// @brief pointer to current node in structure
    std::shared_ptr<XmlNode> nodePointer_;

    /// @brief type of previous node (for finding nodes like <node></node>)
    XmlUtils::XmlNodeType prevType_ = XmlUtils::XmlNodeType::XmlNodeType_None;

    /// @brief vector with nodes of rows being written
    std::vector<std::shared_ptr<XmlNode>> writingRows_;
    /// @brief vector with pointers to repeating nodes that are currently open
    std::map<std::shared_ptr<XmlNode>, _UINT32> openednodes_;

};
