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

#include "SXLI.h"

namespace XLS
{

SXLI::SXLI(int count_)
{
	count = count_;
}


SXLI::~SXLI()
{
}

BaseObjectPtr SXLI::clone()
{
	return BaseObjectPtr(new SXLI(*this));
}

void SXLI::readFields(CFRecord& record)
{
	while(true)
	{
		int size_item = record.getDataSize() - record.getRdPtr(); 

		if (size_item < 8)
		{
			break;
		}
		SXLIItem item = {};
		
		unsigned short flags;

		record >> item.cSic >> item.itmType >> item.isxviMac >> flags;
		
		item.fMultiDataName		= GETBIT(flags, 0);
		item.iData				= GETBITS(flags, 1, 8);
		item.fSbt				= GETBIT(flags, 9);
		item.fBlock				= GETBIT(flags, 10);
		item.fGrand				= GETBIT(flags, 11);
		item.fMultiDataOnAxis	= GETBIT(flags, 12);

		if (item.fGrand)
			item.isxviMac = 1;

		if (item.cSic > item.isxviMac)
			item.cSic = item.isxviMac;

		if (item.fSbt && item.itmType < 0x000D)
			item.isxviMac++;

		for (short i = 0; i < count/*item.isxviMac*/; i++)
		{
			short val; record >> val;
			if (val >= 0x0000 && val <= 0x7EF4)
			{
				item.rgisxvi.push_back(val);
			}
		}

		m_arItems.push_back(item);
	}
}

void SXLIItem::save(CFRecord& record)
{
    unsigned short flags = 0;
    SETBIT(flags, 0, fMultiDataName)
    SETBITS(flags, 1, 8, iData)
    SETBIT(flags, 9, fSbt)
    SETBIT(flags, 10, fBlock)
    SETBIT(flags, 11, fGrand)
    SETBIT(flags, 12, fMultiDataOnAxis)
    if (fGrand)
        isxviMac = 1;
    record << cSic << itmType << isxviMac << flags;
    for(auto i : rgisxvi)
        record << i;
}

void SXLI::writeFields(CFRecord& record)
{
    const auto maxRecSize = 8224;
    while(!m_arItems.empty())
    {
        auto itemPose = record.getRdPtr();
        m_arItems.at(0).save(record);
        if(record.getRdPtr() > maxRecSize)
        {
            auto itemSize = record.getRdPtr() - itemPose;
            record.RollRdPtrBack(itemSize);
            return;
        }
        m_arItems.erase(m_arItems.begin());
    }
}

} // namespace XLS

