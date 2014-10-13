#include "precompiled_xls.h"
#include "DefColWidth.h"

namespace XLS
{;

DefColWidth::DefColWidth()
{
}


DefColWidth::~DefColWidth()
{
}


BaseObjectPtr DefColWidth::clone()
{
	return BaseObjectPtr(new DefColWidth(*this));
}


void DefColWidth::writeFields(CFRecord& record)
{
	record.registerDelayedFilePointerSource(rt_Index);
	record << cchdefColWidth;
}


void DefColWidth::readFields(CFRecord& record)
{
	record >> cchdefColWidth;
}

} // namespace XLS

