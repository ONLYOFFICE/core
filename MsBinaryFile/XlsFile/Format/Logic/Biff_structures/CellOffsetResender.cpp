/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

