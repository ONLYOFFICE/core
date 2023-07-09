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

#include "PCDCALCMEMSEXT.h"
#include "../Biff12_records/BeginPCDCalcMemsExt.h"
#include "../Biff12_unions/PCDCALCMEMEXT.h"
#include "../Biff12_records/EndPCDCalcMemsExt.h"

using namespace XLS;

namespace XLSB
{

    PCDCALCMEMSEXT::PCDCALCMEMSEXT()
    {
    }

    PCDCALCMEMSEXT::~PCDCALCMEMSEXT()
    {
    }

    BaseObjectPtr PCDCALCMEMSEXT::clone()
    {
        return BaseObjectPtr(new PCDCALCMEMSEXT(*this));
    }

    //PCDCALCMEMSEXT = BrtBeginPCDCalcMemsExt 1*2147483647PCDCALCMEMEXT BrtEndPCDCalcMemsExt
    const bool PCDCALCMEMSEXT::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDCalcMemsExt>())
        {
            m_BrtBeginPCDCalcMemsExt = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDCALCMEMEXT>(0, 2147483647);
        while(count > 0)
        {
            m_arPCDCALCMEMEXT.insert(m_arPCDCALCMEMEXT.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDCalcMemsExt>())
        {
            m_bBrtEndPCDCalcMemsExt = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDCalcMemsExt = false;

        return m_BrtBeginPCDCalcMemsExt && !m_arPCDCALCMEMEXT.empty() && m_bBrtEndPCDCalcMemsExt;
    }

	const bool PCDCALCMEMSEXT::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDCalcMemsExt == nullptr)
			m_BrtBeginPCDCalcMemsExt = XLS::BaseObjectPtr(new XLSB::BeginPCDCalcMemsExt());

		if (m_BrtBeginPCDCalcMemsExt != nullptr)
		{
			auto ptrBrtBeginPCDCalcMemsExt = static_cast<XLSB::BeginPCDCalcMemsExt*>(m_BrtBeginPCDCalcMemsExt.get());

			if (ptrBrtBeginPCDCalcMemsExt != nullptr)
				ptrBrtBeginPCDCalcMemsExt->cCalcMems = m_arPCDCALCMEMEXT.size();

			proc.mandatory(*m_BrtBeginPCDCalcMemsExt);
		}

		for (auto &item : m_arPCDCALCMEMEXT)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDCalcMemsExt>();

		return true;
	}

} // namespace XLSB

