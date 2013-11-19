#include "stdafx.h"
#include "Format.h"

namespace XLS
{;

Format::Format()
{
}


Format::~Format()
{
}


BaseObjectPtr Format::clone()
{
	return BaseObjectPtr(new Format(*this));
}


void Format::writeFields(CFRecord& record)
{
	record << ifmt << stFormat;
}


void Format::readFields(CFRecord& record)
{
	record >> ifmt >> stFormat;
}

} // namespace XLS

