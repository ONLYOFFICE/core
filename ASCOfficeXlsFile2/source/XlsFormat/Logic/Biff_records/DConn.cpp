#include "precompiled_xls.h"
#include "DConn.h"

namespace XLS
{;

DConn::DConn()
{
}


DConn::~DConn()
{
}


BaseObjectPtr DConn::clone()
{
	return BaseObjectPtr(new DConn(*this));
}


void DConn::writeFields(CFRecord& record)
{
#pragma message("####################### DConn record is not implemented")
	Log::error("DConn record is not implemented.");
	//record << some_value;
}


void DConn::readFields(CFRecord& record)
{
#pragma message("####################### DConn record is not implemented")
	Log::error("DConn record is not implemented.");

	//record >> some_value;
}

} // namespace XLS

