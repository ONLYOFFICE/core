
#include "SxItm.h"

namespace XLS
{

SxItm::SxItm()
{
}


SxItm::~SxItm()
{
}


BaseObjectPtr SxItm::clone()
{
	return BaseObjectPtr(new SxItm(*this));
}


void SxItm::writeFields(CFRecord& record)
{
#pragma message("####################### SxItm record is not implemented")
	Log::error("SxItm record is not implemented.");
	//record << some_value;
}


void SxItm::readFields(CFRecord& record)
{
#pragma message("####################### SxItm record is not implemented")
	Log::error("SxItm record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

