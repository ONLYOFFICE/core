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
#include  <utility>

/// @brief wrapper class over xmlLiteReader for converting xml nodes to table rows
class XML2TableConverter
{
public:

    /// @brief constructor that loads reader with parsed xml into the object
    /// @param reader xmlLiteReader with loaded xml document
    XML2TableConverter(XmlUtils::CXmlLiteReader &reader);

    /// @brief method that reads the next row from xml
    /// @param string map with string data as keys and their column numbers for insertion as values
    /// @return row number on successful read or -1 on error
    bool ReadNextElement(std::map<_UINT32, std::wstring> &string);

private:

    /// @brief reads attributes of the current node
    void readAttributes();

    /// @brief processes the current node
    /// @param type type of the node being processed
    /// @return true if nodes within the row are read and can exit, otherwise false
    void processNode(const XmlUtils::XmlNodeType &type);

    /// @brief checks if node data can be inserted into table and inserts it on success
    /// @param type type of the node being processed
    void storeData(const XmlUtils::XmlNodeType &type);

    /// @brief fills the passed map with data
    /// @param row map where data and corresponding column numbers will be placed
    /// @return number of the row being inserted
    void insertRow(std::map<_UINT32, std::wstring> &row);

    /// @brief inserts value into temporary internal structure
    /// @param key key by which the value will be inserted
    /// @param value value to insert
    void insertValue(const std::wstring &key, const std::wstring &value);

    /// @brief inserts empty node name
    /// @param key node name
    void insertEmptyNode(const std::wstring &key);

    /// @brief inserts node attribute into temporary internal structure
    /// @param key key by which the value will be inserted
    /// @param value value to insert
    void insertAttribute(const std::wstring &key, const std::wstring &value);

    /// @brief fills map with collected column names for insertion into table
    /// @param names map with column names
    void insertColumnNames(std::map<_UINT32, std::wstring> &names);

    /// @brief Gets unique node name or searches for it in the passed set
    /// @param name node name read from xml
    /// @param names set containing unique names among which the search will be performed
    /// @return found or generated unique node name
    std::wstring getNodeName(const std::wstring &name, std::set<std::wstring> &names);

    /// @brief pointer to reader that read xml data
    XmlUtils::CXmlLiteReader *reader_;

    /// @brief vector with parent nodes and names used at their levels
    std::vector<std::pair<std::wstring, std::set<std::wstring>>> parents_;

    /// @brief map with set of unique name keys and their values for insertion into table
    std::map<std::wstring, std::wstring> keyvalues_;

    /// @brief table column names controller
    ColumnNameController colNames_;

    /// @brief map where data is output when reading a node
    std::map<_UINT32, std::wstring> stringBuffer_;

    /// @brief type of previous node (for finding nodes like <node></node>)
    XmlUtils::XmlNodeType prevType_ = XmlUtils::XmlNodeType::XmlNodeType_None;

};