#include "stdafx.h"
#include "Template.h"
#include <XLS_bin/CFStream.h>

namespace XLS
{;

Template::Template()
{
}


Template::~Template()
{
}


BaseObjectPtr Template::clone()
{
	return BaseObjectPtr(new Template(*this));
}


void Template::writeFields(CFRecord& record)
{
#pragma message("####################### Template record is not implemented")
	Log::error("Template record is not implemented.");
	//record << some_value;
}


void Template::readFields(CFRecord& record)
{
	// No data in this record
}

} // namespace XLS



