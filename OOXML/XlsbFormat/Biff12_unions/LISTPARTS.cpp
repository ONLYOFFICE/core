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

#include "LISTPARTS.h"
#include "../Biff12_records/BeginListParts.h"
#include "../Biff12_records/ListPart.h"
#include "../Biff12_records/EndListParts.h"

using namespace XLS;

namespace XLSB
{

    LISTPARTS::LISTPARTS()
    {
    }

    LISTPARTS::~LISTPARTS()
    {
    }

    BaseObjectPtr LISTPARTS::clone()
    {
        return BaseObjectPtr(new LISTPARTS(*this));
    }

    // LISTPARTS = BrtBeginListParts *BrtListPart BrtEndListParts
    const bool LISTPARTS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginListParts>())
        {
            m_BrtBeginListParts = elements_.back();
            elements_.pop_back();
        }

        else return false;

        int countListPart = proc.repeated<ListPart>(0, static_cast<BeginListParts*>(m_BrtBeginListParts.get())->cParts);

        while(countListPart > 0)
        {
            m_arBrtListPart.insert(m_arBrtListPart.begin(), elements_.back());
            elements_.pop_back();
            countListPart--;
        }

        if (proc.optional<EndListParts>())
        {
			m_bBrtEndListParts = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndListParts = false;

        return m_BrtBeginListParts && !m_arBrtListPart.empty() && m_bBrtEndListParts;
    }

	const bool LISTPARTS::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginListParts == nullptr)
			m_BrtBeginListParts = XLS::BaseObjectPtr(new XLSB::BeginListParts());

		if (m_BrtBeginListParts != nullptr)
		{
			auto ptrBrtBeginListParts = static_cast<XLSB::BeginListParts*>(m_BrtBeginListParts.get());

			if (ptrBrtBeginListParts != nullptr)
				ptrBrtBeginListParts->cParts = m_arBrtListPart.size();

			proc.mandatory(*m_BrtBeginListParts);
		}

		for (auto &item : m_arBrtListPart)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndListParts>();

		return true;
	}

} // namespace XLSB

