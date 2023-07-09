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

#include "PCDFGDISCRETE.h"
#include "../Biff12_records/BeginPCDFGDiscrete.h"
#include "../Biff12_records/PCDIIndex.h"
#include "../Biff12_records/EndPCDFGDiscrete.h"

using namespace XLS;

namespace XLSB
{

    PCDFGDISCRETE::PCDFGDISCRETE()
    {
    }

    PCDFGDISCRETE::~PCDFGDISCRETE()
    {
    }

    BaseObjectPtr PCDFGDISCRETE::clone()
    {
        return BaseObjectPtr(new PCDFGDISCRETE(*this));
    }

    //PCDFGDISCRETE = BrtBeginPCDFGDiscrete 1*BrtPCDIIndex BrtEndPCDFGDiscrete
    const bool PCDFGDISCRETE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDFGDiscrete>())
        {
            m_BrtBeginPCDFGDiscrete = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDIIndex>(0, 0);
        while(count > 0)
        {
            m_arBrtPCDIIndex.insert(m_arBrtPCDIIndex.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDFGDiscrete>())
        {
            m_bBrtEndPCDFGDiscrete = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDFGDiscrete = false;

        return m_BrtBeginPCDFGDiscrete && m_bBrtEndPCDFGDiscrete;
    }

	const bool PCDFGDISCRETE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDFGDiscrete == nullptr)
			m_BrtBeginPCDFGDiscrete = XLS::BaseObjectPtr(new XLSB::BeginPCDFGDiscrete());

		if (m_BrtBeginPCDFGDiscrete != nullptr)
		{
			auto ptrBrtBeginPCDFGDiscrete = static_cast<XLSB::BeginPCDFGDiscrete*>(m_BrtBeginPCDFGDiscrete.get());

			if (ptrBrtBeginPCDFGDiscrete != nullptr)
				ptrBrtBeginPCDFGDiscrete->cItems = m_arBrtPCDIIndex.size();

			proc.mandatory(*m_BrtBeginPCDFGDiscrete);
		}

		for (auto &item : m_arBrtPCDIIndex)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDFGDiscrete>();

		return true;
	}

} // namespace XLSB

