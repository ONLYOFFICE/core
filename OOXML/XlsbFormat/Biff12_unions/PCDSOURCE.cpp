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

#include "PCDSOURCE.h"
#include "../Biff12_records/BeginPCDSource.h"
#include "../Biff12_unions/PCDSRANGE.h"
#include "../Biff12_unions/PCDSCONSOL.h"
#include "../Biff12_unions/PCDSOURCE14.h"
#include "../Biff12_records/EndPCDSource.h"

using namespace XLS;

namespace XLSB
{

    PCDSOURCE::PCDSOURCE()
    {
    }

    PCDSOURCE::~PCDSOURCE()
    {
    }

    BaseObjectPtr PCDSOURCE::clone()
    {
        return BaseObjectPtr(new PCDSOURCE(*this));
    }

    //PCDSOURCE = BrtBeginPCDSource [PCDSRANGE / PCDSCONSOL] PCDSOURCE14 BrtEndPCDSource
    const bool PCDSOURCE::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDSource>())
        {
            m_BrtBeginPCDSource = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSRANGE>())
        {
            m_PCDSRANGE = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSCONSOL>())
        {
            m_PCDSCONSOL = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<PCDSOURCE14>())
        {
            m_PCDSOURCE14 = elements_.back();
            elements_.pop_back();
        }
        if (proc.optional<EndPCDSource>())
        {
            m_bBrtEndPCDSource = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDSource = false;

        return m_BrtBeginPCDSource && m_bBrtEndPCDSource;
    }

	const bool PCDSOURCE::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDSource != nullptr)
			proc.mandatory(*m_BrtBeginPCDSource);

		if (m_PCDSRANGE != nullptr)
			proc.mandatory(*m_PCDSRANGE);

		if (m_PCDSCONSOL != nullptr)
			proc.mandatory(*m_PCDSCONSOL);

		if (m_PCDSOURCE14 != nullptr)
			proc.mandatory(*m_PCDSOURCE14);

		proc.mandatory<EndPCDSource>();

		return true;
	}

} // namespace XLSB

