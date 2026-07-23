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

#include "ItemMoniker.h"

namespace OSHARED
{


ItemMoniker::ItemMoniker()
{
}


ItemMoniker::ItemMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr ItemMoniker::clone()
{
	return XLS::BiffStructurePtr(new ItemMoniker(*this));
}

void ItemMoniker::load(XLS::CFRecord& record)
{
	_UINT32 delimiterLength;
	record >> delimiterLength >> delimiterAnsi;
	
	int sizeof_delimiterUnicode = delimiterLength - (delimiterAnsi.length() + 1);

    if(sizeof_delimiterUnicode > 0)
	{		
#if defined(_WIN32) || defined(_WIN64)
        delimiterUnicode = std::wstring(record.getCurData<wchar_t>(), sizeof_delimiterUnicode / 2);
#else
        delimiterUnicode = convertUtf16ToWString(record.getCurData<UTF16>(), sizeof_delimiterUnicode / 2);
#endif
        record.skipNunBytes(sizeof_delimiterUnicode);
	}

	_UINT32 itemLength;
	record >> itemLength >> itemAnsi;
	
	int sizeof_itemUnicode = itemLength - (itemAnsi.length() + 1);

    if(sizeof_itemUnicode > 0)
	{		
#if defined(_WIN32) || defined(_WIN64)
        itemUnicode = std::wstring(record.getCurData<wchar_t>(), sizeof_itemUnicode / 2);
#else
        itemUnicode = convertUtf16ToWString(record.getCurData<UTF16>(), sizeof_itemUnicode / 2);
#endif
        record.skipNunBytes(sizeof_itemUnicode);
	}

}

void ItemMoniker::save(XLS::CFRecord& record)
{
    if(delimiterAnsi.empty() || delimiterAnsi.at(delimiterAnsi.size()-1) != '\0')
    {
        delimiterAnsi += '\0';
    }
    _UINT32 length = delimiterAnsi.size() + (delimiterUnicode.size() * 2);
    record << length;
    for(auto i : delimiterAnsi)
        record << i;
    for(auto i : delimiterUnicode)
        record << i;

    if(itemAnsi.empty() || itemAnsi.at(itemAnsi.size()-1) != '\0')
    {
        itemAnsi += '\0';
    }
    length = itemAnsi.size() + (itemUnicode.size() * 2);
    record << length;
    for(auto i : itemAnsi)
        record << i;
    for(auto i : itemUnicode)
        record << i;
}

} // namespace OSHARED

