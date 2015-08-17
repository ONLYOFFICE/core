
#include "SXADDLSXMGS.h"
#include <Logic/Biff_unions/SXADDLSXMG.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{


SXADDLSXMGS::SXADDLSXMGS()
{
}


SXADDLSXMGS::~SXADDLSXMGS()
{
}


BaseObjectPtr SXADDLSXMGS::clone()
{
	return BaseObjectPtr(new SXADDLSXMGS(*this));
}


// SXADDLSXMGS = SXAddl_SXCSXMgs_SXDId *SXADDLSXMG *SXAddl_SXCSXMgs_SXDMGrpSXDHMap *UNKNOWNFRT SXAddl_SXCSXMgs_SXDEnd
const bool SXADDLSXMGS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXMgs_SXDId>())
	{
		return false;
	}
	proc.repeated<SXADDLSXMG>(0, 0);
	proc.repeated<SXAddl_SXCSXMgs_SXDMGrpSXDHMap>(0, 0);
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCSXMgs_SXDEnd>();
	return true;
}

} // namespace XLS

