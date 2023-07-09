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

