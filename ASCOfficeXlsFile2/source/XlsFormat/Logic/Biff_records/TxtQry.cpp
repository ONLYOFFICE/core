
#include "TxtQry.h"

namespace XLS
{;

TxtQry::TxtQry()
{
}


TxtQry::~TxtQry()
{
}


BaseObjectPtr TxtQry::clone()
{
	return BaseObjectPtr(new TxtQry(*this));
}


void TxtQry::writeFields(CFRecord& record)
{
#pragma message("####################### TxtQry record is not implemented")
	Log::error("TxtQry record is not implemented.");
	//record << some_value;
}


void TxtQry::readFields(CFRecord& record)
{
#pragma message("####################### TxtQry record is not implemented")
	Log::error("TxtQry record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

