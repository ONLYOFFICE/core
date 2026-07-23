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

#include "BookExt.h"
#include "../Biff_structures/FrtHeader.h"

namespace XLS
{

BookExt::BookExt() : cb(22)
{
	mdFactoidDisplay = 0;
}


BookExt::~BookExt()
{
}


BaseObjectPtr BookExt::clone()
{
	return BaseObjectPtr(new BookExt(*this));
}

void BookExt::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        FrtHeader header(rt_BookExt);
        record >> header;

        record >> cb;
        _UINT32 flags;

        if (record.loadAnyData(flags))
        {
            fDontAutoRecover		= GETBIT(flags, 0);
            fHidePivotList			= GETBIT(flags, 1);
            fFilterPrivacy			= GETBIT(flags, 2);
            fEmbedFactoids			= GETBIT(flags, 3);
            mdFactoidDisplay		= GETBITS(flags, 4, 5);
            fSavedDuringRecovery	= GETBIT(flags, 6);
            fCreatedViaMinimalSave	= GETBIT(flags, 7);
            fOpenedViaDataRecovery	= GETBIT(flags, 8);
            fOpenedViaSafeLoad		= GETBIT(flags, 9);
        }
        else return;


        if(cb > 20)
        {
            record >> grbit1;
        }
        if(cb > 21)
        {
            record >> grbit2;
        }
    }
    else
    {
        unsigned char flags;

        if (record.loadAnyData(flags))
        {
            fDontAutoRecover		= GETBIT(flags, 0);
            fSavedDuringRecovery	= GETBIT(flags, 1);
            fCreatedViaMinimalSave	= GETBIT(flags, 2);
            fOpenedViaDataRecovery	= GETBIT(flags, 3);
            fOpenedViaSafeLoad		= GETBIT(flags, 4);
        }

    }
}

void BookExt::writeFields(CFRecord& record)
{
	FrtHeader header(rt_BookExt);
	record << header << cb;
	 _UINT32 flags = 0;

	SETBIT(flags, 0, fDontAutoRecover)
	SETBIT(flags, 1, fHidePivotList)
	SETBIT(flags, 2, fFilterPrivacy)
	SETBIT(flags, 3, fEmbedFactoids)
	SETBITS(flags, 4, 5, mdFactoidDisplay)
	SETBIT(flags, 6, fSavedDuringRecovery)
	SETBIT(flags, 7, fCreatedViaMinimalSave)
	SETBIT(flags, 8, fOpenedViaDataRecovery)
	SETBIT(flags, 9, fOpenedViaSafeLoad)
	record << flags;

	if(cb > 20)
		record << grbit1;
	if(cb > 21)
		record << grbit2;
}

} // namespace XLS

