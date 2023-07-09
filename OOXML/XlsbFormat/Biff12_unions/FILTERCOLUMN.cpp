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

#include "FILTERCOLUMN.h"
#include "../Biff12_records/BeginFilterColumn.h"
#include "../Biff12_records/DynamicFilter.h"
#include "../Biff12_records/Top10Filter.h"
#include "../Biff12_records/ColorFilter.h"
#include "../Biff12_records/IconFilter.h"
#include "../Biff12_unions/CUSTOMFILTERS.h"
#include "../Biff12_unions/FILTERS.h"
#include "../Biff12_unions/ACFILTERCONTENT.h"
#include "../Biff12_unions/RICHFILTERCONTENT.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndFilterColumn.h"

using namespace XLS;

namespace XLSB
{

    FILTERCOLUMN::FILTERCOLUMN()
    {
    }

    FILTERCOLUMN::~FILTERCOLUMN()
    {
    }

    BaseObjectPtr FILTERCOLUMN::clone()
    {
        return BaseObjectPtr(new FILTERCOLUMN(*this));
    }

    // FILTERCOLUMN = BrtBeginFilterColumn [BrtDynamicFilter / BrtTop10Filter / BrtColorFilter /
    //      BrtIconFilter / CUSTOMFILTERS / FILTERS / ACFILTERCONTENT / RICHFILTERCONTENT] *FRT BrtEndFilterColumn
    const bool FILTERCOLUMN::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginFilterColumn>())
        {
            m_BrtBeginFilterColumn = elements_.back();
            elements_.pop_back();
        }

        bool flag = true;

        if (!proc.optional<DynamicFilter>())
            if (!proc.optional<Top10Filter>())
                if (!proc.optional<ColorFilter>())
                    if (!proc.optional<IconFilter>())
                        if (!proc.optional<CUSTOMFILTERS>())
                            if (!proc.optional<FILTERS>())
                                if (!proc.optional<ACFILTERCONTENT>())
                                    if (!proc.optional<RICHFILTERCONTENT>())
                                    {
                                        flag = false;
                                    }

        if(flag)
        {
            m_source = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndFilterColumn>())
        {
            m_bBrtEndFilterColumn = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFilterColumn = false;

        return m_BrtBeginFilterColumn && m_bBrtEndFilterColumn;
    }

	const bool FILTERCOLUMN::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginFilterColumn != nullptr)
			proc.mandatory(*m_BrtBeginFilterColumn);

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		proc.mandatory<EndFilterColumn>();

		return true;
	}

} // namespace XLSB

