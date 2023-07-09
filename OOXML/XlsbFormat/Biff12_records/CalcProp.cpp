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

#include "CalcProp.h"

using namespace XLS;

namespace XLSB
{

    CalcProp::CalcProp()
    {
    }

    CalcProp::~CalcProp()
    {
    }

    BaseObjectPtr CalcProp::clone()
    {
        return BaseObjectPtr(new CalcProp(*this));
    }

    void CalcProp::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags;
        record >> recalcID >> fAutoRecalc >> cCalcCount >> xnumDelta >> cUserThreadCount;
        record >> flags;

        fFullCalcOnLoad     = GETBIT(flags, 0);
        fRefA1              = GETBIT(flags, 1);
        fIter               = GETBIT(flags, 2);
        fFullPrec           = GETBIT(flags, 3);

        fSomeUncalced       = GETBIT(flags, 4);
        fSaveRecalc         = GETBIT(flags, 5);
        fMTREnabled         = GETBIT(flags, 6);
        fUserSetThreadCount = GETBIT(flags, 7);
        fNoDeps             = GETBIT(flags, 8);

    }

	void CalcProp::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags = 0;
		
		SETBIT(flags, 0, fFullCalcOnLoad)
    	SETBIT(flags, 1, fRefA1)
    	SETBIT(flags, 2, fIter)
		SETBIT(flags, 3, fFullPrec)
    	SETBIT(flags, 4, fSomeUncalced)
		SETBIT(flags, 5, fSaveRecalc)
		SETBIT(flags, 6, fMTREnabled)
		SETBIT(flags, 7, fUserSetThreadCount)
    	SETBIT(flags, 8, fNoDeps)

		record << recalcID << fAutoRecalc << cCalcCount << xnumDelta << cUserThreadCount << flags;
	}

} // namespace XLSB

