#include "stdafx.h"
#include "SXADDLCACHE12.h"
#include <XLS_logic/Biff_records/SXAddl.h>
#include <XLS_logic/Biff_unions/SXADDLCACHEFIELD.h>
#include <XLS_logic/Biff_unions/SXADDLSXDH.h>
#include <XLS_logic/Biff_unions/SXADDLSXMGS.h>

namespace XLS
{;


SXADDLCACHE12::SXADDLCACHE12()
{
}


SXADDLCACHE12::~SXADDLCACHE12()
{
}


// SXADDLCACHE12 = SXAddl_SXCCache_SXDVerUpdInv SXAddl_SXCCache_SXDInfo12 SXAddl_SXCCache_SXDInvRefreshReal
BaseObjectPtr SXADDLCACHE12::clone()
{
	return BaseObjectPtr(new SXADDLCACHE12(*this));
}


//                 *SXADDLCACHEFIELD [SXADDLSXDH] [SXADDLSXMGS] SXAddl_SXCCache_SXDVerUpdInv
const bool SXADDLCACHE12::loadContent(BinProcessor& proc)
{

	if(!proc.mandatory<SXAddl_SXCCache_SXDVerUpdInv>())
	{
		return false;
	}
	proc.mandatory<SXAddl_SXCCache_SXDInfo12>();
	proc.mandatory<SXAddl_SXCCache_SXDInvRefreshReal>();
	proc.repeated<SXADDLCACHEFIELD>(0, 0);
	proc.optional<SXADDLSXDH>();
	proc.optional<SXADDLSXMGS>();
	proc.mandatory<SXAddl_SXCCache_SXDVerUpdInv>();

	return true;
}

} // namespace XLS

