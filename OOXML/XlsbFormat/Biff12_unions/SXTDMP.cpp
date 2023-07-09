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

#include "SXTDMP.h"
#include "../Biff12_records/SXTDMPOrder.h"
#include "../Biff12_records/BeginSXTDMP.h"
#include "../Biff12_records/EndSXTDMP.h"

using namespace XLS;

namespace XLSB
{

    SXTDMP::SXTDMP()
    {
    }

    SXTDMP::~SXTDMP()
    {
    }

    BaseObjectPtr SXTDMP::clone()
    {
        return BaseObjectPtr(new SXTDMP(*this));
    }

    //SXTDMP = (BrtSXTDMPOrder / BrtBeginSXTDMP) BrtEndSXTDMP
    const bool SXTDMP::loadContent(BinProcessor& proc)
    {
        if (proc.optional<SXTDMPOrder>())
        {
            m_BrtSXTDMPOrder = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<BeginSXTDMP>())
        {
            m_BrtBeginSXTDMP = elements_.back();
            elements_.pop_back();
        }

        if (proc.optional<EndSXTDMP>())
        {
            m_bBrtEndSXTDMP = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndSXTDMP = false;

        return (m_BrtSXTDMPOrder || m_BrtBeginSXTDMP) && m_bBrtEndSXTDMP;
    }

	const bool SXTDMP::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtSXTDMPOrder != nullptr)
			proc.mandatory(*m_BrtSXTDMPOrder);

		if (m_BrtBeginSXTDMP != nullptr)
			proc.mandatory(*m_BrtBeginSXTDMP);

		proc.mandatory<EndSXTDMP>();

		return true;
	}

} // namespace XLSB

