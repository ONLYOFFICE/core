
#include "SXADDLFIELD12.h"
#include <Logic/Biff_records/SXAddl.h>
#include <Logic/Biff_unions/SXADDLAUTOSORT.h>
#include <Logic/Biff_unions/UNKNOWNFRT.h>

namespace XLS
{;


SXADDLFIELD12::SXADDLFIELD12()
{
}


SXADDLFIELD12::~SXADDLFIELD12()
{
}


BaseObjectPtr SXADDLFIELD12::clone()
{
	return BaseObjectPtr(new SXADDLFIELD12(*this));
}


/*
SXADDLFIELD12 = SXAddl_SXCField12_SXDId *Continue_SxaddlSxString SXAddl_SXCField12_SXDVer12Info 
				SXAddl_SXCField12_SXDVerUpdInv [SXAddl_SXCField12_SXDMemberCaption *Continue_SxaddlSxString] 
				[SXAddl_SXCField12_SXDAutoshow] [SXAddl_SXCField12_SXDISXTH] [SXADDLAUTOSORT] 
				SXAddl_SXCField12_SXDVerUpdInv *UNKNOWNFRT SXAddl_SXCField12_SXDEnd
*/
const bool SXADDLFIELD12::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<SXAddl_SXCField12_SXDId>())
	{
		return false;
	}
	proc.repeated<Continue_SxaddlSxString>(0, 0);
	proc.mandatory<SXAddl_SXCField12_SXDVer12Info>();
	proc.mandatory<SXAddl_SXCField12_SXDVerUpdInv>();
	if(proc.optional<SXAddl_SXCField12_SXDMemberCaption>())
	{
		proc.repeated<Continue_SxaddlSxString>(0, 0);
	}
	proc.optional<SXAddl_SXCField12_SXDAutoshow>();
	proc.optional<SXAddl_SXCField12_SXDISXTH>();
	proc.optional<SXADDLAUTOSORT>();
	proc.mandatory<SXAddl_SXCField12_SXDVerUpdInv>();
	proc.repeated<UNKNOWNFRT>(0, 0);
	proc.mandatory<SXAddl_SXCField12_SXDEnd>();

	return true;
}

} // namespace XLS

