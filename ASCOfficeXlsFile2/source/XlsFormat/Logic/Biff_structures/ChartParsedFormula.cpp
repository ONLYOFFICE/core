
#include "ChartParsedFormula.h"
#include <Binary/CFRecord.h>

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


void ChartParsedFormula::store(CFRecord& record)
{
	record.registerDelayedDataReceiver(NULL, sizeof(unsigned short)/*cce*/);
	rgce.store(record);
	record.registerDelayedDataSource(rgce.getCCE(), rt_Formula);
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
	for (int i = 0 ; i < cell_ranges.size(); i++)
	{
		if (cell_ranges[i].inRange(ref))
		{
			return true;
		}
	}

	return false;
}



} // namespace XLS

