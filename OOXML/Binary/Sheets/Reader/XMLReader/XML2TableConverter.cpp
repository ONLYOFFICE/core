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

#include "XML2TableConverter.h"

#include <algorithm>

XML2TableConverter::XML2TableConverter(XmlUtils::CXmlLiteReader &reader)
:reader_{&reader}
{
    parents_.push_back(std::make_pair(L"root", std::set<std::wstring>()));
};

bool XML2TableConverter::ReadNextElement(std::map<_UINT32, std::wstring> &string)
{
    XmlUtils::XmlNodeType nodeType;

    while(reader_->Read(nodeType))
    {
        readAttributes();
        storeData(nodeType);
        processNode(nodeType);
        if(!stringBuffer_.empty())
        {
            string = stringBuffer_;
            stringBuffer_.clear();
            return true;
        }
    }

    insertColumnNames(string);
    return false;

}

void XML2TableConverter::readAttributes()
{
    ///@todo check if parents have nodes with the same name for insertion into their column instead of creating a new one
    if(!reader_->GetAttributesCount())
    {
        return;
    }
    reader_->MoveToFirstAttribute();

    insertAttribute(reader_->GetName(), reader_->GetText());

    while(reader_->MoveToNextAttribute())
    {
        insertAttribute(reader_->GetName(), reader_->GetText());
    }

    reader_->MoveToElement();
}

void XML2TableConverter::insertValue(const std::wstring &key, const std::wstring &value)
{
    std::wstring uniqueKey = {};
    if(parents_.size() > 2)
    {
        auto parentsIndex = parents_.size()-3;
        uniqueKey = getNodeName(key, parents_.at(parentsIndex).second);
    }
    else if(parents_.size() > 1)
    {
        auto parentsIndex = parents_.size()-2;
        uniqueKey = getNodeName(key, parents_.at(parentsIndex).second);
    }
    else
    {
        uniqueKey = getNodeName(key, parents_.at(0).second);
    }

    if(!value.empty())
    {
        keyvalues_.emplace(uniqueKey, value);
    }
}

void XML2TableConverter::insertAttribute(const std::wstring &key, const std::wstring &value)
{
    auto uniqueKey = getNodeName(key, parents_.back().second);

    if(!value.empty())
    {
        keyvalues_.emplace(uniqueKey, value);
    }
}

void XML2TableConverter::insertEmptyNode (const std::wstring &key)
{
    std::wstring uniqueKey = {};
    if(parents_.size() > 1)
    {
        auto parentsIndex = parents_.size()-2;
        uniqueKey = getNodeName(key, parents_.at(parentsIndex).second);
    }
    else
    {
        uniqueKey = getNodeName(key, parents_.at(0).second);
    }
}

std::wstring XML2TableConverter::getNodeName(const std::wstring &name, std::set<std::wstring> &names)
{
    /// search for the needed name among used names
    for(auto i = names.begin(); i != names.end(); i++)
    {
        if(colNames_.GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    /// if not found, create and insert it
    auto resultName = name;
    colNames_.CreateColumnName(resultName);
    names.insert(resultName);
    return resultName;
}

 void XML2TableConverter::insertColumnNames(std::map<_UINT32, std::wstring> &names)
 {
   auto tempNames = colNames_.GetColumnNames();
   for(auto i = tempNames.begin(); i != tempNames.end(); i++)
   {
        names.emplace(i->second, i->first);
   }
 }

void XML2TableConverter::insertRow(std::map<_UINT32, std::wstring> &row)
{
    for(auto i = keyvalues_.begin(); i != keyvalues_.end(); i++)
    {
        row.emplace(colNames_.GetColumnNumber(i->first), i->second);
    }
}

void XML2TableConverter::processNode(const XmlUtils::XmlNodeType &type)
{
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && !reader_->IsEmptyNode())
    {
        parents_.push_back(std::make_pair(reader_->GetName(), std::set<std::wstring>()));
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && reader_->IsEmptyNode())
    {
        insertRow(stringBuffer_);
        keyvalues_.clear();
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_EndElement)
    {
        // inserting node of type <node></node>
        if(prevType_ == XmlUtils::XmlNodeType::XmlNodeType_Element)
        {
            insertValue(parents_.back().first, L"");
        }
        insertRow(stringBuffer_);
        keyvalues_.clear();
        parents_.pop_back();
    }
    prevType_ = type;
}

void XML2TableConverter::storeData(const XmlUtils::XmlNodeType &type)
{
    ///@todo check if parents have nodes with the same name for insertion into their column instead of creating a new one
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Text || type == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
    {
        auto text = reader_->GetText();
        insertValue(parents_.at(parents_.size() -1).first, text);
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && reader_->IsEmptyNode() && reader_->GetAttributesCount() == 0)
    {
        insertEmptyNode(reader_->GetName());
    }
}
