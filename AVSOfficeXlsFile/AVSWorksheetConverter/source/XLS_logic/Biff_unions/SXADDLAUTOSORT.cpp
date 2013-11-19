#include "stdafx.h"
#include "SXADDLAUTOSORT.h"
#include <XLS_logic/Biff_unions/SXADDLSXRULE.h>
#include <XLS_logic/Biff_records/SXAddl.h>

namespace XLS
{;


SXADDLAUTOSORT::SXADDLAUTOSORT()
{
}


SXADDLAUTOSORT::~SXADDLAUTOSORT()
{
}


BaseObjectPtr SXADDLAUTOSORT::clone()
{
	return BaseObjectPtr(new SXADDLAUTOSORT(*this));
}


// SXADDLAUTOSORT = SXAddl_SXCAutoSort_SXDId SXADDLSXRULE SXAddl_SXCAutoSort_SXDEnd
const bool SXADDLAUTOSORT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCAutoSort_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXADDLSXRULE>();
	proc.mandatory<SXAddl_SXCAutoSort_SXDEnd>();
	return true;
}

} // namespace XLS

