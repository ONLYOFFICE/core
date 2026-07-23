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

#include "FMTS.h"
#include "../Biff12_records/BeginFmts.h"
#include "../Biff12_unions/ACFMT.h"
#include "../Biff12_records/Fmt.h"
#include "../Biff12_records/EndFmts.h"

using namespace XLS;

namespace XLSB
{

    FMTS::FMTS()
    {
    }

    FMTS::~FMTS()
    {
    }

    BaseObjectPtr FMTS::clone()
    {
        return BaseObjectPtr(new FMTS(*this));
    }

    //FMTS = BrtBeginFmts 1*(2ACFMT / BrtFmt) BrtEndFmts
    const bool FMTS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginFmts>())
        {
            m_BrtBeginFmts = elements_.back();
            elements_.pop_back();
        }

        while(true)
        {
            if (proc.optional<Fmt>())
            {
				m_arBrtFmt.push_back(elements_.back());
                elements_.pop_back();
            }

            else if (proc.optional<ACFMT>())
            {
				m_arBrtFmt.push_back(elements_.back());
                elements_.pop_back();
            }

            else break;
        }

        if (proc.optional<EndFmts>())
        {
			m_bBrtEndFmts = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFmts = false;

        return m_BrtBeginFmts && !m_arBrtFmt.empty() && m_bBrtEndFmts;
    }

	const bool FMTS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginFmts == nullptr)
			m_BrtBeginFmts = XLS::BaseObjectPtr(new XLSB::BeginFmts());

		if (m_BrtBeginFmts != nullptr)
		{
			auto ptrBrtBeginFmts = static_cast<XLSB::BeginFmts*>(m_BrtBeginFmts.get());

			if (ptrBrtBeginFmts != nullptr)
				ptrBrtBeginFmts->cfmts = m_arBrtFmt.size();

			proc.mandatory(*m_BrtBeginFmts);
		}

		for (auto &item : m_arBrtFmt)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndFmts>();

		return true;
	}

} // namespace XLSB

