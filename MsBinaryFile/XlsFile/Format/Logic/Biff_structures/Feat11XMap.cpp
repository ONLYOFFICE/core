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

#include "Feat11XMap.h"

namespace XLS
{

Feat11XMapEntry2::Feat11XMapEntry2()
{
}

BiffStructurePtr Feat11XMapEntry2::clone()
{
	return BiffStructurePtr(new Feat11XMapEntry2(*this));
}

void Feat11XMapEntry2::load(CFRecord& record)
{
	record >> dwMapId >> rgbXPath;
}
void Feat11XMapEntry2::save(CFRecord& record)
{
	record << dwMapId << rgbXPath;
}

//----------------------------------------------------------------------
Feat11XMapEntry::Feat11XMapEntry()
{
}

BiffStructurePtr Feat11XMapEntry::clone()
{
	return BiffStructurePtr(new Feat11XMapEntry(*this));
}

void Feat11XMapEntry::load(CFRecord& record)
{
	_UINT32 flags;
	record >> flags >> entry;

	fLoadXMap		= GETBIT(flags, 1);
	fCanBeSingle	= GETBIT(flags, 2);
}

void Feat11XMapEntry::save(CFRecord& record)
{
	_UINT32 flags = 0;
	SETBIT(flags, 1, fLoadXMap)
	SETBIT(flags, 2, fCanBeSingle)
	record << flags << entry;
}

//----------------------------------------------------------------------
Feat11XMap::Feat11XMap()
{
}

BiffStructurePtr Feat11XMap::clone()
{
	return BiffStructurePtr(new Feat11XMap(*this));
}

void Feat11XMap::load(CFRecord& record)
{
	_UINT16 cbFmla;
	record >> cbFmla;

	for (_UINT16 i = 0; i < cbFmla; i++)
	{
		Feat11XMapEntry entry;
		
		entry.load(record);
		rgXmap.push_back(entry);

	}
}

void Feat11XMap::save(CFRecord& record)
{
	_UINT16 iXmapMac = rgXmap.size();
	record << iXmapMac;
	for(auto i : rgXmap)
		i.save(record);
}


} // namespace XLS

