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

#include "BoundSheet8.h"

namespace XLS
{

BoundSheet8::BoundSheet8()
{
}


BoundSheet8::~BoundSheet8()
{
}


BaseObjectPtr BoundSheet8::clone()
{
	return BaseObjectPtr(new BoundSheet8(*this));
}

void BoundSheet8::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> lbPlyPos >> flags;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString stName;
		record >> stName;
		name_ = stName.value();
	}
	else
	{
		ShortXLUnicodeString stName;
		record >> stName;
		name_ = stName.value();
	}

	switch(GETBITS(flags, 0, 1))
	{
		case 0:
			hsState = std::wstring (L"visible");
			break;
		case 1:
			hsState = std::wstring (L"hidden");
			break;
		case 2:
			hsState = std::wstring (L"veryHidden");
			break;
	}
	if (name_.length() > 31)
	{
		name_ = name_.substr(0, 31);
		//cell_links_broken.xls
	}
	if (name_.empty())
	{//file(6).xls
		name_ = L"Sheet_" + std::to_wstring(record.getGlobalWorkbookInfo()->current_sheet + 1);
	}

	GlobalWorkbookInfo::_sheet_info sheet_info;
	sheet_info.state = hsState;
	sheet_info.name = name_;
	record.getGlobalWorkbookInfo()->sheets_info.push_back(sheet_info);
	
	dt = GETBITS(flags, 8, 15);
}

void BoundSheet8::writeFields(CFRecord& record)
{
    record << lbPlyPos;
    unsigned short flags = 0;
    if( hsState == L"hidden")
    {
        SETBITS(flags, 0, 1, 1)
    }
    else if( hsState == L"veryHidden")
    {
        SETBITS(flags, 0, 1, 2)
    }
    SETBITS(flags, 8, 15, dt)
    record << flags;

    ShortXLUnicodeString stName;
    stName = name_;
    record << stName;
}

} // namespace XLS

