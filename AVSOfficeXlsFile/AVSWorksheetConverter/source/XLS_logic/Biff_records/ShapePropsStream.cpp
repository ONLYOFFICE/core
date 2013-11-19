#include "stdafx.h"
#include "ShapePropsStream.h"

namespace XLS
{;

ShapePropsStream::ShapePropsStream()
{
}


ShapePropsStream::~ShapePropsStream()
{
}


BaseObjectPtr ShapePropsStream::clone()
{
	return BaseObjectPtr(new ShapePropsStream(*this));
}


void ShapePropsStream::writeFields(CFRecord& record)
{
#pragma message("####################### ShapePropsStream record is not implemented")
	Log::error("ShapePropsStream record is not implemented.");
	//record << some_value;
}


void ShapePropsStream::readFields(CFRecord& record)
{
#pragma message("####################### ShapePropsStream record is not implemented")
	Log::error("ShapePropsStream record is not implemented.");
	//record >> some_value;	
}

} // namespace XLS

