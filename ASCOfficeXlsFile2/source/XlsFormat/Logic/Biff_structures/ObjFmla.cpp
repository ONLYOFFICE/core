/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "ObjFmla.h"
#include <Binary/CFRecord.h>

namespace XLS
{


ObjFmla::ObjFmla(const bool is_part_of_FtPictFmla)
:	is_part_of_FtPictFmla_(is_part_of_FtPictFmla)
{
}

BiffStructurePtr ObjFmla::clone()
{
	return BiffStructurePtr(new ObjFmla(*this));
}


void ObjFmla::load(CFRecord& record)
{
	unsigned short cbFmla;
	record >> cbFmla;
	size_t start_ptr = record.getRdPtr();

	if(0 != cbFmla)
	{
		fmla.load(record);

		if(is_part_of_FtPictFmla_ && fmla.HasPtgTbl())
		{
			record >> embedInfo;
		}
	}


	size_t data_size = record.getRdPtr() - start_ptr;
	size_t padding_size = cbFmla - data_size;
	if(0 != padding_size && (record.getRdPtr() + padding_size) <= record.getDataSize())
	{
		record.skipNunBytes(padding_size);
	}
}


void ObjFmla::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cbFmla*/);
	size_t start_ptr = record.getDataSize();

	if(fmla_found)
	{
		fmla.store(record);
	}

	if(true)
	{
		record << embedInfo;
	}

	size_t data_size = record.getDataSize() - start_ptr;
	if(0 != (data_size & 1))
	{
		record.reserveNunBytes(1); // padding
		data_size++;
	}
	record.registerDelayedDataSource(data_size, rt_Obj);
}


} // namespace XLS

