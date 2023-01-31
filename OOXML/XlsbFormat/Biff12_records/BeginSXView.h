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
#pragma once

#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BiffRecord.h"
#include "../../XlsxFormat/WritingElement.h"
#include "../Biff12_structures/XLWideString.h"

namespace XLSB
{
    // Logical representation of BrtBeginSXView record in BIFF12
    class BeginSXView: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginSXView)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginSXView)
        public:
            BeginSXView();
            ~BeginSXView();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record);

            static const XLS::ElementType	type = XLS::typeBeginSXView;

            BYTE         bVerSxMacro;
            bool         fDisplayImmediateItems;
            bool         fEnableDataEd;
            bool         fDisableFList;
            bool         fReenterOnLoadOnce;
            bool         fNotViewCalculatedMembers;
            bool         fNotVisualTotals;
            bool         fPageMultipleItemLabel;
            //H - reserved1 (1 bit)
            bool         fHideDDData;
            //J - reserved2 (3 bits)
            bool         fHideDrillIndicators;
            bool         fPrintDrillIndicators;
            bool         fMemPropsInTips;
            bool         fNoPivotTips;
            BYTE         cIndentInc;//7 bit
            bool         fNoHeaders;
            bool         fNoStencil;
            bool         fHideTotAnnotation;
            bool         fIncludeEmptyRw;
            bool         fIncludeEmptyCol;
            bool         fEnableWizard;
            bool         fEnableDrilldown;
            bool         fEnableFieldDialog;
            bool         fPreserveFormatting;
            bool         fAutoFormat;
            bool         fDisplayErrorString;
            bool         fDisplayNullString;
            bool         fAcrossPageLay;
            bool         fSubtotalHiddenPageItems;
            bool         fRwGrand;
            bool         fColGrand;
            bool         fPrintTitles;
            //f - unused (1 bit)
            bool         fRepeatItemsOnEachPrintedPage;
            bool         fMergeLabels;
            bool         fDisplayData;
            bool         fDisplayGrand;
            bool         fDisplayPageFieldStyle;
            bool         fDisplayTableStyle;
            bool         fDisplayVacateStyle;
            bool         ibitAtrNum;
            bool         ibitAtrFnt;
            bool         ibitAtrAlc;
            bool         ibitAtrBdr;
            bool         ibitAtrPat;
            bool         ibitAtrProt;
            bool         fDisplayTag;
            //u - reserved3 (1 bit)
            bool         fDefaultCompact;
            bool         fDefaultOutline;
            bool         fOutlineData;
            bool         fCompactData;
            bool         fNewDropZones;
            bool         fPublished;
            bool         fEmptyDisplayErrorString;
            bool         fEmptyDisplayNullString;
            bool         fTurnOffImmersive;
            bool         fSingleFilterPerField;
            bool         fUseRwHdrName;
            bool         fUseColHdrName;
            bool         fNonDefaultSortInFlist;
            //ι - reserved4 (1 bit)
            bool         fDontUseCustomLists;
            BYTE         sxaxis4Data;
            BYTE         cWrapPage;
            BYTE         bVerSxLastUpdated;
            BYTE         bVerSxUpdateableMin;
            _INT32       ipos4Data;
            _UINT16      itblAutoFmt;
            //reserved6 (2 bytes):
            _UINT32      dwCrtFmtId;
            _UINT32      idCache;
            XLWideString irstName;
            XLWideString irstData;
            XLWideString irstGrand;
            XLWideString irstErrorString;
            XLWideString irstNullString;
            XLWideString irstPageFieldStyle;
            XLWideString irstTableStyle;
            XLWideString irstVacateStyle;
            XLWideString irstTag;
            XLWideString irstColHdrName;
            XLWideString irstRwHdrName;
    };

} // namespace XLSB

