
#include "CellWatch.h"

namespace XLS
{

CellWatch::CellWatch()
{
}


CellWatch::~CellWatch()
{
}


BaseObjectPtr CellWatch::clone()
{
	return BaseObjectPtr(new CellWatch(*this));
}


void CellWatch::writeFields(CFRecord& record)
{
#pragma message("####################### CellWatch record is not implemented")
	Log::error("CellWatch record is not implemented.");
	//record << some_value;
}


void CellWatch::readFields(CFRecord& record)
{
	record >> frtRefHeaderU;
	record.skipNunBytes(4); // reserved
}

} // namespace XLS

