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

#include "CELL.h"
#include "../Biff12_unions/DATACELL.h"
#include "../Biff12_unions/FMLACELL.h"
#include "../Biff12_unions/SHRFMLACELL.h"
#include "../Biff12_unions/TABLECELL.h"
#include "../Biff12_unions/CELLMETA.h"
#include "../Biff12_unions/FRT.h"

#include "../Biff12_records/ACBegin.h"

using namespace XLS;

namespace XLSB
{

    CELL::CELL(_INT32 row, std::vector<XLS::CellRangeRef>& shared_formulas_locations_ref)
        : m_Row(row), shared_formulas_locations_ref_(shared_formulas_locations_ref)
    {
    }

    CELL::~CELL()
    {
    }

    BaseObjectPtr CELL::clone()
    {
        return BaseObjectPtr(new CELL(*this));
    }

    // CELL = (DATACELL / FMLACELL / SHRFMLACELL / TABLECELL) *FRT
    const bool CELL::loadContent(BinProcessor& proc)
    {
        if(!proc.optional<TABLECELL>())
        {
            if(proc.optional<CELLMETA>())
            {
                m_CELLMETA = elements_.back();
                elements_.pop_back();
            }

            if(!proc.optional<DATACELL>())
            {
                FMLACELL fmlacell(m_Row, shared_formulas_locations_ref_);
                if(proc.optional(fmlacell))
                {
                    //m_FMLACELL = elements_.back();
                    m_source = elements_.back();
                    elements_.pop_back();
                    SHRFMLACELL shrfmlacell(m_Row, static_cast<FMLACELL*>(m_source.get())->m_Col, shared_formulas_locations_ref_);
                    if(proc.optional(shrfmlacell))
                    {
                        //m_SHRFMLACELL = elements_.back();
                        auto sharedfmla = static_cast<SHRFMLACELL*>(elements_.back().get());
                        sharedfmla->_fmlacell = m_source;
                        m_source = elements_.back();
                        elements_.pop_back();
                    }
                }
                else return false;
            }
            else
            {
                //m_DATACELL = elements_.back();
                m_source = elements_.back();
                elements_.pop_back();
            }
        }
        else
        {
            m_CELLMETA = static_cast<TABLECELL*>(elements_.back().get())->m_CELLMETA;
            //m_TABLECELL = elements_.back();
            m_source = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        return m_source != nullptr;
    }

} // namespace XLSB

