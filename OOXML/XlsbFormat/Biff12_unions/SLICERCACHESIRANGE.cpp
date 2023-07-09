﻿/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
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

