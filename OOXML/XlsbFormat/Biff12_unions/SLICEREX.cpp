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

#include "SLICEREX.h"
#include "../Biff12_records/BeginSlicerEx.h"
#include "../Biff12_records/EndSlicerEx.h"

using namespace XLS;

namespace XLSB
{

    SLICEREX::SLICEREX()
    {
    }

    SLICEREX::~SLICEREX()
    {
    }

    BaseObjectPtr SLICEREX::clone()
    {
        return BaseObjectPtr(new SLICEREX(*this));
    }

    // SLICEREX = BrtBeginSlicerEx BrtEndSlicerEx
    const bool SLICEREX::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSlicerEx>())
        {
            m_BrtBeginSlicerEx = elements_.back();
            elements_.pop_back();
        }     

        if (proc.optional<EndSlicerEx>())
        {
            m_bBrtEndSlicerEx = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSlicerEx = false;

        return m_BrtBeginSlicerEx && m_bBrtEndSlicerEx;
    }

	const bool SLICEREX::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginSlicerEx != nullptr)
			proc.mandatory(*m_BrtBeginSlicerEx);

		proc.mandatory<EndSlicerEx>();

		return true;
	}

} // namespace XLSB

