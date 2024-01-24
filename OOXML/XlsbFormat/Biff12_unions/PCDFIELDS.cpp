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

#include "PCDFIELDS.h"
#include "../Biff12_records/BeginPCDFields.h"
#include "../Biff12_unions/PCDFIELD.h"
#include "../Biff12_records/EndPCDFields.h"

using namespace XLS;

namespace XLSB
{

    PCDFIELDS::PCDFIELDS()
    {
    }

    PCDFIELDS::~PCDFIELDS()
    {
    }

    BaseObjectPtr PCDFIELDS::clone()
    {
        return BaseObjectPtr(new PCDFIELDS(*this));
    }

    //PCDFIELDS = BrtBeginPCDFields *PCDFIELD BrtEndPCDFields
    const bool PCDFIELDS::loadContent(BinProcessor& proc)
    {
        if (proc.optional<BeginPCDFields>())
        {
            m_BrtBeginPCDFields = elements_.back();
            elements_.pop_back();
        }

        auto count = proc.repeated<PCDFIELD>(0, 0);
        if(count > 0)
            proc.getGlobalWorkbookInfo()->currentPivotCacheRecord++;
        while(count > 0)
        {
            m_arPCDFIELD.insert(m_arPCDFIELD.begin(), elements_.back());
            elements_.pop_back();
            count--;
        }

        if (proc.optional<EndPCDFields>())
        {
            m_bBrtEndPCDFields = true;
            elements_.pop_back();
        }
		else
			m_bBrtEndPCDFields = false;
        global_info = proc.getGlobalWorkbookInfo();

        return m_BrtBeginPCDFields && m_bBrtEndPCDFields;
    }

	const bool PCDFIELDS::saveContent(XLS::BinProcessor & proc)
	{
		if (m_BrtBeginPCDFields == nullptr)
			m_BrtBeginPCDFields = XLS::BaseObjectPtr(new XLSB::BeginPCDFields());

		if (m_BrtBeginPCDFields != nullptr)
		{
			auto ptrBrtBeginPCDFields = static_cast<XLSB::BeginPCDFields*>(m_BrtBeginPCDFields.get());

			if (ptrBrtBeginPCDFields != nullptr)
				ptrBrtBeginPCDFields->cFields = m_arPCDFIELD.size();

			proc.mandatory(*m_BrtBeginPCDFields);
		}

		for (auto &item : m_arPCDFIELD)
		{
			proc.mandatory(*item);
		}

		proc.mandatory<EndPCDFields>();

		return true;
	}

} // namespace XLSB

