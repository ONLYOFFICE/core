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
    // Logical representation of BrtBeginPCDHierarchy record in BIFF12
    class BeginPCDHierarchy: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginPCDHierarchy)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginPCDHierarchy)
        public:
            BeginPCDHierarchy();
            ~BeginPCDHierarchy();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

            //static const XLS::ElementType	type = XLS::typeBeginPCDHierarchy;

            bool            fMeasure = false;
            bool            fSet = false;
            bool            fAttributeHierarchy = false;
            bool            fMeasureHierarchy = false;
            bool            fOnlyOneField = false;
            bool            fTimeHierarchy = false;
            bool            fKeyAttributeHierarchy = false;
            bool            fAttributeMemberValueTypeKnown = false;
            bool            fUnbalancedRealKnown = false;
            bool            fUnbalancedReal = false;
            bool            fUnbalancedGroupKnown = false;
            bool            fUnbalancedGroup = false;
            bool            fHidden = false;
            _UINT32         cLevels = 0;
            _INT32          isetParent = -1;
            XLS::KPISets    iconSet;
            bool            fLoadDimUnq = false;
            bool            fLoadDefaultUnq = false;
            bool            fLoadAllUnq = false;
            bool            fLoadAllDisp = false;
            bool            fLoadDispFld = false;
            bool            fLoadMeasGrp = false;
            _UINT16         wAttributeMemberValueType = 0;
            XLWideString    stUnique;
            XLWideString    stCaption;
            XLWideString    stDimUnq;
            XLWideString    stDefaultUnq;
            XLWideString    stAllUnq;
            XLWideString    stAllDisp;
            XLWideString    stDispFld;
            XLWideString    stMeasGrp;
    };

} // namespace XLSB

