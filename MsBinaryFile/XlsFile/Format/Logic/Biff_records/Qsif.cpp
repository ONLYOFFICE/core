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

#include "Qsif.h"
#include "../Biff_structures/BIFF12/XLWideString.h"

namespace XLS
{

Qsif::Qsif()
{
}

Qsif::~Qsif()
{
}

BaseObjectPtr Qsif::clone()
{
	return BaseObjectPtr(new Qsif(*this));
}

void Qsif::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16  flags1, flags2;
        XLUnicodeString rgbTitle;

        record >> frtHeaderOld >> flags1 >> flags2 >> idField;

        fUserIns	= GETBIT(flags1, 0);
        fFillDown	= GETBIT(flags1, 1);
        fSortDes	= GETBIT(flags1, 2);
        iSortKey	= GETBITS(flags1, 3, 10);
        fRowNums	= GETBIT(flags1, 11);
        fSorted		= GETBIT(flags1, 13);

        fClipped	= GETBIT(flags2, 0);

        if (record.getRdPtr() >= record.getDataSize())
            return;

        record >> idList >> rgbTitle;

        name        = rgbTitle.value();
    }

    else
    {
        _UINT32            flags;
        XLSB::XLWideString irstName;
        record >> flags >> idField >> idList;

        fUserIns	= GETBIT(flags, 0);
        fFillDown	= GETBIT(flags, 1);
        fRowNums	= GETBIT(flags, 2);
        fClipped	= GETBIT(flags, 3);
        fIrstName	= GETBIT(flags, 4);

        if(fIrstName)
        {
            record >> irstName;
            name    = irstName.value();
        }
    }
}

void Qsif::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16			flags1 = 0, flags2 = 0;
		XLUnicodeString rgbTitle;

		SETBIT(flags1, 0, fUserIns)
		SETBIT(flags1, 1, fFillDown)
		SETBIT(flags1, 2, fSortDes)
		SETBITS(flags1, 3, 10, iSortKey)
		SETBIT(flags1, 11, fRowNums)
		SETBIT(flags1, 13, fSorted)

		SETBIT(flags2, 0, fClipped)

		record << frtHeaderOld << flags1 << flags2 << idField << idList;

		rgbTitle = name;

		record << rgbTitle;

	}

	else
	{
		_UINT32            flags = 0;
		XLSB::XLWideString irstName;

		SETBIT(flags, 0, fUserIns)
		SETBIT(flags, 1, fFillDown)
		SETBIT(flags, 2, fRowNums)
		SETBIT(flags, 3, fClipped)
		SETBIT(flags, 4, fIrstName)

		record << flags << idField << idList;

		if (fIrstName)
		{
			irstName = name;
			record << irstName;
		}
	}
}

} // namespace XLS

