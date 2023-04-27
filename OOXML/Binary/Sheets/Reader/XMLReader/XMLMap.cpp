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

#include "XMLMap.h"

bool XMLMap::ReadXmlStructure(XmlUtils::CXmlLiteReader &reader, ColumnNameController &nameController, std::shared_ptr<XmlNode> nodeTree)
{
    reader_ = &reader;
    colNames_ = &nameController;
    parents_.push_back(nodeTree);

    XmlUtils::XmlNodeType nodeType;
    while(reader_->Read(nodeType))
    {
        if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_Element)
        {
            openNode();
        }
        else if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_Text || nodeType == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
        {
            insertValue(parents_.back()->name);
        }
        else if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_EndElement)
        {
            closeNode();
        }
        prevType_ = nodeType;
    }

    return true;
}

void XMLMap::readAttributes()
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
    if(!reader_->GetAttributesCount())
    {
        return;
    }
    reader_->MoveToFirstAttribute();

    insertValue(reader_->GetName());

    while(reader_->MoveToNextAttribute())
    {
        insertValue(reader_->GetName());
    }

    reader_->MoveToElement();
}

void XMLMap::insertValue(const std::wstring &key)
{
   auto parent = parents_.at(parents_.size()-2);
   parent->childColumns.emplace(getNodeName(key, parent->childColumns));
   parents_.back()->columns.insert(key);
}

std::wstring XMLMap::getNodeName(const std::wstring &name, std::set<std::wstring> &names)
{
    /// ищем среди использовавшихся имён нужное
    for(auto i = names.begin(); i != names.end(); i++)
    {
        if(colNames_->GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    /// если не нашли, создаём его и вставляем
    auto resultName = name;
    colNames_->CreateColumnName(resultName);
    names.insert(resultName);
    return resultName;
}


std::shared_ptr<XmlNode> XMLMap::searchSameNode(const std::wstring &name)
{
    _UINT32 nodeCount = 0;
    for(auto i = parents_.back()->childs.begin(); i != parents_.back()->childs.end(); i++)
    {
        if(name == (*i)->name)
        {

            return (*i);
        }
    }
    return nullptr;
}


void XMLMap::openNode()
{
    auto nodeName = reader_->GetName();

    auto newElem = searchSameNode(nodeName);
    if(newElem)
    {
        newElem->counter++;
    }
    else
    {
        newElem = std::make_shared<XmlNode>();
        newElem->name = reader_->GetName();
        newElem->parent = parents_.back();
        newElem->counter = 1;
        parents_.back()->childs.emplace(newElem);
    }

    if(!reader_->IsEmptyNode())
    {
        parents_.push_back(newElem);
        readAttributes();
    }
    else if(reader_->GetAttributesCount() == 0)
    {
        insertValue(newElem->name);
    }
    else
    {
        parents_.push_back(newElem);
        readAttributes();
        closeNode();
    }
}

void XMLMap::closeNode()
{
    //вставка ноды типа <node></node>
    if(prevType_ == XmlUtils::XmlNodeType::XmlNodeType_Element)
    {
        insertValue(parents_.back()->name);
    }
    auto lastElem = parents_.back();
    parents_.pop_back();
    parents_.back()->childColumns.insert(lastElem->childColumns.begin(), lastElem->childColumns.end());

    for(auto i = lastElem->childs.begin(); i != lastElem->childs.end(); i++)
    {
        if((*i)->counter < 2)
        {
            (*i)->counter = 0;
        }
    }
    if(!(lastElem->columns.empty() || lastElem->childColumns.empty()) && lastElem->counter < 2)
    {
        lastElem->heritable = true;
    }
}

