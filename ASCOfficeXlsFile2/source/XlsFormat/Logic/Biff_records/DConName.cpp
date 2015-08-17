
#include "DConName.h"

namespace XLS
{

DConName::DConName()
{
}


DConName::~DConName()
{
}


BaseObjectPtr DConName::clone()
{
	return BaseObjectPtr(new DConName(*this));
}


void DConName::writeFields(CFRecord& record)
{
#pragma message("####################### DConName record is not implemented")
	Log::error("DConName record is not implemented.");
	//record << some_value;
}


void DConName::readFields(CFRecord& record)
{
#pragma message("####################### DConName record is not implemented")
	Log::error("DConName record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

