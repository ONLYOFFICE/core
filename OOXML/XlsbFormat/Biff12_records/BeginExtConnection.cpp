﻿/*
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

