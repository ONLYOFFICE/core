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

#include "SXFDB.h"

namespace XLS
{

SXFDB::SXFDB()
{
}


SXFDB::~SXFDB()
{
}

BaseObjectPtr SXFDB::clone()
{
	return BaseObjectPtr(new SXFDB(*this));
}

void SXFDB::readFields(CFRecord& record)
{
	unsigned short	flags;
	record >> flags >> ifdbParent >> ifdbBase >> citmUnq >> csxoper >> cisxoper >> catm;
	record >> stFieldName;

	fAllAtoms			= GETBIT(flags, 0);
	fSomeUnhashed		= GETBIT(flags, 1);
	fUsed				= GETBIT(flags, 2);
	fHasParent			= GETBIT(flags, 3);
	fRangeGroup			= GETBIT(flags, 4);
	fNumField			= GETBIT(flags, 5);
	fTextEtcField		= GETBIT(flags, 7);
	fnumMinMaxValid		= GETBIT(flags, 8);
	fShortIitms			= GETBIT(flags, 9);
	fNonDates			= GETBIT(flags, 10);
	fDateInField		= GETBIT(flags, 11);
	fServerBased		= GETBIT(flags, 13);
	fCantGetUniqueItems	= GETBIT(flags, 14);
	fCalculatedField	= GETBIT(flags, 15);

	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	global_info->arPivotCacheFieldShortSize.push_back(fShortIitms);
	global_info->arPivotCacheFields.push_back(fAllAtoms);
}

void SXFDB::writeFields(CFRecord& record)
{
	unsigned short	flags = 0;

	SETBIT(flags, 0, fAllAtoms)
	SETBIT(flags, 1, fSomeUnhashed)
	SETBIT(flags, 2, fUsed)
	SETBIT(flags, 3, fHasParent)
	SETBIT(flags, 4, fRangeGroup)
	SETBIT(flags, 5, fNumField)
	SETBIT(flags, 7, fTextEtcField)
	SETBIT(flags, 8, fnumMinMaxValid)
	SETBIT(flags, 9, fShortIitms)
	SETBIT(flags, 10, fNonDates)
	SETBIT(flags, 11, fDateInField)
	SETBIT(flags, 13, fServerBased)
	SETBIT(flags, 14, fCantGetUniqueItems)
	SETBIT(flags, 15, fCalculatedField)

	record << flags << ifdbParent << ifdbBase << citmUnq << csxoper << cisxoper << catm;
	record << stFieldName;
}

} // namespace XLS

