
#include "ChartNumNillable.h"
#include <Binary/CFRecord.h>

namespace XLS
{


ChartNumNillable::ChartNumNillable(const unsigned short nil_type)
:	nil_type_(nil_type)
{
}


BiffStructurePtr ChartNumNillable::clone()
{
	return BiffStructurePtr(new ChartNumNillable(*this));
}

void ChartNumNillable::store(CFRecord& record)
{
	record << data.xnum;
}


void ChartNumNillable::load(CFRecord& record)
{
	record >> data.xnum;
}


const bool ChartNumNillable::isNil()
{
	return data.fExprO == 0xffff;
}


} // namespace XLS

