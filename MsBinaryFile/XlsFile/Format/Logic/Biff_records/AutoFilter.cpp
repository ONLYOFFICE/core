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
//
#include "AutoFilter.h"

//#include <algorithm>
//#include <functional>

namespace XLS
{
// trim from both ends
static inline void trim(std::wstring &s) 
{
	int new_size = s.length();
	for (int i = new_size - 1; i >=0; i--)
	{
		if (s[i] != 0) break;
		else new_size--;
	}
	if (new_size < (int)s.length())
		s.erase(new_size);
}

AutoFilter::AutoFilter()
{
	wTopN = wJoin = 0;
	size = 0xffffffff;
	bExist = false;
}


AutoFilter::~AutoFilter()
{
}


BaseObjectPtr AutoFilter::clone()
{
	return BaseObjectPtr(new AutoFilter(*this));
}

void AutoFilter::readFields(CFRecord& record)
{
	size_t pos_record = record.getRdPtr();


	if (size == 0xffffffff)
		size = record.getDataSize() - pos_record;
	else if(record.getDataSize() < pos_record + size)
	{
		//size error
		return;
	}

	if (size > 0)
	{
		bExist = true;
	}

	m_bAutoFilter12 = false;

	unsigned short flags;
	record >> iEntry >> flags;
	
	wJoin		= GETBITS(flags, 0, 1);
	
	fSimple1	= GETBIT(flags, 2);
	fSimple2	= GETBIT(flags, 3);
	fTopN		= GETBIT(flags, 4);
	fTop		= GETBIT(flags, 5); //top(1) or bottom(0)
	fPercent	= GETBIT(flags, 6);
	
	wTopN		= GETBITS(flags, 7, 15);

	unsigned short	_iEntry		= iEntry;
	unsigned char	_wJoin		= wJoin;			
	unsigned short	_wTopN		= wTopN;			
	
	if (fTopN != 1)
	{
		record >> doper1;			
		record >> doper2;
	}

	if (doper1.vt == 0 && doper2.vt == 0)
	{
		m_bAutoFilter12 = true;	
		return;
	}

	if (doper1.vt == 0x06)
	{
		XLUnicodeStringNoCch s;
		s.setSize(doper1.vtValueStr.cch);
		record >> s;	

		str1 = s.value();
		trim(str1);
	}

	if (doper2.vt == 0x06)
	{
		XLUnicodeStringNoCch s;
		s.setSize(doper2.vtValueStr.cch);
		record >> s;
		
		str2 = s.value();
		trim(str2);
	}	

	if (record.getRdPtr() - pos_record < size)
	{
		int sz = size - (record.getRdPtr() - pos_record);
		sz = (std::min)(sz, (int)(record.getDataSize() - record.getRdPtr()));

		char *dd = new char [sz];
		
		memcpy(dd, record.getCurData<char>(), sz);

		record.skipNunBytes(sz);
		delete []dd;
	}
}

void AutoFilter::writeFields(CFRecord& record)
{
    unsigned short flags = 0;

    SETBITS(flags, 0, 1, wJoin)

    SETBIT(flags, 2, fSimple1)
    SETBIT(flags, 3, fSimple2)
    SETBIT(flags, 4, fTopN)
    SETBIT(flags, 5, fTop) //top(1) or bottom(0)
    SETBIT(flags, 6, fPercent)

    SETBITS(flags, 7, 15, wTopN)

    record << iEntry << flags;
    if (fTopN != 1)
    {
        record << doper1;
        record << doper2;
        if(doper1.vt == 0x06)
        {
            XLUnicodeStringNoCch s;
            s.setSize(doper1.vtValueStr.cch);
            s = str1;
            record << s;
        }
        if(doper2.vt == 0x06)
        {
            XLUnicodeStringNoCch s;
            s.setSize(doper2.vtValueStr.cch);
            s = str2;
            record << s;
        }
    }
    else
    {
        record.reserveNunBytes(20);
    }

}

} // namespace XLS

