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
            auto pSUPADDIN = dynamic_cast<SUPADDIN*>(m_source.get());
            for(auto &pPlaceholderName : pSUPADDIN->m_arBrtPlaceholderName)
            {
                auto pBrtPlaceholderName = dynamic_cast<PlaceholderName*>(pPlaceholderName.get());
                if(pBrtPlaceholderName != nullptr && !pBrtPlaceholderName->name.value().empty())
                    arNames.push_back(pBrtPlaceholderName->name.value());
            }
        }
        elements_.pop_back();
        return true;
    }

	const bool SUP::saveContent(BinProcessor& proc)
	{		
		if (m_source->get_type() == XLS::typeSUPSAME)
		{
			auto pSUPSAME = dynamic_cast<SUPSAME*>(m_source.get());
			for (auto &pPlaceholderName : arNames)
			{
				XLS::BaseObjectPtr item(new PlaceholderName());
				auto pBrtPlaceholderName = dynamic_cast<PlaceholderName*>(item.get());
				if(pBrtPlaceholderName != nullptr)
				{
					pBrtPlaceholderName->name = pPlaceholderName;
					pSUPSAME->m_arBrtPlaceholderName.push_back(item);
				}
			}			
		}

		if (m_source->get_type() == XLS::typeSUPADDIN)
		{
			auto pSUPADDIN = dynamic_cast<SUPADDIN*>(m_source.get());
			for (auto &pPlaceholderName : arNames)
			{
				XLS::BaseObjectPtr item(new PlaceholderName());
				auto pBrtPlaceholderName = dynamic_cast<PlaceholderName*>(item.get());
				if (pBrtPlaceholderName != nullptr)
				{
					pBrtPlaceholderName->name = pPlaceholderName;
					pSUPADDIN->m_arBrtPlaceholderName.push_back(item);
				}
			}
		}

		if (m_source != nullptr)
			proc.mandatory(*m_source);

		return true;
	}

} // namespace XLSB

