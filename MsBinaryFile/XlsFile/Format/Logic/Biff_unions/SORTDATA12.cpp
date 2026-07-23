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

#include "SORTDATA12.h"
#include "../Biff_records/SortData.h"
#include "../Biff_records/ContinueFrt12.h"

namespace XLS
{


SORTDATA12::SORTDATA12()
{
}


SORTDATA12::~SORTDATA12()
{
}


BaseObjectPtr SORTDATA12::clone()
{
	return BaseObjectPtr(new SORTDATA12(*this));
}


// SORTDATA12 = SortData *ContinueFrt12
const bool SORTDATA12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SortData>())
	{
		return false;
	}
	m_SortData = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<ContinueFrt12>(0, 0); // processed inside of SortData

	return true;
}
const bool SORTDATA12::saveContent(BinProcessor& proc)
{
    if(m_SortData == nullptr)
        return false;
    proc.mandatory(*m_SortData);
	auto castedPtr = static_cast<SortData*>(m_SortData.get());
	for(auto i : castedPtr->sortCond12Array)
	{
		CFRecord binDataRec(rt_ContinueFrt12, proc.getGlobalWorkbookInfo());
		i->save(binDataRec);
		ContinueFrt12 tempRecord;
		tempRecord.rgb.resize(binDataRec.getRdPtr());
		auto copyData = binDataRec.getCurStaticData<char>() - binDataRec.getRdPtr();
		memcpy(tempRecord.rgb.data(), copyData, binDataRec.getRdPtr());
		proc.mandatory(tempRecord);
	}
    return true;
}

int SORTDATA12::serialize(std::wostream & stream)
{
	if (m_SortData == NULL) return 0;

	SortData *info = dynamic_cast<SortData*>(m_SortData.get());

	std::wstring ref = info->rfx;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"sortState")
		{	
			CP_XML_ATTR(L"ref", ref);
			if (info->fCol)				CP_XML_ATTR(L"columnSort",		1);
			if (info->fCaseSensitive)	CP_XML_ATTR(L"caseSensitive",	1);
			if (info->fAltMethod)		CP_XML_ATTR(L"sortMethod",		L"stroke");

			for (size_t i = 0 ; i < info->sortCond12Array.size(); i++)
			{
				SortCond12 * sortCond = dynamic_cast<SortCond12 *>(info->sortCond12Array[i].get());
				if (sortCond == NULL) continue;

				CP_XML_NODE(L"sortCondition")
				{
                    CP_XML_ATTR(L"ref", sortCond->rfx.toString());
				}
			}
		}
	}
	return 0;
}
} // namespace XLS

