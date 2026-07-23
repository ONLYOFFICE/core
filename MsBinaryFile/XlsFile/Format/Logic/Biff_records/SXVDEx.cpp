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

#include "SXVDEx.h"

namespace XLS
{

SXVDEx::SXVDEx()
{
}

SXVDEx::~SXVDEx()
{
}

BaseObjectPtr SXVDEx::clone()
{
	return BaseObjectPtr(new SXVDEx(*this));
}


void SXVDEx::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	unsigned short	flags1;
	unsigned char	flags2;

	record >> flags1 >> flags2 >> citmAutoShow >> isxdiAutoSort >> isxdiAutoShow >> ifmt;
	ifmt = global_info->RegisterNumFormat(ifmt, L""); // return update
	
	fShowAllItems	= GETBIT(flags1, 0);
	fDragToRow		= GETBIT(flags1, 1);
	fDragToColumn	= GETBIT(flags1, 2);
	fDragToPage		= GETBIT(flags1, 3);
	fDragToHide		= GETBIT(flags1, 4);
	fNotDragToData	= GETBIT(flags1, 5);
	
	fServerBased	= GETBIT(flags1, 7);

	fAutoSort				= GETBIT(flags1, 9);
	fAscendSort				= GETBIT(flags1, 10);
	fAutoShow				= GETBIT(flags1, 11);
	fTopAutoShow			= GETBIT(flags1, 12);
	fCalculatedField		= GETBIT(flags1, 13);
	fPageBreaksBetweenItems	= GETBIT(flags1, 14);
	fHideNewItems			= GETBIT(flags1, 15);
	
	fOutline		= GETBIT(flags2, 5);
	fInsertBlankRow	= GETBIT(flags2, 6);
	fSubtotalAtTop	= GETBIT(flags2, 7);

	_UINT32	reserved1, reserved2;
	record >> cchSubName >> reserved1 >> reserved2;

	if (cchSubName > 0 && cchSubName < 0xffff)
	{
		stSubName.setSize(cchSubName);
		record >> stSubName;
	}
}

void SXVDEx::writeFields(CFRecord& record)
{
    unsigned short	flags1 = 0;
    unsigned char	flags2 = 0;
    SETBIT(flags1, 0, fShowAllItems)
    SETBIT(flags1, 1, fDragToRow)
    SETBIT(flags1, 2, fDragToColumn)
    SETBIT(flags1, 3, fDragToPage)
    SETBIT(flags1, 4, fDragToHide)
    SETBIT(flags1, 5, fNotDragToData)

    SETBIT(flags1, 7, fServerBased)

    SETBIT(flags1, 9, fAutoSort)
    SETBIT(flags1, 10, fAscendSort)
    SETBIT(flags1, 11, fAutoShow)
    SETBIT(flags1, 12, fTopAutoShow)
    SETBIT(flags1, 13, fCalculatedField)
    SETBIT(flags1, 14, fPageBreaksBetweenItems)
    SETBIT(flags1, 15, fHideNewItems)

    SETBIT(flags2, 5, fOutline)
    SETBIT(flags2, 6, fInsertBlankRow)
    SETBIT(flags2, 7, fSubtotalAtTop)
    record << flags1 << flags2 << citmAutoShow << isxdiAutoSort << isxdiAutoShow << ifmt;
    cchSubName = stSubName.getSize();
    if (cchSubName > 0 && cchSubName < 0xffff)
    {
        record << cchSubName;
        record.reserveNunBytes(8);
        record << stSubName;
    }
	else
	{
		cchSubName = 0xffff;
		record << cchSubName;
		record.reserveNunBytes(8);
	}
}

} // namespace XLS

