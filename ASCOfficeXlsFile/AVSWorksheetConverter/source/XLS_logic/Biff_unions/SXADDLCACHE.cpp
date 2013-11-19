#include "stdafx.h"
#include "SXADDLCACHE.h"
#include <XLS_logic/Biff_records/SXAddl.h>
#include <XLS_logic/Biff_unions/SXADDLCACHE12.h>
#include <XLS_logic/Biff_unions/SXADDLDBQUERY.h>
#include <XLS_logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLCACHE::SXADDLCACHE()
{
}


SXADDLCACHE::~SXADDLCACHE()
{
}


// SXADDLCACHE = SXAddl_SXCCache_SXDId SXAddl_SXCCache_SXDVer10Info [SXAddl_SXCCache_SXDVerSXMacro] 
BaseObjectPtr SXADDLCACHE::clone()
{
	return BaseObjectPtr(new SXADDLCACHE(*this));
}


//               [SXADDLCACHE12] [SXADDLDBQUERY] *UNKNOWNFRT SXAddl_SXCCache_SXDEnd
const bool SXADDLCACHE::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCCache_SXDId>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCCache_SXDVer10Info>();
	proc.optional<SXAddl_SXCCache_SXDVerSXMacro>();
	proc.optional<SXADDLCACHE12>();
	proc.optional<SXADDLDBQUERY>();
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCCache_SXDEnd>();
	return true;
}

} // namespace XLS

