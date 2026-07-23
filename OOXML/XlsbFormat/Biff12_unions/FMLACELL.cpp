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

#include "FMLACELL.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/Fmla.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/CellRef.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/CellRangeRef.h"

using namespace XLS;

namespace XLSB
{

    FMLACELL::FMLACELL(_INT32 row, std::vector<CellRangeRef>& shared_formulas_locations_ref)
        : m_Row(row), shared_formulas_locations_ref_(shared_formulas_locations_ref), isShared(false), m_sharedIndex(-1)
    {
    }

    FMLACELL::~FMLACELL()
    {
    }

    BaseObjectPtr FMLACELL::clone()
    {
        return BaseObjectPtr(new FMLACELL(*this));
    }

    // FMLACELL = CELLMETA (BrtFmlaString/ BrtFmlaNum/ BrtFmlaBool/ BrtFmlaError)
    const bool FMLACELL::loadContent(BinProcessor& proc)
    {
        FmlaString  fmlaString;
        FmlaNum		fmlaNum;
        FmlaBool	fmlaBool;
        FmlaError	fmlaError;

        if(proc.optional(fmlaString))
        {
            m_Col = fmlaString.cell.column;
        }
        else if(proc.optional(fmlaNum))
        {
            m_Col = fmlaNum.cell.column;
        }
        else if(proc.optional(fmlaBool))
        {
            m_Col = fmlaBool.cell.column;
        }
        else if(proc.optional(fmlaError))
        {
            m_Col = fmlaError.cell.column;
        }
        else
        {
            return false;
        }

        for (size_t i = 0; i < shared_formulas_locations_ref_.size(); i++)
        {
            auto location = XLSB::RgceLoc(m_Row - 1, m_Col, true, true);
            if (shared_formulas_locations_ref_[i].inRange(location))
            {
                m_sharedIndex = i;
                isShared = true;
            }
        }

        m_source = elements_.back();
        elements_.pop_back();
        return true;
    }

	const bool FMLACELL::saveContent(BinProcessor& proc)
	{
		if (m_source != nullptr)
			proc.mandatory(*m_source);
		else
			return false;

		return true;
	}

} // namespace XLSB

