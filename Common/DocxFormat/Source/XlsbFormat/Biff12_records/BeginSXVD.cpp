/*
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

} // namespace XLSB

