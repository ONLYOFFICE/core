
#include "ScenarioProtect.h"

namespace XLS
{

ScenarioProtect::ScenarioProtect()
{
}


ScenarioProtect::~ScenarioProtect()
{
}


BaseObjectPtr ScenarioProtect::clone()
{
	return BaseObjectPtr(new ScenarioProtect(*this));
}


void ScenarioProtect::writeFields(CFRecord& record)
{
	record << fScenProtect;
}


void ScenarioProtect::readFields(CFRecord& record)
{
	record >> fScenProtect;
}

} // namespace XLS

