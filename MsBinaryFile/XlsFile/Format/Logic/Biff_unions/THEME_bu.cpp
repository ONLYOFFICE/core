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

#include "THEME.h"
#include "../Biff_records/Theme.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


THEME::THEME()
{
}


THEME::~THEME()
{
}


BaseObjectPtr THEME::clone()
{
	return BaseObjectPtr(new THEME(*this));
}


// THEME = Theme *ContinueFrt12
const bool THEME::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<Theme>())
	{
		return false;
	}
	m_Theme = elements_.back();
	elements_.pop_back();
	
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

const bool THEME::saveContent(BinProcessor& proc)
{
	if(m_Theme == nullptr)
		return false;
	proc.mandatory(*m_Theme);
	const auto maxSize = 8208;
	auto castedPtr = static_cast<Theme*>(m_Theme.get());
	if(castedPtr->nThemeDataSize > maxSize)
	{
		auto currentPos = maxSize;
		while(currentPos+1 < castedPtr->nThemeDataSize)
		{
			auto fragmentSize = 0;
			ContinueFrt12 continueRecord;
			if(currentPos + maxSize >= castedPtr->nThemeDataSize)
			{
				fragmentSize = castedPtr->nThemeDataSize - currentPos;
			}
			else
			{
				fragmentSize = maxSize;
			}
			continueRecord.rgb.reserve(fragmentSize);
			memcpy(continueRecord.rgb.data(), (castedPtr->pThemeData.get() + currentPos), fragmentSize);
			proc.mandatory(continueRecord);
			if(currentPos + maxSize >= castedPtr->nThemeDataSize)
			{
				break;
			}
			currentPos += maxSize;
		}
	}
	return true;
}

} // namespace XLS

