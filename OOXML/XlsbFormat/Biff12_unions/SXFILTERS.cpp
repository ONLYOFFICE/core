/*
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

#include "SXFILTERS.h"
#include "../Biff12_records/BeginSXFilters.h"
#include "../Biff12_unions/SXFILTER.h"
#include "../Biff12_records/EndSXFilters.h"

using namespace XLS;

namespace XLSB
{

    SXFILTERS::SXFILTERS()
    {
    }

    SXFILTERS::~SXFILTERS()
    {
    }

    BaseObjectPtr SXFILTERS::clone()
    {
        return BaseObjectPtr(new SXFILTERS(*this));
    }

    //SXFILTERS = BrtBeginSXFilters 1*SXFILTER BrtEndSXFilters
    const bool SXFILTERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXFilters>())
        {
            m_BrtBeginSXFilters = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXFILTER>(0, 0);
        while(count > 0)
        {
            m_arSXFILTER.insert(m_arSXFILTER.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXFilters>())
        {
            m_bBrtEndSXFilters = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXFilters = false;

        return m_BrtBeginSXFilters && !m_arSXFILTER.empty() && m_bBrtEndSXFilters;
    }

	const bool SXFILTERS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXFilters == nullptr)
			m_BrtBeginSXFilters = XLS::BaseObjectPtr(new XLSB::BeginSXFilters());

		if (m_BrtBeginSXFilters != nullptr)
		{
			auto ptrBrtBeginSXFilters = static_cast<XLSB::BeginSXFilters*>(m_BrtBeginSXFilters.get());

			if (ptrBrtBeginSXFilters != nullptr)
				ptrBrtBeginSXFilters->csxfilter = m_arSXFILTER.size();

			proc.mandatory(*m_BrtBeginSXFilters);
		}

		for (auto &item : m_arSXFILTER)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXFilters>();

		return true;
	}

} // namespace XLSB

