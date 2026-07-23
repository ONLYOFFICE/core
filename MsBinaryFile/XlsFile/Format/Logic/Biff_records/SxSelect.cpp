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

#include "SxSelect.h"

namespace XLS
{

SxSelect::SxSelect()
{
	cClick = 0;
}


SxSelect::~SxSelect()
{
}


BaseObjectPtr SxSelect::clone()
{
	return BaseObjectPtr(new SxSelect(*this));
}


void SxSelect::readFields(CFRecord& record)
{
	short reserved1, flags;
	unsigned char reserved2;
	
	record >> reserved1 >> pnn >> reserved2 >> sxaxisAct >> iDimAct >> iLiStart >> iLiAct >> iLiMin >> iLiMax;
	record >> rwClick >> colClick >> rwClickPrev >> colClickPrev >> flags;
	
	cClick				= GETBITS(flags, 0, 4);
	fLabelOnly			= GETBIT(flags, 5);
	fDataOnly			= GETBIT(flags, 6);
	fToggleDataHeader	= GETBIT(flags, 7);
	fSelectionClick		= GETBIT(flags, 8);
	fExtendable			= GETBIT(flags, 9);
}

void SxSelect::writeFields(CFRecord& record)
{
    record.reserveNunBytes(2);
    record << pnn;
    record.reserveNunBytes(1);
    record << sxaxisAct << iDimAct << iLiStart << iLiAct << iLiMin << iLiMax;
    record << rwClick << colClick << rwClickPrev << colClickPrev;

    unsigned short flags = 0;
    SETBITS(flags, 0, 4, cClick)
    SETBIT(flags, 5, fLabelOnly)
    SETBIT(flags, 6, fDataOnly)
    SETBIT(flags, 7, fToggleDataHeader)
    SETBIT(flags, 8, fSelectionClick)
    SETBIT(flags, 9, fExtendable)
    record << flags;
}

} // namespace XLS

