#include "precompiled_xls.h"
#include "RecalcId.h"

namespace XLS
{;

RecalcId::RecalcId()
{
}


RecalcId::~RecalcId()
{
}


BaseObjectPtr RecalcId::clone()
{
	return BaseObjectPtr(new RecalcId(*this));
}


void RecalcId::writeFields(CFRecord& record)
{
	unsigned __int16 rt = 449;
	record << rt;
	record.reserveNunBytes(2); // reserved
	record << dwBuild;
}


void RecalcId::readFields(CFRecord& record)
{
	unsigned __int16 rt;
	record >> rt;
	record.skipNunBytes(2); // reserved
	record >> dwBuild;
}

} // namespace XLS

