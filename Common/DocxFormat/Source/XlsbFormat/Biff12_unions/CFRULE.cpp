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

#include "CFRULE.h"
#include "../Biff12_records/BeginCFRule.h"
#include "../Biff12_unions/COLORSCALE.h"
#include "../Biff12_unions/DATABAR.h"
#include "../Biff12_unions/ICONSET.h"
#include "../Biff12_unions/FRTCFRULE.h"
#include "../Biff12_records/EndCFRule.h"

using namespace XLS;

namespace XLSB
{

    CFRULE::CFRULE(const CellRef& cell_base_ref) : ref(cell_base_ref)
    {
    }

    CFRULE::~CFRULE()
    {
    }

    BaseObjectPtr CFRULE::clone()
    {
        return BaseObjectPtr(new CFRULE(*this));
    }

    // CFRULE = BrtBeginCFRule [COLORSCALE / DATABAR / ICONSET] FRTCFRULE BrtEndCFRule
    const bool CFRULE::loadContent(BinProcessor& proc)
    {
        BeginCFRule bCFRule(ref);
        if (proc.optional(bCFRule))
        {
            m_BrtBeginCFRule = elements_.back();
            elements_.pop_back();
        }

        if(proc.optional<COLORSCALE>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<DATABAR>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }
        else if(proc.optional<ICONSET>())
        {
            m_source = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<FRTCFRULE>())
        {
            m_FRTRULE = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndCFRule>())
        {
            m_BrtEndCFRule = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginCFRule && m_BrtEndCFRule;
    }

} // namespace XLSB

