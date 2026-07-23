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

#include "OfficeArtFSP.h"

namespace ODRAW
{


OfficeArtFSP::OfficeArtFSP()
:	OfficeArtRecord(0x02, FSP)
{
}


XLS::BiffStructurePtr OfficeArtFSP::clone()
{
	return XLS::BiffStructurePtr(new OfficeArtFSP(*this));
}

void OfficeArtFSP::loadFields(XLS::CFRecord& record)
{
	_UINT32 flags;
	record >> spid >> flags;

	fGroup = GETBIT(flags, 0);
	fChild = GETBIT(flags, 1);
	fPatriarch = GETBIT(flags, 2);
	fDeleted = GETBIT(flags, 3);
	fOleShape = GETBIT(flags, 4);
	fHaveMaster = GETBIT(flags, 5);
	fFlipH = GETBIT(flags, 6);
	fFlipV = GETBIT(flags, 7);
	fConnector = GETBIT(flags, 8);
	fHaveAnchor = GETBIT(flags, 9);
	fBackground = GETBIT(flags, 10);
	fHaveSpt = GETBIT(flags, 11);

	shape_id = rh_own.recInstance;
}

void OfficeArtFSP::save(XLS::CFRecord& record)
{
	rh_own.recVer = 0x2;
	rh_own.recInstance = shape_id;
	rh_own.recType = 0xF00A;
	rh_own.recLen =  0x00000008;
	record << rh_own;
	_UINT32 flags = 0;
	SETBIT(flags, 0, fGroup)
	SETBIT(flags, 1, fChild)
	SETBIT(flags, 2, fPatriarch)
	SETBIT(flags, 3, fDeleted)
	SETBIT(flags, 4, fOleShape)
	SETBIT(flags, 5, fHaveMaster)
	SETBIT(flags, 6, fFlipH)
	SETBIT(flags, 7, fFlipV)
	SETBIT(flags, 8, fConnector)
	SETBIT(flags, 9, fHaveAnchor)
	SETBIT(flags, 10, fBackground)
	SETBIT(flags, 11, fHaveSpt)

	record << spid << flags;
}

const unsigned short OfficeArtFSP::GetInstanceToStore()
{
	return shape_id;
}


} // namespace XLS
