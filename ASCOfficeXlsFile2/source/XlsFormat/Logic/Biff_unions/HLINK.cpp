#include "precompiled_xls.h"
#include "HLINK.h"
#include <Logic/Biff_records/HLink.h>
#include <Logic/Biff_records/HLinkTooltip.h>

namespace XLS
{;


HLINK::HLINK()
{
}


HLINK::~HLINK()
{
}


BaseObjectPtr HLINK::clone()
{
	return BaseObjectPtr(new HLINK(*this));
}


// HLINK = HLink [HLinkTooltip]
const bool HLINK::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<HLink>())
	{
		return false;
	}
	proc.optional<HLinkTooltip>();

	return true;
}

} // namespace XLS

