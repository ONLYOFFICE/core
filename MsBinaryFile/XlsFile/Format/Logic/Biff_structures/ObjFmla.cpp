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

#include "ObjFmla.h"

namespace XLS
{


ObjFmla::ObjFmla(const bool is_part_of_FtPictFmla) : is_part_of_FtPictFmla_(is_part_of_FtPictFmla), bFmlaExist(false), bInfoExist(false)
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
		bFmlaExist = true;

		fmla.load(record);

		if(is_part_of_FtPictFmla_ && fmla.HasPtgTbl())
		{
			record >> embedInfo;
			bInfoExist = true;
		}
	}

	size_t data_size = record.getRdPtr() - start_ptr;
	size_t padding_size = cbFmla - data_size;
	if(0 != padding_size && (record.getRdPtr() + padding_size) <= record.getDataSize())
	{
		record.skipNunBytes(padding_size);
	}
}

void ObjFmla::save(CFRecord& record)
{
	record.reserveNunBytes(2);
	auto cbPose = record.getRdPtr();
	fmla.save(record);
	if(is_part_of_FtPictFmla_)
		embedInfo.save(record);
	unsigned short cbFmla = record.getRdPtr() - cbPose;
	record.RollRdPtrBack(cbFmla + 2);
	if(cbFmla % 2 != 0)
		cbFmla++;
	record << cbFmla;
	record.skipNunBytes(cbFmla);

}


} // namespace XLS

