﻿/*
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

