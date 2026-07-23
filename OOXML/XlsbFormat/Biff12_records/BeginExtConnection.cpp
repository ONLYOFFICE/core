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

#include "BeginExtConnection.h"

using namespace XLS;

namespace XLSB
{

    BeginExtConnection::BeginExtConnection()
    {
    }

    BeginExtConnection::~BeginExtConnection()
    {
    }

    BaseObjectPtr BeginExtConnection::clone()
    {
        return BaseObjectPtr(new BeginExtConnection(*this));
    }

    void BeginExtConnection::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags;

        record >> bVerRefreshed >> bVerRefreshableMin >> pc;

        record.skipNunBytes(1);

        record >> wInterval >> flags;

        fMaintain                 = GETBIT(flags, 0);
        fNewQuery                 = GETBIT(flags, 1);
        fDeleted                  = GETBIT(flags, 2);
        fAlwaysUseConnectionFile  = GETBIT(flags, 3);
        fBackgroundQuery          = GETBIT(flags, 4);
        fRefreshOnLoad            = GETBIT(flags, 5);
        fSaveData                 = GETBIT(flags, 6);
        fLoadSourceDataFile       = GETBIT(flags, 16);
        fLoadSourceConnectionFile = GETBIT(flags, 17);
        fLoadConnectionDesc       = GETBIT(flags, 18);
        fLoadSSOApplicationID     = GETBIT(flags, 20);

        record >> idbtype >> irecontype >> dwConnID >> iCredMethod;

        if(fLoadSourceDataFile)
            record >> stDataFile;

        if(fLoadSourceConnectionFile)
            record >> stConnectionFile;

        if(fLoadConnectionDesc)
            record >> stConnDesc;

        record >> stConnName;

        if(fLoadSSOApplicationID)
            record >> stSso;
    }

	void BeginExtConnection::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		record << bVerRefreshed << bVerRefreshableMin << pc;

		record.reserveNunBytes(1);

		SETBIT(flags, 0, fMaintain)
		SETBIT(flags, 1, fNewQuery)
		SETBIT(flags, 2, fDeleted)
		SETBIT(flags, 3, fAlwaysUseConnectionFile)
		SETBIT(flags, 4, fBackgroundQuery)
		SETBIT(flags, 5, fRefreshOnLoad)
		SETBIT(flags, 6, fSaveData)
		SETBIT(flags, 16, fLoadSourceDataFile)
		SETBIT(flags, 17, fLoadSourceConnectionFile)
		SETBIT(flags, 18, fLoadConnectionDesc)
        SETBIT(flags, 19, 1)
		SETBIT(flags, 20, fLoadSSOApplicationID)

		record << wInterval << flags;

		record << idbtype << irecontype << dwConnID << iCredMethod;

		if (fLoadSourceDataFile)
			record << stDataFile;

		if (fLoadSourceConnectionFile)
			record << stConnectionFile;

		if (fLoadConnectionDesc)
			record << stConnDesc;

		record << stConnName;

		if (fLoadSSOApplicationID)
			record << stSso;
	}

} // namespace XLSB

