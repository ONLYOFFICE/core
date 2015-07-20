
#include "SXADDLSXFILT.h"
#include <Logic/Biff_records/SXAddl.h>

namespace XLS
{;


SXADDLSXFILT::SXADDLSXFILT()
{
}


SXADDLSXFILT::~SXADDLSXFILT()
{
}


BaseObjectPtr SXADDLSXFILT::clone()
{
	return BaseObjectPtr(new SXADDLSXFILT(*this));
}


// SXADDLSXFILT = SXAddl_SXCSXfilt_SXDId SXAddl_SXCSXfilt_SXDSXfilt [SXAddl_SXCSXfilt_SXDSXItm] SXAddl_SXCSXfilt_SXDEnd
const bool SXADDLSXFILT::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCSXfilt_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCSXfilt_SXDSXfilt>();
	proc.optional<SXAddl_SXCSXfilt_SXDSXItm>();
	proc.mandatory<SXAddl_SXCSXfilt_SXDEnd>();

	return true;
}

} // namespace XLS

