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

#include "SerStr.h"

#include <boost/algorithm/string.hpp>

namespace XLS
{


SerStr::SerStr()
{
    fixed_type = typeSerStr;
}


SerStr::SerStr(const std::wstring& word)
:	string_(std::wstring (word.c_str()))
{
}


BiffStructurePtr SerStr::clone()
{
	return BiffStructurePtr(new SerStr(*this));
}

void SerStr::load(CFRecord& record)
{	
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
        record >> string_;
    else
    {
        record >> cch;
        WCHAR value;
        for(int i = 0; i < cch; ++i)
        {
            record.loadAnyData(value);
            rgch.push_back(value);
        }
    }
}

void SerStr::save(CFRecord& record)
{
    char serType;
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        serType = 2;
        record << serType << string_;
    }
    else
    {
        serType = 1;
        rgch = string_;
        cch = string_.getSize();
        record <<serType << cch;
        for(auto i:rgch)
        {
            record.storeAnyData(i);
        }
    }
}

const std::wstring SerStr::toString() const
{
	std::wstring tempVal = rgch;
	tempVal.erase(std::remove(tempVal.begin(), tempVal.end(), L'\0'), tempVal.end());
	if(tempVal.size() > 255)
		tempVal.resize(255);
	return L"\"" + boost::algorithm::replace_all_copy(tempVal, L"\"", L"\"\"") + L"\"";
}


} // namespace XLS
