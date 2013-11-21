#include "stdafx.h"
#include "TextPropsStream.h"

namespace XLS
{;

TextPropsStream::TextPropsStream()
{
}


TextPropsStream::~TextPropsStream()
{
}


BaseObjectPtr TextPropsStream::clone()
{
	return BaseObjectPtr(new TextPropsStream(*this));
}


void TextPropsStream::writeFields(CFRecord& record)
{
#pragma message("####################### TextPropsStream record is not implemented")
	Log::error("TextPropsStream record is not implemented.");
	//record << some_value;
}


void TextPropsStream::readFields(CFRecord& record)
{
#pragma message("####################### TextPropsStream record is not implemented")
	Log::error("TextPropsStream record is not implemented.");
	//record >> some_value;	
}

} // namespace XLS

