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

#include "BOF.h"

namespace XLS
{

BOF::BOF()
{
	fWin		= true;
	fRisc		= false;
	fBeta		= false;
	fWinAny		= true;
	fMacAny		= false;
	fBetaAny	= false;
	fRiscAny	= false;
	fOOM		= false;
	fGlJmp		= false;
	fFontLimit	= false;
}


BOF::~BOF()
{
}

BaseObjectPtr BOF::clone()
{
	return BaseObjectPtr(new BOF(*this));
}

void BOF::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();

	type_id_ = record.getTypeId();

	record >> vers >> dt;
	
	if (type_id_ == rt_BOF_BIFF8)
	{
		if (vers > 0x0700) vers = 0x0600;

		record >> rupBuild >> rupYear;  // biff 5 - 8
		
		if ( record.checkFitReadSafe(8)) // biff 8
		{
			_UINT32 flags;
			record >> flags;
			
			fWin		= GETBIT(flags, 0);
			fRisc		= GETBIT(flags, 1);
			fBeta		= GETBIT(flags, 2);
			fWinAny		= GETBIT(flags, 3);
			fMacAny		= GETBIT(flags, 4);
			fBetaAny	= GETBIT(flags, 5);
			fRiscAny	= GETBIT(flags, 8);
			fOOM		= GETBIT(flags, 9);
			fGlJmp		= GETBIT(flags, 10);
			fFontLimit	= GETBIT(flags, 13);
			
			verXLHigh = GETBITS(flags, 14, 17);

			record >> verLowestBiff;
			unsigned char flags2;
			record >> flags2;
			verLastXLSaved = GETBITS(flags2, 0, 3);
			
			stream_ptr = record.getStreamPointer();
			record.skipNunBytes(2); // reserved
		}
		else
		{ //ts_2500_06_gruzi 05 06 вып.xls
			if (global_info->CodePage == 0 && global_info->lcid_user > 0)
			{
				record.getGlobalWorkbookInfo()->CodePage = global_info->lcid_converter.get_codepage(record.getGlobalWorkbookInfo()->lcid_user);
			}
		}
	}
	else
	{
		short not_used = 0;
		if (type_id_ == rt_BOF_BIFF3 || type_id_ == rt_BOF_BIFF4)
		{
			record >> not_used;
		}

		switch(type_id_)
		{
		case rt_BOF_BIFF8: verLowestBiff = 6; break;
		case rt_BOF_BIFF4: verLowestBiff = 4; vers = 0x0400; break;
		case rt_BOF_BIFF3: verLowestBiff = 3; vers = 0x0300; break;
		case rt_BOF_BIFF2: verLowestBiff = 2; vers = 0x0200; break;;
		default:
			break;
		}		 
	}
}

void BOF::writeFields(CFRecord& record)
{
    record << vers << dt << rupBuild << rupYear;
    _UINT32 flags = 0;

    SETBIT(flags, 0, fWin);
    SETBIT(flags, 1, fRisc);
    SETBIT(flags, 2, fBeta);
    SETBIT(flags, 3, fWinAny);
    SETBIT(flags, 4, fMacAny);
    SETBIT(flags, 5, fBetaAny);
    SETBIT(flags, 8, fRiscAny);
    SETBIT(flags, 9, fOOM);
    SETBIT(flags, 10, fGlJmp);
    SETBIT(flags, 13, fFontLimit);

    SETBITS(flags, 14, 17, verXLHigh);

    record << flags;
    record << verLowestBiff << verLastXLSaved;
    record.reserveNunBytes(2);// reserved
}

unsigned short BOF::getSubstreamType()
{
	return dt;
}


} // namespace XLS


