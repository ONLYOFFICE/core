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

#include "SXVI.h"

namespace XLS
{

SXVI::SXVI()
{
}


SXVI::~SXVI()
{
}


BaseObjectPtr SXVI::clone()
{
	return BaseObjectPtr(new SXVI(*this));
}

void SXVI::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> itmType>> flags >> iCache >> cchName;

	fHidden		= GETBIT(flags, 0);
	fHideDetail	= GETBIT(flags, 1);
	fFormula	= GETBIT(flags, 3);
	fMissing	= GETBIT(flags, 4);
	
	if (cchName > 0 && cchName < 0xFFFF)
	{
		stName.setSize(cchName);
		record >> stName;

	}
}

void SXVI::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fHidden)
    SETBIT(flags, 1, fHideDetail)
    SETBIT(flags, 3, fFormula)
    SETBIT(flags, 4, fMissing)
    cchName = stName.getSize();
	if(cchName == 0)
		cchName = 0xFFFF;

    record << itmType << flags << iCache << cchName;
    if (cchName > 0 && cchName < 0xFFFF)
    {
        record << stName;
    }
}

int SXVI::serialize(std::wostream & strm)
{
	CP_XML_WRITER(strm)
	{
		CP_XML_NODE(L"item")
		{
			if (fHidden)	CP_XML_ATTR(L"h", 1); 
			
			if (fMissing)	CP_XML_ATTR(L"m", 1); 
			if (fHideDetail)CP_XML_ATTR(L"sd", 0); 
			if (fFormula)	CP_XML_ATTR(L"f", 1);

			if (!stName.value().empty())
			{
				CP_XML_ATTR(L"n", stName.value()); 
			}

			if (itmType == 0)
			{
				CP_XML_ATTR(L"x", iCache); 
			}
			switch(itmType)
			{
			case 0x0001: CP_XML_ATTR(L"t", L"default"); break;
			case 0x0002: CP_XML_ATTR(L"t", L"sum");		break;
			case 0x0003: CP_XML_ATTR(L"t", L"countA");	break;
			case 0x0004: CP_XML_ATTR(L"t", L"avg");		break;
			case 0x0005: CP_XML_ATTR(L"t", L"max");		break;
			case 0x0006: CP_XML_ATTR(L"t", L"min");		break;
			case 0x0007: CP_XML_ATTR(L"t", L"product"); break;
			case 0x0008: CP_XML_ATTR(L"t", L"count");	break;
			case 0x0009: CP_XML_ATTR(L"t", L"stdDev");	break;
			case 0x000a: CP_XML_ATTR(L"t", L"stdDevP"); break;
			case 0x000b: CP_XML_ATTR(L"t", L"var");		break;
			case 0x000c: CP_XML_ATTR(L"t", L"varP");	break;
			}
		}
	}
	return 0;
}
} // namespace XLS

