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

#include "RevLblName.h"
#include "../../Binary/CFRecord.h"
#include "BIFF12/XLWideString.h"

namespace XLS
{


RevLblName::RevLblName()
{
}


RevLblName::RevLblName(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevLblName::clone()
{
	return BiffStructurePtr(new RevLblName(*this));
}

void RevLblName::load(CFRecord& record)
{
    XLUnicodeString       st_raw;
    XLSB::XLWideString    st_raw_xlsb;

    record >> iBuiltin;

    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> st_raw;
    }
    else
    {
        record >> st_raw_xlsb;
    }

	switch(iBuiltin)
	{
		case 0x00:        
            if (record.getGlobalWorkbookInfo()->Version < 0x0800)
            {
                st = st_raw.value();
            }
            else
            {
                st = st_raw_xlsb.value();
            }
			break;
		case 0x01:
			st = L"_xlnm.Consolidate_Area";
			break;
		case 0x02:
			st = L"_xlnm.Auto_Open";
			break;
		case 0x03:
			st = L"_xlnm.Auto_Close";
			break;
		case 0x04:
			st = L"_xlnm.Extract";
			break;
		case 0x05:
			st = L"_xlnm.Database";
			break;
		case 0x06:
			st = L"_xlnm.Criteria";
			break;
		case 0x07:
			st = L"_xlnm.Print_Area";
			break;
		case 0x08:
			st = L"_xlnm.Print_Titles";
			break;
		case 0x09:
			st = L"_xlnm.Recorder";
			break;
		case 0x0A:
			st = L"_xlnm.Data_Form";
			break;
		case 0x0B:
			st = L"_xlnm.Auto_Activate";
			break;
		case 0x0C:
			st = L"_xlnm.Auto_Deactivate";
			break;
		case 0x0D:
			st = L"_xlnm.Sheet_Title";
			break;
		case 0x0E:
			st = L"_xlnm._FilterDatabase";
			break;
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported value of built-in name.", record.getTypeString());
			break;
	}
}

void RevLblName::save(CFRecord& record)
{
	XLUnicodeString       st_raw;
	XLSB::XLWideString    st_raw_xlsb;

	record << iBuiltin;

	switch (iBuiltin)
	{
	case 0x00:
		if (record.getGlobalWorkbookInfo()->Version < 0x0800)
		{
			st_raw = st;
			record << st_raw;
		}
		else
		{
			st_raw_xlsb = st;
			record << st_raw_xlsb;
		}
		break;

	}
}


const std::wstring RevLblName::toString() const
{
	return st;
}


} // namespace XLS

