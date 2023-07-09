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

#include "DXFS15.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginDXFs15.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndDXFs15.h"

using namespace XLS;

namespace XLSB
{

    DXFS15::DXFS15()
    {
    }

    DXFS15::~DXFS15()
    {
    }

    BaseObjectPtr DXFS15::clone()
    {
        return BaseObjectPtr(new DXFS15(*this));
    }

    //DXFS15 = BrtFRTBegin BrtBeginDXFs15 1*2147483647BrtDXF14 BrtEndDXFs15 BrtFRTEnd
    const bool DXFS15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginDXFs15>())
        {
            m_BrtBeginDXFs15 = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<DXF15>(0, 2147483647);
        while(count > 0)
        {
            m_arDXF15.insert(m_arDXF15.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDXFs15>())
        {
            m_bBrtEndDXFs15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDXFs15 = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;

        return m_BrtBeginDXFs15 && !m_arDXF15.empty() && m_bBrtEndDXFs15;
    }

	const bool DXFS15::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		if (m_BrtBeginDXFs15 == nullptr)
			m_BrtBeginDXFs15 = XLS::BaseObjectPtr(new XLSB::BeginDXFs15());

		if (m_BrtBeginDXFs15 != nullptr)
		{
			auto ptrBrtBeginDXFs15 = static_cast<XLSB::BeginDXFs15*>(m_BrtBeginDXFs15.get());

			if (ptrBrtBeginDXFs15 != nullptr)
				ptrBrtBeginDXFs15->cdxfs = m_arDXF15.size();

			proc.mandatory(*m_BrtBeginDXFs15);
		}

		proc.mandatory<EndDXFs15>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

