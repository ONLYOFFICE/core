/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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

#include "SORTANDFILTER.h"
#include "AUTOFILTER.h"
#include "SORTDATA12.h"
#include "../Biff_records/Sort.h"
#include "../Biff_records/FilterMode.h"
#include "../Biff_records/DropDownObjIds.h"

namespace XLS
{


SORTANDFILTER::SORTANDFILTER()
{
}


SORTANDFILTER::~SORTANDFILTER()
{
}


BaseObjectPtr SORTANDFILTER::clone()
{
	return BaseObjectPtr(new SORTANDFILTER(*this));
}


// SORTANDFILTER = [Sort] [SORTDATA12] [FilterMode] [DropDownObjIds] [AUTOFILTER]
const bool SORTANDFILTER::loadContent(BinProcessor& proc)
{
	bool res1 = proc.optional<Sort>();
	if (res1)
	{
		m_Sort = elements_.back();
		elements_.pop_back();
	}		
	bool res2 = proc.optional<SORTDATA12>();
	if (res2)
	{
		m_SORTDATA12 = elements_.back();
		elements_.pop_back();
	}
	bool res3 = proc.optional<FilterMode>();
	if (res3)
	{
		m_FilterMode = elements_.back();
		elements_.pop_back();
	}
	bool res4 = proc.optional<DropDownObjIds>();
	if (res4)
	{
		m_DropDownObjIds = elements_.back();
		elements_.pop_back();
	}
	bool res5 = proc.optional<AUTOFILTER>();
	if (res5)
	{
		m_AUTOFILTER = elements_.back();
		elements_.pop_back();
	}
	return res1 || res2 || res3 || res4 || res5;
}

int SORTANDFILTER::serialize(std::wostream & stream)
{
	if (m_AUTOFILTER)
	{
		m_AUTOFILTER->serialize(stream);
	}
	if (m_SORTDATA12)
	{
		m_SORTDATA12->serialize(stream);
	}
	return 0;
}

} // namespace XLS

