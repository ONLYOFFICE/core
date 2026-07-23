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

#include "WbProp.h"

using namespace XLS;

namespace XLSB
{

    WbProp::WbProp()
    {
    }

    WbProp::~WbProp()
    {
    }

    BaseObjectPtr WbProp::clone()
    {
        return BaseObjectPtr(new WbProp(*this));
    }

    void WbProp::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags;
        record >> flags;

        f1904                    = GETBIT(flags, 0);
        fHideBorderUnselLists	 = GETBIT(flags, 2);
        fFilterPrivacy           = GETBIT(flags, 3);
        fBuggedUserAboutSolution = GETBIT(flags, 4);

        fShowInkAnnotation       = GETBIT(flags, 5);
        fBackup                  = GETBIT(flags, 6);
        fNoSaveSup               = GETBIT(flags, 7);
        grbitUpdateLinks         = GETBITS(flags, 8, 9);
        fHidePivotTableFList     = GETBIT(flags, 10);
        fPublishedBookItems      = GETBIT(flags, 11);
        fCheckCompat             = GETBIT(flags, 12);
        mdDspObj                 = GETBITS(flags, 13, 14);
        fShowPivotChartFilter    = GETBIT(flags, 15);
        fAutoCompressPictures    = GETBIT(flags, 16);
        fRefreshAll              = GETBIT(flags, 17);

        record >> dwThemeVersion;
        record >> strName;
    }

	void WbProp::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		SETBIT(flags, 0, f1904)
		SETBIT(flags, 2, fHideBorderUnselLists)
		SETBIT(flags, 3, fFilterPrivacy)
		SETBIT(flags, 4, fBuggedUserAboutSolution)
		SETBIT(flags, 5, fShowInkAnnotation)
		SETBIT(flags, 6, fBackup)
		SETBIT(flags, 7, fNoSaveSup)
		SETBITS(flags, 8, 9, grbitUpdateLinks)
		SETBIT(flags, 10, fHidePivotTableFList)
		SETBIT(flags, 11, fPublishedBookItems)
		SETBIT(flags, 12, fCheckCompat)
		SETBITS(flags, 13, 14, mdDspObj);
		SETBIT(flags, 15, fShowPivotChartFilter)
		SETBIT(flags, 16, fAutoCompressPictures)
		SETBIT(flags, 17, fRefreshAll)

		record << flags;
		record << dwThemeVersion;
		record << strName;
	}

} // namespace XLSB

