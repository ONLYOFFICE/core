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

#include "Formula.h"

namespace XLS
{

Formula::Formula()
:	formula(false)
{
}


Formula::~Formula()
{
}


BaseObjectPtr Formula::clone()
{
	return BaseObjectPtr(new Formula(*this));
}

void Formula::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> cell >> val >> flags;
	
	fAlwaysCalc		= GETBIT(flags, 0);
	fFill			= GETBIT(flags, 2);
	fShrFmla		= GETBIT(flags, 3);
	fClearErrors	= GETBIT(flags, 5);
	
	if (bBiff_3_4 && record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
	}
	else
	{
		_UINT32 chn = 0;
		record >> chn; // cache
	}
	
	formula.load(record);
}

void Formula::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fAlwaysCalc);
    SETBIT(flags, 2, fFill);
    SETBIT(flags, 3, fShrFmla);
    SETBIT(flags, 5, fClearErrors);
    record << cell << val << flags;
    record.reserveNunBytes(4);
    formula.save(record);
}

const CellRef Formula::getLocation() const
{
	return cell.getLocation();
}

Formula_BIFF3::Formula_BIFF3()
{
	bBiff_3_4 = true;
}

Formula_BIFF3::~Formula_BIFF3()
{}

BaseObjectPtr Formula_BIFF3::clone()
{
	return BaseObjectPtr(new Formula_BIFF3(*this));
}
Formula_BIFF4::Formula_BIFF4()
{
	bBiff_3_4 = true;
}

Formula_BIFF4::~Formula_BIFF4()
{}

BaseObjectPtr Formula_BIFF4::clone()
{
	return BaseObjectPtr(new Formula_BIFF4(*this));
}

} // namespace XLS

