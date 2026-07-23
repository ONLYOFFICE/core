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

#include "BkHim.h"

namespace XLS
{

BkHim::BkHim()
{
}


BkHim::~BkHim()
{
}


BaseObjectPtr BkHim::clone()
{
	return BaseObjectPtr(new BkHim(*this));
}

void BkHim::readFields(CFRecord& record)
{
	record >> cf;
	record.skipNunBytes(2);
	record >> lcb;//imageBlob size

	unsigned int sz = record.getDataSize() - record.getRdPtr();

	if (cf == 0x09)
	{
		//bitmap
	}
	else
	{
		//native
	}

	if (lcb < 1) return;
	
	std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

	while (record.getRdPtr() + lcb > record.getDataSize() && !recs.empty())
	{
		record.appendRawData(recs.front());
		recs.pop_front();
	}

	if (record.checkFitReadSafe(lcb))
	{
		pData = boost::shared_array<char>(new char[lcb]);

		memcpy(pData.get(), record.getCurData<char>(), lcb);

		record.skipNunBytes(lcb);
	}
}

void BkHim::writeFields(CFRecord& record)
{
    record << cf;
    unsigned short reserve = 1;
    record << reserve;
    record << lcb;
    auto maxRecordSize = 8224;
    if(pData)
    {
        if(lcb < maxRecordSize)
        {
            for(auto i = 0; i < lcb; i++)
                record << pData[i];
        }
        else
        {
            for(auto i = 0; i < maxRecordSize; i++)
                record << pData[i];
        }
    }
}

} // namespace XLS

