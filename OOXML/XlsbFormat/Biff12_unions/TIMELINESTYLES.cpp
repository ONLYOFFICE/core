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

#include "TIMELINESTYLES.h"
#include "../Biff12_records/BeginTimelineStyles.h"
#include "../Biff12_unions/TIMELINESTYLE.h"
#include "../Biff12_records/EndTimelineStyles.h"

using namespace XLS;

namespace XLSB
{

    TIMELINESTYLES::TIMELINESTYLES()
    {
    }

    TIMELINESTYLES::~TIMELINESTYLES()
    {
    }

    BaseObjectPtr TIMELINESTYLES::clone()
    {
        return BaseObjectPtr(new TIMELINESTYLES(*this));
    }

    //TIMELINESTYLES = BrtBeginTimelineStyles *2147483647TIMELINESTYLE BrtEndTimelineStyles
    const bool TIMELINESTYLES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginTimelineStyles>())
        {
            m_BrtBeginTimelineStyles = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<TIMELINESTYLE>(0, 2147483647);

        while(count > 0)
        {
            m_arTIMELINESTYLE.insert(m_arTIMELINESTYLE.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndTimelineStyles>())
        {
            m_bBrtEndTimelineStyles = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndTimelineStyles = false;

        return m_BrtBeginTimelineStyles && m_bBrtEndTimelineStyles;
    }

	const bool TIMELINESTYLES::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginTimelineStyles != nullptr)
			proc.mandatory(*m_BrtBeginTimelineStyles);

		for (auto &item : m_arTIMELINESTYLE)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndTimelineStyles>();

		return true;
	}

} // namespace XLSB

