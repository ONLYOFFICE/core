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
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
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
    /// ищем среди использовавшихся имён нужное
    for(auto i = names.begin(); i != names.end(); i++)
    {
        if(colNames_.GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    /// если не нашли, создаём его и вставляем
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
        //вставка ноды типа <node></node>
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
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
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
