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
#pragma once

#include "../../XlsxFormat/WritingElement.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_records/BiffRecord.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"

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

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

            static const XLS::ElementType	type = XLS::typeBeginSXView;

            BYTE         bVerSxMacro = 0;
            bool         fDisplayImmediateItems = false;
            bool         fEnableDataEd = false;
            bool         fDisableFList = false;
            bool         fReenterOnLoadOnce = false;
            bool         fNotViewCalculatedMembers = false;
            bool         fNotVisualTotals = false;
            bool         fPageMultipleItemLabel = false;
            //H - reserved1 (1 bit)
            bool         fHideDDData = false;
            //J - reserved2 (3 bits)
            bool         fHideDrillIndicators = false;
            bool         fPrintDrillIndicators = false;
            bool         fMemPropsInTips = false;
            bool         fNoPivotTips = false;
            BYTE         cIndentInc = 0;//7 bit
            bool         fNoHeaders = false;
            bool         fNoStencil = false;
            bool         fHideTotAnnotation = false;
            bool         fIncludeEmptyRw = false;
            bool         fIncludeEmptyCol = false;
            bool         fEnableWizard = false;
            bool         fEnableDrilldown = false;
            bool         fEnableFieldDialog = false;
            bool         fPreserveFormatting = false;
            bool         fAutoFormat = false;
            bool         fDisplayErrorString = false;
            bool         fDisplayNullString = false;
            bool         fAcrossPageLay = false;
            bool         fSubtotalHiddenPageItems = false;
            bool         fRwGrand = false;
            bool         fColGrand = false;
            bool         fPrintTitles = false;
            //f - unused (1 bit)
            bool         fRepeatItemsOnEachPrintedPage = false;
            bool         fMergeLabels = false;
            bool         fDisplayData = false;
            bool         fDisplayGrand = false;
            bool         fDisplayPageFieldStyle = false;
            bool         fDisplayTableStyle = false;
            bool         fDisplayVacateStyle = false;
            bool         ibitAtrNum = false;
            bool         ibitAtrFnt = false;
            bool         ibitAtrAlc = false;
            bool         ibitAtrBdr = false;
            bool         ibitAtrPat = false;
            bool         ibitAtrProt = false;
            bool         fDisplayTag = false;
            //u - reserved3 (1 bit)
            bool         fDefaultCompact = false;
            bool         fDefaultOutline = false;
            bool         fOutlineData = false;
            bool         fCompactData = false;
            bool         fNewDropZones = false;
            bool         fPublished = false;
            bool         fEmptyDisplayErrorString = false;
            bool         fEmptyDisplayNullString = true;
            bool         fTurnOffImmersive = false;
            bool         fSingleFilterPerField = false;
            bool         fUseRwHdrName = false;
            bool         fUseColHdrName = false;
            bool         fNonDefaultSortInFlist = false;
            //ι - reserved4 (1 bit)
            bool         fDontUseCustomLists = false;
            BYTE         sxaxis4Data = 0;
            BYTE         cWrapPage = 0;
            BYTE         bVerSxLastUpdated = 0;
            BYTE         bVerSxUpdateableMin = 0;
            _INT32       ipos4Data = -1;
            _UINT16      itblAutoFmt = 0;
            //reserved6 (2 bytes):
            _UINT32      dwCrtFmtId = 0;
            _UINT32      idCache = 0;
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

