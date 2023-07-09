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

#include "DXF14S.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginDXF14s.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndDXF14s.h"

using namespace XLS;

namespace XLSB
{

    DXF14S::DXF14S()
    {
    }

    DXF14S::~DXF14S()
    {
    }

    BaseObjectPtr DXF14S::clone()
    {
        return BaseObjectPtr(new DXF14S(*this));
    }

    //DXF14S = BrtFRTBegin BrtBeginDXF14s 1*2147483647BrtDXF14 BrtEndDXF14s BrtFRTEnd
    const bool DXF14S::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginDXF14s>())
        {
            m_BrtBeginDXF14s = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<DXF14>(0, 2147483647);
        while(count > 0)
        {
            m_arDXF14.insert(m_arDXF14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDXF14s>())
        {
            m_bBrtEndDXF14s = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDXF14s = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;

        return m_BrtBeginDXF14s && !m_arDXF14.empty() && m_bBrtEndDXF14s;
    }

	const bool DXF14S::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if (m_BrtBeginDXF14s == nullptr)
			m_BrtBeginDXF14s = XLS::BaseObjectPtr(new XLSB::BeginDXF14s());

		if (m_BrtBeginDXF14s != nullptr)
		{
			auto ptrBrtBeginDXF14s = static_cast<XLSB::BeginDXF14s*>(m_BrtBeginDXF14s.get());

			if (ptrBrtBeginDXF14s != nullptr)
				ptrBrtBeginDXF14s->cdxfs = m_arDXF14.size();

			proc.mandatory(*m_BrtBeginDXF14s);
		}

		proc.mandatory<EndDXF14s>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

