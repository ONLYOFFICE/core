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
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12//RelID.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/XLWideString.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/Xnum.h"

namespace XLSB
{
    // Logical representation of BrtBeginPivotCacheDef record in BIFF12
    class BeginPivotCacheDef: public XLS::BiffRecord
    {
            BIFF_RECORD_DEFINE_TYPE_INFO(BeginPivotCacheDef)
            BASE_OBJECT_DEFINE_CLASS_NAME(BeginPivotCacheDef)
        public:
            BeginPivotCacheDef();
            ~BeginPivotCacheDef();

            XLS::BaseObjectPtr clone();

            void readFields(XLS::CFRecord& record) override;
			void writeFields(XLS::CFRecord& record) override;

            static const XLS::ElementType	type = XLS::typeBeginPivotCacheDef;

            BYTE                    bVerCacheLastRefresh;
            BYTE                    bVerCacheRefreshableMin;
            BYTE                    bVerCacheCreated;
            bool                    fSaveData;
            bool                    fInvalid;
            bool                    fRefreshOnLoad;
            bool                    fOptimizeCache;
            bool                    fEnableRefresh;
            bool                    fBackgroundQuery;
            bool                    fUpgradeOnRefresh;
            bool                    fSheetData;
            _INT32                  citmGhostMax;
            XLS::Xnum               xnumRefreshedDate;
            bool                    fLoadRefreshedWho;
            bool                    fLoadRelIDRecords;
            bool                    fSupportSubquery;
            bool                    fSupportAttribDrill;
            _UINT32                 cRecords;
            XLWideString            stRefreshedWho;
            RelID                   stRelIDRecords;
    };

} // namespace XLSB

