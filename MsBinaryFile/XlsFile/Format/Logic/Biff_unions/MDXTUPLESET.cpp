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
					auto CopyData =  tempRecord.getCurStaticData<char>() - tempRecord.getRdPtr();
					memcpy(continueRecord.rgb.data(), CopyData, capacity);
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
					auto CopyData =  tempRecord.getCurStaticData<char>() - tempRecord.getRdPtr();
					memcpy(continueRecord.rgb.data(), CopyData, capacity);
					proc.mandatory(continueRecord);
				}
				currentPose = limit;
			}
		}
	}
	return true;
}

} // namespace XLS

