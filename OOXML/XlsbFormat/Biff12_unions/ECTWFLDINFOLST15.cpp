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

#include "ECTWFLDINFOLST15.h"
#include "../Biff12_records/BeginECTWFldInfoLst15.h"
#include "../Biff12_records/BeginECTwFldInfo15.h"
#include "../Biff12_records/EndECTWFldInfoLst15.h"

using namespace XLS;

namespace XLSB
{

    ECTWFLDINFOLST15::ECTWFLDINFOLST15()
    {
    }

    ECTWFLDINFOLST15::~ECTWFLDINFOLST15()
    {
    }

    BaseObjectPtr ECTWFLDINFOLST15::clone()
    {
        return BaseObjectPtr(new ECTWFLDINFOLST15(*this));
    }

    //ECTWFLDINFOLST15 = BrtBeginECTWFldInfoLst15 1*BrtBeginECTwFldInfo15 BrtEndECTWFldInfoLst15
    const bool ECTWFLDINFOLST15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginECTWFldInfoLst15>())
        {
            m_BrtBeginECTWFldInfoLst15 = elements_.back();
            elements_.pop_back();
        }

        int countBeginECTwFldInfo15 = proc.repeated<BeginECTwFldInfo15>(0, 0);

        while(countBeginECTwFldInfo15 > 0)
        {
            m_arBrtBeginECTwFldInfo15.insert(m_arBrtBeginECTwFldInfo15.begin(), elements_.back());
            elements_.pop_back();
            countBeginECTwFldInfo15--;
        }

        if (proc.optional<EndECTWFldInfoLst15>())
        {
			m_bBrtEndECTWFldInfoLst15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndECTWFldInfoLst15 = false;

        return m_BrtBeginECTWFldInfoLst15 && m_bBrtEndECTWFldInfoLst15;
    }

	const bool ECTWFLDINFOLST15::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginECTWFldInfoLst15 == nullptr)
			m_BrtBeginECTWFldInfoLst15 = XLS::BaseObjectPtr(new XLSB::BeginECTWFldInfoLst15());

		if (m_BrtBeginECTWFldInfoLst15 != nullptr)
		{
			auto ptrBrtBeginECTWFldInfoLst15 = static_cast<XLSB::BeginECTWFldInfoLst15*>(m_BrtBeginECTWFldInfoLst15.get());

			if (ptrBrtBeginECTWFldInfoLst15 != nullptr)
				ptrBrtBeginECTWFldInfoLst15->cFields = m_arBrtBeginECTwFldInfo15.size();

			proc.mandatory(*m_BrtBeginECTWFldInfoLst15);
		}

		for (auto &item : m_arBrtBeginECTwFldInfo15)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndECTWFldInfoLst15>();

		return true;
	}

} // namespace XLSB

