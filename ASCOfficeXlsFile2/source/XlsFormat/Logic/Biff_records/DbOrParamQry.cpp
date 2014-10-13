#include "precompiled_xls.h"
#include "DbOrParamQry.h"

namespace XLS
{;

DbOrParamQry::DbOrParamQry()
{
}


DbOrParamQry::~DbOrParamQry()
{
}


BaseObjectPtr DbOrParamQry::clone()
{
	return BaseObjectPtr(new DbOrParamQry(*this));
}


void DbOrParamQry::writeFields(CFRecord& record)
{
#pragma message("####################### DbOrParamQry record is not implemented")
	Log::error("DbOrParamQry record is not implemented.");
	//record << some_value;
}


void DbOrParamQry::readFields(CFRecord& record)
{
#pragma message("####################### DbOrParamQry record is not implemented")
	Log::error("DbOrParamQry record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

