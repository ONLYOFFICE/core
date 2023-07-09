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

#include "PCDCALCITEMS.h"
#include "../Biff12_records/BeginPCDCalcItems.h"
#include "../Biff12_unions/PCDCALCITEM.h"
#include "../Biff12_records/EndPCDCalcItems.h"

using namespace XLS;

namespace XLSB
{

    PCDCALCITEMS::PCDCALCITEMS()
    {
    }

    PCDCALCITEMS::~PCDCALCITEMS()
    {
    }

    BaseObjectPtr PCDCALCITEMS::clone()
    {
        return BaseObjectPtr(new PCDCALCITEMS(*this));
    }

    //PCDCALCITEMS = BrtBeginPCDCalcItems 1*PCDCALCITEM BrtEndPCDCalcItems
    const bool PCDCALCITEMS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDCalcItems>())
        {
            m_BrtBeginPCDCalcItems = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDCALCITEM>(0, 0);
        while(count > 0)
        {
            m_arPCDCALCITEM.insert(m_arPCDCALCITEM.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDCalcItems>())
        {
            m_bBrtEndPCDCalcItems = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDCalcItems = false;

        return m_BrtBeginPCDCalcItems && m_bBrtEndPCDCalcItems;
    }

	const bool PCDCALCITEMS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDCalcItems == nullptr)
			m_BrtBeginPCDCalcItems = XLS::BaseObjectPtr(new XLSB::BeginPCDCalcItems());

		if (m_BrtBeginPCDCalcItems != nullptr)
		{
			auto ptrBrtBeginPCDCalcItems = static_cast<XLSB::BeginPCDCalcItems*>(m_BrtBeginPCDCalcItems.get());

			if (ptrBrtBeginPCDCalcItems != nullptr)
				ptrBrtBeginPCDCalcItems->cItems = m_arPCDCALCITEM.size();

			proc.mandatory(*m_BrtBeginPCDCalcItems);
		}

		for (auto &item : m_arPCDCALCITEM)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDCalcItems>();

		return true;
	}

} // namespace XLSB

