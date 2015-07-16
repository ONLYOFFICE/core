#include "precompiled_xls.h"
#include "TableStyles.h"

namespace XLS
{;

TableStyles::TableStyles()
{
}


TableStyles::~TableStyles()
{
}


BaseObjectPtr TableStyles::clone()
{
	return BaseObjectPtr(new TableStyles(*this));
}


void TableStyles::writeFields(CFRecord& record)
{
#pragma message("####################### TableStyles record is not implemented")
	Log::error("TableStyles record is not implemented.");
	//record << some_value;
}


void TableStyles::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	unsigned short cchDefTableStyle;
	unsigned short cchDefPivotStyle;
	record >> cts >> cchDefTableStyle >> cchDefPivotStyle;
	rgchDefTableStyle.setSize(cchDefTableStyle);
	rgchDefPivotStyle.setSize(cchDefPivotStyle);
	record >> rgchDefTableStyle >> rgchDefPivotStyle;
}

} // namespace XLS

