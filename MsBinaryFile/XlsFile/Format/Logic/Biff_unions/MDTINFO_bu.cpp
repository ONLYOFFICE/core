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

#include "MDTINFO.h"
#include "../Biff_records/MDTInfo.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


MDTINFO::MDTINFO()
{
}


MDTINFO::~MDTINFO()
{
}


BaseObjectPtr MDTINFO::clone()
{
	return BaseObjectPtr(new MDTINFO(*this));
}


// MDTINFO = MDTInfo *ContinueFrt12
const bool MDTINFO::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDTInfo>())
	{
		return false;
	}
	m_MDTInfo = elements_.back();	elements_.pop_back();
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

const bool MDTINFO::saveContent(BinProcessor& proc)
{
	constexpr auto maxStNameLen = 4104;
	if(m_MDTInfo == nullptr)
		return false;
	proc.mandatory(*m_MDTInfo);
	auto castedPtr = static_cast<MDTInfo*>(m_MDTInfo.get());
	if(castedPtr->stName.getSize() > maxStNameLen)
	{	auto NameSize = castedPtr->stName.getSize();
		auto Stpos = maxStNameLen;
		while(Stpos+1 < NameSize)
		{
			auto tempLen = 0;
			if(NameSize < Stpos + maxStNameLen)
				tempLen = NameSize - Stpos;
			else
				tempLen = maxStNameLen;
			{
				ContinueFrt12 continueRecord;
				continueRecord.rgb.reserve(tempLen*2);
				memcpy(continueRecord.rgb.data(), (castedPtr->stName.value().c_str() + Stpos), tempLen*2);
				proc.mandatory(continueRecord);
			}
			Stpos+=tempLen;
		}
	}
	return true;
}

} // namespace XLS

