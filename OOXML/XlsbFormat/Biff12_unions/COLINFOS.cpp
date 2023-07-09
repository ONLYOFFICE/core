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

#include "COLINFOS.h"
#include "../Biff12_records/BeginColInfos.h"
#include "../Biff12_records/CommonRecords.h"
#include "../Biff12_records/EndColInfos.h"

using namespace XLS;

namespace XLSB
{

    COLINFOS::COLINFOS()
    {
    }

    COLINFOS::~COLINFOS()
    {
    }

    BaseObjectPtr COLINFOS::clone()
    {
        return BaseObjectPtr(new COLINFOS(*this));
    }

    // COLINFOS = m_BrtBeginColInfos ...
    const bool COLINFOS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginColInfos>())
        {
            m_bBrtBeginColInfos = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginColInfos = false;

        while (proc.optional<ColInfo>())
        {
            m_arBrtColInfo.push_back(elements_.back());
            elements_.pop_back();
        }

        if (proc.optional<EndColInfos>())
        {
            m_bBrtEndColInfos = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndColInfos = false;

        return m_bBrtBeginColInfos && !m_arBrtColInfo.empty() && m_bBrtEndColInfos;
    }

	const bool COLINFOS::saveContent(BinProcessor& proc)
	{
		proc.mandatory<BeginColInfos>();

		for (auto &item : m_arBrtColInfo)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndColInfos>();

		return true;
	}

} // namespace XLSB

