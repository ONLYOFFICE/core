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

#include "COLORSCALE14.h"
#include "../Biff12_records/BeginColorScale14.h"
#include "../Biff12_unions/uCFVO14.h"
#include "../Biff12_records/Color14.h"
#include "../Biff12_records/EndColorScale14.h"

using namespace XLS;

namespace XLSB
{

    COLORSCALE14::COLORSCALE14()
    {
    }

    COLORSCALE14::~COLORSCALE14()
    {
    }

    BaseObjectPtr COLORSCALE14::clone()
    {
        return BaseObjectPtr(new COLORSCALE14(*this));
    }

    // COLORSCALE14 = BrtBeginColorScale14 ((2CFVO14 2BrtColor14) / (3CFVO14 3BrtColor14)) BrtEndColorScale14
    const bool COLORSCALE14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginColorScale14>())
        {
            m_bBrtBeginColorScale14 = true;
            elements_.pop_back();
        }        
        else
            return false;

        int countCFVO14 = proc.repeated<uCFVO14>(2, 3);

        while(countCFVO14 > 0)
        {
            m_arCFVO14.insert(m_arCFVO14.begin(), elements_.back());
            elements_.pop_back();
            countCFVO14--;
        }

        int countColor14 = proc.repeated<Color14>(2, 3);

        while(countColor14 > 0)
        {
            m_arBrtColor14.insert(m_arBrtColor14.begin(), elements_.back());
            elements_.pop_back();
            countColor14--;
        }

        if (proc.optional<EndColorScale14>())
        {
            m_bBrtEndColorScale14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndColorScale14 = false;

        return m_bBrtBeginColorScale14 && !m_arCFVO14.empty() && !m_arBrtColor14.empty() && m_bBrtEndColorScale14;
    }

	const bool COLORSCALE14::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginColorScale14>();

		for (auto &item : m_arCFVO14)
		{
			proc.mandatory(*item);
		}

		for (auto &item : m_arBrtColor14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndColorScale14>();

		return true;
	}

} // namespace XLSB

