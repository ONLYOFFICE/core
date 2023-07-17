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

