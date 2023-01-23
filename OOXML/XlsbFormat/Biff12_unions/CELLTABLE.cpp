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

#include "CELLTABLE.h"
#include "../Biff12_records/BeginSheetData.h"
#include "../Biff12_unions/ACCELLTABLE.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_unions/CELL.h"
#include "../Biff12_records/EndSheetData.h"
#include "../Biff12_records/CommonRecords.h"

using namespace XLS;

namespace XLSB
{

    CELLTABLE::CELLTABLE()
    {
    }

    CELLTABLE::~CELLTABLE()
    {
    }

    BaseObjectPtr CELLTABLE::clone()
    {
        return BaseObjectPtr(new CELLTABLE(*this));
    }

    //CELLTABLE = BrtBeginSheetData *1048576([ACCELLTABLE] BrtRowHdr *16384CELL *FRT) BrtEndSheetData
    const bool CELLTABLE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSheetData>())
        {
            m_BrtBeginSheetData = elements_.back();
            elements_.pop_back();
        }
        Parenthesis_CELLTABLE cell_group(shared_formulas_locations_ref_);
        /*while(proc.optional(cell_group))
        {
            m_arParenthesis_CELLTABLE.insert(m_arParenthesis_CELLTABLE.begin(), elements_.back());
            elements_.pop_back();
        }*/

        int countParenthesis_CELLTABLE = proc.repeated(cell_group, 0, 1048576);

        m_arParenthesis_CELLTABLE.reserve(countParenthesis_CELLTABLE);
        std::move(std::begin(elements_), std::end(elements_), std::back_inserter(m_arParenthesis_CELLTABLE));

        /*while(countParenthesis_CELLTABLE > 0)
        {
            m_arParenthesis_CELLTABLE.insert(m_arParenthesis_CELLTABLE.begin(), elements_.back());
            elements_.pop_back();
            countParenthesis_CELLTABLE--;
        }*/

        if (proc.optional<EndSheetData>())
        {
            m_BrtEndSheetData = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginSheetData && countParenthesis_CELLTABLE > 0 && m_BrtEndSheetData;
    }

    Parenthesis_CELLTABLE::Parenthesis_CELLTABLE(std::vector<CellRangeRef>& shared_formulas_locations_ref) :
                                shared_formulas_locations_ref_(shared_formulas_locations_ref)
    {
    }
    Parenthesis_CELLTABLE::~Parenthesis_CELLTABLE()
    {
    }
    BaseObjectPtr Parenthesis_CELLTABLE::clone()
    {
        return BaseObjectPtr(new Parenthesis_CELLTABLE(*this));
    }

    const bool Parenthesis_CELLTABLE::loadContent(XLS::BinProcessor& proc)
    {
        if (proc.optional<ACCELLTABLE>())
        {
            m_ACCELLTABLE = elements_.back();
            elements_.pop_back();
        }
        if(proc.optional<RowHdr>())
        {
            m_BrtRowHdr = elements_.back();
            elements_.pop_back();
        }
        else return false;


        CELL cell(static_cast<RowHdr*>(m_BrtRowHdr.get())->rw + 1, shared_formulas_locations_ref_);

        /*while(proc.optional(cell))
        {
            m_arCELL.insert(m_arCELL.begin(), elements_.back());
            elements_.pop_back();
        }*/

        int countCELL = proc.repeated(cell, 0, 16384);

        /*while(countCELL > 0)
        {
            m_arCELL.insert(m_arCELL.begin(), elements_.back());
            elements_.pop_back();
            countCELL--;
        }*/

        m_arCELL.reserve(countCELL);
        std::move(std::begin(elements_), std::end(elements_), std::back_inserter(m_arCELL));

        int countFRT = proc.repeated<FRT>(0, 0);

        while(countFRT > 0)
        {
           // m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            countFRT--;
        }

        return true;
    }

} // namespace XLSB

