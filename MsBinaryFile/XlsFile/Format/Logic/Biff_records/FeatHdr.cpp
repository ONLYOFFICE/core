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

#include "FeatHdr.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

FeatHdr::FeatHdr(const bool is_called_from_Globals)
:	is_contained_in_Globals(is_called_from_Globals)
{
}


FeatHdr::~FeatHdr()
{
}


BaseObjectPtr FeatHdr::clone()
{
	return BaseObjectPtr(new FeatHdr(*this));
}

void FeatHdr::readFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_FeatHdr);
	record >> frtHeader;
	record >> isf;
	record.skipNunBytes(1); // reserved
	record >> cbHdrData;
	if(cbHdrData)
	{
		switch(isf)
		{
			case 0x0002://ISFPROTECTION:
				record >> protection;
				break;
			case 0x0004://ISFFACTOID:
				if(is_contained_in_Globals)
				{
					record >> prop;
				}
				break;
		}
	}

}

void FeatHdr::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_FeatHdr);
	record << frtHeader << isf;
	unsigned char reserved = 1;
	record << reserved << cbHdrData;
	if(cbHdrData = 0)
		return;
	switch(isf)
	{
	case 0x0002://ISFPROTECTION:
		record << protection;
		break;
	case 0x0004://ISFFACTOID:
		if(is_contained_in_Globals)
		{
			record << prop;
		}
		break;
	}

}

} // namespace XLS

