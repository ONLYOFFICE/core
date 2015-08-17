
#include "SXADDLSXFILTERS12.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLSXFILTER12.h>

namespace XLS
{


SXADDLSXFILTERS12::SXADDLSXFILTERS12()
{
}


SXADDLSXFILTERS12::~SXADDLSXFILTERS12()
{
}


BaseObjectPtr SXADDLSXFILTERS12::clone()
{
	return BaseObjectPtr(new SXADDLSXFILTERS12(*this));
}


// SXADDLSXFILTERS12 = SXAddl_SXCSXFilters12_SXDId *SXADDLSXFILTER12 SXAddl_SXCSXFilters12_SXDEnd
const bool SXADDLSXFILTERS12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXFilters12_SXDId>())
	{
		return false;
	}
	proc.repeated<SXADDLSXFILTER12>(0, 0);
	proc.mandatory<SXAddl_SXCSXFilters12_SXDEnd>();

	return true;
}

} // namespace XLS

