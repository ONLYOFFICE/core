/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
 */

#include "MDXSTR.h"
#include "../Biff_records/MDXStr.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


MDXSTR::MDXSTR()
{
}


MDXSTR::~MDXSTR()
{
}


BaseObjectPtr MDXSTR::clone()
{
	return BaseObjectPtr(new MDXSTR(*this));
}


// MDXSTR = MDXStr *ContinueFrt12
const bool MDXSTR::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDXStr>())
	{
		return false;
	}
	m_MDXStr = elements_.back();	elements_.pop_back();
	
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

const bool MDXSTR::saveContent(BinProcessor& proc)
{
	if(m_MDXStr == nullptr)
		return false;
	proc.mandatory(*m_MDXStr);
	auto maxStNameLen = 4104;
	auto castedPtr = static_cast<MDXStr*>(m_MDXStr.get());
	if(castedPtr->st.getSize() > maxStNameLen)
	{	auto StSize = castedPtr->st.getSize();
		auto Stpos = maxStNameLen;
		while(Stpos+1 < StSize)
		{
			auto tempLen = 0;
			if(StSize < Stpos + maxStNameLen)
				tempLen = StSize - Stpos;
			else
				tempLen = maxStNameLen;
			{
				ContinueFrt12 continueRecord;
				continueRecord.rgb.reserve(tempLen*2);
				memcpy(continueRecord.rgb.data(), (castedPtr->st.value().c_str() + Stpos), tempLen*2);
				proc.mandatory(continueRecord);
			}
			Stpos+=tempLen;
		}
	}
	return true;
}

} // namespace XLS

