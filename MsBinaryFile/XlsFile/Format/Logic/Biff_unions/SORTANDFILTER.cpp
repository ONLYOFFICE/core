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

const bool SORTANDFILTER::saveContent(BinProcessor& proc)
{
    if(m_Sort != nullptr)
        proc.mandatory(*m_Sort);
    if(m_SORTDATA12 != nullptr)
        proc.mandatory(*m_SORTDATA12);
    if(m_FilterMode != nullptr)
        proc.mandatory(*m_FilterMode);
    if(m_DropDownObjIds != nullptr)
        proc.mandatory(*m_DropDownObjIds);
    if(m_AUTOFILTER != nullptr)
        proc.mandatory(*m_AUTOFILTER);
    return true;
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

