#include "stdafx.h"
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
	WORD rt = 449;
	record << rt;
	record.reserveNBytes(2); // reserved
	record << dwBuild;
}


void RecalcId::readFields(CFRecord& record)
{
	WORD rt;
	record >> rt;
	record.skipNBytes(2); // reserved
	record >> dwBuild;
}

} // namespace XLS

