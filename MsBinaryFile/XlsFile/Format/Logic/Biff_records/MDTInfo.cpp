﻿/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

