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

#include "Pls.h"

namespace XLS
{

Pls::Pls()
{
}

Pls::~Pls()
{
}

BaseObjectPtr Pls::clone()
{
	return BaseObjectPtr(new Pls());
}

void Pls::readFields(CFRecord& record)
{
	record.skipNunBytes(2); // reserved
	
	if (record.loadAnyData(rgb) == false) return;

	if (continue_records.size() > 0)
	{
		std::list<CFRecordPtr>& recs = continue_records[rt_Continue];

		if (recs.size())
		{
			while( !recs.empty() )
			{
				record.appendRawData(recs.front()->getData(), recs.front()->getDataSize());
				recs.pop_front();
			}
		}
	}

	int size			= record.getDataSize() - 2;
	const BYTE* data	= (BYTE*)record.getData() + 2;
	
	boost::shared_array<BYTE> buffer(new BYTE[size]);
    memcpy(buffer.get(), data, size);
	
	bin_data_id = -1;

	GlobalWorkbookInfo* globla_info = record.getGlobalWorkbookInfo().get();
	if (globla_info)
	{
		globla_info->bin_data.push_back(std::pair<boost::shared_array<BYTE>, size_t>(buffer, size));

		bin_data_id = globla_info->bin_data.size() - 1;
	}

	record.skipNunBytes(record.getDataSize() - record.getRdPtr()); // avoid size inconsistency warning
}

} // namespace XLS

