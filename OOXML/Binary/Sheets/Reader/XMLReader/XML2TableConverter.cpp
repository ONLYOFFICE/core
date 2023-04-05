/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
bool XML2TableConverter::GetTableData(XmlUtils::CXmlLiteReader &reader, XmlData &data)
{
    readAttributes(reader);
    depth_ = reader.GetDepth();
    data_.push_back(std::map<std::wstring, std::vector<std::wstring>>{});
    while(readSiblings(reader))
    {
        depth_++;
        data_.push_back(std::map<std::wstring, std::vector<std::wstring>>{});
    }
    data_.pop_back();
    data = data_;
    return true;
}

void XML2TableConverter::readAttributes(XmlUtils::CXmlLiteReader &reader)
{
    if(!reader.GetAttributesCount())
    {
        return;
    }
    reader.MoveToFirstAttribute();

    insertValue(reader.GetName(), reader.GetText());

    while(reader.MoveToNextAttribute())
    {
        insertValue(reader.GetName(), reader.GetText());
    }

    reader.MoveToElement();
}

void XML2TableConverter::insertValue(const std::wstring &key, const std::wstring &value)
{

    if(data_.at(depth_).find(key) == data_.at(depth_).end())
    {
        data_.at(depth_).insert({key, std::vector<std::wstring>{}});
    }

    data_.at(depth_)[key].push_back(value);
}

void XML2TableConverter::processNode(XmlUtils::CXmlLiteReader &reader)
{
    readAttributes(reader);
    auto text = reader.GetText2();
    if(!text.empty())
    {
        insertValue(reader.GetName(), text);
    }
}

bool XML2TableConverter::readSiblings(XmlUtils::CXmlLiteReader &reader)
{
    if(!reader.ReadNextSiblingNode(depth_))
    {
        return false;
    }

    processNode(reader);

    while(reader.ReadNextSiblingNode(depth_))
    {
        processNode(reader);
    }
    return true;
}
