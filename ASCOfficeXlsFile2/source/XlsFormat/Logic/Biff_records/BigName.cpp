
#include "BigName.h"

namespace XLS
{

BigName::BigName()
{
}


BigName::~BigName()
{
}


BaseObjectPtr BigName::clone()
{
	return BaseObjectPtr(new BigName(*this));
}


void BigName::writeFields(CFRecord& record)
{
#pragma message("####################### BigName record is not implemented")
	Log::error("BigName record is not implemented.");
	//record << some_value;
}


void BigName::readFields(CFRecord& record)
{
#pragma message("####################### BigName record is not implemented")
	Log::error("BigName record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

