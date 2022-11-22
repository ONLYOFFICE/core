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

#include "SUP.h"
#include "../Biff12_records/SupSelf.h"
#include "../Biff12_records/SupBookSrc.h"
#include "../Biff12_records/PlaceholderName.h"
#include "SUPSAME.h"
#include "SUPADDIN.h"

using namespace XLS;

namespace XLSB
{

    SUP::SUP()
    {
    }

    SUP::~SUP()
    {
    }

    BaseObjectPtr SUP::clone()
    {
        return BaseObjectPtr(new SUP(*this));
    }

    // SUP = BrtSupSelf/ SUPSAME / SUPADDIN / BrtSupBookSrc
    const bool SUP::loadContent(BinProcessor& proc)
    {
        if(!proc.optional<SupSelf>())
        {
            if(!proc.optional<SUPSAME>())
            {
                if(!proc.optional<SUPADDIN>())
                {
                    if(!proc.optional<SupBookSrc>())
                    {
                        return false;
                    }
                }
            }
        }
        m_source = elements_.back();
        if(m_source->get_type() == XLS::typeSUPSAME)
        {
            auto pSUPSAME = dynamic_cast<SUPSAME*>(m_source.get());
            for(auto &pPlaceholderName : pSUPSAME->m_arBrtPlaceholderName)
            {
                auto pBrtPlaceholderName = dynamic_cast<PlaceholderName*>(pPlaceholderName.get());
                if(pBrtPlaceholderName != nullptr && !pBrtPlaceholderName->name.value().empty())
                    arNames.push_back(pBrtPlaceholderName->name.value());
            }
        }

        if(m_source->get_type() == XLS::typeSUPADDIN)
        {
            auto pSUPSAME = dynamic_cast<SUPADDIN*>(m_source.get());
            for(auto &pPlaceholderName : pSUPSAME->m_arBrtPlaceholderName)
            {
                auto pBrtPlaceholderName = dynamic_cast<PlaceholderName*>(pPlaceholderName.get());
                if(pBrtPlaceholderName != nullptr && !pBrtPlaceholderName->name.value().empty())
                    arNames.push_back(pBrtPlaceholderName->name.value());
            }
        }
        elements_.pop_back();
        return true;
    }

} // namespace XLSB

