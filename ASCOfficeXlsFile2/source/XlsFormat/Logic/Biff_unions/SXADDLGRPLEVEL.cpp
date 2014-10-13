#include "precompiled_xls.h"
#include "SXADDLGRPLEVEL.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLGROUP.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLGRPLEVEL::SXADDLGRPLEVEL()
{
}


SXADDLGRPLEVEL::~SXADDLGRPLEVEL()
{
}


BaseObjectPtr SXADDLGRPLEVEL::clone()
{
	return BaseObjectPtr(new SXADDLGRPLEVEL(*this));
}


/*
SXADDLGRPLEVEL = SXAddl_SXCGrpLevel_SXDId *Continue_SxaddlSxString SXAddl_SXCGrpLevel_SXDGrpLevelInfo
				*SXADDLGROUP *UNKNOWNFRT SXAddl_SXCGrpLevel_SXDEnd
*/
const bool SXADDLGRPLEVEL::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCGrpLevel_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.mandatory<SXAddl_SXCGrpLevel_SXDGrpLevelInfo>();
	proc.repeated<SXADDLGROUP>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCGrpLevel_SXDEnd>();

	return true;
}

} // namespace XLS

