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


XMLConverter::XMLConverter(XmlUtils::CXmlLiteReader &reader, std::shared_ptr<XmlNode> xmlStruct, ColumnNameController &nameController):
    reader_{&reader},
    nodeTree_{xmlStruct},
    colNames_{&nameController},
    nodePointer_{*nodeTree_->childs.begin()}
{
    parents_.push_back(xmlStruct);
}

void XMLConverter::ConvertXml(std::vector<std::vector<std::wstring>> &table)
{
    XmlUtils::XmlNodeType nodeType;

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

}

void XMLConverter::openNode()
{
    auto nodename = reader_->GetName();
    for(auto i = parents_.back()->childs.begin(); i != parents_.back()->childs.end(); i++)
    {
        if((*i)->name == nodename)
        {
            nodePointer_ = *i;
        }
    }
    if(reader_->IsEmptyNode() && !nodePointer_->columns.empty())
    {
        readAttributes();
        nodePointer_ = nodePointer_->parent;
    }
    else if(reader_->IsEmptyNode() && nodePointer_->columns.empty())
    {
        insertValue(nodePointer_->name, L"");
        nodePointer_ = nodePointer_->parent;
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
    nodePointer_ = nodePointer_->parent;
    parents_.pop_back();
}

void insertValue(const std::wstring &key, const std::wstring &value)
{

}