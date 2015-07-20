
#include "RTD.h"
#include <Logic/Biff_records/RealTimeData.h>
#include <Logic/Biff_records/ContinueFrt.h>

namespace XLS
{;


RTD::RTD()
{
}


RTD::~RTD()
{
}


BaseObjectPtr RTD::clone()
{
	return BaseObjectPtr(new RTD(*this));
}


// RTD = RealTimeData *ContinueFrt
const bool RTD::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<RealTimeData>())
	{
		return false;
	}
#pragma message("####################### RTD union is not implemented")
	Log::info("RTD union is not implemented!!!!!!!!!!!!!!!");

	proc.repeated<ContinueFrt>(0, 0);
	return true;
}

} // namespace XLS

