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

