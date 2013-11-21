#include "stdafx.h"
#include "CodeName.h"

namespace XLS
{;

CodeName::CodeName()
{
}


CodeName::~CodeName()
{
}


BaseObjectPtr CodeName::clone()
{
	return BaseObjectPtr(new CodeName(*this));
}


void CodeName::writeFields(CFRecord& record)
{
	record << codeName;
}


void CodeName::readFields(CFRecord& record)
{
	record >> codeName;
}

} // namespace XLS

