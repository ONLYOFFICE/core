﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

