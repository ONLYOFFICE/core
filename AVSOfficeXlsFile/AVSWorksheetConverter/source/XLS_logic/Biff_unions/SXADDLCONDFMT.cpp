#include "stdafx.h"
#include "SXADDLCONDFMT.h"
#include <XLS_logic/Biff_records/SXAddl.h>
#include <XLS_logic/Biff_unions/SXADDLSXRULE.h>

namespace XLS
{;


SXADDLCONDFMT::SXADDLCONDFMT()
{
}


SXADDLCONDFMT::~SXADDLCONDFMT()
{
}


BaseObjectPtr SXADDLCONDFMT::clone()
{
	return BaseObjectPtr(new SXADDLCONDFMT(*this));
}


// SXADDLCONDFMT = SXAddl_SXCSXCondFmt_SXDSXCondFmt *SXADDLSXRULE SXAddl_SXCSXCondFmt_SXDEnd
const bool SXADDLCONDFMT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXCondFmt_SXDSXCondFmt>())
	{
		return false;
	}
	proc.repeated<SXADDLSXRULE>(0, 0);
	proc.mandatory<SXAddl_SXCSXCondFmt_SXDEnd>();

	return true;
}

} // namespace XLS

