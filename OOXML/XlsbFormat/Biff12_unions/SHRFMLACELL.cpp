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

#include "SHRFMLACELL.h"
#include "../Biff12_records/CommonRecords.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/CellRef.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/CellRangeRef.h"

using namespace XLS;

namespace XLSB
{

    SHRFMLACELL::SHRFMLACELL(_INT32 row, _INT32 col, std::vector<XLS::CellRangeRef>& shared_formulas_locations_ref)
        : m_Row(row), m_Col(col), shared_formulas_locations_ref_(shared_formulas_locations_ref), m_sharedIndex(-1)
    {
    }

    SHRFMLACELL::~SHRFMLACELL()
    {
    }

    BaseObjectPtr SHRFMLACELL::clone()
    {
        return BaseObjectPtr(new SHRFMLACELL(*this));
    }

    // SHRFMLACELL = FMLACELL (BrtShrFmla / BrtArrFmla)
    const bool SHRFMLACELL::loadContent(BinProcessor& proc)
    {
        bool isShared = true;
        ShrFmla shrFmla(XLSB::RgceLoc(m_Row - 1, m_Col, true, true));
        ArrFmla arrFmla(XLSB::RgceLoc(m_Row - 1, m_Col, true, true));
        if(!proc.optional(shrFmla))
        {
            if(!proc.optional(arrFmla))
            {
                return false;
            }
            else
            {
                isShared = false;
            }
        }       

        m_sharedIndex = shared_formulas_locations_ref_.size();
        if(isShared)
            shared_formulas_locations_ref_.push_back(shrFmla.rfx);
        else
            shared_formulas_locations_ref_.push_back(arrFmla.rfx);

        m_source = elements_.back();
        elements_.pop_back();
        return true;
    }

	const bool SHRFMLACELL::saveContent(BinProcessor& proc)
	{
		if(_fmlacell != nullptr)
			proc.mandatory(*_fmlacell);

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		return true;
	}

} // namespace XLSB

