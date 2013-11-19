#include "stdafx.h"
#include "CHART.h"
#include <XLS_logic/Biff_records/BOF.h>
#include <XLS_logic/ChartSheetSubstream.h>

namespace XLS
{;


CHART::CHART()
{
}


CHART::~CHART()
{
}


// CHART = CHARTSHEET *Continue
// hope that CHARTSHEET is not the same as the so-called substream
BaseObjectPtr CHART::clone()
{
	return BaseObjectPtr(new CHART(*this));
}


// Shit!!! It is.
const bool CHART::loadContent(BinProcessor& proc)
{
	WORD ss_type;
	if(!proc.getNextSubstreamType(ss_type) || BOF::st_Chart != ss_type)
	{
		return false;
	}
	if(!proc.mandatory<ChartSheetSubstream>())
	{
		return false;
	}

//	reader.SeekNextSubstream();
	return true;
}

} // namespace XLS

