/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "ESSTR.h"
#include "../Biff12_records/BeginEsstr.h"
#include "../Biff12_records/Str.h"
#include "../Biff12_records/EndEsstr.h"


using namespace XLS;

namespace XLSB
{

    ESSTR::ESSTR()
    {
    }

    ESSTR::~ESSTR()
    {
    }

    BaseObjectPtr ESSTR::clone()
    {
        return BaseObjectPtr(new ESSTR(*this));
    }

    //ESSTR = [BrtESSTR] [BrtValueMeta]
    const bool ESSTR::loadContent(BinProcessor& proc)
    {
        m_BrtEndEsstr = false;
        if (proc.optional<BeginEsstr>())
        {
            m_BrtBeginEsstr = elements_.back();
            elements_.pop_back();
        }
        else 
            return false;

        int countStr = proc.repeated<Str>(0, 0);

        while(countStr > 0)
        {
            m_BrtStrs.insert(m_BrtStrs.begin(), elements_.back());
            elements_.pop_back();
            countStr--;
        }

        if (proc.optional<EndEsstr>())
        {
			m_BrtEndEsstr = true;
            elements_.pop_back();
        }
		else
			m_BrtEndEsstr = false;

        return m_BrtBeginEsstr && m_BrtEndEsstr;
    }

	const bool ESSTR::saveContent(BinProcessor& proc)
	{
		if (m_BrtBeginEsstr == nullptr)
        {
            auto beginPtr(new BeginEsstr);
            beginPtr->Cstr = m_BrtStrs.size();
            m_BrtBeginEsstr = BaseObjectPtr(beginPtr);
        }
        proc.mandatory(*m_BrtBeginEsstr);

		for(auto i : m_BrtStrs)
            proc.mandatory(*i);
            
        proc.mandatory<EndEsstr>();
		return true;
	}

} // namespace XLSB

