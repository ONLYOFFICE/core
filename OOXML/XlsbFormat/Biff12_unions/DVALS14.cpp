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

#include "DVALS14.h"
#include "../Biff12_records/BeginDVals14.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_unions/ACDVALLIST.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndDVals14.h"

using namespace XLS;

namespace XLSB
{

    DVALS14::DVALS14()
    {
    }

    DVALS14::~DVALS14()
    {
    }

    class Parenthesis_DVALS14: public XLS::CompositeObject
        {
            BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_DVALS14)
        public:
            BaseObjectPtr clone()
            {
                return BaseObjectPtr(new Parenthesis_DVALS14(*this));
            }

            const bool loadContent(XLS::BinProcessor& proc)
            {               
                if (proc.optional<ACDVALLIST>())
                {
                    //m_ACDVALLIST = elements_.back();
                    elements_.pop_back();
                }
                if(proc.optional<DVal14>())
                {
                    m_BrtDVal14 = elements_.back();
                    elements_.pop_back();
                }
                else return false;

                return true;
            }

            //BaseObjectPtr   m_ACDVALLIST;
            BaseObjectPtr	m_BrtDVal14;
        };

    BaseObjectPtr DVALS14::clone()
    {
        return BaseObjectPtr(new DVALS14(*this));
    }

    // DVALS14 = BrtFRTBegin BrtBeginDVals14 1*65534([ACDVALLIST] BrtDVal14) BrtEndDVals14 BrtFRTEnd
    const bool DVALS14::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginDVals14>())
        {
            m_BrtBeginDVals14 = elements_.back();
            elements_.pop_back();
        }

        int count = proc.repeated<Parenthesis_DVALS14>(0, 65534);
        while(count > 0)
        {
            m_arBrtDVal14.insert(m_arBrtDVal14.begin(), static_cast<Parenthesis_DVALS14*>(elements_.back().get())->m_BrtDVal14);
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDVals14>())
        {
            m_bBrtEndDVals14 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDVals14 = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;

        return m_BrtBeginDVals14 && m_bBrtEndDVals14 && m_bBrtFRTEnd;
    }

	const bool DVALS14::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		proc.mandatory<BeginDVals14>();

		for (auto& item : m_arBrtDVal14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDVals14>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

