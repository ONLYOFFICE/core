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

#include "BeginSparklineGroup.h"

using namespace XLS;

namespace XLSB
{

    BeginSparklineGroup::BeginSparklineGroup()
    {
    }

    BeginSparklineGroup::~BeginSparklineGroup()
    {
    }

    BaseObjectPtr BeginSparklineGroup::clone()
    {
        return BaseObjectPtr(new BeginSparklineGroup(*this));
    }

    void BeginSparklineGroup::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags;
        record >> FRTheader >> flags;

        fDateAxis              = GETBIT(flags, 0);
        fShowEmptyCellAsZero   = GETBITS(flags, 1, 2);
        fMarkers               = GETBIT(flags, 3);
        fHigh                  = GETBIT(flags, 4);
        fLow                   = GETBIT(flags, 5);
        fFirst                 = GETBIT(flags, 6);
        fLast                  = GETBIT(flags, 7);
        fNegative              = GETBIT(flags, 8);
        fAxis                  = GETBIT(flags, 9);
        fDisplayHidden         = GETBIT(flags, 10);
        fIndividualAutoMax     = GETBIT(flags, 11);
        fIndividualAutoMin     = GETBIT(flags, 12);
        fGroupAutoMax          = GETBIT(flags, 13);
        fGroupAutoMin          = GETBIT(flags, 14);
        fRTL                   = GETBIT(flags, 15);

        brtcolorSeries.readFields(record);
        brtcolorNegative.readFields(record);
        brtcolorAxis.readFields(record);
        brtcolorMarkers.readFields(record);
        brtcolorFirst.readFields(record);
        brtcolorLast.readFields(record);
        brtcolorHigh.readFields(record);
        brtcolorLow.readFields(record);

        record >> dManualMax >> dManualMin >> dLineWeight >> isltype;
    }

	void BeginSparklineGroup::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;

		SETBIT(flags, 0, fDateAxis)
		SETBITS(flags, 1, 2, fShowEmptyCellAsZero)
		SETBIT(flags, 3, fMarkers)
		SETBIT(flags, 4, fHigh)
		SETBIT(flags, 5, fLow)
		SETBIT(flags, 6, fFirst)
		SETBIT(flags, 7, fLast)
		SETBIT(flags, 8, fNegative)
		SETBIT(flags, 9, fAxis)
		SETBIT(flags, 10, fDisplayHidden)
		SETBIT(flags, 11, fIndividualAutoMax)
		SETBIT(flags, 12, fIndividualAutoMin)
		SETBIT(flags, 13, fGroupAutoMax)
		SETBIT(flags, 14, fGroupAutoMin)
		SETBIT(flags, 15, fRTL)

		record << FRTheader << flags;

		brtcolorSeries.writeFields(record);
		brtcolorNegative.writeFields(record);
		brtcolorAxis.writeFields(record);
		brtcolorMarkers.writeFields(record);
		brtcolorFirst.writeFields(record);
		brtcolorLast.writeFields(record);
		brtcolorHigh.writeFields(record);
		brtcolorLow.writeFields(record);

		record << dManualMax << dManualMin << dLineWeight << isltype;
	}

} // namespace XLSB

