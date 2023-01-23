/*
 * (c) Copyright Ascensio System SIA 2010-2021
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */

#include "FMLACELL.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/Fmla.h"
#include "../Biff12_structures/CellRef.h"

using namespace XLS;

namespace XLSB
{

    FMLACELL::FMLACELL(_INT32 row, std::vector<XLS::CellRangeRef>& shared_formulas_locations_ref)
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

} // namespace XLSB

