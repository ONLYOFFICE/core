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

#include "COLORPALETTE.h"
#include "../Biff12_records/BeginColorPalette.h"
#include "../Biff12_unions/INDEXEDCOLORS.h"
#include "../Biff12_unions/MRUCOLORS.h"
#include "../Biff12_records/EndColorPalette.h"

using namespace XLS;

namespace XLSB
{

    COLORPALETTE::COLORPALETTE()
    {
    }

    COLORPALETTE::~COLORPALETTE()
    {
    }

    BaseObjectPtr COLORPALETTE::clone()
    {
        return BaseObjectPtr(new COLORPALETTE(*this));
    }

    //COLORPALETTE = BrtBeginColorPalette [INDEXEDCOLORS] [MRUCOLORS] BrtEndColorPalette
    const bool COLORPALETTE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginColorPalette>())
        {
            m_bBrtBeginColorPalette = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginColorPalette = false;

        if (proc.optional<INDEXEDCOLORS>())
        {
            m_INDEXEDCOLORS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<MRUCOLORS>())
        {
            m_MRUCOLORS = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndColorPalette>())
        {
			m_bBrtEndColorPalette = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndColorPalette = false;

        return m_bBrtBeginColorPalette && m_bBrtEndColorPalette;
    }

	const bool COLORPALETTE::saveContent(XLS::BinProcessor & proc)
	{
		proc.mandatory<BeginColorPalette>();

		if (m_INDEXEDCOLORS != nullptr)
			proc.mandatory(*m_INDEXEDCOLORS);

		if (m_MRUCOLORS != nullptr)
			proc.mandatory(*m_MRUCOLORS);

		proc.mandatory<EndColorPalette>();

		return true;
	}

} // namespace XLSB

