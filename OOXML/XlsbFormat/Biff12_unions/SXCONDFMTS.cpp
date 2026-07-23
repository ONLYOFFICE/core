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

#include "SXCONDFMTS.h"
#include "../Biff12_records/BeginSXCondFmts.h"
#include "../Biff12_unions/SXCONDFMT.h"
#include "../Biff12_records/EndSXCondFmts.h"

using namespace XLS;

namespace XLSB
{

    SXCONDFMTS::SXCONDFMTS()
    {
    }

    SXCONDFMTS::~SXCONDFMTS()
    {
    }

    BaseObjectPtr SXCONDFMTS::clone()
    {
        return BaseObjectPtr(new SXCONDFMTS(*this));
    }

    //SXCONDFMTS = BrtBeginSXCondFmts 1*SXCONDFMT BrtEndSXCondFmts
    const bool SXCONDFMTS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXCondFmts>())
        {
            m_BrtBeginSXCondFmts = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<SXCONDFMT>(0, 0);
        while(count > 0)
        {
            m_arSXCONDFMT.insert(m_arSXCONDFMT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSXCondFmts>())
        {
            m_bBrtEndSXCondFmts = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXCondFmts = false;

        return m_BrtBeginSXCondFmts && !m_arSXCONDFMT.empty() && m_bBrtEndSXCondFmts;
    }

	const bool SXCONDFMTS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXCondFmts == nullptr)
			m_BrtBeginSXCondFmts = XLS::BaseObjectPtr(new XLSB::BeginSXCondFmts());

		if (m_BrtBeginSXCondFmts != nullptr)
		{
			auto ptrBrtBeginSXCondFmts = static_cast<XLSB::BeginSXCondFmts*>(m_BrtBeginSXCondFmts.get());

			if (ptrBrtBeginSXCondFmts != nullptr)
				ptrBrtBeginSXCondFmts->csxcondfmts = m_arSXCONDFMT.size();

			proc.mandatory(*m_BrtBeginSXCondFmts);
		}

		for (auto &item : m_arSXCONDFMT)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSXCondFmts>();

		return true;
	}

} // namespace XLSB

