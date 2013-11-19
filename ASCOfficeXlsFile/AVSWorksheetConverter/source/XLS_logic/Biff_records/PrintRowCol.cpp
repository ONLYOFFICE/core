#include "stdafx.h"
#include "PrintRowCol.h"

namespace XLS
{;

PrintRowCol::PrintRowCol()
{
}


PrintRowCol::~PrintRowCol()
{
}


BaseObjectPtr PrintRowCol::clone()
{
	return BaseObjectPtr(new PrintRowCol(*this));
}


void PrintRowCol::writeFields(CFRecord& record)
{
	record << printRwCol;
}


void PrintRowCol::readFields(CFRecord& record)
{
	record >> printRwCol;
}

} // namespace XLS

