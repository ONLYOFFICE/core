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

#include "PtgExtraMem.h"

namespace XLS
{


PtgExtraMem::PtgExtraMem()
{
}


PtgExtraMem::PtgExtraMem(CFRecord& record)
{
	load(record);
}


BiffStructurePtr PtgExtraMem::clone()
{
	return BiffStructurePtr(new PtgExtraMem(*this));
}


void PtgExtraMem::load(CFRecord& record)
{	
    global_info = record.getGlobalWorkbookInfo();
    if (global_info->Version < 0x0800)
    {
        unsigned short count;
        record >> count;
        for(int i = 0; i < count; ++i)
        {
            Ref8U ref8;
            record >> ref8;
            array_.push_back(ref8);
        }
    }
    else
    {
        unsigned int count;
        record >> count;
        for(int i = 0; i < count; ++i)
        {
            XLSB::UncheckedRfX uncheckedRfX;
            record >> uncheckedRfX;
            array_xlsb.push_back(uncheckedRfX);
        }
    }
}

void PtgExtraMem::save(CFRecord& record)
{
	global_info = record.getGlobalWorkbookInfo();
	if (global_info->Version < 0x0800)
	{
		unsigned short count;
		count = array_.size();
		record << count;
		for (auto& item : array_)
		{
			record << item;
		}
	}
	else
	{
		unsigned int count;
		count = array_.size();
		record << count;
		for (auto& item : array_xlsb)
		{
			record << item;
		}
	}
}


const std::wstring PtgExtraMem::toString() const
{
	std::wstring range;
    if (global_info->Version < 0x0800)
    {
        for(std::vector<Ref8U>::const_iterator it = array_.begin(), itEnd = --array_.end(); it != itEnd; ++it)
        {
            range += it->toString() + L';';
        }
        return range + array_.back().toString();
    }
    else
    {
        for(std::vector<XLSB::UncheckedRfX>::const_iterator it = array_xlsb.begin(), itEnd = --array_xlsb.end(); it != itEnd; ++it)
        {
            range += it->toString() + L';';
        }
        return range + array_xlsb.back().toString();
    }

    return L"";
}


} // namespace XLS

