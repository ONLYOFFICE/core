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

#include "PCDHIERARCHIES.h"
#include "../Biff12_records/BeginPCDHierarchies.h"
#include "../Biff12_unions/PCDHIERARCHY.h"
#include "../Biff12_records/EndPCDHierarchies.h"

using namespace XLS;

namespace XLSB
{

    PCDHIERARCHIES::PCDHIERARCHIES()
    {
    }

    PCDHIERARCHIES::~PCDHIERARCHIES()
    {
    }

    BaseObjectPtr PCDHIERARCHIES::clone()
    {
        return BaseObjectPtr(new PCDHIERARCHIES(*this));
    }

    //PCDHIERARCHIES = BrtBeginPCDHierarchies 1*PCDHIERARCHY BrtEndPCDHierarchies
    const bool PCDHIERARCHIES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDHierarchies>())
        {
            m_BrtBeginPCDHierarchies = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDHIERARCHY>(0, 0);
        while(count > 0)
        {
            m_arPCDHIERARCHY.insert(m_arPCDHIERARCHY.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDHierarchies>())
        {
            m_bBrtEndPCDHierarchies = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDHierarchies = false;

        return m_BrtBeginPCDHierarchies && m_bBrtEndPCDHierarchies;
    }

	const bool PCDHIERARCHIES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDHierarchies == nullptr)
			m_BrtBeginPCDHierarchies = XLS::BaseObjectPtr(new XLSB::BeginPCDHierarchies());

		if (m_BrtBeginPCDHierarchies != nullptr)
		{
			auto ptrBrtBeginPCDHierarchies = static_cast<XLSB::BeginPCDHierarchies*>(m_BrtBeginPCDHierarchies.get());

			if (ptrBrtBeginPCDHierarchies != nullptr)
				ptrBrtBeginPCDHierarchies->cHier = m_arPCDHIERARCHY.size();

			proc.mandatory(*m_BrtBeginPCDHierarchies);
		}

		for (auto &item : m_arPCDHIERARCHY)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDHierarchies>();

		return true;
	}

} // namespace XLSB

