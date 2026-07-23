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
#include "BoolErr.h"

namespace XLS
{

BoolErr::BoolErr()
{
}


BoolErr::~BoolErr()
{
}


BaseObjectPtr BoolErr::clone()
{
	return BaseObjectPtr(new BoolErr(*this));
}

void BoolErr::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	record >> cell >> bes;
}

void BoolErr::writeFields(CFRecord& record)
{
    record << cell << bes;
}

const CellRef BoolErr::getLocation() const
{
	return cell.getLocation();
}

int BoolErr::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);
			if (bes.fError)
				CP_XML_ATTR(L"t", L"e");
			else
				CP_XML_ATTR(L"t", L"b");

			if (cell.ixfe - global_info_->cellStyleXfs_count > 0)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
			CP_XML_NODE(L"v")
			{
				if (bes.fError)
				{
					switch (bes.bBoolErr)
					{
					case 0x00: CP_XML_STREAM() << L"#NULL!"; break;
					case 0x07: CP_XML_STREAM() << L"#DIV/0!"; break;
					case 0x0F: CP_XML_STREAM() << L"#VALUE!"; break;
					case 0x17: CP_XML_STREAM() << L"#REF!"; break;
					case 0x1D: CP_XML_STREAM() << L"#NAME?"; break;
					case 0x24: CP_XML_STREAM() << L"#NUM!"; break;
					case 0x2A: CP_XML_STREAM() << L"#N/A"; break;
					case 0x2B: CP_XML_STREAM() << L"#GETTING_DATA"; break;
					default:
						break;
					}
				}
				else
				{
					CP_XML_STREAM() << std::to_wstring(bes.bBoolErr);
				}
			}
		}
	}
	return 0;
}

} // namespace XLS

