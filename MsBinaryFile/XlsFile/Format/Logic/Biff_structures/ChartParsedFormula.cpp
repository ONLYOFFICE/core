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

#include "ChartParsedFormula.h"

#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

namespace XLS
{

ChartParsedFormula::ChartParsedFormula()
:	ParsedFormula(CellRef())
{
}


BiffStructurePtr ChartParsedFormula::clone()
{
	return BiffStructurePtr(new ChartParsedFormula(*this));
}

void ChartParsedFormula::load(CFRecord& record)
{
	unsigned short cce;

	record >> cce;	
	rgce.load(record, cce);

	if (cce > 0)
	{
		std::wstring strRef = getAssembledFormula();
	
		std::vector<std::wstring> arDistance;
		boost::algorithm::split(arDistance, strRef, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);
		
		BOOST_FOREACH(std::wstring &d, arDistance)
		{
			std::wstring sheet;
			std::vector<std::wstring> arRange;
			std::vector<std::wstring> arCells;

			boost::algorithm::split(arRange, d, boost::algorithm::is_any_of(L":"), boost::algorithm::token_compress_on);

			BOOST_FOREACH(std::wstring &c, arRange)
			{
				const ::std::string::size_type colon = c.find('!');
				arCells.push_back(c.substr(colon+1));
				if (sheet.size()<1)
					sheet=c.substr(0, colon);
			}
			//todoooo вернуть листы !!

			CellRef ref(arCells[0]);
			if (arCells.size() > 1)
			{
				CellRef ref2(arCells[1]);

				CellRangeRef range(ref, ref2);
				cell_ranges.push_back(range);
			}
			else
			{
				CellRangeRef range(ref, CellRangeRef::stSingleCell);
				cell_ranges.push_back(range);
			}
		}
	}
}

bool ChartParsedFormula::inRange(const CellRef & ref)
{
	for (size_t i = 0 ; i < cell_ranges.size(); i++)
	{
		if (cell_ranges[i].inRange(ref))
		{
			return true;
		}
	}

	return false;
}



} // namespace XLS

