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

#include "BookBool.h"

namespace XLS
{

BookBool::BookBool()
{
}


BookBool::~BookBool()
{
}


BaseObjectPtr BookBool::clone()
{
	return BaseObjectPtr(new BookBool(*this));
}

void BookBool::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> flags;
	
	fNoSaveSup			= GETBIT(flags, 0);
	fHasEnvelope		= GETBIT(flags, 2);
	fEnvelopeVisible	= GETBIT(flags, 3);
	fEnvelopeInitDone	= GETBIT(flags, 4);
	
	unsigned char grUpdateLinks_num = GETBITS(flags, 5, 6);
	switch(grUpdateLinks_num)
	{
		case 0:
			grUpdateLinks = std::wstring (L"userSet");
			break;
		case 1:
			grUpdateLinks = std::wstring (L"never");
			break;
		case 2:
			grUpdateLinks = std::wstring (L"always");
			break;
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported value of grUpdateLinks.", record.getTypeString());
			break;
	}
	
	fHideBorderUnselLists = GETBIT(flags, 8);
}

void BookBool::writeFields(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fNoSaveSup)
    SETBIT(flags, 2, fHasEnvelope)
    SETBIT(flags, 3, fEnvelopeVisible)
    SETBIT(flags, 4, fEnvelopeInitDone)
    unsigned char grUpdateLinks_num = 0;
    if(grUpdateLinks == L"never")
        grUpdateLinks_num = 1;
    else if(grUpdateLinks == L"always")
        grUpdateLinks_num = 2;
    SETBITS(flags, 5, 6, grUpdateLinks_num)
    SETBIT(flags, 8, fHideBorderUnselLists)
    record << flags;
}

} // namespace XLS

