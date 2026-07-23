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

#include "ColInfo.h"

namespace XLS
{
	ColWidth::ColWidth()
	{}
	ColWidth::~ColWidth()
	{}
	BaseObjectPtr ColWidth::clone()
	{
		return BaseObjectPtr(new ColWidth(*this));
	}
	void ColWidth::readFields(CFRecord& record)
	{
		BYTE colLast_1b, colFirst_1b;
		_UINT16 coldx_2b;

		record >> colFirst_1b >> colLast_1b >> coldx_2b;

		colFirst = colFirst_1b;
		colLast = colLast_1b;

		coldx = coldx_2b;
	}

//--------------------------------------------------------------
	ColInfo::ColInfo() : iOutLevel(0)
	{}
	ColInfo::~ColInfo()
	{}
	BaseObjectPtr ColInfo::clone()
	{
		return BaseObjectPtr(new ColInfo(*this));
	}
	void ColInfo::readFields(CFRecord& record)
	{
		if (record.getGlobalWorkbookInfo()->Version < 0x0800)
		{
			_UINT16		colFirst_2b;
			_UINT16		colLast_2b;
			_UINT16		coldx_2b;

			_UINT16		flags;
			record >> colFirst_2b >> colLast_2b >> coldx_2b >> ixfe >> flags;

			fHidden = GETBIT(flags, 0);
			fUserSet = GETBIT(flags, 1);
			fBestFit = GETBIT(flags, 2);
			fPhonetic = GETBIT(flags, 3);
			iOutLevel = GETBITS(flags, 8, 10);
			fCollapsed = GETBIT(flags, 12);

			colFirst = colFirst_2b;
			colLast = colLast_2b;
			coldx = coldx_2b;

			record.skipNunBytes(record.getDataSize() - record.getRdPtr()); // unused //0x0600 - 2 bytes; lower - 1 byte
		}
		else
		{
			_UINT16		flags;
			record >> colFirst >> colLast >> coldx >> ixfeXLSB >> flags;

			fHidden = GETBIT(flags, 0);
			fUserSet = GETBIT(flags, 1);
			fBestFit = GETBIT(flags, 2);
			fPhonetic = GETBIT(flags, 3);
			iOutLevel = GETBITS(flags, 8, 10);
			fCollapsed = GETBIT(flags, 12);
		}

																	   
	}

	void ColInfo::writeFields(CFRecord& record)
	{
		if (record.getGlobalWorkbookInfo()->Version < 0x0800)
		{
			_UINT16		colFirst_2b;
			_UINT16		colLast_2b;
			_UINT16		coldx_2b;

			_UINT16		flags = 0;
			colFirst_2b = colFirst;
			colLast_2b	= colLast;
			coldx_2b	= coldx;

			SETBIT(flags, 0, fHidden)
			SETBIT(flags, 1, fUserSet)
			SETBIT(flags, 2, fBestFit)
			SETBIT(flags, 3, fPhonetic)
			SETBITS(flags, 8, 10, iOutLevel)
			SETBIT(flags, 12, fCollapsed)

			record << colFirst_2b << colLast_2b << coldx_2b << ixfe << flags;

			if (record.getGlobalWorkbookInfo()->Version < 0x0600)
				record.reserveNunBytes(1); // unused //0x0600 - 2 bytes; lower - 1 byte
			else
				record.reserveNunBytes(2);
		}
		else
		{
			_UINT16		flags = 0;

			SETBIT(flags, 0, fHidden)
			SETBIT(flags, 1, fUserSet)
			SETBIT(flags, 2, fBestFit)
			SETBIT(flags, 3, fPhonetic)
			SETBITS(flags, 8, 10, iOutLevel)
			SETBIT(flags, 12, fCollapsed)

			record << colFirst << colLast << coldx << ixfeXLSB << flags;
		}
	}

} // namespace XLS

