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

#include "SLICERCACHESELECTIONS.h"
#include "../Biff12_records/BeginSlicerCacheSelections.h"
#include "../Biff12_records/SlicerCacheSelection.h"
#include "../Biff12_records/EndSlicerCacheSelections.h"

using namespace XLS;

namespace XLSB
{

    SLICERCACHESELECTIONS::SLICERCACHESELECTIONS()
    {
    }

    SLICERCACHESELECTIONS::~SLICERCACHESELECTIONS()
    {
    }

    BaseObjectPtr SLICERCACHESELECTIONS::clone()
    {
        return BaseObjectPtr(new SLICERCACHESELECTIONS(*this));
    }

    //SLICERCACHESELECTIONS = BrtBeginSlicerCacheSelections 1*2147483647BrtSlicerCacheSelection BrtEndSlicerCacheSelections
    const bool SLICERCACHESELECTIONS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerCacheSelections>())
        {
            m_BrtBeginSlicerCacheSelections = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SlicerCacheSelection>(0, 2147483647);
        while(count > 0)
        {
            m_arBrtSlicerCacheSelection.insert(m_arBrtSlicerCacheSelection.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSlicerCacheSelections>())
        {
            m_bBrtEndSlicerCacheSelections = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerCacheSelections = false;

        return m_BrtBeginSlicerCacheSelections && !m_arBrtSlicerCacheSelection.empty() && m_bBrtEndSlicerCacheSelections;
    }

	const bool SLICERCACHESELECTIONS::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSlicerCacheSelections == nullptr)
			m_BrtBeginSlicerCacheSelections = XLS::BaseObjectPtr(new XLSB::BeginSlicerCacheSelections());

		if (m_BrtBeginSlicerCacheSelections != nullptr)
		{
			auto ptrBrtBeginSlicerCacheSelections = static_cast<XLSB::BeginSlicerCacheSelections*>(m_BrtBeginSlicerCacheSelections.get());

			if (ptrBrtBeginSlicerCacheSelections != nullptr)
				ptrBrtBeginSlicerCacheSelections->cUniqueNames = m_arBrtSlicerCacheSelection.size();

			proc.mandatory(*m_BrtBeginSlicerCacheSelections);
		}

		for (auto &item : m_arBrtSlicerCacheSelection)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSlicerCacheSelections>();

		return true;
	}

} // namespace XLSB

