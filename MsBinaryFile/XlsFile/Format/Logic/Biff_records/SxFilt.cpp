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

#include "SxFilt.h"

namespace XLS
{

SxFilt::SxFilt()
{
	isxvd = iDim = cisxvi = grbitSbt = 0;
}


SxFilt::~SxFilt()
{
}

BaseObjectPtr SxFilt::clone()
{
	return BaseObjectPtr(new SxFilt(*this));
}


void SxFilt::readFields(CFRecord& record)
{
	unsigned short flags1, flags2;

	record >> flags1 >> flags2 >> grbitSbt >> cisxvi;
	
	sxaxisRw	= GETBIT(flags1, 0);
	sxaxisCol	= GETBIT(flags1, 1);
	sxaxisPage	= GETBIT(flags1, 2);
	sxaxisData	= GETBIT(flags1, 3);
	iDim		= GETBITS(flags1, 6, 15);

	isxvd		= GETBITS(flags2, 0, 9);
	fSelected	= GETBIT(flags2, 10);
}

void SxFilt::writeFields(CFRecord& record)
{
    unsigned short flags1 = 0, flags2 = 0;
    SETBIT(flags1, 0, sxaxisRw)
    SETBIT(flags1, 1, sxaxisCol)
    SETBIT(flags1, 2, sxaxisPage)
    SETBIT(flags1, 3, sxaxisData)
    SETBITS(flags1, 6, 15, iDim)

    SETBITS(flags2, 0, 9, isxvd)
    SETBIT(flags2, 10, fSelected)
    record << flags1 << flags2 << grbitSbt << cisxvi;
}

} // namespace XLS

