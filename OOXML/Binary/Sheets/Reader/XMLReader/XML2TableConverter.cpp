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

#include <algorithm>

bool XML2TableConverter::GetTableData(XmlUtils::CXmlLiteReader &reader, XmlData &data)
{
    depth_ = reader.GetDepth();
    auto maxDepth = depth_;

    data_.push_back(std::vector<std::vector<std::wstring>>{});
    auto nodeType = reader.GetNodeType();

    processNode(reader, nodeType);
    while(reader.Read(nodeType))
    {
        depth_ = reader.GetDepth();
        /// расширяем вектор, если глубина увеличилась
        if(depth_ > maxDepth)
        {
            maxDepth= depth_;
            data_.push_back(std::vector<std::vector<std::wstring>>{});
        }

        processNode(reader, nodeType);
    }
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
    /// пытаемся найти вектор с таким же ключём на этой глубине
    auto searchResult = std::find_if(data_[depth_].begin(), data_[depth_].end(),
                         [&key](std::vector<std::wstring>& column) { return column[0] == key; });

    if(searchResult != data_[depth_].end())
    {
        searchResult->push_back(value);
        return;
    }

    std::vector<std::wstring> newColumn = {key, value};
    data_.at(depth_).push_back(newColumn);
}

void XML2TableConverter::processNode(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type)
{
    readAttributes(reader);
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Text || type == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
    {
        auto text = reader.GetText();
        if(!text.empty())
        {
            /// подставляем имя последней ноды в иерархии
            insertValue(parents_.at(parents_.size() -1), text);
        }
    }
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Element)
    {
        parents_.push_back(reader.GetName());
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_EndElement && parents_.size() > 0)
    {
        parents_.pop_back();
    }
}
