
#include "SxFormat.h"

namespace XLS
{;

SxFormat::SxFormat()
{
}


SxFormat::~SxFormat()
{
}


BaseObjectPtr SxFormat::clone()
{
	return BaseObjectPtr(new SxFormat(*this));
}


void SxFormat::writeFields(CFRecord& record)
{
#pragma message("####################### SxFormat record is not implemented")
	Log::error("SxFormat record is not implemented.");
	//record << some_value;
}


void SxFormat::readFields(CFRecord& record)
{
#pragma message("####################### SxFormat record is not implemented")
	Log::error("SxFormat record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

