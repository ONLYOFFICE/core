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

#include "PRFILTERS.h"
#include "../Biff12_records/BeginPRFilters.h"
#include "../Biff12_unions/PRFILTER.h"
#include "../Biff12_records/EndPRFilters.h"

using namespace XLS;

namespace XLSB
{

    PRFILTERS::PRFILTERS()
    {
    }

    PRFILTERS::~PRFILTERS()
    {
    }

    BaseObjectPtr PRFILTERS::clone()
    {
        return BaseObjectPtr(new PRFILTERS(*this));
    }

    //PRFILTERS = BrtBeginPRFilters 1*PRFILTER BrtEndPRFilters
    const bool PRFILTERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPRFilters>())
        {
            m_BrtBeginPRFilters = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PRFILTER>(0, 0);
        while(count > 0)
        {
            m_arPRFILTER.insert(m_arPRFILTER.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPRFilters>())
        {
            m_bBrtEndPRFilters = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPRFilters = false;

        return m_BrtBeginPRFilters && !m_arPRFILTER.empty() && m_bBrtEndPRFilters;
    }

	const bool PRFILTERS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPRFilters == nullptr)
			m_BrtBeginPRFilters = XLS::BaseObjectPtr(new XLSB::BeginPRFilters());

		if (m_BrtBeginPRFilters != nullptr)
		{
			auto ptrBrtBeginPRFilters = static_cast<XLSB::BeginPRFilters*>(m_BrtBeginPRFilters.get());

			if (ptrBrtBeginPRFilters != nullptr)
				ptrBrtBeginPRFilters->cfilters = m_arPRFILTER.size();

			proc.mandatory(*m_BrtBeginPRFilters);
		}

		for (auto &item : m_arPRFILTER)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPRFilters>();

		return true;
	}

} // namespace XLSB

