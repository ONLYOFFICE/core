
#include "SXTbl.h"

namespace XLS
{;

SXTbl::SXTbl()
{
}


SXTbl::~SXTbl()
{
}


BaseObjectPtr SXTbl::clone()
{
	return BaseObjectPtr(new SXTbl(*this));
}


void SXTbl::writeFields(CFRecord& record)
{
#pragma message("####################### SXTbl record is not implemented")
	Log::error("SXTbl record is not implemented.");
	//record << some_value;
}


void SXTbl::readFields(CFRecord& record)
{
#pragma message("####################### SXTbl record is not implemented")
	Log::error("SXTbl record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

