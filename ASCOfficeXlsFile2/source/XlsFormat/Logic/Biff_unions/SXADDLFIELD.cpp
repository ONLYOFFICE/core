#include "precompiled_xls.h"
#include "SXADDLFIELD.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLFIELD12.h>

namespace XLS
{;


SXADDLFIELD::SXADDLFIELD()
{
}


SXADDLFIELD::~SXADDLFIELD()
{
}


BaseObjectPtr SXADDLFIELD::clone()
{
	return BaseObjectPtr(new SXADDLFIELD(*this));
}


/*
SXADDLFIELD = [SXAddl_SXCField_SXDId *Continue_SxaddlSxString SXAddl_SXCField_SXDVer10Info SXAddl_SXCField_SXDEnd]
[SXADDLFIELD12]
*/
const bool SXADDLFIELD::loadContent(BinProcessor& proc)
{

	if(proc.optional<SXAddl_SXCField_SXDId>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
		proc.mandatory<SXAddl_SXCField_SXDVer10Info>();
		proc.mandatory<SXAddl_SXCField_SXDEnd>();
		proc.optional<SXADDLFIELD12>();
		return true;
	}
	return proc.optional<SXADDLFIELD12>();
}

} // namespace XLS

