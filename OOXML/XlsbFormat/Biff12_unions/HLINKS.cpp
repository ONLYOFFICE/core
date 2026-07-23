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

#include "HLINKS.h"
#include "../Biff12_records/HLink.h"
#include "../Biff12_unions/ACUID.h"

using namespace XLS;

namespace XLSB
{

    HLINKS::HLINKS()
    {
    }

    HLINKS::~HLINKS()
    {
    }

    class Parenthesis_HLINKS: public XLS::CompositeObject
        {
            BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_HLINKS)
        public:
            BaseObjectPtr clone()
            {
                return BaseObjectPtr(new Parenthesis_HLINKS(*this));
            }

            const bool loadContent(XLS::BinProcessor& proc)
            {
                if (proc.optional<ACUID>())
                {
                    ////m_ACUID = elements_.back();
                    elements_.pop_back();
                }
                if(proc.optional<HLink>())
                {
                    m_BrtHLink = elements_.back();
                    elements_.pop_back();
                }
                else return false;

                return true;
            };

            //BaseObjectPtr   m_ACUID;
            BaseObjectPtr	m_BrtHLink;
        };

    BaseObjectPtr HLINKS::clone()
    {
        return BaseObjectPtr(new HLINKS(*this));
    }

    // HLINKS = 1*([ACUID] BrtHLink)
    const bool HLINKS::loadContent(BinProcessor& proc)
    {
        int count = proc.repeated<Parenthesis_HLINKS>(0, 0);
        while(count > 0)
        {
            m_arHlinks.insert(m_arHlinks.begin(), static_cast<Parenthesis_HLINKS*>(elements_.back().get())->m_BrtHLink);
            elements_.pop_back();
            count--;
        }

        return m_arHlinks.size() > 0;
    }

	const bool HLINKS::saveContent(BinProcessor& proc)
	{
		for (auto &item : m_arHlinks)
		{
			proc.mandatory(*item);
		}

		return true;
	}

} // namespace XLSB

