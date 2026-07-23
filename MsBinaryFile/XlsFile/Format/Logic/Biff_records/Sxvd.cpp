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

#include "Sxvd.h"

namespace XLS
{

Sxvd::Sxvd()
{
	cchName = 0;
}


Sxvd::~Sxvd()
{
}

BaseObjectPtr Sxvd::clone()
{
	return BaseObjectPtr(new Sxvd(*this));
}

void Sxvd::readFields(CFRecord& record)
{
	short flags;
	record >> sxaxis >> cSub >> flags;
	
	fDefault	= GETBIT(flags, 0);
	fSum		= GETBIT(flags, 1);
	fCounta		= GETBIT(flags, 2);
	fAverage	= GETBIT(flags, 3);
	fMax		= GETBIT(flags, 4);
	fMin		= GETBIT(flags, 5);
	fProduct	= GETBIT(flags, 6);
	fCount		= GETBIT(flags, 7);
	fStdev		= GETBIT(flags, 8);
	fStdevp		= GETBIT(flags, 9);
	fVariance	= GETBIT(flags, 10);
	fVariancep	= GETBIT(flags, 11);

	record >> cItm >> cchName;

	if(cchName && cchName != 0xffff)
	{
		stName.setSize(cchName);
		record >> stName;
	}
	int skip = record.getDataSize() - record.getRdPtr();
    record.skipNunBytes(skip);
}

void Sxvd::writeFields(CFRecord& record)
{
    short flags = 0;

    SETBIT(flags, 0, fDefault)
    SETBIT(flags, 1, fSum)
    SETBIT(flags, 2, fCounta)
    SETBIT(flags, 3, fAverage)
    SETBIT(flags, 4, fMax)
    SETBIT(flags, 5, fMin)
    SETBIT(flags, 6, fProduct)
    SETBIT(flags, 7, fCount)
    SETBIT(flags, 8, fStdev)
    SETBIT(flags, 9, fStdevp)
    SETBIT(flags, 10, fVariance)
    SETBIT(flags, 11, fVariancep)

    record << sxaxis << cSub << flags;
    cchName = stName.getSize();
	if(cchName == 0)
		cchName = 0xffff;
    record << cItm << cchName;
    if(cchName && cchName != 0xffff)
    {
        record << stName;
    }
}

} // namespace XLS

