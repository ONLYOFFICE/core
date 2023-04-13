/*
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

#include "CONDITIONALFORMATTINGS.h"
#include "../Biff12_records/BeginConditionalFormattings.h"
#include "../Biff12_unions/CONDITIONALFORMATTING14.h"
#include "../Biff12_records/FRTBegin.h"
#include "../Biff12_records/FRTEnd.h"
#include "../Biff12_records/EndConditionalFormattings.h"

using namespace XLS;

namespace XLSB
{
	class EndConditionalFormatting;

	CONDITIONALFORMATTINGS::CONDITIONALFORMATTINGS()
    {
    }

    CONDITIONALFORMATTINGS::~CONDITIONALFORMATTINGS()
    {
    }

    BaseObjectPtr CONDITIONALFORMATTINGS::clone()
    {
        return BaseObjectPtr(new CONDITIONALFORMATTINGS(*this));
    }

    // CONDITIONALFORMATTINGS = BrtFRTBegin BrtBeginConditionalFormattings *2147483647CONDITIONALFORMATTING14
    //                                                                  BrtEndConditionalFormattings BrtFRTEnd
    const bool CONDITIONALFORMATTINGS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<FRTBegin>())
        {
            m_BrtFRTBegin = elements_.back();
            elements_.pop_back();
        }        

        if (proc.optional<BeginConditionalFormattings>())
        {
			m_bBrtBeginConditionalFormattings = true;
            elements_.pop_back();
        }
		else
			m_bBrtBeginConditionalFormattings = false;

        int count = proc.repeated<CONDITIONALFORMATTING14>(0, 2147483647);

        while(count > 0)
        {
            m_arCONDITIONALFORMATTING14.insert(m_arCONDITIONALFORMATTING14.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndConditionalFormattings>())
        {
            m_bBrtEndConditionalFormattings = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndConditionalFormattings = false;

        if (proc.optional<FRTEnd>())
        {
            m_bBrtFRTEnd = true;
            elements_.pop_back();
        }
		else
			m_bBrtFRTEnd = false;


        return m_BrtFRTBegin && m_bBrtBeginConditionalFormattings && !m_arCONDITIONALFORMATTING14.empty() && m_bBrtEndConditionalFormattings;
    }

	const bool CONDITIONALFORMATTINGS::saveContent(BinProcessor& proc)
	{
		if (m_BrtFRTBegin != nullptr)
			proc.mandatory(*m_BrtFRTBegin);
		else
			proc.mandatory<FRTBegin>();
		
		proc.mandatory<BeginConditionalFormattings>();

		for (auto& item : m_arCONDITIONALFORMATTING14)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndConditionalFormattings>();

		proc.mandatory<FRTEnd>();

		return true;
	}

} // namespace XLSB

