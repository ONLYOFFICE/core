#include "stdafx.h"
#include "NamePublish.h"

namespace XLS
{;

NamePublish::NamePublish()
{
}


NamePublish::~NamePublish()
{
}


BaseObjectPtr NamePublish::clone()
{
	return BaseObjectPtr(new NamePublish(*this));
}


void NamePublish::writeFields(CFRecord& record)
{
#pragma message("####################### NamePublish record is not implemented")
	Log::error("NamePublish record is not implemented.");
	//record << some_value;
}


void NamePublish::readFields(CFRecord& record)
{
	record.skipNBytes(12);
#pragma message("############################ frtHeader skipped here")
	WORD flags;
	record >> flags >> strName;
	fPublished = GETBIT(flags, 0);
	fWorkbookParam = GETBIT(flags, 0);
}

} // namespace XLS

