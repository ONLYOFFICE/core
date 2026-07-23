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

#include "PtgStr.h"
#include <boost/algorithm/string.hpp>

namespace XLS
{

PtgStr::PtgStr(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgStr::PtgStr(const std::wstring & str)
:	string_(str),
	OperandPtg(fixed_id)
{
}


BiffStructurePtr PtgStr::clone()
{
	return BiffStructurePtr(new PtgStr(*this));
}



void PtgStr::loadFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        ShortXLUnicodeString s;
        record >> s;
        string_ = s;
    }

    else
    {
        _UINT16 cch;
        record >> cch;
        WCHAR value;
        for(int i = 0; i < cch; ++i)
        {
            record.loadAnyData(value);
            string_.push_back(value);
        }
    }


	int pos1 = string_.find(L"\"");
	int pos2 = string_.rfind(L"\"");

	if (pos1 == 0 && pos2 >= string_.length() - 1)
	{
		string_ = string_.substr(1, string_.length() - 2);
	}
    else if (pos1 > -1)
	{//012_JKH.OPEN.INFO.PRICE.VO...
		boost::algorithm::replace_all(string_, L"\"", L"\"\"");
	}

	string_ = L"\"" + string_ + L"\"";
}

void PtgStr::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		ShortXLUnicodeString s;
		s = string_;
		record << s;
	}

	else
	{
		_UINT16 cch;
		cch = string_.size();
		record << cch;
		WCHAR value;
		for (int i = 0; i < cch; ++i)
		{
			value = string_[i];
			record.storeAnyData(value);
		}
	}

}

void PtgStr::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	ptg_stack.push(string_);
}


} // namespace XLS

