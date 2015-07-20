
#include "OleDbConn.h"

namespace XLS
{;

OleDbConn::OleDbConn()
{
}


OleDbConn::~OleDbConn()
{
}


BaseObjectPtr OleDbConn::clone()
{
	return BaseObjectPtr(new OleDbConn(*this));
}


void OleDbConn::writeFields(CFRecord& record)
{
#pragma message("####################### OleDbConn record is not implemented")
	Log::error("OleDbConn record is not implemented.");
	//record << some_value;
}


void OleDbConn::readFields(CFRecord& record)
{
#pragma message("####################### OleDbConn record is not implemented")
	Log::error("OleDbConn record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

