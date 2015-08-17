
#include "SxTbpg.h"

namespace XLS
{

SxTbpg::SxTbpg()
{
}


SxTbpg::~SxTbpg()
{
}


BaseObjectPtr SxTbpg::clone()
{
	return BaseObjectPtr(new SxTbpg(*this));
}


void SxTbpg::writeFields(CFRecord& record)
{
#pragma message("####################### SxTbpg record is not implemented")
	Log::error("SxTbpg record is not implemented.");
	//record << some_value;
}


void SxTbpg::readFields(CFRecord& record)
{
#pragma message("####################### SxTbpg record is not implemented")
	Log::error("SxTbpg record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

