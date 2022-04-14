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

#include "RICHFILTERS.h"
#include "../Biff12_records/BeginRichFilters.h"
#include "../Biff12_records/RichFilter.h"
#include "../Biff12_records/RichFilterDateGroupItem.h"
#include "../Biff12_unions/FRT.h"
#include "../Biff12_records/EndRichFilters.h"

using namespace XLS;

namespace XLSB
{

    RICHFILTERS::RICHFILTERS()
    {
    }

    RICHFILTERS::~RICHFILTERS()
    {
    }

    BaseObjectPtr RICHFILTERS::clone()
    {
        return BaseObjectPtr(new RICHFILTERS(*this));
    }

    // RICHFILTERS = BrtBeginRichFilters *BrtRichFilter *BrtRichFilterDateGroupItem *FRT BrtEndRichFilters
    const bool RICHFILTERS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginRichFilters>())
        {
            m_BrtBeginRichFilters = elements_.back();
            elements_.pop_back();
        }        

        while (proc.optional<RichFilter>())
        {
            m_arBrtRichFilter.push_back(elements_.back());
            elements_.pop_back();
        }

        while (proc.optional<RichFilterDateGroupItem>())
        {
            m_arBrtRichFilterDateGroupItem.push_back(elements_.back());
            elements_.pop_back();
        }

        int count = proc.repeated<FRT>(0, 0);

        while(count > 0)
        {
            //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndRichFilters>())
        {
            m_BrtEndRichFilters = elements_.back();
            elements_.pop_back();
        }

        return m_BrtBeginRichFilters && m_BrtEndRichFilters;
    }

} // namespace XLSB

