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

#include "MDBLOCK.h"
#include "../Biff_records/MDB.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


MDBLOCK::MDBLOCK()
{
}


MDBLOCK::~MDBLOCK()
{
}


BaseObjectPtr MDBLOCK::clone()
{
	return BaseObjectPtr(new MDBLOCK(*this));
}


// MDBLOCK = MDB *ContinueFrt12
const bool MDBLOCK::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<MDB>())
	{
		return false;
	}
	m_MDB = elements_.back();	elements_.pop_back();
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

const bool MDBLOCK::saveContent(BinProcessor& proc)
{
	if(m_MDB == nullptr)
		return false;
	proc.mandatory(*m_MDB);
	auto castedPtr = static_cast<MDB*>(m_MDB.get());
	auto maxElemCount = 1026; //max size for one record storage
	if(castedPtr->rgmdir.size() > maxElemCount)
	{
		auto arraySize = castedPtr->rgmdir.size();
		auto Stpos = maxElemCount;
		while(Stpos+1 < arraySize)
		{
			auto tempLen = 0;
			if(arraySize < Stpos + maxElemCount)
				tempLen = arraySize - Stpos;
			else
				tempLen = maxElemCount;
			{
				ContinueFrt12 continueRecord;
				continueRecord.rgb.reserve(tempLen*8);
				CFRecord TempRecord(rt_ContinueFrt12, proc.getGlobalWorkbookInfo());
				for(auto i = Stpos; i < Stpos+ tempLen; i++)
					TempRecord << *(castedPtr->rgmdir[i]);
				auto copyData = TempRecord.getCurStaticData<char>() - TempRecord.getRdPtr();
				memcpy(continueRecord.rgb.data(), copyData, tempLen*8);
				proc.mandatory(continueRecord);
			}
			Stpos+=tempLen;
		}
	}
	return true;

}

} // namespace XLS

