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

#include "Dimensions.h"
#include "../Biff_structures/CellRangeRef.h"

namespace XLS
{

Dimensions::Dimensions()
{
}


Dimensions::~Dimensions()
{
}


BaseObjectPtr Dimensions::clone()
{
	return BaseObjectPtr(new Dimensions(*this));
}



void Dimensions::readFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		_UINT16	Mic, Mac;
		record >> Mic >> Mac;
		
		rwMic = Mic;
		rwMac = Mac;
	}
	else
	{
		record >> rwMic >> rwMac;
	}
	record >> colMic >> colMac;
	
	record.skipNunBytes(2); // reserved
	
	if(rwMac >= 0 && colMac >= 0)
	{
		ref_ = static_cast<std::wstring >(CellRangeRef(CellRef(rwMic, colMic, true, true), CellRef(rwMac - 1, colMac - 1, true, true)).toString(false).c_str());
	}
	else
	{
		ref_ = std::wstring (L"");
	}
}

void Dimensions::writeFields(CFRecord& record)
{
    if(ref_ != L"")
    {
        CellRangeRef rangeRef;
        rangeRef.fromString(ref_);
        rwMic = rangeRef.rowFirst;
        colMic = rangeRef.columnFirst;
		if(rwMac > rangeRef.rowLast)
			rwMac = rangeRef.rowLast;
		if(colMac > rangeRef.columnLast)
			colMac = rangeRef.columnLast;
    }
    record << rwMic << rwMac;
    record << colMic << colMac;
    record.reserveNunBytes(2); // reserved
}

int Dimensions::serialize(std::wostream & stream)
{
	if (ref_.empty()) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"dimension")
		{
			CP_XML_ATTR(L"ref", ref_);
		}
	}
	return 0;
}
} // namespace XLS

