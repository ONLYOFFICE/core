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

#include "PIVOTRULES.h"
#include "../Biff12_records/BeginSXRules.h"
#include "../Biff12_unions/PIVOTRULE.h"
#include "../Biff12_records/EndSXRules.h"

using namespace XLS;

namespace XLSB
{

    PIVOTRULES::PIVOTRULES()
    {
    }

    PIVOTRULES::~PIVOTRULES()
    {
    }

    BaseObjectPtr PIVOTRULES::clone()
    {
        return BaseObjectPtr(new PIVOTRULES(*this));
    }

    //PIVOTRULES = BrtBeginSXRules PIVOTRULE BrtEndSxRules // скорее всего ошибка и должно быть *PIVOTRULE
    const bool PIVOTRULES::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginSXRules>())
        {
            m_BrtBeginSXRules = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<PIVOTRULE>())
        {
            m_PIVOTRULE = elements_.back();
            elements_.pop_back();
        }       

        if (proc.optional<EndSXRules>())
        {
            m_bBrtEndSXRules = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXRules = false;

        return m_BrtBeginSXRules && m_bBrtEndSXRules;
    }

	const bool PIVOTRULES::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginSXRules != nullptr)
			proc.mandatory(*m_BrtBeginSXRules);

		if (m_PIVOTRULE != nullptr)
			proc.mandatory(*m_PIVOTRULE);

		proc.mandatory<EndSXRules>();

		return true;
	}

} // namespace XLSB

