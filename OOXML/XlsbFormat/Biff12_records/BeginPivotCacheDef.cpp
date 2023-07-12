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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

