
#include "HLinkTooltip.h"
#include <Logic/Biff_structures/CellRangeRef.h>
#include <Logic/Biff_structures/FrtRefHeaderNoGrbit.h>

namespace XLS
{

HLinkTooltip::HLinkTooltip()
{
}


HLinkTooltip::~HLinkTooltip()
{
}


BaseObjectPtr HLinkTooltip::clone()
{
	return BaseObjectPtr(new HLinkTooltip(*this));
}


void HLinkTooltip::writeFields(CFRecord& record)
{
	Ref8U ref8(ref_);
	FrtRefHeaderNoGrbit frtRefHeaderNoGrbit(rt_HLinkTooltip, ref8);
	record << frtRefHeaderNoGrbit;

	std::wstring wzTooltip_prep = wzTooltip;//(static_cast<wchar_t*>(static_cast<std::wstring >(wzTooltip)));
	record << wzTooltip_prep;
}


void HLinkTooltip::readFields(CFRecord& record)
{
	FrtRefHeaderNoGrbit frtRefHeaderNoGrbit(rt_HLinkTooltip);
	record >> frtRefHeaderNoGrbit;
	std::wstring wzTooltip_prep;
	record >> wzTooltip_prep;
	wzTooltip = std::wstring (wzTooltip_prep.c_str());
}

} // namespace XLS

