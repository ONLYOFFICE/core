
#include "SxRule.h"

namespace XLS
{;

SxRule::SxRule()
{
}


SxRule::~SxRule()
{
}


BaseObjectPtr SxRule::clone()
{
	return BaseObjectPtr(new SxRule(*this));
}


void SxRule::writeFields(CFRecord& record)
{
#pragma message("####################### SxRule record is not implemented")
	Log::error("SxRule record is not implemented.");
	//record << some_value;
}


void SxRule::readFields(CFRecord& record)
{
#pragma message("####################### SxRule record is not implemented")
	Log::error("SxRule record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

