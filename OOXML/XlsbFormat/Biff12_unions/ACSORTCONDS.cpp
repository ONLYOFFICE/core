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

#include "ACSORTCONDS.h"
#include "../Biff12_records/ACBegin.h"
#include "../Biff12_unions/SORTCOND14.h"
#include "../Biff12_records/ACEnd.h"

using namespace XLS;

namespace XLSB
{

    ACSORTCONDS::ACSORTCONDS()
    {
    }

    ACSORTCONDS::~ACSORTCONDS()
    {
    }

    BaseObjectPtr ACSORTCONDS::clone()
    {
        return BaseObjectPtr(new ACSORTCONDS(*this));
    }

    // ACSORTCONDS = BrtACBegin *64SORTCOND14 BrtACEnd
    const bool ACSORTCONDS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<ACBegin>())
        {
            m_BrtACBegin = elements_.back();
            elements_.pop_back();
        }

        int countSORTCOND14 = proc.repeated<SORTCOND14>(0, 64);
        while(countSORTCOND14 > 0)
        {
            m_arSORTCOND14.insert(m_arSORTCOND14.begin(), elements_.back());
            elements_.pop_back();
            --countSORTCOND14;
        }

        if (proc.optional<ACEnd>())
        {
            m_bBrtACEnd = true;
            elements_.pop_back();
        }

        return m_BrtACBegin && !m_arSORTCOND14.empty() && m_bBrtACEnd;
    }

	const bool ACSORTCONDS::saveContent(BinProcessor& proc)
	{
		if (m_BrtACBegin != nullptr)
			proc.mandatory(*m_BrtACBegin);
		else
			proc.mandatory<ACBegin>();

		for (auto &item : m_arSORTCOND14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<ACEnd>();

		return true;
	}

} // namespace XLSB

