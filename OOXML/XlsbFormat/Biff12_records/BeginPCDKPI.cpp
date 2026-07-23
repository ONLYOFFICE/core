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

#include "BeginPCDKPI.h"

using namespace XLS;

namespace XLSB
{

    BeginPCDKPI::BeginPCDKPI()
    {
    }

    BeginPCDKPI::~BeginPCDKPI()
    {
    }

    BaseObjectPtr BeginPCDKPI::clone()
    {
        return BaseObjectPtr(new BeginPCDKPI(*this));
    }

    void BeginPCDKPI::readFields(XLS::CFRecord& record)
    {
        BYTE flags;

        record >> flags;

        fLoadDispFld    = GETBIT(flags, 0);
        fLoadMeasGrp    = GETBIT(flags, 1);
        fLoadParent     = GETBIT(flags, 2);
        fLoadGoal       = GETBIT(flags, 3);
        fLoadStatus     = GETBIT(flags, 4);
        fLoadTrend      = GETBIT(flags, 5);
        fLoadWeight     = GETBIT(flags, 6);
        fLoadCurTimeMbr = GETBIT(flags, 7);

        record >> stUnique >> stCaption;

        if(fLoadDispFld)
            record >> stDispFolder;

        if(fLoadMeasGrp)
            record >> stMeasGrp;

        if(fLoadParent)
            record >> stParent;

        record >> stValue;

        if(fLoadGoal)
            record >> stGoal;

        if(fLoadStatus)
            record >> stStatus;

        if(fLoadTrend)
            record >> stTrend;

        if(fLoadWeight)
            record >> stWeight;

        if(fLoadCurTimeMbr)
            record >> stCurTimeMbr;
    }

	void BeginPCDKPI::writeFields(XLS::CFRecord& record)
	{
		BYTE flags = 0;

		SETBIT(flags, 0, fLoadDispFld)
		SETBIT(flags, 1, fLoadMeasGrp)
		SETBIT(flags, 2, fLoadParent)
		SETBIT(flags, 3, fLoadGoal)
		SETBIT(flags, 4, fLoadStatus)
		SETBIT(flags, 5, fLoadTrend)
		SETBIT(flags, 6, fLoadWeight)
		SETBIT(flags, 7, fLoadCurTimeMbr)

		record << flags;
		record << stUnique << stCaption;

		if (fLoadDispFld)
			record << stDispFolder;

		if (fLoadMeasGrp)
			record << stMeasGrp;

		if (fLoadParent)
			record << stParent;

		record << stValue;

		if (fLoadGoal)
			record << stGoal;

		if (fLoadStatus)
			record << stStatus;

		if (fLoadTrend)
			record << stTrend;

		if (fLoadWeight)
			record << stWeight;

		if (fLoadCurTimeMbr)
			record << stCurTimeMbr;
	}

} // namespace XLSB

