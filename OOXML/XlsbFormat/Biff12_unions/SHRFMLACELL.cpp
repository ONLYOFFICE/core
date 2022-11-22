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

#include "SHRFMLACELL.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_structures/CellRef.h"

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

} // namespace XLSB

