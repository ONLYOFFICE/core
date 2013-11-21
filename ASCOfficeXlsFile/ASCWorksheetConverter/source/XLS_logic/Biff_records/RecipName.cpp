#include "stdafx.h"
#include "RecipName.h"

namespace XLS
{;

RecipName::RecipName()
{
}


RecipName::~RecipName()
{
}


BaseObjectPtr RecipName::clone()
{
	return BaseObjectPtr(new RecipName(*this));
}


void RecipName::writeFields(CFRecord& record)
{
#pragma message("####################### RecipName record is not implemented")
	Log::error("RecipName record is not implemented.");
	//record << some_value;
}


void RecipName::readFields(CFRecord& record)
{
#pragma message("####################### RecipName record is not implemented")
	Log::error("RecipName record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

