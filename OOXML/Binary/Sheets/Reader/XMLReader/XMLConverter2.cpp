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

#include "XMLConverter2.h"


XMLConverter::XMLConverter(XmlUtils::CXmlLiteReader &reader, std::shared_ptr<XmlNode> xmlStruct, ColumnNameController &nameController
, std::set<std::wstring> &repeatebleValues):
    reader_{&reader},
    nodeTree_{xmlStruct},
    colNames_{&nameController},
    nodePointer_{*nodeTree_->childs.begin()},
    listableColumns_{&repeatebleValues}
{
    parents_.push_back(xmlStruct);
}

void XMLConverter::ConvertXml(XLSXTableController &table)
{
    XmlUtils::XmlNodeType nodeType;

    for(auto i : nodeTree_->childColumns)
    {
        data_.emplace(i, std::vector<std::wstring>());
    }

    while(reader_->Read(nodeType))
    {
        if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_Element)
        {
            openNode();
        }
        else if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_Text || nodeType == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
        {
            insertValue(parents_.back()->name, reader_->GetText());
        }
        else if(nodeType == XmlUtils::XmlNodeType::XmlNodeType_EndElement)
        {
            closeNode();
        }
        prevType_ = nodeType;
    }
    ///выписываем однократно содержимое нод, если нет повторяющихся
    if(writingRows_.empty())
    {
        for(auto i :data_)
        {
            table.AddCell(*data_.at(i.first).begin(), 2, colNames_->GetColumnNumber(i.first));
        }
        return;
    }
    for(auto nodeCount = 0; nodeCount < writingRows_.size(); nodeCount++)
    {
        auto nodeName = writingRows_.at(nodeCount)->ValueColumnName;
        auto rowNumber = nodeCount + 2;
        if(!nodeName.empty())
        {
            table.AddCell(data_.at(nodeName).at(nodeCount), rowNumber, colNames_->GetColumnNumber(nodeName));
        }
        for( auto i : writingRows_.at(nodeCount)->childColumns)
        {
            table.AddCell(data_.at(i).at(nodeCount), rowNumber, colNames_->GetColumnNumber(i));
        }
        for( auto i : writingRows_.at(nodeCount)->attributes)
        {
            table.AddCell(data_.at(i).at(nodeCount), rowNumber, colNames_->GetColumnNumber(i));
        }
        std::set<std::wstring> writedColumns = {};
        for(auto i = writingRows_.at(nodeCount)->parent; i; i = i->parent)
        {
            fillAttribures(table, i, writedColumns, rowNumber);
        }
    }

}

void XMLConverter::openNode()
{
    auto nodename = reader_->GetName();
    for(auto i = parents_.back()->childs.begin(); i != parents_.back()->childs.end(); i++)
    {
        if((*i)->name == nodename)
        {
            nodePointer_ = *i;
            parents_.push_back(nodePointer_);
            break;
        }
    }

    if(reader_->IsEmptyNode() && !nodePointer_->attributes.empty())
    {
        readAttributes();
        prevType_ = XmlUtils::XmlNodeType::XmlNodeType_Text;
        closeNode();
    }
    else if(reader_->IsEmptyNode() && nodePointer_->attributes.empty())
    {
        insertValue(nodePointer_->name, L"");
        prevType_ = XmlUtils::XmlNodeType::XmlNodeType_Text;
        closeNode();
    }
    else
    {
        readAttributes();
    }
}

void XMLConverter::readAttributes()
{
    if(!reader_->GetAttributesCount())
    {
        return;
    }
    reader_->MoveToFirstAttribute();

    insertValue(reader_->GetName(), reader_->GetText());

    while(reader_->MoveToNextAttribute())
    {
        insertValue(reader_->GetName(), reader_->GetText());
    }

    reader_->MoveToElement();
}


void XMLConverter::closeNode()
{
    if(prevType_ == XmlUtils::XmlNodeType::XmlNodeType_Element)
    {
        insertValue(nodePointer_->name, L"");
    }
    if(nodePointer_->counter > 1)
    {
        writingRows_.push_back(nodePointer_);
        for(auto i = listableColumns_->begin(); i != listableColumns_->end(); i++)
        {
            if(data_.at(*i).size() < writingRows_.size())
            {
                data_.at(*i).push_back(L"");
            }
        }
    }

    nodePointer_ = nodePointer_->parent;
    parents_.pop_back();
}

std::wstring XMLConverter::getNodeName(const std::wstring &name)
{
    /// ищем среди использовавшихся имён нужное
    for(auto i = nodePointer_->attributes.begin(); i != nodePointer_->attributes.end(); i++)
    {
        if(colNames_->GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    return nodePointer_->ValueColumnName;
}

void XMLConverter::insertValue(const std::wstring &key, const std::wstring &value)
{
    auto uniqueKey = getNodeName(key);
    auto dataRow = data_.find(uniqueKey);
    if(dataRow != data_.end())
    {
        dataRow->second.push_back(value);
    }
}

void XMLConverter::fillAttribures(XLSXTableController &table, std::shared_ptr<XmlNode> attribNode, std::set<std::wstring> &filledValues,
        const _UINT32 &rowNumber)
{
    for(auto i:attribNode->attributes)
    {
        if(filledValues.find(i) == filledValues.end())
        {
            table.AddCell(data_.at(i).at(0), rowNumber, colNames_->GetColumnNumber(i));
            filledValues.insert(i);
        }
    }
    for(auto i:attribNode->childColumns)
    {
        if(filledValues.find(i) == filledValues.end() && listableColumns_->find(i) == listableColumns_->end())
        {
            table.AddCell(data_.at(i).at(0), rowNumber, colNames_->GetColumnNumber(i));
            filledValues.insert(i);
        }
    }
}