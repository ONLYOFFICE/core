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

#include "DATAFEEDPR15.h"
#include "../Biff12_records/BeginDataFeedPr15.h"
#include "../Biff12_unions/DBTABLES15.h"
#include "../Biff12_records/EndDataFeedPr15.h"

using namespace XLS;

namespace XLSB
{

    DATAFEEDPR15::DATAFEEDPR15()
    {
    }

    DATAFEEDPR15::~DATAFEEDPR15()
    {
    }

    BaseObjectPtr DATAFEEDPR15::clone()
    {
        return BaseObjectPtr(new DATAFEEDPR15(*this));
    }

    //DATAFEEDPR15 = BrtBeginDataFeedPr15 DBTABLES15 BrtEndDataFeedPr15
    const bool DATAFEEDPR15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginDataFeedPr15>())
        {
            m_BrtBeginDataFeedPr15 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<DBTABLES15>())
        {
            m_DBTABLES15 = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndDataFeedPr15>())
        {
			m_bBrtEndDataFeedPr15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndDataFeedPr15 = false;

        return m_BrtBeginDataFeedPr15 && m_DBTABLES15 && m_bBrtEndDataFeedPr15;
    }

	const bool DATAFEEDPR15::saveContent(XLS::BinProcessor & proc)
	{
		if(m_BrtBeginDataFeedPr15 != nullptr)
			proc.mandatory(*m_BrtBeginDataFeedPr15);

		if (m_DBTABLES15 != nullptr)
			proc.mandatory(*m_DBTABLES15);

		proc.mandatory<EndDataFeedPr15>();

		return true;
	}

} // namespace XLSB

