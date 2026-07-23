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

#include "Row.h"

namespace XLS
{
Row::Row() : iOutLevel(0), ixfe_val(0)
{}
Row::~Row()
{}
BaseObjectPtr Row::clone()
{
	return BaseObjectPtr(new Row(*this));
}

Row_BIFF2::Row_BIFF2() : Row()
{}
Row_BIFF2::~Row_BIFF2()
{}
BaseObjectPtr Row_BIFF2::clone()
{
	return BaseObjectPtr(new Row_BIFF2(*this));
}
//---------------------------------------------------------------------------
void Row::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();
	
	if (global_info_->Version < 0x0800)
    {
        Rw rw_2b;
		_UINT16 ixfe_val_2b = 0xffff;
        record >> rw_2b >> colMic >> colMac >> miyRw;

		_UINT16 unused1, rel_offset = 0xffff, flags1, flags2;
		record >> unused1;
		
		if (global_info_->Version == 0x0200)
		{
			BYTE flag;
			record >> flag >> rel_offset;
			fGhostDirty = flag;
			
			if (fGhostDirty)
			{
				record >> flag >> flags1 >> ixfe_val_2b;
			}
			bValid = true;
		}
		else
		{
			if (global_info_->Version == 0x0300 ||
				global_info_->Version == 0x0400)
			{
				record >> rel_offset; // relative offset stream for first cell in row
			}
			else
				record >> unused1;

			record >> flags1 >> flags2;

			iOutLevel = GETBITS(flags1, 0, 2);
			fCollapsed = GETBIT(flags1, 4);
			fDyZero = GETBIT(flags1, 5);
			fUnsynced = GETBIT(flags1, 6);
			fGhostDirty = GETBIT(flags1, 7);
			
			ixfe_val_2b = GETBITS(flags2, 0, 11);
			fExAsc = GETBIT(flags2, 12);
			fExDes = GETBIT(flags2, 13);
			fPhonetic = GETBIT(flags2, 14);

			bValid = (flags1 != 0 || flags2 != 0);
		}
		if (ixfe_val_2b != 0xffff)
			ixfe_val = ixfe_val_2b;
		rw = rw_2b;
    }
    else
    {
        record >> rw >> ixfe_val >> miyRw;

        _UINT16 flags;
        BYTE  flags2;

        record >> flags >> flags2 >> ccolspan;

        fExAsc      = GETBIT(flags, 0);
        fExDes      = GETBIT(flags, 1);
        iOutLevel	= GETBITS(flags, 8, 10);
        fCollapsed	= GETBIT(flags, 11);
        fDyZero 	= GETBIT(flags, 12);
        fUnsynced   = GETBIT(flags, 13);
        fGhostDirty = GETBIT(flags, 14);

        fPhonetic	= GETBIT(flags2, 0);

        for(auto i = 0; i < ccolspan; i++)
        {
            XLSB::ColSpan temp;
            record >> temp;
            rgBrtColspan.push_back(temp);
        }

        bValid = (flags != 0 || flags2 != 0);
    }

}

void Row::writeFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	if (global_info_->Version < 0x0800)
	{
		Rw rw_2b;
		rw_2b = rw;
		_UINT16 ixfe_val_2b = 0xffff;
		record << rw_2b << colMic << colMac << miyRw;

		_UINT16 rel_offset = 0xffff, flags1 = 0, flags2 = 0;
		record.reserveNunBytes(2);// unused1

		if(ixfe_val != 0)
		{
			ixfe_val += global_info_->cellStyleXfs_count;
		}
		if (ixfe_val != 0xffff)
			ixfe_val_2b = ixfe_val;

		if (global_info_->Version == 0x0200)
		{
			BYTE flag = 0;
			flag = fGhostDirty;
			record << flag << rel_offset;			

			if (fGhostDirty)
			{
				record << flag << flags1 << ixfe_val_2b;
			}
			bValid = true;
		}
		else
		{
			if (global_info_->Version == 0x0300 ||
				global_info_->Version == 0x0400)
			{
				record << rel_offset; // relative offset stream for first cell in row
			}
			else
				record.reserveNunBytes(2);// unused1

			SETBITS(flags1, 0, 2, iOutLevel)
			SETBIT(flags1, 4, fCollapsed)
			SETBIT(flags1, 5, fDyZero)
			SETBIT(flags1, 6, fUnsynced)
			SETBIT(flags1, 7, fGhostDirty)
			SETBIT(flags1, 8, 1)// reserved3 must be 1

			SETBITS(flags2, 0, 11, ixfe_val_2b)
			SETBIT(flags2, 12, fExAsc)
			SETBIT(flags2, 13, fExDes)
			SETBIT(flags2, 14, fPhonetic)

			bValid = (flags1 != 0 || flags2 != 0);

			record << flags1 << flags2;
		}		
		
	}
	else
	{
		record << rw << ixfe_val << miyRw;

		_UINT16 flags = 0;
		BYTE	flags2 = 0;

		SETBIT(flags, 0, fExAsc)
		SETBIT(flags, 1, fExDes)
		SETBITS(flags, 8, 10, iOutLevel)
		SETBIT(flags, 11, fCollapsed)
		SETBIT(flags, 12, fDyZero)
		SETBIT(flags, 13, fUnsynced)
		SETBIT(flags, 14, fGhostDirty)

		SETBIT(flags2, 0, fPhonetic)

		ccolspan = rgBrtColspan.size();
		record << flags << flags2 << ccolspan;

		for (auto& item : rgBrtColspan)
		{
			record << item;
		}

		bValid = (flags != 0 || flags2 != 0);
	}

}

int Row::serialize(std::wostream &stream)
{
	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"row")
		{	
			CP_XML_ATTR(L"r", rw + 1);

			bool xf_set = true;
			if (fGhostDirty == false) xf_set = false;
			
			if (colMic >= 0 && colMac > colMic)
			{
				CP_XML_ATTR(L"spans", std::to_wstring(colMic + 1) + L":" + std::to_wstring(colMac));
			}
			if (xf_set && bValid)
			{
				int xf = ixfe_val >= global_info_->cellStyleXfs_count ? ixfe_val - global_info_->cellStyleXfs_count : -1/*ixfe_val*/;
				
				if (xf < global_info_->cellXfs_count && xf >= 0)
				{
					CP_XML_ATTR(L"s", xf);
					CP_XML_ATTR(L"customFormat", true);
				}
			}
			if (miyRw > 0 && miyRw < 0x8000 && bValid && 
				((fUnsynced && fGhostDirty) || !fGhostDirty))
			{
				CP_XML_ATTR(L"ht", miyRw / 20.);
				if (fUnsynced)
				{
					CP_XML_ATTR(L"customHeight", true);			
				}
				else
				{
					//auto set
				}

			}
			if (iOutLevel > 0)
			{
				CP_XML_ATTR(L"outlineLevel", iOutLevel);
			}
			if (fCollapsed)
			{
				CP_XML_ATTR(L"collapsed", fCollapsed);
			}
			if (fExAsc)
			{
				CP_XML_ATTR(L"thickTop", true);
			}
			if (fExDes)
			{
				CP_XML_ATTR(L"thickBot", true);
			}
			if (fDyZero)
			{
				CP_XML_ATTR(L"hidden", true);
			}
		}
	}

	return 0;
}

} // namespace XLS

