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

#include "DATACELL.h"
#include "../Biff12_records/Cell.h"

using namespace XLS;

namespace XLSB
{

    DATACELL::DATACELL()
    {
    }

    DATACELL::~DATACELL()
    {
    }

    BaseObjectPtr DATACELL::clone()
    {
        return BaseObjectPtr(new DATACELL(*this));
    }

    // DATACELL = CELLMETA (BrtCellBlank/ BrtCellRk/ BrtCellError/ BrtCellBool/ BrtCellReal/ BrtCellIsst/ BrtCellSt)
    const bool DATACELL::loadContent(BinProcessor& proc)
    {
        auto type = proc.getNextRecordType();
        switch (type)
        {
            case rt_CellBlank:
            {
                CellBlank   cellBlank;
                proc.optional(cellBlank);
                m_Col = cellBlank.cell.column;
                break;
            }
            case rt_CellRk:
            {
                CellRk		cellRk;
                proc.optional(cellRk);
                m_Col = cellRk.cell.column;
                break;
            }
            case rt_CellError:
            {
                CellError	cellError;
                proc.optional(cellError);
                m_Col = cellError.cell.column;
                break;
            }
            case rt_CellBool:
            {
                CellBool	cellBool;
                proc.optional(cellBool);
                m_Col = cellBool.cell.column;
                break;
            }
            case rt_CellReal:
            {
                CellReal	cellReal;
                proc.optional(cellReal);
                m_Col = cellReal.cell.column;
                break;
            }
            case rt_CellSt:
            {
                CellSt		cellSt;
                proc.optional(cellSt);
                m_Col = cellSt.cell.column;
                break;
            }
            case rt_CellIsst:
            {
                CellIsst    cellIsst;
                proc.optional(cellIsst);
                m_Col = cellIsst.cell.column;
                break;
            }
            default:
                return false;
        }
        m_source = std::move(elements_.back());
        elements_.pop_back();

        return true;
    }

	const bool DATACELL::saveContent(BinProcessor& proc)
	{
		if (m_source != nullptr)
			proc.mandatory(*m_source);
		else
			return false;

		return true;
	}

} // namespace XLSB
