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

#include "BeginSXVD.h"

using namespace XLS;

namespace XLSB
{

    BeginSXVD::BeginSXVD()
    {
    }

    BeginSXVD::~BeginSXVD()
    {
    }

    BaseObjectPtr BeginSXVD::clone()
    {
        return BaseObjectPtr(new BeginSXVD(*this));
    }

    void BeginSXVD::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags1, flags2;

        record >> flags1 >> ifmt >> flags2;

        sxaxis.bRw                    = GETBIT(flags1, 0);
        sxaxis.bCol                   = GETBIT(flags1, 1);
        sxaxis.bPage                  = GETBIT(flags1, 2);
        sxaxis.bData                  = GETBIT(flags1, 3);
        fDefault                      = GETBIT(flags1, 8);
        fSum                          = GETBIT(flags1, 9);
        fCounta                       = GETBIT(flags1, 10);
        fAverage                      = GETBIT(flags1, 11);
        fMax                          = GETBIT(flags1, 12);
        fMin                          = GETBIT(flags1, 13);
        fProduct                      = GETBIT(flags1, 14);
        fCount                        = GETBIT(flags1, 15);
        fStdev                        = GETBIT(flags1, 16);
        fStdevp                       = GETBIT(flags1, 17);
        fVar                          = GETBIT(flags1, 18);
        fVarp                         = GETBIT(flags1, 19);
        fDrilledLevel                 = GETBIT(flags1, 24);
        fHideDD                       = GETBIT(flags1, 25);
        fHiddenLvl                    = GETBIT(flags1, 26);
        fUseMemPropCaption            = GETBIT(flags1, 27);
        fCompact                      = GETBIT(flags1, 28);
        fDisplayName                  = GETBIT(flags1, 29);
        fDisplaySub                   = GETBIT(flags1, 30);
        fTensorSort                   = GETBIT(flags1, 31);

        fDragToRow                    = GETBIT(flags2, 0);
        fDragToColumn                 = GETBIT(flags2, 1);
        fDragToPage                   = GETBIT(flags2, 2);
        fDragToHide                   = GETBIT(flags2, 3);
        fDragToData                   = GETBIT(flags2, 4);
        fShowAllItems                 = GETBIT(flags2, 5);
        fOutline                      = GETBIT(flags2, 6);
        fInsertBlankRow               = GETBIT(flags2, 7);
        fSubtotalAtTop                = GETBIT(flags2, 8);
        fServerBased                  = GETBIT(flags2, 9);
        fPageBreaksBetweenItems       = GETBIT(flags2, 11);
        fAutoSort                     = GETBIT(flags2, 12);
        fAscendSort                   = GETBIT(flags2, 13);
        fAutoShow                     = GETBIT(flags2, 14);
        fTopAutoShow                  = GETBIT(flags2, 15);
        fHideNewItems                 = GETBIT(flags2, 16);
        fHasAdvFilter                 = GETBIT(flags2, 17);
        fFilterInclusive              = GETBIT(flags2, 18);
        fEnableMultiplePageItems      = GETBIT(flags2, 19);
        fNotAutoSortDft               = GETBIT(flags2, 20);
        fMemPropDisplayInReport       = GETBIT(flags2, 21);
        fMemPropDisplayInTip          = GETBIT(flags2, 22);
        fMemPropDisplayInCaption      = GETBIT(flags2, 23);
        fItemsDrilledByDefault        = GETBIT(flags2, 24);

        record >> citmAutoShow >> isxdiAutoShow;

        if(fDisplayName)
            record >> irstName;

        if(fDisplaySub)
            record >> irstSub;

        if(fUseMemPropCaption)
            record >> irstMemberPropertyCaption;
    }

	void BeginSXVD::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags1 = 0, flags2 = 0;

		 SETBIT(flags1, 0, sxaxis.bRw)
		 SETBIT(flags1, 1, sxaxis.bCol)
		 SETBIT(flags1, 2, sxaxis.bPage)
		 SETBIT(flags1, 3, sxaxis.bData)
		 SETBIT(flags1, 8, fDefault)
		 SETBIT(flags1, 9, fSum)
		 SETBIT(flags1, 10, fCounta)
		 SETBIT(flags1, 11, fAverage)
		 SETBIT(flags1, 12, fMax)
		 SETBIT(flags1, 13, fMin)
		 SETBIT(flags1, 14, fProduct)
		 SETBIT(flags1, 15, fCount)
		 SETBIT(flags1, 16, fStdev)
		 SETBIT(flags1, 17, fStdevp)
		 SETBIT(flags1, 18, fVar)
		 SETBIT(flags1, 19, fVarp)
		 SETBIT(flags1, 24, fDrilledLevel)
		 SETBIT(flags1, 25, fHideDD)
		 SETBIT(flags1, 26, fHiddenLvl)
		 SETBIT(flags1, 27, fUseMemPropCaption)
		 SETBIT(flags1, 28, fCompact)
		 SETBIT(flags1, 29, fDisplayName)
		 SETBIT(flags1, 30, fDisplaySub)
		 SETBIT(flags1, 31, fTensorSort)

		 SETBIT(flags2, 0, fDragToRow)
		 SETBIT(flags2, 1, fDragToColumn)
		 SETBIT(flags2, 2, fDragToPage)
		 SETBIT(flags2, 3, fDragToHide)
		 SETBIT(flags2, 4, fDragToData)
		 SETBIT(flags2, 5, fShowAllItems)
		 SETBIT(flags2, 6, fOutline)
		 SETBIT(flags2, 7, fInsertBlankRow)
		 SETBIT(flags2, 8, fSubtotalAtTop)
		 SETBIT(flags2, 9, fServerBased)
		 SETBIT(flags2, 11, fPageBreaksBetweenItems)
		 SETBIT(flags2, 12, fAutoSort)
		 SETBIT(flags2, 13, fAscendSort)
		 SETBIT(flags2, 14, fAutoShow)
		 SETBIT(flags2, 15, fTopAutoShow)
		 SETBIT(flags2, 16, fHideNewItems)
		 SETBIT(flags2, 17, fHasAdvFilter)
		 SETBIT(flags2, 18, fFilterInclusive)
		 SETBIT(flags2, 19, fEnableMultiplePageItems)
		 SETBIT(flags2, 20, fNotAutoSortDft)
		 SETBIT(flags2, 21, fMemPropDisplayInReport)
		 SETBIT(flags2, 22, fMemPropDisplayInTip)
		 SETBIT(flags2, 23, fMemPropDisplayInCaption)
		 SETBIT(flags2, 24, fItemsDrilledByDefault)

		record << flags1 << ifmt << flags2;
		record << citmAutoShow << isxdiAutoShow;

		if (fDisplayName)
			record << irstName;

		if (fDisplaySub)
			record << irstSub;

		if (fUseMemPropCaption)
			record << irstMemberPropertyCaption;
	}

} // namespace XLSB

