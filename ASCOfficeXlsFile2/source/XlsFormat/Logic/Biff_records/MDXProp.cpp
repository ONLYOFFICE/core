#include "precompiled_xls.h"
#include "MDXProp.h"

namespace XLS
{;

MDXProp::MDXProp()
{
}


MDXProp::~MDXProp()
{
}


BaseObjectPtr MDXProp::clone()
{
	return BaseObjectPtr(new MDXProp(*this));
}


void MDXProp::writeFields(CFRecord& record)
{
#pragma message("####################### MDXProp record is not implemented")
	Log::error("MDXProp record is not implemented.");
	//record << some_value;
}


void MDXProp::readFields(CFRecord& record)
{
	record.skipNunBytes(12);
#pragma message("############################ frtHeader skipped here")
	record >> istrConnName >> tfnSrc >> istrMbr >> istrProp;
}

} // namespace XLS

