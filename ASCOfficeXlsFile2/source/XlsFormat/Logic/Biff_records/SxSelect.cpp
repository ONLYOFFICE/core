
#include "SxSelect.h"

namespace XLS
{;

SxSelect::SxSelect()
{
}


SxSelect::~SxSelect()
{
}


BaseObjectPtr SxSelect::clone()
{
	return BaseObjectPtr(new SxSelect(*this));
}


void SxSelect::writeFields(CFRecord& record)
{
#pragma message("####################### SxSelect record is not implemented")
	Log::error("SxSelect record is not implemented.");
	//record << some_value;
}


void SxSelect::readFields(CFRecord& record)
{
#pragma message("####################### SxSelect record is not implemented")
	Log::error("SxSelect record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

