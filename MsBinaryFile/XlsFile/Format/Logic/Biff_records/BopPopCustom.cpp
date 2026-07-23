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

#include "BopPopCustom.h"

namespace XLS
{

BopPopCustom::BopPopCustom()
{
}


BopPopCustom::~BopPopCustom()
{
}


BaseObjectPtr BopPopCustom::clone()
{
	return BaseObjectPtr(new BopPopCustom(*this));
}


void BopPopCustom::readFields(CFRecord& record)
{
	record >> rggrbit;
}

void BopPopCustom::writeFields(CFRecord& record)
{
    record << rggrbit;
}

BiffStructurePtr BopPopCustomPiesIndices::clone()
{
	return BiffStructurePtr(new BopPopCustomPiesIndices(*this));
}


void BopPopCustomPiesIndices::load(CFRecord& record)
{
	record >> cxi;

	const unsigned short padding = 8 - cxi % 8;

	unsigned char bit_data = 0;

	for(unsigned short i = padding; i < cxi - 1 + padding; i++)
	{
		if(0 == i % 8 || padding == i)
		{
			record >> bit_data;
		}

		unsigned char mask = 1 << (7 - i % 8);

		if(0 != (bit_data & mask))
		{
			pie_indices.push_back(i - padding);
		}
	}
}

void BopPopCustomPiesIndices::save(CFRecord& record)
{
   // 1. Save the number of points
    record << cxi;

    const unsigned short padding = (8 - (cxi % 8)) % 8;
    const unsigned short total_bits = padding + cxi + 1; // +1 = final flag bit
    const unsigned short total_bytes = (total_bits + 7) / 8;

    // 2. Create array of zero bytes
    std::vector<unsigned char> rggrbit(total_bytes, 0);

    // 3. Set the point bits
    for (unsigned short idx : pie_indices)
    {
        if (idx >= cxi)
            continue; // ignore invalid indices

        unsigned short bit_pos = padding + idx;
        unsigned short byte_index = bit_pos / 8;
        unsigned short bit_in_byte = 7 - (bit_pos % 8); // MSB-first

        rggrbit[byte_index] |= (1 << bit_in_byte);
    }

    // 4. Set the final bit (last bit of last byte)
    bool no_secondary = pie_indices.empty();
    if (no_secondary)
    {
        rggrbit.back() |= 0x01; // LSB = 1
    }

    // 5. Write all bytes to stream
    for (unsigned char b : rggrbit)
    {
        record << b;
    }
}


} // namespace XLS

