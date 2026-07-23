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

#include "INDEXEDCOLORS.h"
#include "../Biff12_records/BeginIndexedColors.h"
#include "../Biff12_records/IndexedColor.h"
#include "../Biff12_records/EndIndexedColors.h"

using namespace XLS;

namespace XLSB
{

    INDEXEDCOLORS::INDEXEDCOLORS()
    {
    }

    INDEXEDCOLORS::~INDEXEDCOLORS()
    {
    }

    BaseObjectPtr INDEXEDCOLORS::clone()
    {
        return BaseObjectPtr(new INDEXEDCOLORS(*this));
    }

    //INDEXEDCOLORS = BrtIndexedColors 64BrtIndexedColor BrtEndIndexedColors
    const bool INDEXEDCOLORS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginIndexedColors>())
        {
			m_bBrtBeginIndexedColors = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginIndexedColors = false;

        int count = proc.repeated<IndexedColor>(0, 64);

        while(count > 0)
        {
            m_arIndexedColor.insert(m_arIndexedColor.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndIndexedColors>())
        {
            m_bBrtEndIndexedColors = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndIndexedColors = false;

        return m_bBrtBeginIndexedColors && !m_arIndexedColor.empty() && m_bBrtEndIndexedColors;
    }

	const bool INDEXEDCOLORS::saveContent(XLS::BinProcessor & proc)
	{
		proc.mandatory<BeginIndexedColors>();

		for (auto &item : m_arIndexedColor)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndIndexedColors>();

		return true;
	}

} // namespace XLSB

