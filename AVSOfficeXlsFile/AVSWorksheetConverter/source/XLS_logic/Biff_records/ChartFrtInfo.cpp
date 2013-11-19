#include "stdafx.h"
#include "ChartFrtInfo.h"
#include <XLS_logic/Biff_structures/FrtHeaderOld.h>

namespace XLS
{;

ChartFrtInfo::ChartFrtInfo()
{
}


ChartFrtInfo::~ChartFrtInfo()
{
}


BaseObjectPtr ChartFrtInfo::clone()
{
	return BaseObjectPtr(new ChartFrtInfo(*this));
}


void ChartFrtInfo::writeFields(CFRecord& record)
{
#pragma message("####################### ChartFrtInfo record is not implemented")
	Log::error("ChartFrtInfo record is not implemented.");
	//record << some_value;
}


void ChartFrtInfo::readFields(CFRecord& record)
{
	FrtHeaderOld frtHeaderOld;
	record >> frtHeaderOld >> verOriginator >> verWriter;
	WORD cCFRTID;
	record >> cCFRTID;

#pragma message("####################### ChartFrtInfo record is not implemented")
	Log::error("ChartFrtInfo record is not implemented.");
}

} // namespace XLS

