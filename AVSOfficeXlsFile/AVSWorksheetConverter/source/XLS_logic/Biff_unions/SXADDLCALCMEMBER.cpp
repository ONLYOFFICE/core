#include "stdafx.h"
#include "SXADDLCALCMEMBER.h"
#include <XLS_logic/Biff_records/SXAddl.h>

namespace XLS
{;


SXADDLCALCMEMBER::SXADDLCALCMEMBER()
{
}


SXADDLCALCMEMBER::~SXADDLCALCMEMBER()
{
}


BaseObjectPtr SXADDLCALCMEMBER::clone()
{
	return BaseObjectPtr(new SXADDLCALCMEMBER(*this));
}


// SXADDLCALCMEMBER = (SXAddl_SXCView_SXDCalcMember [SXAddl_SXCView_SXDCalcMemString *Continue_SxaddlSxString])
const bool SXADDLCALCMEMBER::loadContent(BinProcessor& proc)
{
#pragma message("####################### SXADDLCALCMEMBER union is not implemented")
	Log::info("SXADDLCALCMEMBER union is not implemented!!!!!!!!!!!!!!!");

	if(!proc.mandatory<SXAddl_SXCView_SXDCalcMember>())
	{
		return false;
	}
	if(proc.optional<SXAddl_SXCView_SXDCalcMemString>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}

	return true;
}

} // namespace XLS

