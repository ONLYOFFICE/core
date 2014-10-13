#include "precompiled_xls.h"
#include "DataLabExt.h"
#include <Logic/Biff_structures/FrtHeader.h>

namespace XLS
{;

DataLabExt::DataLabExt()
{
}


DataLabExt::~DataLabExt()
{
}


BaseObjectPtr DataLabExt::clone()
{
	return BaseObjectPtr(new DataLabExt(*this));
}


void DataLabExt::writeFields(CFRecord& record)
{
	FrtHeader frtHeader(rt_DataLabExt);
	record << frtHeader;
}


void DataLabExt::readFields(CFRecord& record)
{
	FrtHeader frtHeader;
	record >> frtHeader;
}

} // namespace XLS

