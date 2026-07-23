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

#include "Bes.h"

#include <boost/algorithm/string.hpp>

namespace XLS
{


BiffStructurePtr Bes::clone()
{
	return BiffStructurePtr(new Bes(*this));
}


void Bes::load(CFRecord& record)
{
	record >> bBoolErr >> fError;
}

void Bes::save(CFRecord& record)
{
    record << bBoolErr << fError;
}


const std::wstring Bes::toString()
{
	if(fError || bBoolErr > 0x01)
	{
		switch(bBoolErr)
		{
		case 0x00:
			return L"#NULL!";
		case 0x07:
			return L"#DIV/0!";
		case 0x0F:
			return L"#VALUE!";
		case 0x17:
			return L"#REF!";
		case 0x1D:
			return L"#NAME?";
		case 0x24:
			return L"#NUM!";
		case 0x2A:
			return L"#N/A";
		case 0x2B:
			return L"#GETTING_DATA";
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
			break;
		}
	}
	else
	{
		return Boolean<unsigned char>(bBoolErr);
	}
    
    return L""; // TODO: need fixed
}


void Bes::fromString(const std::wstring str)
{
	const std::wstring upper_str = boost::algorithm::to_upper_copy(str);
	if(L"#NULL!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x00;
	}
	else if(L"#DIV/0!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x07;
	}
	else if(L"#VALUE!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x0F;
	}
	else if(L"#REF!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x17;
	}
	else if(L"#NAME?" == upper_str)
	{
		fError = true;
		bBoolErr = 0x1D;
	}
	else if(L"#NUM!" == upper_str)
	{
		fError = true;
		bBoolErr = 0x24;
	}
	else if(L"#N/A" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2A;
	}
	else if(L"#GETTING_DATA" == upper_str)
	{
		fError = true;
		bBoolErr = 0x2B;
	}
	else if(L"TRUE" == upper_str || L"1" == str)
	{
		fError = false;
		bBoolErr = 0x01;
	}
	else if(L"FALSE" == upper_str || L"0" == str)
	{
		fError = false;
		bBoolErr = 0x00;
	}
	else
	{
		// EXCEPT::RT::WrongBiffRecord("Unsupported type of Bes.", "unknown");
	}
}


} // namespace XLS

