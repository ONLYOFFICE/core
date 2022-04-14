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

#include "BeginSXView.h"

using namespace XLS;

namespace XLSB
{

    BeginSXView::BeginSXView()
    {
    }

    BeginSXView::~BeginSXView()
    {
    }

    BaseObjectPtr BeginSXView::clone()
    {
        return BaseObjectPtr(new BeginSXView(*this));
    }

    void BeginSXView::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags1, flags2, flags3;

        record >> flags1 >> flags2 >> flags3;

        bVerSxMacro                   = GETBITS(flags1, 0, 7);
        fDisplayImmediateItems        = GETBIT(flags1, 8);
        fEnableDataEd                 = GETBIT(flags1, 9);
        fDisableFList                 = GETBIT(flags1, 10);
        fReenterOnLoadOnce            = GETBIT(flags1, 11);
        fNotViewCalculatedMembers     = GETBIT(flags1, 12);
        fNotVisualTotals              = GETBIT(flags1, 13);
        fPageMultipleItemLabel        = GETBIT(flags1, 14);
        fHideDDData                   = GETBIT(flags1, 16);
        fHideDrillIndicators          = GETBIT(flags1, 20);
        fPrintDrillIndicators         = GETBIT(flags1, 21);
        fMemPropsInTips               = GETBIT(flags1, 22);
        fNoPivotTips                  = GETBIT(flags1, 23);
        cIndentInc                    = GETBITS(flags1, 24, 30);
        fNoHeaders                    = GETBIT(flags1, 31);

        fNoStencil                    = GETBIT(flags2, 0);
        fHideTotAnnotation            = GETBIT(flags2, 1);
        fIncludeEmptyRw               = GETBIT(flags2, 2);
        fIncludeEmptyCol              = GETBIT(flags2, 3);
        fEnableWizard                 = GETBIT(flags2, 4);
        fEnableDrilldown              = GETBIT(flags2, 5);
        fEnableFieldDialog            = GETBIT(flags2, 6);
        fPreserveFormatting           = GETBIT(flags2, 7);
        fAutoFormat                   = GETBIT(flags2, 8);
        fDisplayErrorString           = GETBIT(flags2, 9);
        fDisplayNullString            = GETBIT(flags2, 10);
        fAcrossPageLay                = GETBIT(flags2, 11);
        fSubtotalHiddenPageItems      = GETBIT(flags2, 12);
        fRwGrand                      = GETBIT(flags2, 13);
        fColGrand                     = GETBIT(flags2, 14);
        fPrintTitles                  = GETBIT(flags2, 15);
        fRepeatItemsOnEachPrintedPage = GETBIT(flags2, 17);
        fMergeLabels                  = GETBIT(flags2, 18);
        fDisplayData                  = GETBIT(flags2, 19);
        fDisplayGrand                 = GETBIT(flags2, 20);
        fDisplayPageFieldStyle        = GETBIT(flags2, 21);
        fDisplayTableStyle            = GETBIT(flags2, 22);
        fDisplayVacateStyle           = GETBIT(flags2, 23);
        ibitAtrNum                    = GETBIT(flags2, 24);
        ibitAtrFnt                    = GETBIT(flags2, 25);
        ibitAtrAlc                    = GETBIT(flags2, 26);
        ibitAtrBdr                    = GETBIT(flags2, 27);
        ibitAtrPat                    = GETBIT(flags2, 28);
        ibitAtrProt                   = GETBIT(flags2, 29);
        fDisplayTag                   = GETBIT(flags2, 30);

        fDefaultCompact               = GETBIT(flags3, 0);
        fDefaultOutline               = GETBIT(flags3, 1);
        fOutlineData                  = GETBIT(flags3, 2);
        fCompactData                  = GETBIT(flags3, 3);
        fNewDropZones                 = GETBIT(flags3, 4);
        fPublished                    = GETBIT(flags3, 5);
        fEmptyDisplayErrorString      = GETBIT(flags3, 6);
        fEmptyDisplayNullString       = GETBIT(flags3, 7);
        fTurnOffImmersive             = GETBIT(flags3, 8);
        fSingleFilterPerField         = GETBIT(flags3, 9);
        fUseRwHdrName                 = GETBIT(flags3, 10);
        fUseColHdrName                = GETBIT(flags3, 11);
        fNonDefaultSortInFlist        = GETBIT(flags3, 12);
        fDontUseCustomLists           = GETBIT(flags3, 14);

        record >> sxaxis4Data >> cWrapPage >> bVerSxLastUpdated >> bVerSxUpdateableMin;
        record >> ipos4Data >> itblAutoFmt;

        record.skipNunBytes(2);

        record >> dwCrtFmtId >> idCache >> irstName;

        if(fDisplayData)
            record >> irstData;

        if(fDisplayGrand)
            record >> irstGrand;

        if(!fEmptyDisplayErrorString)
            record >> irstErrorString;

        if(!fEmptyDisplayNullString)
            record >> irstNullString;

        if(fDisplayPageFieldStyle)
            record >> irstPageFieldStyle;

        if(fDisplayTableStyle)
            record >> irstTableStyle;

        if(fDisplayVacateStyle)
            record >> irstVacateStyle;

        if(fDisplayTag)
            record >> irstTag;

        if(fUseColHdrName)
            record >> irstColHdrName;

        if(fUseRwHdrName)
            record >> irstRwHdrName;

    }

} // namespace XLSB

