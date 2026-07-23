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

#include "BOOKVIEWS.h"
#include "../Biff12_records/BeginBookViews.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/EndBookViews.h"
#include "../Biff12_unions/ACUID.h"
#include "../Biff12_unions/FRT.h"

using namespace XLS;

namespace XLSB
{

    BOOKVIEWS::BOOKVIEWS()
    {
    }

    BOOKVIEWS::~BOOKVIEWS()
    {
    }

    class Parenthesis_BOOKVIEWS: public XLS::CompositeObject
    {
        BASE_OBJECT_DEFINE_CLASS_NAME(Parenthesis_BOOKVIEWS)
    public:
        BaseObjectPtr clone()
        {
            return BaseObjectPtr(new Parenthesis_BOOKVIEWS(*this));
        }

        const bool loadContent(XLS::BinProcessor& proc) override
        {
            if (proc.optional<ACUID>())
            {
                //m_ACUID = elements_.back();
                elements_.pop_back();
            }
            if(proc.optional<BookView>())
            {
                m_BrtBookView = elements_.back();
                elements_.pop_back();
            }
            else return false;

            int count = proc.repeated<FRT>(0, 0);
			elements_.clear();
            //while(count > 0)
            //{
            //    //m_arFRT.insert(m_arFRT.begin(), elements_.back());
            //    elements_.pop_back();
            //    count--;
            //}

            return true;
        }

        //BaseObjectPtr               m_ACUID;
        BaseObjectPtr               m_BrtBookView;
        //std::vector<XLS::BaseObjectPtr>  m_arFRT;
    };

    BaseObjectPtr BOOKVIEWS::clone()
    {
        return BaseObjectPtr(new BOOKVIEWS(*this));
    }

    // BOOKVIEWS = BrtBeginBookViews 1*([ACUID] BrtBookView *FRT) BrtEndBookViews
    const bool BOOKVIEWS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginBookViews>())
        {
            m_bBrtBeginBookViews = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginBookViews = false;

        int count = proc.repeated<Parenthesis_BOOKVIEWS>(0, 0);

        while(count > 0)
        {
            m_arBrtBookView.insert(m_arBrtBookView.begin(), static_cast<Parenthesis_BOOKVIEWS*>(elements_.back().get())->m_BrtBookView);
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndBookViews>())
        {
            m_bBrtEndBookViews = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndBookViews = false;

        return m_bBrtBeginBookViews && !m_arBrtBookView.empty() && m_bBrtEndBookViews;
    }

	const bool BOOKVIEWS::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginBookViews>();

		for (auto &item : m_arBrtBookView)
		{
			proc.mandatory(*item);
		}
		proc.mandatory<EndBookViews>();

		return true;
	}

} // namespace XLSB

