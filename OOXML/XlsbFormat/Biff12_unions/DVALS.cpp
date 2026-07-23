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

#include "DVALS.h"
#include "../Biff12_records/BeginDVals.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_unions/ACDVALLIST.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/EndDVals.h"

using namespace XLS;

namespace XLSB
{

    DVALS::DVALS()
    {
    }

    DVALS::~DVALS()
    {
    }

    class Parenthesis_DVALS: public XLS::CompositeObject
        {
            BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DVALS)
        public:
            BaseObjectPtr clone()
            {
                return BaseObjectPtr(new Parenthesis_DVALS(*this));
            }

            const bool loadContent(XLS::BinProcessor& proc)
            {
                if (proc.optional<ACUID>())
                {
                    ////m_ACUID = elements_.back();
                    elements_.pop_back();
                }
                if (proc.optional<ACDVALLIST>())
                {
                    //m_ACDVALLIST = elements_.back();
                    elements_.pop_back();
                }
                if(proc.optional<DVal>())
                {
                    m_BrtDVal = elements_.back();
                    elements_.pop_back();
                }
                else return false;

                return true;
            };

            //BaseObjectPtr   m_ACUID;
            //BaseObjectPtr   m_ACDVALLIST;
            BaseObjectPtr	m_BrtDVal;
        };

    BaseObjectPtr DVALS::clone()
    {
        return BaseObjectPtr(new DVALS(*this));
    }

    // DVALS = BrtBeginDVals 1*65534([ACUID] [ACDVALLIST] BrtDVal) BrtEndDVals
    const bool DVALS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDVals>())
        {
            m_BrtBeginDVals = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<Parenthesis_DVALS>(0, 65534);
        while(count > 0)
        {
            m_arBrtDVal.insert(m_arBrtDVal.begin(), static_cast<Parenthesis_DVALS*>(elements_.back().get())->m_BrtDVal);
            elements_.pop_back();
            count--;
        }

		if (proc.optional<EndDVals>())
		{
			m_bBrtEndDVals = true;
			elements_.pop_back();
		}
		else
			m_bBrtEndDVals = false;

        return m_BrtBeginDVals && m_bBrtEndDVals;
    }

	const bool DVALS::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginDVals != nullptr)
			proc.mandatory(*m_BrtBeginDVals);

		for(auto& item : m_arBrtDVal)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDVals>();

		return true;
	}

} // namespace XLSB

