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

#include "TableStyle.h"
#include "../Biff_structures/BIFF12/XLWideString.h"

namespace XLS
{

TableStyle::TableStyle()
{
}


TableStyle::~TableStyle()
{
}


BaseObjectPtr TableStyle::clone()
{
	return BaseObjectPtr(new TableStyle(*this));
}

void TableStyle::readFields(CFRecord& record)
{
	_UINT16 flags;	

    if(record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> frtHeader >> flags;

        fIsPivot = GETBIT(flags, 1);
        fIsTable = GETBIT(flags, 2);

        _UINT16 cchName;
        record >> ctse >> cchName;
        LPWideStringNoCch	rgchName_;
        rgchName_.setSize(cchName);
        record >> rgchName_;

        rgchName = rgchName_.value();
    }
    else
    {
        record >> flags >> ctse;

        fIsPivot = GETBIT(flags, 1);
        fIsTable = GETBIT(flags, 2);

        XLSB::XLNullableWideString    strName;
        record >> strName;

        rgchName = strName.value();
    }
}

void TableStyle::writeFields(CFRecord& record)
{
	_UINT16 flags = 0;

	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		SETBIT(flags, 1, fIsPivot)
		SETBIT(flags, 2, fIsTable)
        frtHeader.rt = 0x088F;
		record << frtHeader << flags;

		_UINT16 cchName;

		record << ctse;

		LPWideStringNoCch	rgchName_(rgchName);
		cchName = rgchName_.getSize();

		record << cchName << rgchName_;
	}
	else
	{
		SETBIT(flags, 1, fIsPivot)
		SETBIT(flags, 2, fIsTable)

		record << flags << ctse;

		XLSB::XLNullableWideString    strName(rgchName);
		record << strName;
	}
}

} // namespace XLS

