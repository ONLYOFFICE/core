#include "stdafx.h"
#include "RichTextStream.h"

namespace XLS
{;

RichTextStream::RichTextStream()
{
}


RichTextStream::~RichTextStream()
{
}


BaseObjectPtr RichTextStream::clone()
{
	return BaseObjectPtr(new RichTextStream(*this));
}


void RichTextStream::writeFields(CFRecord& record)
{
#pragma message("####################### RichTextStream record is not implemented")
	Log::error("RichTextStream record is not implemented.");
	//record << some_value;
}


void RichTextStream::readFields(CFRecord& record)
{
#pragma message("####################### RichTextStream record is not implemented")
	Log::error("RichTextStream record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

