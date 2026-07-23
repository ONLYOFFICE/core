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
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/FRTHeader.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/Color.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/Xnum.h"

namespace XLSB
{
    // Logical representation of BrtBeginSparklineGroup record in BIFF12
    class BeginSparklineGroup: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginSparklineGroup)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginSparklineGroup)
        public:
            BeginSparklineGroup();
            ~BeginSparklineGroup();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

            //static const XLS::ElementType	type = XLS::typeBeginSparklineGroup;

            FRTHeader   FRTheader;
            bool        fDateAxis;
            BYTE        fShowEmptyCellAsZero;
            bool        fMarkers;
            bool        fHigh;
            bool        fLow;
            bool        fFirst;
            bool        fLast;
            bool        fNegative;
            bool        fAxis;
            bool        fDisplayHidden;
            bool        fIndividualAutoMax;
            bool        fIndividualAutoMin;
            bool        fGroupAutoMax;
            bool        fGroupAutoMin;
            bool        fRTL;
            Color       brtcolorSeries;
            Color       brtcolorNegative;
            Color       brtcolorAxis;
            Color       brtcolorMarkers;
            Color       brtcolorFirst;
            Color       brtcolorLast;
            Color       brtcolorHigh;
            Color       brtcolorLow;
            XLS::Xnum   dManualMax;
            XLS::Xnum   dManualMin;
            XLS::Xnum   dLineWeight;
            _UINT32     isltype;
    };

} // namespace XLSB

