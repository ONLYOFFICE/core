
#include "SBaseRef.h"

namespace XLS
{;

SBaseRef::SBaseRef()
{
}


SBaseRef::~SBaseRef()
{
}


BaseObjectPtr SBaseRef::clone()
{
	return BaseObjectPtr(new SBaseRef(*this));
}


void SBaseRef::writeFields(CFRecord& record)
{
#pragma message("####################### SBaseRef record is not implemented")
	Log::error("SBaseRef record is not implemented.");
	//record << some_value;
}


void SBaseRef::readFields(CFRecord& record)
{
#pragma message("####################### SBaseRef record is not implemented")
	Log::error("SBaseRef record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

