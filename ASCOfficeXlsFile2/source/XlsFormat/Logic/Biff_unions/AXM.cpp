
#include "AXM.h"
#include <Logic/Biff_records/YMult.h>
#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_records/EndObject.h>

namespace XLS
{


AXM::AXM()
{
}


AXM::~AXM()
{
}


BaseObjectPtr AXM::clone()
{
	return BaseObjectPtr(new AXM(*this));
}


// AXM = YMult StartObject ATTACHEDLABEL EndObject
const bool AXM::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<YMult>())
	{
		return false;
	}
	proc.mandatory<StartObject>();	elements_.pop_back();
	proc.mandatory<ATTACHEDLABEL>();
	proc.mandatory<EndObject>();	elements_.pop_back();

	return true;
}

} // namespace XLS

