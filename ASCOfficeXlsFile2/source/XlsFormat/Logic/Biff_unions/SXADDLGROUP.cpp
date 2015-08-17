
#include "SXADDLGROUP.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{


SXADDLGROUP::SXADDLGROUP()
{
}


SXADDLGROUP::~SXADDLGROUP()
{
}


BaseObjectPtr SXADDLGROUP::clone()
{
	return BaseObjectPtr(new SXADDLGROUP(*this));
}


/*
SXADDLGROUP = SXAddl_SXCGroup_SXDId *Continue_SxaddlSxString SXAddl_SXCGroup_SXDGrpInfo 
				*SXAddl_SXCGroup_SXDMember *UNKNOWNFRT SXAddl_SXCGroup_SXDEnd
*/
const bool SXADDLGROUP::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCGroup_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.mandatory<SXAddl_SXCGroup_SXDGrpInfo>();
	proc.repeated<SXAddl_SXCGroup_SXDMember>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCGroup_SXDEnd>();

	return true;
}

} // namespace XLS

