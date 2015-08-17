
#include "SXAddl.h"

namespace XLS
{

SXAddl::SXAddl()
{
}


SXAddl::~SXAddl()
{
}


BaseObjectPtr SXAddl::clone()
{
	return BaseObjectPtr(new SXAddl(*this));
}


void SXAddl::writeFields(CFRecord& record)
{
#pragma message("####################### SXAddl record is not implemented")
	Log::error("SXAddl record is not implemented.");
	//record << some_value;
}


void SXAddl::readFields(CFRecord& record)
{
#pragma message("####################### SXAddl record is not implemented")
	Log::error("SXAddl record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

