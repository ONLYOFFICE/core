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

#include "TIMELINESTYLE.h"
#include "../Biff12_records/BeginTimelineStyle.h"
#include "../Biff12_records/BeginTimelineStyleElements.h"
#include "../Biff12_records/TimelineStyleElement.h"
#include "../Biff12_records/EndTimelineStyleElements.h"
#include "../Biff12_records/EndTimelineStyle.h"

using namespace XLS;

namespace XLSB
{

    TIMELINESTYLE::TIMELINESTYLE()
    {
    }

    TIMELINESTYLE::~TIMELINESTYLE()
    {
    }

    BaseObjectPtr TIMELINESTYLE::clone()
    {
        return BaseObjectPtr(new TIMELINESTYLE(*this));
    }

    //TIMELINESTYLE = BrtBeginTimelineStyle BrtBeginTimelineStyleElements *7BrtTimelineStyleElement
                //BrtEndTimelineStyleElements BrtEndTimelineStyle
    const bool TIMELINESTYLE::loadContent(BinProcessor& proc)
    {       
        if (proc.optional<BeginTimelineStyle>())
        {
            m_BrtBeginTimelineStyle = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginTimelineStyleElements>())
        {
            m_bBrtBeginTimelineStyleElements = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginTimelineStyleElements = false;

        int count = proc.repeated<TimelineStyleElement>(0, 7);

        while(count > 0)
        {
            m_arBrtTimelineStyleElement.insert(m_arBrtTimelineStyleElement.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndTimelineStyleElements>())
        {
            m_bBrtEndTimelineStyleElements = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndTimelineStyleElements = false;

        if (proc.optional<EndTimelineStyle>())
        {
            m_bBrtEndTimelineStyle = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndTimelineStyle = false;

        return m_BrtBeginTimelineStyle && m_bBrtBeginTimelineStyleElements && m_bBrtEndTimelineStyleElements && m_bBrtEndTimelineStyle;
    }

	const bool TIMELINESTYLE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginTimelineStyle != nullptr)
			proc.mandatory(*m_BrtBeginTimelineStyle);

		proc.mandatory<BeginTimelineStyleElements>();

		for (auto &item : m_arBrtTimelineStyleElement)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndTimelineStyleElements>();

		proc.mandatory<EndTimelineStyle>();

		return true;
	}

} // namespace XLSB

