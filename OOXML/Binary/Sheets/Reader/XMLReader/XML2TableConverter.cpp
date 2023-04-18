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

/// @brief номер строки в которую устанавлииваются имена столбцов
constexpr auto ColNamesRowNumber = 1;


XML2TableConverter::XML2TableConverter(XmlUtils::CXmlLiteReader &reader)
:reader_{&reader}{};

_INT32 XML2TableConverter::ReadNextString(std::map<std::wstring, _UINT32> &string)
{

    depth_ = reader_->GetDepth();

    auto nodeType = reader_->GetNodeType();

    readAttributes();
    storeData(nodeType);

    while(reader_->Read(nodeType))
    {
        depth_ = reader_->GetDepth();
        readAttributes();
        storeData(nodeType);
        processNode(nodeType);
        if(!stringBuffer_.empty())
        {
            string = stringBuffer_;
            stringBuffer_.clear();
            return rowIndex_-1;
        }

    }
    if(!xmlReaded_)
    {
        xmlReaded_ = true;
        insertColumnNames(string);
        return 1;
    }
    return -1;
}

void XML2TableConverter::readAttributes()
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
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

void XML2TableConverter::insertValue(const std::wstring &key, const std::wstring &value)
{
    auto uniqueKey = getNodeName(key);
    keyvalues_.emplace(uniqueKey, value);
}

std::wstring XML2TableConverter::getNodeName(const std::wstring &name)
{
    std::set<std::wstring> valueNames = {};
    auto parentsIndex = 0;
    if(depth_ == 0)
    {
        valueNames = parents_.at(0).second;
    }
    else if(parents_.size() > 3)
    {
        parentsIndex = parents_.size()-3;
        valueNames  = parents_.at(parentsIndex).second;
    }
    /// ищем среди использовавшихся имён нужное
    for(auto i = valueNames.begin(); i != valueNames.end(); i++)
    {
        if(colNames_.GetXmlName(*i) == name)
        {
            return *i;
        }
    }
    /// если не нашли, создаём его и вставляем
    auto resultName = name;
    colNames_.CreateColumnName(resultName);
    parents_.at(parentsIndex).second.insert(resultName);
    return resultName;
}

 void XML2TableConverter::insertColumnNames(std::map<std::wstring, _UINT32> &names)
 {
    names = colNames_.GetColumnNames();
 }


void XML2TableConverter::insertRow(std::map<std::wstring, _UINT32> &row)
{
    for(auto i = keyvalues_.begin(); i != keyvalues_.end(); i++)
    {
        row.emplace(i->second, colNames_.GetColumnNumber(i->first));
    }

    for(auto i = parentValues_.begin(); i != parentValues_.end(); i++)
    {
        row.emplace(i->second, colNames_.GetColumnNumber(i->first));
    }
    rowIndex_++;
}

void XML2TableConverter::processNode(const XmlUtils::XmlNodeType &type)
{
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && !reader_->IsEmptyNode())
    {
        parents_.push_back(std::make_pair(reader_->GetName(), std::set<std::wstring>()));
        tempDepth_= reader_->GetDepth();

    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_EndElement && parents_.size() > 0)
    {

        /// если глубина уменьшается извлекаем данные родительской ноды и записываем данные текущей
        if(tempDepth_ > reader_->GetDepth())
        {
            auto testName = reader_->GetName();
            /// если есть данные для записи, записываем их и чистим буфер с данными
            if(!keyvalues_.empty())
            {
                insertRow(stringBuffer_);
                keyvalues_.clear();
            }

            /// очистка предков от данных ноды из которой вышли и удаление узла предка
            auto parentNames = parents_.back().second;
            for(auto i = parentNames.begin(); i != parentNames.end(); i++)
            {
                parentValues_.erase(*i);
            }
        }
        /// если глубина растет, не пишем ноды в таблицу а передаем их как атрибуты для дочерних нод
        else if(tempDepth_ < reader_->GetDepth())
        {
            parentValues_.insert(keyvalues_.begin(), keyvalues_.end());
            keyvalues_.clear();
        }
        auto testVar = parents_.back().first;
        auto nodeNaaame = reader_->GetName();
        parents_.pop_back();
    }
}

void XML2TableConverter::storeData(const XmlUtils::XmlNodeType &type)
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Text || type == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
    {
            auto text = reader_->GetText();
            if(!text.empty())
            {
                insertValue(parents_.at(parents_.size() -1).first, text);
            }
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && reader_->IsEmptyNode() && reader_->GetAttributesCount() == 0)
    {
        insertValue(reader_->GetName(), L"");
    }
}
