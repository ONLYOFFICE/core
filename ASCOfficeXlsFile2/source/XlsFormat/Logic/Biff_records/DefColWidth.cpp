
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
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	record >> cchdefColWidth;

	global_info->defaultColumnWidth = cchdefColWidth ;
}

} // namespace XLS

