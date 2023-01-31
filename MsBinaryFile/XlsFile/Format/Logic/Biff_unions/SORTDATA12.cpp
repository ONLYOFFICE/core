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

