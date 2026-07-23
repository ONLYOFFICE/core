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

#include "MERGECELLS.h"
#include "../Biff12_records/BeginMergeCells.h"
#include "../Biff12_records/MergeCell.h"
#include "../Biff12_records/EndMergeCells.h"

using namespace XLS;

namespace XLSB
{

    MERGECELLS::MERGECELLS()
    {
    }

    MERGECELLS::~MERGECELLS()
    {
    }

    BaseObjectPtr MERGECELLS::clone()
    {
        return BaseObjectPtr(new MERGECELLS(*this));
    }

    // MERGECELLS = BrtBeginMergeCells 1*BrtMergeCell BrtEndMergeCells
    const bool MERGECELLS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginMergeCells>())
        {
            m_BrtBeginMergeCells = elements_.back();
            elements_.pop_back();
        }

        int count = dynamic_cast<BeginMergeCells*>(m_BrtBeginMergeCells.get())->cmcs;

        for (auto i = 0; i < count; i++)
        {
            if (proc.optional<MergeCell>())
            {
                m_arBrtMergeCell.push_back(elements_.back());
                elements_.pop_back();
            }
        }

        if (proc.optional<EndMergeCells>())
        {
			m_bBrtEndMergeCells = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndMergeCells = false;

        return m_BrtBeginMergeCells && !m_arBrtMergeCell.empty() && m_bBrtEndMergeCells;
    }

	const bool MERGECELLS::saveContent(BinProcessor& proc)
	{
		if(m_BrtBeginMergeCells == nullptr)
			m_BrtBeginMergeCells = XLS::BaseObjectPtr(new XLSB::BeginMergeCells());

		if (m_BrtBeginMergeCells != nullptr)
		{
			auto ptrBrtBeginMergeCells = static_cast<XLSB::BeginMergeCells*>(m_BrtBeginMergeCells.get());

			if (ptrBrtBeginMergeCells != nullptr)
				ptrBrtBeginMergeCells->cmcs = m_arBrtMergeCell.size();

			proc.mandatory(*m_BrtBeginMergeCells);
		}

		for (auto &item : m_arBrtMergeCell)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndMergeCells>();

		return true;
	}	

} // namespace XLSB

