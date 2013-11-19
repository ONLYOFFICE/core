#include "stdafx.h"
#include "SXVDTEx.h"

namespace XLS
{;

SXVDTEx::SXVDTEx()
{
}


SXVDTEx::~SXVDTEx()
{
}


BaseObjectPtr SXVDTEx::clone()
{
	return BaseObjectPtr(new SXVDTEx(*this));
}


void SXVDTEx::writeFields(CFRecord& record)
{
#pragma message("####################### SXVDTEx record is not implemented")
	Log::error("SXVDTEx record is not implemented.");
	//record << some_value;
}


void SXVDTEx::readFields(CFRecord& record)
{
#pragma message("####################### SXVDTEx record is not implemented")
	Log::error("SXVDTEx record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

