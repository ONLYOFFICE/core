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

#include "BeginPivotCacheDef.h"

using namespace XLS;

namespace XLSB
{

    BeginPivotCacheDef::BeginPivotCacheDef()
    {
    }

    BeginPivotCacheDef::~BeginPivotCacheDef()
    {
    }

    BaseObjectPtr BeginPivotCacheDef::clone()
    {
        return BaseObjectPtr(new BeginPivotCacheDef(*this));
    }

    void BeginPivotCacheDef::readFields(XLS::CFRecord& record)
    {
        BYTE flags1, flags2;

        record >> bVerCacheLastRefresh >> bVerCacheRefreshableMin >> bVerCacheCreated >> flags1;

        fSaveData           = GETBIT(flags1, 0);
        fInvalid            = GETBIT(flags1, 1);
        fRefreshOnLoad      = GETBIT(flags1, 2);
        fOptimizeCache      = GETBIT(flags1, 3);
        fEnableRefresh      = GETBIT(flags1, 4);
        fBackgroundQuery    = GETBIT(flags1, 5);
        fUpgradeOnRefresh   = GETBIT(flags1, 6);
        fSheetData          = GETBIT(flags1, 7);

        record >> citmGhostMax >> xnumRefreshedDate >> flags2;

        fLoadRefreshedWho   = GETBIT(flags2, 0);
        fLoadRelIDRecords   = GETBIT(flags2, 1);
        fSupportSubquery    = GETBIT(flags2, 2);
        fSupportAttribDrill = GETBIT(flags2, 3);

        record >> cRecords;

        if(fLoadRefreshedWho)
            record >> stRefreshedWho;

        if(fLoadRelIDRecords)
            record >> stRelIDRecords;

        if(!fLoadRefreshedWho)
            record.skipNunBytes(4);

    }

	void BeginPivotCacheDef::writeFields(XLS::CFRecord& record)
	{
		BYTE flags1 = 0, flags2 = 0;

		record << bVerCacheLastRefresh << bVerCacheRefreshableMin << bVerCacheCreated;

		SETBIT(flags1, 0, fSaveData)
		SETBIT(flags1, 1, fInvalid)
		SETBIT(flags1, 2, fRefreshOnLoad)
		SETBIT(flags1, 3, fOptimizeCache)
		SETBIT(flags1, 4, fEnableRefresh)
		SETBIT(flags1, 5, fBackgroundQuery)
		SETBIT(flags1, 6, fUpgradeOnRefresh)
		SETBIT(flags1, 7, fSheetData)

		record << flags1;

		record << citmGhostMax << xnumRefreshedDate;

		SETBIT(flags2, 0, fLoadRefreshedWho)
		SETBIT(flags2, 1, fLoadRelIDRecords)
		SETBIT(flags2, 2, fSupportSubquery)
		SETBIT(flags2, 3, fSupportAttribDrill)

		record << flags2;
		record << cRecords;

		if (fLoadRefreshedWho)
			record << stRefreshedWho;

		if (fLoadRelIDRecords)
			record << stRelIDRecords;

		if (!fLoadRefreshedWho)
			record.reserveNunBytes(4);

	}

} // namespace XLSB

