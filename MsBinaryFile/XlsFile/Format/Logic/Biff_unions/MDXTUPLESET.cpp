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

#include "MDXTUPLESET.h"

#include "../Biff_records/MDXTuple.h"
#include "../Biff_records/MDXSet.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


MDXTUPLESET::MDXTUPLESET()
{
}


MDXTUPLESET::~MDXTUPLESET()
{
}


BaseObjectPtr MDXTUPLESET::clone()
{
	return BaseObjectPtr(new MDXTUPLESET(*this));
}


// MDXTUPLESET = (MDXTuple / MDXSet) *ContinueFrt12
const bool MDXTUPLESET::loadContent(BinProcessor& proc)
{
	if(!proc.optional<MDXTuple>())
	{
		if(!proc.optional<MDXSet>())
		{
			return false;
		}
	}
	m_MDXVarious = elements_.back();	elements_.pop_back();
	
	proc.repeated<ContinueFrt12>(0, 0);
	return true;
}

const bool MDXTUPLESET::saveContent(BinProcessor& proc)
{
	if(m_MDXVarious == nullptr)
		return false;
	proc.mandatory(*m_MDXVarious);
	auto type = m_MDXVarious->get_type();
	if(type == typeMDXTuple)
	{
		auto tuple = static_cast<MDXTuple*>(m_MDXVarious.get());
		auto maxSize = 2050;
		if(tuple->cistr > maxSize)
		{
			auto currentPose = maxSize;
			while(currentPose < tuple->cistr)
			{

				CFRecord tempRecord(rt_ContinueFrt12, proc.getGlobalWorkbookInfo());
				auto limit = 0;
				if(currentPose + maxSize > tuple->cistr)
					limit = tuple->cistr;
				else
					limit = currentPose + maxSize;
				for(auto i = currentPose; i < limit; i++)
					tempRecord << *(tuple->rgistr[i]);
				{
					ContinueFrt12 continueRecord;
					auto capacity = 4*(limit - currentPose);
					continueRecord.rgb.reserve(capacity);
					memcpy(continueRecord.rgb.data(), tempRecord.getCurStaticData<char>(), capacity);
					proc.mandatory(continueRecord);
				}
				currentPose = limit;
			}
		}
	}
	else
	{
		auto set = static_cast<MDXSet*>(m_MDXVarious.get());
		auto maxSize = 2049;
		if(set->cistr > maxSize)
		{
			auto currentPose = maxSize;
			while(currentPose < set->cistr)
			{

				CFRecord tempRecord(rt_ContinueFrt12, proc.getGlobalWorkbookInfo());
				auto limit = 0;
				if(currentPose + maxSize > set->cistr)
					limit = set->cistr;
				else
					limit = currentPose + maxSize;
				for(auto i = currentPose; i < limit; i++)
					tempRecord << *(set->rgistr[i]);
				{
					ContinueFrt12 continueRecord;
					auto capacity = 4*(limit - currentPose);
					continueRecord.rgb.reserve(capacity);
					memcpy(continueRecord.rgb.data(), tempRecord.getCurStaticData<char>(), capacity);
					proc.mandatory(continueRecord);
				}
				currentPose = limit;
			}
		}
	}
	return true;
}

} // namespace XLS

