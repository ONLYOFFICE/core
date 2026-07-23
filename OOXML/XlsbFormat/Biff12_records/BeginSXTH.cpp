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

#include "BeginSXTH.h"

using namespace XLS;

namespace XLSB
{

    BeginSXTH::BeginSXTH()
    {
    }

    BeginSXTH::~BeginSXTH()
    {
    }

    BaseObjectPtr BeginSXTH::clone()
    {
        return BaseObjectPtr(new BeginSXTH(*this));
    }

    void BeginSXTH::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags1;
        _UINT16 flags2;
        record >> flags1 >> flags2;

        fOutlineMode                = GETBIT(flags1, 0);
        fEnableMultiplePageItems    = GETBIT(flags1, 1);
        fSubtotalAtTop              = GETBIT(flags1, 2);
        fDontShowFList              = GETBIT(flags1, 3);
        fDragToRow                  = GETBIT(flags1, 4);
        fDragToColumn               = GETBIT(flags1, 5);
        fDragToPage                 = GETBIT(flags1, 6);
        fDragToHide                 = GETBIT(flags1, 7);
        fDragToData                 = GETBIT(flags1, 8);
        fFilterInclusive            = GETBIT(flags1, 9);

        fLoadCap                    = GETBIT(flags2, 0);

        if(fLoadCap)
            record >> irstCaption;
    }

	void BeginSXTH::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags1 = 0;
		_UINT16 flags2 = 0;

		 SETBIT(flags1, 0, fOutlineMode)
		 SETBIT(flags1, 1, fEnableMultiplePageItems)
		 SETBIT(flags1, 2, fSubtotalAtTop)
		 SETBIT(flags1, 3, fDontShowFList)
		 SETBIT(flags1, 4, fDragToRow)
		 SETBIT(flags1, 5, fDragToColumn)
		 SETBIT(flags1, 6, fDragToPage)
		 SETBIT(flags1, 7, fDragToHide)
		 SETBIT(flags1, 8, fDragToData)
		 SETBIT(flags1, 9, fFilterInclusive)

		 SETBIT(flags2, 0, fLoadCap)

		record << flags1 << flags2;

		if (fLoadCap)
			record << irstCaption;
	}

} // namespace XLSB

