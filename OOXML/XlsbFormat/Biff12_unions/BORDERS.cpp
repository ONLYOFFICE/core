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

#include "BORDERS.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginBorders.h"
#include "../Biff12_records/Border.h"
#include "../Biff12_records/EndBorders.h"

using namespace XLS;

namespace XLSB
{

    BORDERS::BORDERS()
    {
    }

    BORDERS::~BORDERS()
    {
    }

    BaseObjectPtr BORDERS::clone()
    {
        return BaseObjectPtr(new BORDERS(*this));
    }

    //BORDERS = BrtBeginBorders 1*65430BrtBorder BrtEndBorders
    const bool BORDERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginBorders>())
        {
            m_BrtBeginBorders = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<Border>(1, 65430);
        while(count > 0)
        {
            m_arBrtBorder.insert(m_arBrtBorder.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

		if (proc.optional<EndBorders>())
		{
			m_bBrtEndBorders = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndBorders = false;

        return m_BrtBeginBorders && !m_arBrtBorder.empty() && m_bBrtEndBorders;
    }

	const bool BORDERS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginBorders == nullptr)
			m_BrtBeginBorders = XLS::BaseObjectPtr(new XLSB::BeginBorders());

		if (m_BrtBeginBorders != nullptr)
		{
			auto ptrBrtBeginBorders = static_cast<XLSB::BeginBorders*>(m_BrtBeginBorders.get());

			if (ptrBrtBeginBorders != nullptr)
				ptrBrtBeginBorders->cborders = m_arBrtBorder.size();

			proc.mandatory(*m_BrtBeginBorders);
		}

		for (auto &item : m_arBrtBorder)
		{
			proc.mandatory(*item);
		}
		
		proc.mandatory<EndBorders>();

		return true;
	}

} // namespace XLSB

