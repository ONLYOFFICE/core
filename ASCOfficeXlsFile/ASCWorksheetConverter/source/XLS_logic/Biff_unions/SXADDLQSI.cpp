#include "stdafx.h"
#include "SXADDLQSI.h"
#include <XLS_logic/Biff_records/SXAddl.h>
#include <XLS_logic/Biff_unions/SXADDLDBQUERY.h>
#include <XLS_logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLQSI::SXADDLQSI()
{
}


SXADDLQSI::~SXADDLQSI()
{
}


BaseObjectPtr SXADDLQSI::clone()
{
	return BaseObjectPtr(new SXADDLQSI(*this));
}


// SXADDLQSI = SXAddl_SXCQsi_SXDId SXADDLDBQUERY *UNKNOWNFRT SXAddl_SXCQsi_SXDEnd
const bool SXADDLQSI::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCQsi_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXADDLDBQUERY>();
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCQsi_SXDEnd>();

	return true;
}

} // namespace XLS

