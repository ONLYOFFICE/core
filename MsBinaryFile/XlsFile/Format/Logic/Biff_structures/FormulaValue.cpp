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

#include "FormulaValue.h"
#include "BitMarkedStructs.h"

namespace XLS
{


BiffStructurePtr FormulaValue::clone()
{
	return BiffStructurePtr(new FormulaValue(*this));
}


std::wstring FormulaValue::getValue()
{
	std::wstring result;

	if(0xffff != data.fExprO)
	{
		result = STR::double2str(data.xnum);
	}
	else
	{
		switch(data.Byte1)
		{
			case 0x00: // String
				result = L"";	// there must a String value follow this Formula record
				break;
			case 0x01: // Boolean
				result = data.Byte3 ? L"true" : L"false";
				break;
			case 0x02: // Error
			{
				BErr error;
				error.err = data.Byte3;
				result = error.toString();
				break;
			}
			case 0x03: // Blank
				result = L"";
				break;
		}
	}
	return result;
}


void FormulaValue::load(CFRecord& record)
{
	record >> data.xnum;
}

void FormulaValue::save(CFRecord& record)
{
    record << data.xnum;
}

unsigned char FormulaValue::getType() 
{
	if(0xffff != data.fExprO)
	{
		return 4; // Would mean that the value is numeric
	}
	else
	{
		return data.Byte1;
	}
}

} // namespace XLS

