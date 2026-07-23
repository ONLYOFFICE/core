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

#include "TABLECELL.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/Cell.h"
#include "CELLMETA.h"

using namespace XLS;

namespace XLSB
{

    TABLECELL::TABLECELL()
    {
    }

    TABLECELL::~TABLECELL()
    {
    }

    BaseObjectPtr TABLECELL::clone()
    {
        return BaseObjectPtr(new TABLECELL(*this));
    }

    // TABLECELL = BrtTable CELLMETA (BrtCellRk / BrtCellError / BrtCellBool / BrtCellReal / BrtCellSt)
    const bool TABLECELL::loadContent(BinProcessor& proc)
    {
        if(proc.optional<Table>())
        {
           m_BrtTable = elements_.back();
           elements_.pop_back();
        }
        else return false;

        if(proc.optional<CELLMETA>())
        {
           m_CELLMETA = elements_.back();
           elements_.pop_back();
        }

        CellRk		cellRk;
        CellError	cellError;
        CellBool	cellBool;
        CellReal	cellReal;
        CellSt		cellSt;

        if(proc.optional(cellRk))
        {
            m_Col = cellRk.cell.column;
        }
        else if(proc.optional(cellError))
        {
            m_Col = cellError.cell.column;
        }
        else if(proc.optional(cellBool))
        {
            m_Col = cellBool.cell.column;
        }
        else if(proc.optional(cellReal))
        {
            m_Col = cellReal.cell.column;
        }
        else if(proc.optional(cellSt))
        {
            m_Col = cellSt.cell.column;
        }
        else
        {
            return false;
        }
		
        m_source = elements_.back();
        elements_.pop_back();
        return true;
    }

	const bool TABLECELL::saveContent(BinProcessor& proc)
	{
		if (m_BrtTable != nullptr)		
			proc.mandatory(*m_BrtTable);
		else 
			return false;

		if (m_CELLMETA != nullptr)
			proc.mandatory(*m_CELLMETA);

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		return true;
	}

} // namespace XLSB

