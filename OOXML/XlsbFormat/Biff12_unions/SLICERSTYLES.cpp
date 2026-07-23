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

#include "SLICERSTYLES.h"
#include "../Biff12_records/BeginSlicerStyles.h"
#include "../Biff12_unions/SLICERSTYLE.h"
#include "../Biff12_records/EndSlicerStyles.h"

using namespace XLS;

namespace XLSB
{

    SLICERSTYLES::SLICERSTYLES()
    {
    }

    SLICERSTYLES::~SLICERSTYLES()
    {
    }

    BaseObjectPtr SLICERSTYLES::clone()
    {
        return BaseObjectPtr(new SLICERSTYLES(*this));
    }

    //SLICERSTYLES = BrtBeginSlicerStyles *2147483647SLICERSTYLE BrtEndSlicerStyles
    const bool SLICERSTYLES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerStyles>())
        {
            m_BrtBeginSlicerStyles = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<SLICERSTYLE>(0, 2147483647);

        while(count > 0)
        {
            m_arSLICERSTYLE.insert(m_arSLICERSTYLE.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndSlicerStyles>())
        {
            m_bBrtEndSlicerStyles = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerStyles = false;

        return m_BrtBeginSlicerStyles && m_bBrtEndSlicerStyles;
    }

	const bool SLICERSTYLES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSlicerStyles != nullptr)
			proc.mandatory(*m_BrtBeginSlicerStyles);

		for (auto &item : m_arSLICERSTYLE)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndSlicerStyles>();

		return true;
	}

} // namespace XLSB

