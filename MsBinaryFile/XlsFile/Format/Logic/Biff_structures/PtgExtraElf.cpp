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

#include "PtgExtraElf.h"
#include "CellRangeRef.h"

namespace XLS
{


PtgExtraElf::PtgExtraElf()
{
}


PtgExtraElf::PtgExtraElf(CFRecord& record)
{
	load(record);
}


BiffStructurePtr PtgExtraElf::clone()
{
	return BiffStructurePtr(new PtgExtraElf(*this));
}

void PtgExtraElf::load(CFRecord& record)
{
	_UINT32 flags;
	record >> flags;
	
	_UINT32 count = flags & 0x3fffffff;
	
	fRel = GETBIT(flags, 31);
	for(_UINT32 i = 0; i < count; ++i)
	{
		RgceElfLocExtra cell_ref;
		record >> cell_ref;
		array_.push_back(cell_ref);
	}
}

void PtgExtraElf::save(CFRecord& record)
{
	_UINT32 flags = 0;
	SETBIT(flags, 31, fRel);
	record << flags;

	for (auto& item : array_)
	{
		record << item;
	}
}


const std::wstring PtgExtraElf::toString() const
{
	std::wstring range;
	for(std::vector<RgceElfLocExtra>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += it->toString() + L';';
	}
	return range + array_.back().toString();
}


const std::wstring PtgExtraElf::toColumnsSet() const
{
	std::wstring range;
	for(std::vector<RgceElfLocExtra>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
	{
		range += CellRangeRef(*it, CellRangeRef::stColumnBelow).toString() + L';';
	}
	return range + CellRangeRef(array_.back(), CellRangeRef::stColumnBelow).toString();
}


} // namespace XLS

