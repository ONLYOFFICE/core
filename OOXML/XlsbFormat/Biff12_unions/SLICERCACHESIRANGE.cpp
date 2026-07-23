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

#include "SLICERCACHESIRANGE.h"
#include "../Biff12_records/BeginSlicerCacheSiRange.h"
#include "../Biff12_records/SlicerCacheOlapItem.h"
#include "../Biff12_records/EndSlicerCacheSiRange.h"

using namespace XLS;

namespace XLSB
{

    SLICERCACHESIRANGE::SLICERCACHESIRANGE()
    {
    }

    SLICERCACHESIRANGE::~SLICERCACHESIRANGE()
    {
    }

    BaseObjectPtr SLICERCACHESIRANGE::clone()
    {
        return BaseObjectPtr(new SLICERCACHESIRANGE(*this));
    }

    //SLICERCACHESIRANGE = BrtBeginSlicerCacheSiRange 1*1000BrtSlicerCacheOlapItem BrtEndSlicerCacheSiRange
    const bool SLICERCACHESIRANGE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerCacheSiRange>())
        {
            m_BrtBeginSlicerCacheSiRange = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SlicerCacheOlapItem>(0, 1000);
        while(count > 0)
        {
            m_arBrtSlicerCacheOlapItem.insert(m_arBrtSlicerCacheOlapItem.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSlicerCacheSiRange>())
        {
            m_bBrtEndSlicerCacheSiRange = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCacheSiRange = false;

        return m_BrtBeginSlicerCacheSiRange && !m_arBrtSlicerCacheOlapItem.empty() && m_bBrtEndSlicerCacheSiRange;
    }

	const bool SLICERCACHESIRANGE::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSlicerCacheSiRange == nullptr)
			m_BrtBeginSlicerCacheSiRange = XLS::BaseObjectPtr(new XLSB::BeginSlicerCacheSiRange());

		if (m_BrtBeginSlicerCacheSiRange != nullptr)
		{
			auto ptrBrtBeginSlicerCacheSiRange = static_cast<XLSB::BeginSlicerCacheSiRange*>(m_BrtBeginSlicerCacheSiRange.get());

			if (ptrBrtBeginSlicerCacheSiRange != nullptr)
				ptrBrtBeginSlicerCacheSiRange->crange = m_arBrtSlicerCacheOlapItem.size();

			proc.mandatory(*m_BrtBeginSlicerCacheSiRange);
		}

		for (auto &item : m_arBrtSlicerCacheOlapItem)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSlicerCacheSiRange>();

		return true;
	}

} // namespace XLSB

