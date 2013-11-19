#include "stdafx.h"
#include "HLINK.h"
#include <XLS_logic/Biff_records/HLink.h>
#include <XLS_logic/Biff_records/HLinkTooltip.h>

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

