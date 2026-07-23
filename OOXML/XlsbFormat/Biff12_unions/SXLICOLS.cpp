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

#include "SXLICOLS.h"
#include "../Biff12_records/BeginSXLICols.h"
#include "../Biff12_unions/SXLI.h"
#include "../Biff12_records/EndSXLICols.h"

using namespace XLS;

namespace XLSB
{

    SXLICOLS::SXLICOLS()
    {
    }

    SXLICOLS::~SXLICOLS()
    {
    }

    BaseObjectPtr SXLICOLS::clone()
    {
        return BaseObjectPtr(new SXLICOLS(*this));
    }

    //SXLICOLS = BrtBeginSXLICols 1*SXLI BrtEndSXLICols
    const bool SXLICOLS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXLICols>())
        {
            m_BrtBeginSXLICols = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXLI>(0, 0);
        while(count > 0)
        {
            m_arSXLI.insert(m_arSXLI.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXLICols>())
        {
            m_bBrtEndSXLICols = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXLICols = false;

        return m_BrtBeginSXLICols && !m_arSXLI.empty() && m_bBrtEndSXLICols;
    }

	const bool SXLICOLS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXLICols == nullptr)
			m_BrtBeginSXLICols = XLS::BaseObjectPtr(new XLSB::BeginSXLICols());

		if (m_BrtBeginSXLICols != nullptr)
		{
			auto ptrBrtBeginSXLICols = static_cast<XLSB::BeginSXLICols*>(m_BrtBeginSXLICols.get());

			if (ptrBrtBeginSXLICols != nullptr)
				ptrBrtBeginSXLICols->csxlis = m_arSXLI.size();

			proc.mandatory(*m_BrtBeginSXLICols);
		}

		for (auto &item : m_arSXLI)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXLICols>();

		return true;
	}

} // namespace XLSB

