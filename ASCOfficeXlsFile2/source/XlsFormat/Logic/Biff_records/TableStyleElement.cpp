
#include "TableStyleElement.h"

namespace XLS
{;

TableStyleElement::TableStyleElement()
{
}


TableStyleElement::~TableStyleElement()
{
}


BaseObjectPtr TableStyleElement::clone()
{
	return BaseObjectPtr(new TableStyleElement(*this));
}


void TableStyleElement::writeFields(CFRecord& record)
{
#pragma message("####################### TableStyleElement record is not implemented")
	Log::error("TableStyleElement record is not implemented.");
	//record << some_value;
}


void TableStyleElement::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> tseType >> size >> index;
}

} // namespace XLS

