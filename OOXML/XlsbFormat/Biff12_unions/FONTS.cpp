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

#include "FONTS.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/BeginFonts.h"
#include "../Biff12_unions/ACFONTS.h"
#include "../Biff12_records/EndFonts.h"

using namespace XLS;

namespace XLSB
{

    FONTS::FONTS()
    {
    }

    FONTS::~FONTS()
    {
    }

    BaseObjectPtr FONTS::clone()
    {
        return BaseObjectPtr(new FONTS(*this));
    }

    //FONTS = BrtBeginFonts 1*65491BrtFont [ACFONTS] BrtEndFonts
    const bool FONTS::loadContent(BinProcessor& proc)
    {        
        //global_info = proc.getGlobalWorkbookInfo();

        if (proc.optional<BeginFonts>())
        {
            m_BrtBeginFonts = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<XLSB::Font>(1, 65491);
        while(count > 0)
        {
            //XLSB::Font *font = dynamic_cast<XLSB::Font *>(elements_.back().get());
            //if ((font) && (font->correct))
            //{
               // global_info->m_arFonts.push_back(elements_.back());
            //}
            m_arBrtFont.insert(m_arBrtFont.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<ACFONTS>())
        {
            m_ACFONTS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndFonts>())
        {
            m_bBrtEndFonts = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndFonts = false;

        return m_BrtBeginFonts && !m_arBrtFont.empty() && m_bBrtEndFonts;
    }

	const bool FONTS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginFonts == nullptr)
			m_BrtBeginFonts = XLS::BaseObjectPtr(new XLSB::BeginFonts());

		if (m_BrtBeginFonts != nullptr)
		{
			auto ptrBrtBeginFonts = static_cast<XLSB::BeginFonts*>(m_BrtBeginFonts.get());

			if (ptrBrtBeginFonts != nullptr)
				ptrBrtBeginFonts->cfonts = m_arBrtFont.size();

			proc.mandatory(*m_BrtBeginFonts);
		}

		for (auto &item : m_arBrtFont)
		{
			proc.mandatory(*item);
		}

		if (m_ACFONTS != nullptr)
			proc.mandatory(*m_ACFONTS);

		proc.mandatory<EndFonts>();

		return true;
	}

} // namespace XLSB

