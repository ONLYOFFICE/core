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

#include "BeginWebPubItem.h"

using namespace XLS;

namespace XLSB
{

    BeginWebPubItem::BeginWebPubItem()
    {
    }

    BeginWebPubItem::~BeginWebPubItem()
    {
    }

    BaseObjectPtr BeginWebPubItem::clone()
    {
        return BaseObjectPtr(new BeginWebPubItem(*this));
    }

    void BeginWebPubItem::readFields(XLS::CFRecord& record)
    {
        _UINT32 flags1;
        BYTE    flags2;
        record >> tws >> flags1;

        fAutoRepublish = GETBIT(flags1, 1);
        fMhtml         = GETBIT(flags1, 3);

        record >> nStyleId;

        if(tws.value().get() == Tws::TWSREF)
            record >> rfx;
        else
            record.skipNunBytes(16);

		record >> flags2;

        fName          = GETBIT(flags2, 1);
        fTitle         = GETBIT(flags2, 3);

        record >> stBkmk;

        if(fName)
            record >> stBkmk;

        record >> stFile;

        if(fTitle)
            record >> stTitle;
    }

	void BeginWebPubItem::writeFields(XLS::CFRecord& record)
	{
		_UINT32 flags1 = 0;
		BYTE    flags2 = 0;
		record << tws;

		SETBIT(flags1, 1, fAutoRepublish)
		SETBIT(flags1, 3, fMhtml)

    	record << flags1;
		record << nStyleId;

		if (tws.value().get() == Tws::TWSREF)
			record << rfx;
		else
			record.reserveNunBytes(16);

		SETBIT(flags2, 1, fName)
		SETBIT(flags2, 3, fTitle)

    	record << flags2;

		record << stBkmk;

		if (fName)
			record << stBkmk;

		record << stFile;

		if (fTitle)
			record << stTitle;
	}

} // namespace XLSB

