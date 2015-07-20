
#include "SXADDLSXRULE.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLSXFILT.h>

namespace XLS
{;


SXADDLSXRULE::SXADDLSXRULE()
{
}


SXADDLSXRULE::~SXADDLSXRULE()
{
}


BaseObjectPtr SXADDLSXRULE::clone()
{
	return BaseObjectPtr(new SXADDLSXRULE(*this));
}


// SXADDLSXRULE = SXAddl_SXCSXrule_SXDId SXAddl_SXCSXrule_SXDSXrule *SXADDLSXFILT SXAddl_SXCSXrule_SXDEnd
const bool SXADDLSXRULE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXrule_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCSXrule_SXDSXrule>();
	proc.repeated<SXADDLSXFILT>(0, 0);
	proc.mandatory<SXAddl_SXCSXrule_SXDEnd>();

	return true;
}

} // namespace XLS

