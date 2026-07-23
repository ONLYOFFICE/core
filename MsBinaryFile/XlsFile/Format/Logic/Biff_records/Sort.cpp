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

#include "Sort.h"

namespace XLS
{

Sort::Sort()
{
}


Sort::~Sort()
{
}


BaseObjectPtr Sort::clone()
{
	return BaseObjectPtr(new Sort(*this));
}

void Sort::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fCol		= GETBIT(flags, 0);
	fKey1Dsc	= GETBIT(flags, 1);
	fKey2Dsc	= GETBIT(flags, 2);
	fKey3Dsc	= GETBIT(flags, 3);
	fCaseSensitive = GETBIT(flags, 4);
	iOrder		= GETBITS(flags, 5, 9);
	fAltMethod	= GETBIT(flags, 10);

	unsigned char cchKey1;
	unsigned char cchKey2;
	unsigned char cchKey3;
	record >> cchKey1 >> cchKey2 >> cchKey3;

	if (record.getGlobalWorkbookInfo()->Version >= 0x0600)
	{
		XLUnicodeStringNoCch stKey1_, stKey2_, stKey3_;

		if(cchKey1)
		{
			stKey1_.setSize(cchKey1);
			record >> stKey1_;
			stKey1 = stKey1_.value();
		}
		if(cchKey2)
		{
			stKey2_.setSize(cchKey2);
			record >> stKey2_;
			stKey2 = stKey2_.value();
		}
		if(cchKey3)
		{
			stKey3_.setSize(cchKey3);
			record >> stKey3_;
			stKey3 = stKey3_.value();
		}
	}
	else
	{
		LPAnsiStringNoCch stKey1_, stKey2_, stKey3_;

		if(cchKey1)
		{
			stKey1_.setSize(cchKey1);
			record >> stKey1_;
			stKey1 = stKey1_.value();
		}
		if(cchKey2)
		{
			stKey2_.setSize(cchKey2);
			record >> stKey2_;
			stKey2 = stKey2_.value();
		}
		if(cchKey3)
		{
			stKey3_.setSize(cchKey3);
			record >> stKey3_;
			stKey3 = stKey3_.value();
		}
	}
	record.skipNunBytes(1); // reserved
}

void Sort::writeFields(CFRecord& record)
{
    unsigned short flags = 0;

    SETBIT(flags, 0, fCol);
    SETBIT(flags, 1, fKey1Dsc);
    SETBIT(flags, 2, fKey2Dsc);
    SETBIT(flags, 3, fKey3Dsc);
    SETBIT(flags, 4, fCaseSensitive);
    SETBITS(flags, 5, 9, iOrder);
    SETBIT(flags, 10, fAltMethod);
    record << flags;

    unsigned char cchKey1 = stKey1.size();
    unsigned char cchKey2 = stKey2.size();
    unsigned char cchKey3 = stKey3.size();
    record << cchKey1 << cchKey2 << cchKey3;
    if(cchKey1)
    {
        XLUnicodeStringNoCch stKey1_ = stKey1;
        record << stKey1_;
    }
    if(cchKey2)
    {
        XLUnicodeStringNoCch stKey2_ = stKey2;
        record << stKey2_;
    }
    if(cchKey3)
    {
        XLUnicodeStringNoCch stKey3_ = stKey3;
        record << stKey3_;
    }
    record.reserveNunBytes(1);
}

} // namespace XLS

