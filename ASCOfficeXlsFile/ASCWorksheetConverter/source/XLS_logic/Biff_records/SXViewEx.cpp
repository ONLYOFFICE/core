#include "stdafx.h"
#include "SXViewEx.h"

namespace XLS
{;

SXViewEx::SXViewEx()
{
}


SXViewEx::~SXViewEx()
{
}


BaseObjectPtr SXViewEx::clone()
{
	return BaseObjectPtr(new SXViewEx(*this));
}


void SXViewEx::writeFields(CFRecord& record)
{
#pragma message("####################### SXViewEx record is not implemented")
	Log::error("SXViewEx record is not implemented.");
	//record << some_value;
}


void SXViewEx::readFields(CFRecord& record)
{
#pragma message("####################### SXViewEx record is not implemented")
	Log::error("SXViewEx record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

