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

#include "XMLMap.h"

bool XMLMap::ReadXmlStructure(XmlUtils::CXmlLiteReader &reader, ColumnNameController &nameController, std::shared_ptr<XmlNode> nodeTree,
std::set<std::wstring> &repeatebleValues)
{
    reader_ = &reader;
    colNames_ = &nameController;
    repeatebleValues_ = &repeatebleValues;
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
            insertValue();
        }
        else if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_EndElement)
        {
            closeNode();
        }
        if(nodeType !=XmlUtils::XmlNodeType::XmlNodeType_SIGNIFICANT_WHITESPACE && !reader_->IsEmptyNode())
        {
            prevType_ = nodeType;
        }
    }

    return true;
}

void XMLMap::readAttributes()
{
    ///@todo check if parents have nodes with the same name to insert into their column instead of creating a new one
    if(!reader_->GetAttributesCount())
    {
        return;
    }
    reader_->MoveToFirstAttribute();

    insertAttribute(reader_->GetName());

    while(reader_->MoveToNextAttribute())
    {
        insertAttribute(reader_->GetName());
    }

    reader_->MoveToElement();
}

void XMLMap::insertValue()
{
   auto parent = parents_.at(parents_.size()-2);
   auto node = parents_.back();
   auto uniqueName = getNodeName(node->name, parent->childColumns);
   parent->childColumns.emplace(uniqueName);
   node->ValueColumnName = uniqueName;
}

void XMLMap::insertAttribute(const std::wstring &key)
{
   auto parent = parents_.at(parents_.size()-2);
   auto node = parents_.back();
   auto uniqueName = getNodeName(key, parent->childColumns);
   parent->childColumns.emplace(uniqueName);
   node->attributes.emplace(uniqueName);
}

std::wstring XMLMap::getNodeName(const std::wstring &name, std::set<std::wstring> &names)
{
     /// search for the needed name among used names
    for(auto i = names.begin(); i != names.end(); i++)
    {
        if(colNames_->GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    /// if not found, create and insert it
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
        newElem->parents = parents_.back()->parents;
        newElem->parents.insert(newElem->parent);
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
        parents_.push_back(newElem);
        insertValue();
        closeNode();
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
    //insert node like <node></node>
    if(prevType_ == XmlUtils::XmlNodeType::XmlNodeType_Element)
    {
        insertValue();
    }
    auto lastElem = parents_.back();
    parents_.pop_back();
    parents_.back()->childColumns.insert(lastElem->childColumns.begin(), lastElem->childColumns.end());
    parents_.back()->childColumns.insert(lastElem->attributes.begin(), lastElem->attributes.end());
    //remove node value if it has children
    if(!lastElem->childs.empty())
    {
        lastElem->ValueColumnName = L"";
    }

    for(auto i = lastElem->childs.begin(); i != lastElem->childs.end(); i++)
    {
        if((*i)->counter < 2)
        {
            (*i)->counter = 0;
        }
        else
        {
            repeatebleValues_->insert((*i)->childColumns.begin(), (*i)->childColumns.end());
            if(!(*i)->ValueColumnName.empty())
            {
                repeatebleValues_->insert((*i)->ValueColumnName);
            }
            if(!(*i)->attributes.empty())
            {
                repeatebleValues_->insert((*i)->attributes.begin(), (*i)->attributes.end());
            }
        }
    }

}