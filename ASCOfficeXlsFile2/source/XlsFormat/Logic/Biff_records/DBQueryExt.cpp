#include "precompiled_xls.h"
#include "DBQueryExt.h"

namespace XLS
{;

DBQueryExt::DBQueryExt()
{
}


DBQueryExt::~DBQueryExt()
{
}


BaseObjectPtr DBQueryExt::clone()
{
	return BaseObjectPtr(new DBQueryExt(*this));
}


void DBQueryExt::writeFields(CFRecord& record)
{
#pragma message("####################### DBQueryExt record is not implemented")
	Log::error("DBQueryExt record is not implemented.");
	//record << some_value;
}


void DBQueryExt::readFields(CFRecord& record)
{
#pragma message("####################### DBQueryExt record is not implemented")
	Log::error("DBQueryExt record is not implemented.");
	//record >> some_value;
}

} // namespace XLS

