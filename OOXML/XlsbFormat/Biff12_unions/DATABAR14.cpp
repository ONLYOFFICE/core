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

#include "DATABAR14.h"
#include "../Biff12_records/BeginDatabar14.h"
#include "../Biff12_unions/uCFVO14.h"
#include "../Biff12_records/Color14.h"
#include "../Biff12_records/EndDatabar14.h"

using namespace XLS;

namespace XLSB
{

    DATABAR14::DATABAR14()
    {
    }

    DATABAR14::~DATABAR14()
    {
    }

    BaseObjectPtr DATABAR14::clone()
    {
        return BaseObjectPtr(new DATABAR14(*this));
    }

    // DATABAR14 = BrtBeginDatabar14 2CFVO14 *5BrtColor14 BrtEndDatabar14
    const bool DATABAR14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDatabar14>())
        {
            m_BrtBeginDatabar14 = elements_.back();
            elements_.pop_back();
        }
        else
            return false;

        int count = proc.repeated<uCFVO14>(2, 2);

        while(count > 0)
        {
            m_arCFVO14.insert(m_arCFVO14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        int countColor14 = proc.repeated<Color14>(0, 5);

        while(countColor14 > 0)
        {
            m_arBrtColor14.insert(m_arBrtColor14.begin(), elements_.back());
            elements_.pop_back();
            countColor14--;
        }

        if (proc.optional<EndDatabar14>())
        {
            m_bBrtEndDatabar14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDatabar14 = false;

        return m_BrtBeginDatabar14 && !m_arCFVO14.empty() && m_bBrtEndDatabar14;
    }

	const bool DATABAR14::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginDatabar14 != nullptr)
			proc.mandatory(*m_BrtBeginDatabar14);

		for (auto &item : m_arCFVO14)
		{
			proc.mandatory(*item);
		}

		for (auto &item : m_arBrtColor14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDatabar14>();

		return true;
	}

} // namespace XLSB

