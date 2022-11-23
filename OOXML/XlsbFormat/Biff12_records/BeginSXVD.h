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
#include  "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/SxAxis.h"

namespace XLSB
{
    // Logical representation of BrtBeginSXVD record in BIFF12
    class BeginSXVD: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginSXVD)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginSXVD)
        public:
            BeginSXVD();
            ~BeginSXVD();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record);

            //static const XLS::ElementType	type = XLS::typeBeginSXVD;

            XLS::SXAxis  sxaxis;
            bool         fDefault;
            bool         fSum;
            bool         fCounta;
            bool         fAverage;
            bool         fMax;
            bool         fMin;
            bool         fProduct;
            bool         fCount;
            bool         fStdev;
            bool         fStdevp;
            bool         fVar;
            bool         fVarp;
            //M - reserved1 (4 bits)
            bool         fDrilledLevel;
            bool         fHideDD;
            bool         fHiddenLvl;
            bool         fUseMemPropCaption;
            bool         fCompact;
            bool         fDisplayName;
            bool         fDisplaySub;
            bool         fTensorSort;
            _UINT32      ifmt;
            bool         fDragToRow;
            bool         fDragToColumn;
            bool         fDragToPage;
            bool         fDragToHide;
            bool         fDragToData;
            bool         fShowAllItems;
            bool         fOutline;
            bool         fInsertBlankRow;
            bool         fSubtotalAtTop;
            bool         fServerBased;
            //f - reserved2 (1 bit)
            bool         fPageBreaksBetweenItems;
            bool         fAutoSort;
            bool         fAscendSort;
            bool         fAutoShow;
            bool         fTopAutoShow;
            bool         fHideNewItems;
            bool         fHasAdvFilter;
            bool         fFilterInclusive;
            bool         fEnableMultiplePageItems;
            bool         fNotAutoSortDft;
            bool         fMemPropDisplayInReport;
            bool         fMemPropDisplayInTip;
            bool         fMemPropDisplayInCaption;
            bool         fItemsDrilledByDefault;
            //unused (7 bits)
            _INT32       citmAutoShow;
            _INT32       isxdiAutoShow;
            XLWideString irstName;
            XLWideString irstSub;
            XLWideString irstMemberPropertyCaption;

    };

} // namespace XLSB

