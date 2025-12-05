/*
 * (c) Copyright Ascensio System SIA 2010-2023
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

