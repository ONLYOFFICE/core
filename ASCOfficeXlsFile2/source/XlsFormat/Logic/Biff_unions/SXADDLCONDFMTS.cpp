
#include "SXADDLCONDFMTS.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLCONDFMT.h>

namespace XLS
{


SXADDLCONDFMTS::SXADDLCONDFMTS()
{
}


SXADDLCONDFMTS::~SXADDLCONDFMTS()
{
}


BaseObjectPtr SXADDLCONDFMTS::clone()
{
	return BaseObjectPtr(new SXADDLCONDFMTS(*this));
}


// SXADDLCONDFMTS = SXAddl_SXCSXCondFmts_SXDId *SXADDLCONDFMT SXAddl_SXCSXCondFmts_SXDEnd
const bool SXADDLCONDFMTS::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXCondFmts_SXDId>())
	{
		return false;
	}
	proc.repeated<SXADDLCONDFMT>(0, 0);
	proc.mandatory<SXAddl_SXCSXCondFmts_SXDEnd>();

	return true;
}

} // namespace XLS

