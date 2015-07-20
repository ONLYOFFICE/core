
#include "MDXStr.h"

namespace XLS
{;

MDXStr::MDXStr()
{
}


MDXStr::~MDXStr()
{
}


BaseObjectPtr MDXStr::clone()
{
	return BaseObjectPtr(new MDXStr(*this));
}


void MDXStr::writeFields(CFRecord& record)
{
#pragma message("####################### MDXStr record is not implemented")
	Log::error("MDXStr record is not implemented.");
	//record << some_value;
}


void MDXStr::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> st;
}

} // namespace XLS

