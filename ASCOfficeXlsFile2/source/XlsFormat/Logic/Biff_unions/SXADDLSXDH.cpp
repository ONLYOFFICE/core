
#include "SXADDLSXDH.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{


SXADDLSXDH::SXADDLSXDH()
{
}


SXADDLSXDH::~SXADDLSXDH()
{
}


BaseObjectPtr SXADDLSXDH::clone()
{
	return BaseObjectPtr(new SXADDLSXDH(*this));
}


// SXADDLSXDH = SXAddl_SXCSXDH_SXDId *SXAddl_SXCSXDH_SXDSxdh SXAddl_SXCSXDH_SXDEnd
const bool SXADDLSXDH::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<SXAddl_SXCSXDH_SXDId>())
	{
		return false;
	}
	proc.repeated<SXAddl_SXCSXDH_SXDSxdh>(0, 0);
	proc.mandatory<SXAddl_SXCSXDH_SXDEnd>();
	return true;
}

} // namespace XLS

