
#include "HLINK.h"
#include <Logic/Biff_records/HLink.h>
#include <Logic/Biff_records/HLinkTooltip.h>

namespace XLS
{


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
	m_HLink = elements_.back();
	elements_.pop_back();

	if (proc.optional<HLinkTooltip>())
	{
		m_HLinkTooltip = elements_.back();
		elements_.pop_back();
	}

	return true;
}

} // namespace XLS

