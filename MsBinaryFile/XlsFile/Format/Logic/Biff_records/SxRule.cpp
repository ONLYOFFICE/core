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

#include "SxRule.h"

namespace XLS
{

SxRule::SxRule()
{
}

SxRule::~SxRule()
{
}

BaseObjectPtr SxRule::clone()
{
	return BaseObjectPtr(new SxRule(*this));
}


void SxRule::readFields(CFRecord& record)
{
	unsigned short flags, reserved;
	record >> iDim >> isxvd >> flags >> reserved >> csxFilt >> irwFirst >> irwLast >> icolFirst >> icolLast;

	sxaxisRw		= GETBIT(flags, 0);
	sxaxisCol		= GETBIT(flags, 1);
	sxaxisPage		= GETBIT(flags, 2);
	sxaxisData		= GETBIT(flags, 3);
	sxrType			= GETBITS(flags, 4, 7);
	fPart			= GETBIT(flags, 8);
	fDataOnly		= GETBIT(flags, 9);
	fLabelOnly		= GETBIT(flags, 10);
	fGrandRw		= GETBIT(flags, 11);
	fGrandCol		= GETBIT(flags, 12);
	fGrandRwSav		= GETBIT(flags, 13);
	fCacheBased		= GETBIT(flags, 14);
	fGrandColSav	= GETBIT(flags, 15);
}

void SxRule::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, sxaxisRw)
    SETBIT(flags, 1, sxaxisCol)
    SETBIT(flags, 2, sxaxisPage)
    SETBIT(flags, 3, sxaxisData)
    SETBITS(flags, 4, 7, sxrType)
    SETBIT(flags, 8, fPart)
    SETBIT(flags, 9, fDataOnly)
    SETBIT(flags, 10, fLabelOnly)
    SETBIT(flags, 11, fGrandRw)
    SETBIT(flags, 12, fGrandCol)
    SETBIT(flags, 13, fGrandRwSav)
    SETBIT(flags, 14, fCacheBased)
    SETBIT(flags, 15, fGrandColSav)

    record << iDim << isxvd << flags;
    record.reserveNunBytes(2);
    record << csxFilt;
    if(fPart)
    {
        record << irwFirst << irwLast << icolFirst << icolLast;
    }

}

} // namespace XLS

