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

#include "Qsir.h"

namespace XLS
{

Qsir::Qsir()
{
	wVerBeforeRefreshAlert = 0;
}


Qsir::~Qsir()
{
}


BaseObjectPtr Qsir::clone()
{
	return BaseObjectPtr(new Qsir(*this));
}

void Qsir::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT32 flags;

        record >> frtRefHeaderU >> cbQsirSaved >> cbQsifSaved >> flags >> iSortCustom >> cQsif >> cpstDeleted >> idFieldNext >> ccolExtraLeft >> ccolExtraRight;
        record >> idList >> rgbTitle;

        fPersist			= GETBIT(flags, 0);
        fPersistSort		= GETBIT(flags, 1);
        fPersistAutoFilter	= GETBIT(flags, 2);
        fSorted				= GETBIT(flags, 20);
        fCaseSensSort		= GETBIT(flags, 21);
        fHdrRowSort			= GETBIT(flags, 22);
        fidWrapped			= GETBIT(flags, 23);
        fTitlesOld			= GETBIT(flags, 25);
        wVerBeforeRefreshAlert = GETBITS(flags, 26, 30);
    }

    else
    {
        _UINT16 flags;

        record >> flags >> idFieldNext >> ccolExtraLeft >> ccolExtraRight;

        fPersist			= GETBIT(flags, 0);
        fPersistSort		= GETBIT(flags, 1);
        fPersistAutoFilter	= GETBIT(flags, 2);
        fidWrapped			= GETBIT(flags, 3);
        fTitlesOld			= GETBIT(flags, 4);
        wVerBeforeRefreshAlert = GETBITS(flags, 5, 9);
    }
}

void Qsir::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT32 flags = 0;

		SETBIT(flags, 0, fPersist)
		SETBIT(flags, 1, fPersistSort)
		SETBIT(flags, 2, fPersistAutoFilter)
		SETBIT(flags, 20, fSorted)
		SETBIT(flags, 21, fCaseSensSort)
		SETBIT(flags, 22, fHdrRowSort)
		SETBIT(flags, 23, fidWrapped)
		SETBIT(flags, 25, fTitlesOld)
		SETBITS(flags, 26, 30, wVerBeforeRefreshAlert)

		record << frtRefHeaderU << cbQsirSaved << cbQsifSaved << flags << iSortCustom << cQsif << cpstDeleted << idFieldNext << ccolExtraLeft << ccolExtraRight;
		record << idList << rgbTitle;
	}

	else
	{
		_UINT16 flags = 0;

		SETBIT(flags, 0, fPersist)
		SETBIT(flags, 1, fPersistSort)
		SETBIT(flags, 2, fPersistAutoFilter)
		SETBIT(flags, 3, fidWrapped)
		SETBIT(flags, 4, fTitlesOld)
		SETBITS(flags, 5, 9, wVerBeforeRefreshAlert)

		record << flags << idFieldNext << ccolExtraLeft << ccolExtraRight;
	}
}

} // namespace XLS

