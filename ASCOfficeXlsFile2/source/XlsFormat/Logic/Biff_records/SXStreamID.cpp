#include "precompiled_xls.h"
#include "SXStreamID.h"

namespace XLS
{;

SXStreamID::SXStreamID()
{
}


SXStreamID::~SXStreamID()
{
}


BaseObjectPtr SXStreamID::clone()
{
	return BaseObjectPtr(new SXStreamID(*this));
}


void SXStreamID::writeFields(CFRecord& record)
{
#pragma message("####################### SXStreamID record is not implemented")
	Log::error("SXStreamID record is not implemented.");
	//record << some_value;
}


void SXStreamID::readFields(CFRecord& record)
{
#pragma message("####################### SXStreamID record is not implemented")
	Log::error("SXStreamID record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

