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

#include "WsBool.h"

namespace XLS
{

WsBool::WsBool(bool & is_dialog_sheet) : fDialog(is_dialog_sheet)
{
}

WsBool::~WsBool()
{
}

BaseObjectPtr WsBool::clone()
{
	return BaseObjectPtr(new WsBool(*this));
}


void WsBool::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fShowAutoBreaks = GETBIT(flags, 0);
	fDialog			= GETBIT(flags, 4);
	fApplyStyles	= GETBIT(flags, 5);
	fRowSumsBelow	= GETBIT(flags, 6);
	fColSumsRight	= GETBIT(flags, 7);
	fFitToPage		= GETBIT(flags, 8);
	fDspGuts		= GETBIT(flags, 10);
	fSyncHoriz		= GETBIT(flags, 12);
	fSyncVert		= GETBIT(flags, 13);
	fAltExprEval	= GETBIT(flags, 14);
	fAltFormulaEntry = GETBIT(flags, 15);
}

void WsBool::writeFields(CFRecord& record)
{
    unsigned short flags = 0;

    SETBIT(flags, 0, fShowAutoBreaks);
    if(fDialog)
        SETBIT(flags, 4, fDialog);
    SETBIT(flags, 5, fApplyStyles);
    SETBIT(flags, 6, fRowSumsBelow);
    SETBIT(flags, 7, fColSumsRight);
    SETBIT(flags, 8, fFitToPage);
    SETBIT(flags, 10, fDspGuts);
    SETBIT(flags, 12, fSyncHoriz);
    SETBIT(flags, 13, fSyncVert);
    SETBIT(flags, 14, fAltExprEval);
    SETBIT(flags, 15, fAltFormulaEntry);

    record << flags;
}

} // namespace XLS

