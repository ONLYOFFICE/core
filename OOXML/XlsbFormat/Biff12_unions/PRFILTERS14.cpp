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

#include "PRFILTERS14.h"
#include "../Biff12_records/BeginPRFilters14.h"
#include "../Biff12_unions/PRFILTER14.h"
#include "../Biff12_records/EndPRFilters14.h"

using namespace XLS;

namespace XLSB
{

    PRFILTERS14::PRFILTERS14()
    {
    }

    PRFILTERS14::~PRFILTERS14()
    {
    }

    BaseObjectPtr PRFILTERS14::clone()
    {
        return BaseObjectPtr(new PRFILTERS14(*this));
    }

    //PRFILTERS14 = BrtBeginPRFilters14 1*PRFILTER14 BrtEndPRFilters14
    const bool PRFILTERS14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPRFilters14>())
        {
            m_BrtBeginPRFilters14 = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PRFILTER14>(0, 0);
        while(count > 0)
        {
            m_arPRFILTER14.insert(m_arPRFILTER14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPRFilters14>())
        {
            m_bBrtEndPRFilters14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPRFilters14 = false;

        return m_BrtBeginPRFilters14 && !m_arPRFILTER14.empty() && m_bBrtEndPRFilters14;
    }

	const bool PRFILTERS14::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPRFilters14 == nullptr)
			m_BrtBeginPRFilters14 = XLS::BaseObjectPtr(new XLSB::BeginPRFilters14());

		if (m_BrtBeginPRFilters14 != nullptr)
		{
			auto ptrBrtBeginPRFilters14 = static_cast<XLSB::BeginPRFilters14*>(m_BrtBeginPRFilters14.get());

			if (ptrBrtBeginPRFilters14 != nullptr)
				ptrBrtBeginPRFilters14->cfilters = m_arPRFILTER14.size();

			proc.mandatory(*m_BrtBeginPRFilters14);
		}

		for (auto &item : m_arPRFILTER14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPRFilters14>();

		return true;
	}

} // namespace XLSB

