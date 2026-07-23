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

#include "EXTERNVALUEDATA.h"
#include "../Biff12_records/ExternCell.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/CellRef.h"

using namespace XLS;

namespace XLSB
{

    EXTERNVALUEDATA::EXTERNVALUEDATA(_INT32 row) : m_Row(row)
    {
    }

    EXTERNVALUEDATA::~EXTERNVALUEDATA()
    {
    }

    BaseObjectPtr EXTERNVALUEDATA::clone()
    {
        return BaseObjectPtr(new EXTERNVALUEDATA(*this));
    }

    // EXTERNVALUEDATA = BrtExternCellBlank / BrtExternCellReal / BrtExternCellBool / BrtExternCellError / BrtExternCellString
    const bool EXTERNVALUEDATA::loadContent(BinProcessor& proc)
    {
        ExternCellBlank   externCellBlank;
        ExternCellReal    externCellReal;
        ExternCellBool    externCellBool;
        ExternCellError	  externCellError;
        ExternCellString  externCellString;

        if(proc.optional(externCellBlank))
        {
            m_Col = externCellBlank.col;
        }
        else if(proc.optional(externCellReal))
        {
            m_Col = externCellReal.col;
        }
        else if(proc.optional(externCellBool))
        {
            m_Col = externCellBool.col;
        }
        else if(proc.optional(externCellError))
        {
            m_Col = externCellError.col;
        }
        else if(proc.optional(externCellString))
        {
            m_Col = externCellString.col;
        }
        else
        {
            return false;
        }

        m_Ref = XLSB::RgceLoc(m_Row, m_Col, true, true).toString();

        m_source = elements_.back();
        elements_.pop_back();
        return true;
    }

	const bool EXTERNVALUEDATA::saveContent(BinProcessor& proc)
	{
		if (m_source != nullptr)
			proc.mandatory(*m_source);
		else
			return false;

		return true;
	}

} // namespace XLSB
