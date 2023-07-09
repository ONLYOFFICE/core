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

#include "CFRULE14.h"
#include "../Biff12_records/BeginCFRule14.h"
#include "../Biff12_unions/COLORSCALE14.h"
#include "../Biff12_unions/DATABAR14.h"
#include "../Biff12_unions/ICONSET14.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndCFRule14.h"

using namespace XLS;

namespace XLSB
{

    CFRULE14::CFRULE14()
    {
    }

    CFRULE14::~CFRULE14()
    {
    }

    BaseObjectPtr CFRULE14::clone()
    {
        return BaseObjectPtr(new CFRULE14(*this));
    }

    // CFRULE14 = BrtBeginCFRule14 [COLORSCALE14 / DATABAR14 / ICONSET14] *FRT BrtEndCFRule14
    const bool CFRULE14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginCFRule14>())
        {
            m_BrtBeginCFRule14 = elements_.back();
            elements_.pop_back();
        }

        if(proc.optional<COLORSCALE14>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<DATABAR14>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<ICONSET14>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }
        int count = proc.repeated<FRT>(0, 0);

        if(m_source && m_source->get_type() == XLS::typeDATABAR14 && m_BrtBeginCFRule14)
        {
            static_cast<DATABAR14*>(m_source.get())->iPri = static_cast<BeginCFRule14*>(m_BrtBeginCFRule14.get())->iPri;
        }

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndCFRule14>())
        {
			m_bBrtEndCFRule14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndCFRule14 = false;

        return m_BrtBeginCFRule14 && m_bBrtEndCFRule14;
    }

	const bool CFRULE14::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginCFRule14 != nullptr)		
			proc.mandatory(*m_BrtBeginCFRule14);

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		proc.mandatory<EndCFRule14>();

		return true;
	}

} // namespace XLSB

