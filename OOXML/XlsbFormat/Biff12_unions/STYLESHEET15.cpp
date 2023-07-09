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

#include "STYLESHEET15.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/BeginTimelineStyleSheetExt15.h"
#include "../Biff12_unions/TIMELINESTYLES.h"
#include "../Biff12_records/EndTimelineStyleSheetExt15.h"
#include "../Biff12_records/FRTEnd.h"

using namespace XLS;

namespace XLSB
{

    STYLESHEET15::STYLESHEET15()
    {
    }

    STYLESHEET15::~STYLESHEET15()
    {
    }

    BaseObjectPtr STYLESHEET15::clone()
    {
        return BaseObjectPtr(new STYLESHEET15(*this));
    }

    // STYLESHEET15 = BrtFRTBegin BrtBeginTimelineStyleSheetExt15 TIMELINESTYLES
            //BrtEndTimelineStyleSheetExt15 BrtFRTEnd
    const bool STYLESHEET15::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }        

        if (proc.optional<BeginTimelineStyleSheetExt15>())
        {
            m_bBrtBeginTimelineStyleSheetExt15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginTimelineStyleSheetExt15 = false;

        if (proc.optional<TIMELINESTYLES>())
        {
            m_TIMELINESTYLES = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndTimelineStyleSheetExt15>())
        {
            m_bBrtEndTimelineStyleSheetExt15 = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndTimelineStyleSheetExt15 = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;


        return m_bBrtBeginTimelineStyleSheetExt15 && m_TIMELINESTYLES && m_bBrtEndTimelineStyleSheetExt15 && m_bBrtFRTEnd;
    }

	const bool STYLESHEET15::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();

		proc.mandatory<BeginTimelineStyleSheetExt15>();

		if (m_TIMELINESTYLES != nullptr)
			proc.mandatory(*m_TIMELINESTYLES);

		proc.mandatory<EndTimelineStyleSheetExt15>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

