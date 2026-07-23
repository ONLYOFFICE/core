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

#include "MDTInfo.h"

namespace XLS
{

MDTInfo::MDTInfo()
{
}


MDTInfo::~MDTInfo()
{
}


BaseObjectPtr MDTInfo::clone()
{
	return BaseObjectPtr(new MDTInfo(*this));
}

void MDTInfo::readFields(CFRecord& record)
{
	_UINT32 flags;

	record >> frtHeader >> flags;
	
	fGhostRow = GETBIT(flags, 0);
	fGhostCol = GETBIT(flags, 1);
	fEdit = GETBIT(flags, 2);
	fDelete = GETBIT(flags, 3);
	fCopy = GETBIT(flags, 4);
	fPasteAll = GETBIT(flags, 5);
	fPasteFormulas = GETBIT(flags, 6);
	fPasteValues = GETBIT(flags, 7);
	fPasteFormats = GETBIT(flags, 8);
	fPasteComments = GETBIT(flags, 9);
	fPasteDataValidation = GETBIT(flags, 10);
	fPasteBorders = GETBIT(flags, 11);
	fPasteColWidths = GETBIT(flags, 12);
	fPasteNumberFormats = GETBIT(flags, 13);
	fMerge = GETBIT(flags, 14);
	fSplitFirst = GETBIT(flags, 15);
	fSplitAll = GETBIT(flags, 16);
	fRowColShift = GETBIT(flags, 17);
	fClearAll = GETBIT(flags, 18);
	fClearFormats = GETBIT(flags, 19);
	fClearContents = GETBIT(flags, 20);
	fClearComments = GETBIT(flags, 21);
	fAssign = GETBIT(flags, 22);
	fCoerce = GETBIT(flags, 28);
	fAdjust = GETBIT(flags, 29);
	fCellMeta = GETBIT(flags, 30);

	record >> stName;
}

void MDTInfo::writeFields(CFRecord& record)
{
	frtHeader.rt = rt_MDTInfo;
	record << frtHeader;
	_UINT32 flags = 0;

	SETBIT(flags, 0, fGhostRow)
	SETBIT(flags, 1, fGhostCol)
	SETBIT(flags, 2, fEdit)
	SETBIT(flags, 3, fDelete)
	SETBIT(flags, 4, fCopy)
	SETBIT(flags, 5, fPasteAll)
	SETBIT(flags, 6, fPasteFormulas)
	SETBIT(flags, 7, fPasteValues)
	SETBIT(flags, 8, fPasteFormats)
	SETBIT(flags, 9, fPasteComments)
	SETBIT(flags, 10, fPasteDataValidation)
	SETBIT(flags, 11, fPasteBorders)
	SETBIT(flags, 12, fPasteColWidths)
	SETBIT(flags, 13, fPasteNumberFormats)
	SETBIT(flags, 14, fMerge)
	SETBIT(flags, 15, fSplitFirst)
	SETBIT(flags, 16, fSplitAll)
	SETBIT(flags, 17, fRowColShift)
	SETBIT(flags, 18, fClearAll)
	SETBIT(flags, 19, fClearFormats)
	SETBIT(flags, 20, fClearContents)
	SETBIT(flags, 21, fClearComments)
	SETBIT(flags, 22, fAssign)
	SETBIT(flags, 28, fCoerce)
	SETBIT(flags, 29, fAdjust)
	SETBIT(flags, 30, fCellMeta)

	record << flags;
	if(stName.getSize() <= 4104)
		record << stName;
	else
	{
		LPWideString tempVal = stName.value().substr(0, 4104);
		record << tempVal;
	}
}

} // namespace XLS

