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
			//todoooo return sheets !!

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

void ChartParsedFormula::save(CFRecord& record)
{
	unsigned short cce = 0;
	record.reserveNunBytes(2);//cce
	auto rgceBeginPos = record.getRdPtr();
	rgce.save(record);
	cce = record.getRdPtr() - rgceBeginPos;
	record.RollRdPtrBack(cce+2);
	record << cce;
	record.skipNunBytes(cce);
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

