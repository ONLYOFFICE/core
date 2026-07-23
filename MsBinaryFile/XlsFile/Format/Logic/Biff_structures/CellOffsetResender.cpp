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

#include "CellOffsetResender.h"
#include "../../Binary/CFStream.h"

namespace XLS
{


BiffStructurePtr CellOffsetResender::clone()
{
	return BiffStructurePtr(new CellOffsetResender(*this));
}


void CellOffsetResender::resend(CFStream& stream, const unsigned int data_place, const unsigned int data)
{
	unsigned int CELL_start_pos = data_place - sizeof(unsigned short)/*size_short*/ - sizeof(CFRecordType::TypeId);
	CFStream::SourceItems source_items;
	CFStream::SourceItem item;
	item.is_file_ptr = false;
	item.data = CELL_start_pos - data;
	item.receiver_id = rt_DBCell;
	item.source_id = rt_Blank;
	source_items.push_back(item); // to DBCell

	item.is_file_ptr = false;
	item.data = CELL_start_pos;
	item.receiver_id = rt_Blank;
	item.source_id = rt_Blank;
	source_items.push_back(item); // To the next cell

	stream.appendDelayedItems(CFStream::ReceiverItems(), source_items);
}

void CellOffsetResender::load(CFRecord& record)
{
	// do nothing
}


} // namespace XLS

