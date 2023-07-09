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

#include "DELETEDNAMES.h"
#include "../Biff12_records/BeginDeletedNames.h"
#include "../Biff12_unions/DELETEDNAME.h"
#include "../Biff12_records/EndDeletedNames.h"

using namespace XLS;

namespace XLSB
{

    DELETEDNAMES::DELETEDNAMES()
    {
    }

    DELETEDNAMES::~DELETEDNAMES()
    {
    }

    BaseObjectPtr DELETEDNAMES::clone()
    {
        return BaseObjectPtr(new DELETEDNAMES(*this));
    }

    // DELETEDNAMES = BrtBeginDeletedNames 1*DELETEDNAME BrtEndDeletedNames
    const bool DELETEDNAMES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDeletedNames>())
        {
            m_BrtBeginDeletedNames = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<DELETEDNAME>(0, 0);
        while(count > 0)
        {
            m_arDELETEDNAME.insert(m_arDELETEDNAME.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndDeletedNames>())
        {
			m_bBrtEndDeletedNames = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDeletedNames = false;

        return m_BrtBeginDeletedNames && m_bBrtEndDeletedNames;
    }

	const bool DELETEDNAMES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginDeletedNames == nullptr)
			m_BrtBeginDeletedNames = XLS::BaseObjectPtr(new XLSB::BeginDeletedNames());

		if (m_BrtBeginDeletedNames != nullptr)
		{
			auto ptrBrtBeginDeletedNames = static_cast<XLSB::BeginDeletedNames*>(m_BrtBeginDeletedNames.get());

			if (ptrBrtBeginDeletedNames != nullptr)
				ptrBrtBeginDeletedNames->nCols = m_arDELETEDNAME.size();

			proc.mandatory(*m_BrtBeginDeletedNames);
		}

		for (auto &item : m_arDELETEDNAME)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndDeletedNames>();

		return true;
	}

} // namespace XLSB

