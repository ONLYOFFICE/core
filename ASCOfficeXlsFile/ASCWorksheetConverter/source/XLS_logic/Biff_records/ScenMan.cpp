#include "stdafx.h"
#include "ScenMan.h"

namespace XLS
{;

ScenMan::ScenMan()
{
}


ScenMan::~ScenMan()
{
}


BaseObjectPtr ScenMan::clone()
{
	return BaseObjectPtr(new ScenMan(*this));
}


void ScenMan::writeFields(CFRecord& record)
{
#pragma message("####################### ScenMan record is not implemented")
	Log::error("ScenMan record is not implemented.");
	//record << some_value;
}


void ScenMan::readFields(CFRecord& record)
{
#pragma message("####################### ScenMan record is not implemented")
	Log::error("ScenMan record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

