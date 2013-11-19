#include "stdafx.h"
#include "DocRoute.h"

namespace XLS
{;

DocRoute::DocRoute()
{
}


DocRoute::~DocRoute()
{
}


BaseObjectPtr DocRoute::clone()
{
	return BaseObjectPtr(new DocRoute(*this));
}


void DocRoute::writeFields(CFRecord& record)
{
#pragma message("####################### DocRoute record is not implemented")
	Log::error("DocRoute record is not implemented.");
	//record << some_value;
}


void DocRoute::readFields(CFRecord& record)
{
#pragma message("####################### DocRoute record is not implemented")
	Log::error("DocRoute record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

