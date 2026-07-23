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

#include "ACFILTERS.h"
#include "../Biff12_records/ACBegin.h"
#include "../Biff12_records/BeginFilters.h"
#include "../Biff12_records/Filter14.h"
#include "../Biff12_records/EndFilters.h"
#include "../Biff12_records/ACEnd.h"

using namespace XLS;

namespace XLSB
{

    ACFILTERS::ACFILTERS()
    {
    }

    ACFILTERS::~ACFILTERS()
    {
    }

    BaseObjectPtr ACFILTERS::clone()
    {
        return BaseObjectPtr(new ACFILTERS(*this));
    }

    //ACFILTERS = BrtACBegin BrtBeginFilters *BrtFilter14 BrtEndFilters BrtACEnd
    const bool ACFILTERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACBegin>())
        {
            m_BrtACBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginFilters>())
        {
            m_BrtBeginFilters = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<Filter14>(0, 0);

        while(count > 0)
        {
            m_arBrtFilter14.insert(m_arBrtFilter14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndFilters>())
        {
			m_bBrtEndFilters = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFilters = false;

        if (proc.optional<ACEnd>())
        {
            m_bBrtACEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtACEnd = false;

        return m_BrtBeginFilters && m_bBrtEndFilters && m_bBrtACEnd;
    }

	const bool ACFILTERS::saveContent(BinProcessor& proc)
	{		
		if (m_BrtACBegin != nullptr)		
			proc.mandatory(*m_BrtACBegin);
		else
			proc.mandatory<ACBegin>();

		if (m_BrtBeginFilters != nullptr)
			proc.mandatory(*m_BrtBeginFilters);

		for (auto &item : m_arBrtFilter14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndFilters>();

		proc.mandatory<ACEnd>();

		return true;
	}

} // namespace XLSB

