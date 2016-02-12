
#include "AXM.h"
#include <Logic/Biff_records/YMult.h>
#include <Logic/Biff_records/StartObject.h>
#include <Logic/Biff_unions/ATTACHEDLABEL.h>
#include <Logic/Biff_records/EndObject.h>
#include <Logic/Biff_records/FrtWrapper.h>

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
	m_YMult = elements_.back();				elements_.pop_back();
	
	if (proc.optional<StartObject>())		elements_.pop_back();
	
	int count = proc.repeated<FrtWrapper>(0,0);
	if (proc.optional<ATTACHEDLABEL>())
	{
		m_ATTACHEDLABEL = elements_.back();	elements_.pop_back();
	}
	
	if (proc.optional<EndObject>())			elements_.pop_back();

	return true;
}

} // namespace XLS

