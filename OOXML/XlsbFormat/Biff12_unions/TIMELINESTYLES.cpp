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

