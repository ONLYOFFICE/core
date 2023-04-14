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

bool XML2TableConverter::GetTableData(XmlUtils::CXmlLiteReader &reader, XLSXTableController &table)
{
    table_ = &table;

    depth_ = reader.GetDepth();

    auto nodeType = reader.GetNodeType();

    processNode(reader, nodeType);
    while(reader.Read(nodeType))
    {
        depth_ = reader.GetDepth();
        readAttributes(reader);
        tryInsertData(reader, nodeType);
        processNode(reader, nodeType);
    }

    return true;
}

void XML2TableConverter::readAttributes(XmlUtils::CXmlLiteReader &reader)
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
    if(!reader.GetAttributesCount())
    {
        return;
    }
    reader.MoveToFirstAttribute();

    keyvalues_.push_back(std::make_pair(reader.GetName(), reader.GetText()));

    while(reader.MoveToNextAttribute())
    {
        keyvalues_.push_back(std::make_pair(reader.GetName(), reader.GetText()));
    }

    reader.MoveToElement();
}

void XML2TableConverter::insertValue(const std::wstring &key, const std::wstring &value)
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
    auto colname = key;
    auto colCount = colNames_.GetColumnNumber(colname);
    if(colCount < 0)
    {
        colCount = colNames_.CreateColumnName(colname);
    }

    table_->AddCell(value, rowIndex_, colCount);
}

 void XML2TableConverter::insertColumnNames()
 {
    auto names = colNames_.GetColumnNames();
    for(auto i = names.begin(); i != names.end(); i++)
    {
        table_->AddCell(i->first, ColNamesRowNumber, i->second);
    }
 }


void XML2TableConverter::insertRow(const keyValueArray &dataRow, const keyValueArray &parentsRow)
{
    auto index = 0;
    for(auto index = 0; index < dataRow.size(); index++)
    {
        insertValue(dataRow.at(index).first, dataRow.at(index).second);
    }

    for(auto index = 0; index < parentsRow.size(); index++)
    {
        insertValue(parentsRow.at(index).first, parentsRow.at(index).second);
    }
    rowIndex_++;
}

void XML2TableConverter::processNode(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type)
{
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Element)
    {
        parents_.push_back(reader.GetName());
        tempDepth_= reader.GetDepth();

    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_EndElement && parents_.size() > 0)
    {
        parents_.pop_back();

        /// если глубина уменьшается извлекаем данные родительской ноды и записываем данные текущей
        if(tempDepth_ > reader.GetDepth())
        {
            if(!parentNodeValueCount_.empty())
            {
                ///@todo очистка parents от данных ноды из которой мы вышли
                for(auto i = 0; i < parentNodeValueCount_.back(); i++)
                {
                    parentValues_.pop_back();
                }
                parentNodeValueCount_.pop_back();
            }

            /// если есть данные для записи, записываем их и чистим буфер с данными
            if(!keyvalues_.empty())
            {
                insertRow(keyvalues_, parentValues_);
                keyvalues_.clear();
            }
        }
        /// если глубина растет, не пишем ноды в таблицу а передаем их как атрибуты для дочерних нод
        else if(tempDepth_ < reader.GetDepth())
        {
            parentNodeValueCount_.push_back(keyvalues_.size());
            while(!keyvalues_.empty())
            {
                parentValues_.push_back(keyvalues_.front());
                keyvalues_.pop_front();
            }
        }
    }
}

void XML2TableConverter::tryInsertData(XmlUtils::CXmlLiteReader &reader, const XmlUtils::XmlNodeType &type)
{
    ///@todo проверять нет ли в parents нод с таким же именем для вставки в их столбец вместо создания нового
    if(type == XmlUtils::XmlNodeType::XmlNodeType_Text || type == XmlUtils::XmlNodeType::XmlNodeType_CDATA)
    {
            auto text = reader.GetText();
            if(!text.empty())
            {
                keyvalues_.push_back(std::make_pair(parents_.at(parents_.size() -1), text));
            }
    }
    else if(type == XmlUtils::XmlNodeType::XmlNodeType_Element && reader.IsEmptyNode() && reader.GetAttributesCount() == 0)
    {
        keyvalues_.push_back(std::make_pair(reader.GetName(), L""));
    }
}