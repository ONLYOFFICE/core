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

